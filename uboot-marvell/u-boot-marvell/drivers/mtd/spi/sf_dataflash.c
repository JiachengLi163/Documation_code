/*
 * Atmel DataFlash probing
 *
 * Copyright (C) 2004-2009, 2015 Freescale Semiconductor, Inc.
 * Haikun Wang (haikun.wang@freescale.com)
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <spi.h>
#include <spi_flash.h>
#include <div64.h>
#include <linux/err.h>
#include <linux/math64.h>

#include "sf_internal.h"

/* reads can bypass the buffers */
#define OP_READ_CONTINUOUS	0xE8
#define OP_READ_PAGE		0xD2

/* group B requests can run even while status reports "busy" */
#define OP_READ_STATUS		0xD7	/* group B */

/* move data between host and buffer */
#define OP_READ_BUFFER1		0xD4	/* group B */
#define OP_READ_BUFFER2		0xD6	/* group B */
#define OP_WRITE_BUFFER1	0x84	/* group B */
#define OP_WRITE_BUFFER2	0x87	/* group B */

/* erasing flash */
#define OP_ERASE_PAGE		0x81
#define OP_ERASE_BLOCK		0x50

/* move data between buffer and flash */
#define OP_TRANSFER_BUF1	0x53
#define OP_TRANSFER_BUF2	0x55
#define OP_MREAD_BUFFER1	0xD4
#define OP_MREAD_BUFFER2	0xD6
#define OP_MWERASE_BUFFER1	0x83
#define OP_MWERASE_BUFFER2	0x86
#define OP_MWRITE_BUFFER1	0x88	/* sector must be pre-erased */
#define OP_MWRITE_BUFFER2	0x89	/* sector must be pre-erased */

/* write to buffer, then write-erase to flash */
#define OP_PROGRAM_VIA_BUF1	0x82
#define OP_PROGRAM_VIA_BUF2	0x85

/* compare buffer to flash */
#define OP_COMPARE_BUF1		0x60
#define OP_COMPARE_BUF2		0x61

/* read flash to buffer, then write-erase to flash */
#define OP_REWRITE_VIA_BUF1	0x58
#define OP_REWRITE_VIA_BUF2	0x59

/*
 * newer chips report JEDEC manufacturer and device IDs; chip
 * serial number and OTP bits; and per-sector writeprotect.
 */
#define OP_READ_ID		0x9F
#define OP_READ_SECURITY	0x77
#define OP_WRITE_SECURITY_REVC	0x9A
#define OP_WRITE_SECURITY	0x9B	/* revision D */

struct dataflash {
	uint8_t			command[16];
	unsigned short		page_offset;	/* offset in flash address */
};

/* Return the status of the DataFlash device */
static inline int dataflash_status(struct spi_flash *spi_flash)
{
	int ret;
	u8 status;
	/*
	 * NOTE:  at45db321c over 25 MHz wants to write
	 * a dummy byte after the opcode...
	 */
	ret = spi_flash_cmd(spi_flash, OP_READ_STATUS, &status, 1);
	return ret ? -EIO : status;
}

/*
 * Poll the DataFlash device until it is READY.
 * This usually takes 5-20 msec or so; more for sector erase.
 * ready: return > 0
 */
static int dataflash_waitready(struct spi_flash *spi_flash)
{
	int status;
	int timeout = 2 * CONFIG_SYS_HZ;
	int timebase;

	timebase = get_timer(0);
	do {
		status = dataflash_status(spi_flash);
		if (status < 0)
			status = 0;

		if (status & (1 << 7))	/* RDY/nBSY */
			return status;

		mdelay(3);
	} while (get_timer(timebase) < timeout);

	return -ETIME;
}

/* Erase pages of flash */
static int spi_dataflash_erase(struct udevice *dev, u32 offset, size_t len)
{
	struct spi_flash_command	cmd;
	struct dataflash	*dataflash;
	struct spi_flash	*spi_flash;
	struct spi_slave	*spi;
	unsigned		blocksize;
	uint32_t		rem;
	int			status;

	dataflash = dev_get_priv(dev);
	spi_flash = dev_get_uclass_priv(dev);
	spi = spi_flash->spi;

	blocksize = spi_flash->page_size << 3;

	debug("%s: erase addr=0x%x len 0x%x\n", dev->name, offset, len);

	div_u64_rem(len, spi_flash->page_size, &rem);
	if (rem) {
		printf("%s: len(0x%x) isn't the multiple of page size(0x%x)\n",
		       dev->name, len, spi_flash->page_size);
		return -EINVAL;
	}
	div_u64_rem(offset, spi_flash->page_size, &rem);
	if (rem) {
		printf("%s: offset(0x%x) isn't the multiple of page size(0x%x)\n",
		       dev->name, offset, spi_flash->page_size);
		return -EINVAL;
	}

	status = spi_claim_bus(spi);
	if (status) {
		debug("dataflash: unable to claim SPI bus\n");
		return status;
	}

	spi_flash_command_init(&cmd, OP_ERASE_BLOCK, SPI_FLASH_3B_ADDR_LEN,
			       SPI_FCMD_ERASE);
	while (len > 0) {
		unsigned int	pageaddr;
		int		do_block;
		/*
		 * Calculate flash page address; use block erase (for speed) if
		 * we're at a block boundary and need to erase the whole block.
		 */
		pageaddr = div_u64(offset, spi_flash->page_size);
		do_block = (pageaddr & 0x7) == 0 && len >= blocksize;
		pageaddr = pageaddr << dataflash->page_offset;

		cmd.inst = do_block ? OP_ERASE_BLOCK : OP_ERASE_PAGE;
		cmd.addr = pageaddr & 0x00FFFF00;

		debug("%s ERASE %s: (%x) %x %x %x [%d]\n",
		      dev->name, do_block ? "block" : "page",
		      cmd.inst,
		      (cmd.addr >> 16) & 0xff,
		      (cmd.addr >>  8) & 0xff,
		      (cmd.addr >>  0) & 0xff,
		      pageaddr);

		status = spi_flash_cmd_write(spi_flash, &cmd);
		if (status < 0) {
			debug("%s: erase send command error!\n", dev->name);
			return -EIO;
		}

		status = dataflash_waitready(spi_flash);
		if (status < 0) {
			debug("%s: erase waitready error!\n", dev->name);
			return status;
		}

		if (do_block) {
			offset += blocksize;
			len -= blocksize;
		} else {
			offset += spi_flash->page_size;
			len -= spi_flash->page_size;
		}
	}

	spi_release_bus(spi);

	return 0;
}

/*
 * Read from the DataFlash device.
 *   offset : Start offset in flash device
 *   len    : Amount to read
 *   buf    : Buffer containing the data
 */
static int spi_dataflash_read(struct udevice *dev, u32 offset, size_t len,
			      void *buf)
{
	struct spi_flash_command	cmd;
	struct dataflash	*dataflash;
	struct spi_flash	*spi_flash;
	struct spi_slave	*spi;
	unsigned int		addr;
	int			status;

	dataflash = dev_get_priv(dev);
	spi_flash = dev_get_uclass_priv(dev);
	spi = spi_flash->spi;

	debug("%s: read addr=0x%x len 0x%x\n", dev->name, offset, len);

	/* Calculate flash page/byte address */
	addr = (((unsigned)offset / spi_flash->page_size)
	       << dataflash->page_offset)
	       + ((unsigned)offset % spi_flash->page_size);

	status = spi_claim_bus(spi);
	if (status) {
		debug("dataflash: unable to claim SPI bus\n");
		return status;
	}

	/*
	 * Continuous read, max clock = f(car) which may be less than
	 * the peak rate available.  Some chips support commands with
	 * fewer "don't care" bytes.  Both buffers stay unchanged.
	 */
	spi_flash_command_init(&cmd, OP_READ_CONTINUOUS, SPI_FLASH_3B_ADDR_LEN,
			       SPI_FCMD_READ);
	cmd.addr = addr;
	cmd.num_wait_states = 4 * 8; /* 4 "don't care" bytes */
	cmd.data_len = len;
	cmd.rx_data = buf;

	/* plus 4 "don't care" bytes, command len: 4 + 4 "don't care" bytes */
	status = spi_flash_cmd_read(spi_flash, &cmd);

	spi_release_bus(spi);

	return status;
}

/*
 * Write to the DataFlash device.
 *   offset     : Start offset in flash device
 *   len    : Amount to write
 *   buf    : Buffer containing the data
 */
int spi_dataflash_write(struct udevice *dev, u32 offset, size_t len,
			const void *buf)
{
	struct spi_flash_command	cmd;
	struct dataflash	*dataflash;
	struct spi_flash	*spi_flash;
	struct spi_slave	*spi;
	unsigned int		pageaddr, addr, to, writelen;
	size_t			remaining = len;
	u_char			*writebuf = (u_char *)buf;
	int			status = -EINVAL;

	dataflash = dev_get_priv(dev);
	spi_flash = dev_get_uclass_priv(dev);
	spi = spi_flash->spi;

	debug("%s: write 0x%x..0x%x\n", dev->name, offset, (offset + len));

	pageaddr = ((unsigned)offset / spi_flash->page_size);
	to = ((unsigned)offset % spi_flash->page_size);
	if (to + len > spi_flash->page_size)
		writelen = spi_flash->page_size - to;
	else
		writelen = len;

	status = spi_claim_bus(spi);
	if (status) {
		debug("dataflash: unable to claim SPI bus\n");
		return status;
	}

	spi_flash_command_init(&cmd, OP_TRANSFER_BUF1, SPI_FLASH_3B_ADDR_LEN,
			       SPI_FCMD_WRITE);
	while (remaining > 0) {
		debug("write @ %d:%d len=%d\n", pageaddr, to, writelen);

		/*
		 * REVISIT:
		 * (a) each page in a sector must be rewritten at least
		 *     once every 10K sibling erase/program operations.
		 * (b) for pages that are already erased, we could
		 *     use WRITE+MWRITE not PROGRAM for ~30% speedup.
		 * (c) WRITE to buffer could be done while waiting for
		 *     a previous MWRITE/MWERASE to complete ...
		 * (d) error handling here seems to be mostly missing.
		 *
		 * Two persistent bits per page, plus a per-sector counter,
		 * could support (a) and (b) ... we might consider using
		 * the second half of sector zero, which is just one block,
		 * to track that state.  (On AT91, that sector should also
		 * support boot-from-DataFlash.)
		 */

		addr = pageaddr << dataflash->page_offset;

		/* (1) Maybe transfer partial page to Buffer1 */
		if (writelen != spi_flash->page_size) {
			cmd.inst = OP_TRANSFER_BUF1;
			cmd.addr = (addr & 0x00FFFF00);
			cmd.data_len = 0;
			cmd.tx_data = NULL;

			debug("TRANSFER: (%x) %x %x %x\n",
			      cmd.inst,
			      (cmd.addr >> 16) & 0xff,
			      (cmd.addr >>  8) & 0xff,
			      (cmd.addr >>  0) & 0xff);

			status = spi_flash_cmd_write(spi_flash, &cmd);
			if (status < 0) {
				debug("%s: write(<pagesize) command error!\n",
				      dev->name);
				return -EIO;
			}

			status = dataflash_waitready(spi_flash);
			if (status < 0) {
				debug("%s: write(<pagesize) waitready error!\n",
				      dev->name);
				return status;
			}
		}

		/* (2) Program full page via Buffer1 */
		addr += to;
		cmd.inst = OP_PROGRAM_VIA_BUF1;
		cmd.addr = addr;
		cmd.data_len = writelen;
		cmd.tx_data = writebuf;

		debug("PROGRAM: (%x) %x %x %x\n",
		      cmd.inst,
		      (cmd.addr >> 16) & 0xff,
		      (cmd.addr >>  8) & 0xff,
		      (cmd.addr >>  0) & 0xff);

		status = spi_flash_cmd_write(spi_flash, &cmd);
		if (status < 0) {
			debug("%s: write send command error!\n", dev->name);
			return -EIO;
		}

		status = dataflash_waitready(spi_flash);
		if (status < 0) {
			debug("%s: write waitready error!\n", dev->name);
			return status;
		}

#ifdef CONFIG_SPI_DATAFLASH_WRITE_VERIFY
		/* (3) Compare to Buffer1 */
		addr = pageaddr << dataflash->page_offset;
		cmd.inst = OP_COMPARE_BUF1;
		cmd.addr = addr & 0x00FFFF00;
		cmd.data_len = writelen;
		cmd.tx_data = writebuf;

		debug("COMPARE: (%x) %x %x %x\n",
		      cmd.inst,
		      (cmd.addr >> 16) & 0xff,
		      (cmd.addr >>  8) & 0xff,
		      (cmd.addr >>  0) & 0xff);

		status = spi_flash_cmd_write(spi, &cmd);
		if (status < 0) {
			debug("%s: write(compare) send command error!\n",
			      dev->name);
			return -EIO;
		}

		status = dataflash_waitready(spi);

		/* Check result of the compare operation */
		if (status & (1 << 6)) {
			printf("dataflash: write compare page %u, err %d\n",
			       pageaddr, status);
			remaining = 0;
			status = -EIO;
			break;
		} else {
			status = 0;
		}

#endif	/* CONFIG_SPI_DATAFLASH_WRITE_VERIFY */
		remaining = remaining - writelen;
		pageaddr++;
		to = 0;
		writebuf += writelen;

		if (remaining > spi_flash->page_size)
			writelen = spi_flash->page_size;
		else
			writelen = remaining;
	}

	spi_release_bus(spi);

	return 0;
}

static int add_dataflash(struct udevice *dev, char *name, int nr_pages,
			     int pagesize, int pageoffset, char revision)
{
	struct spi_flash *spi_flash;
	struct dataflash *dataflash;

	dataflash = dev_get_priv(dev);
	spi_flash = dev_get_uclass_priv(dev);

	dataflash->page_offset = pageoffset;

	spi_flash->name = name;
	spi_flash->page_size = pagesize;
	spi_flash->size = nr_pages * pagesize;
	spi_flash->erase_size = pagesize;

#ifndef CONFIG_SPL_BUILD
	printf("SPI DataFlash: Detected %s with page size ", spi_flash->name);
	print_size(spi_flash->page_size, ", erase size ");
	print_size(spi_flash->erase_size, ", total ");
	print_size(spi_flash->size, "");
	printf(", revision %c", revision);
	puts("\n");
#endif

	return 0;
}

struct flash_info {
	char		*name;

	/*
	 * JEDEC id has a high byte of zero plus three data bytes:
	 * the manufacturer id, then a two byte device id.
	 */
	uint32_t	jedec_id;

	/* The size listed here is what works with OP_ERASE_PAGE. */
	unsigned	nr_pages;
	uint16_t	pagesize;
	uint16_t	pageoffset;

	uint16_t	flags;
#define SUP_POW2PS	0x0002		/* supports 2^N byte pages */
#define IS_POW2PS	0x0001		/* uses 2^N byte pages */
};

static struct flash_info dataflash_data[] = {
	/*
	 * NOTE:  chips with SUP_POW2PS (rev D and up) need two entries,
	 * one with IS_POW2PS and the other without.  The entry with the
	 * non-2^N byte page size can't name exact chip revisions without
	 * losing backwards compatibility for cmdlinepart.
	 *
	 * Those two entries have different name spelling format in order to
	 * show their difference obviously.
	 * The upper case refer to the chip isn't in normal 2^N bytes page-size
	 * mode.
	 * The lower case refer to the chip is in normal 2^N bytes page-size
	 * mode.
	 *
	 * These newer chips also support 128-byte security registers (with
	 * 64 bytes one-time-programmable) and software write-protection.
	 */
	{ "AT45DB011B",  0x1f2200, 512, 264, 9, SUP_POW2PS},
	{ "at45db011d",  0x1f2200, 512, 256, 8, SUP_POW2PS | IS_POW2PS},

	{ "AT45DB021B",  0x1f2300, 1024, 264, 9, SUP_POW2PS},
	{ "at45db021d",  0x1f2300, 1024, 256, 8, SUP_POW2PS | IS_POW2PS},

	{ "AT45DB041x",  0x1f2400, 2048, 264, 9, SUP_POW2PS},
	{ "at45db041d",  0x1f2400, 2048, 256, 8, SUP_POW2PS | IS_POW2PS},

	{ "AT45DB081B",  0x1f2500, 4096, 264, 9, SUP_POW2PS},
	{ "at45db081d",  0x1f2500, 4096, 256, 8, SUP_POW2PS | IS_POW2PS},

	{ "AT45DB161x",  0x1f2600, 4096, 528, 10, SUP_POW2PS},
	{ "at45db161d",  0x1f2600, 4096, 512, 9, SUP_POW2PS | IS_POW2PS},

	{ "AT45DB321x",  0x1f2700, 8192, 528, 10, 0},		/* rev C */

	{ "AT45DB321x",  0x1f2701, 8192, 528, 10, SUP_POW2PS},
	{ "at45db321d",  0x1f2701, 8192, 512, 9, SUP_POW2PS | IS_POW2PS},

	{ "AT45DB642x",  0x1f2800, 8192, 1056, 11, SUP_POW2PS},
	{ "at45db642d",  0x1f2800, 8192, 1024, 10, SUP_POW2PS | IS_POW2PS},
};

static struct flash_info *jedec_probe(struct spi_flash *spi_flash)
{
	int			tmp;
	uint8_t			id[5];
	uint32_t		jedec;
	struct flash_info	*info;
	int status;

	/*
	 * JEDEC also defines an optional "extended device information"
	 * string for after vendor-specific data, after the three bytes
	 * we use here.  Supporting some chips might require using it.
	 *
	 * If the vendor ID isn't Atmel's (0x1f), assume this call failed.
	 * That's not an error; only rev C and newer chips handle it, and
	 * only Atmel sells these chips.
	 */
	tmp = spi_flash_cmd(spi_flash, CMD_READ_ID, id, sizeof(id));
	if (tmp < 0) {
		printf("dataflash: error %d reading JEDEC ID\n", tmp);
		return ERR_PTR(tmp);
	}
	if (id[0] != 0x1f)
		return NULL;

	jedec = id[0];
	jedec = jedec << 8;
	jedec |= id[1];
	jedec = jedec << 8;
	jedec |= id[2];

	for (tmp = 0, info = dataflash_data;
			tmp < ARRAY_SIZE(dataflash_data);
			tmp++, info++) {
		if (info->jedec_id == jedec) {
			if (info->flags & SUP_POW2PS) {
				status = dataflash_status(spi_flash);
				if (status < 0) {
					debug("dataflash: status error %d\n",
					      status);
					return NULL;
				}
				if (status & 0x1) {
					if (info->flags & IS_POW2PS)
						return info;
				} else {
					if (!(info->flags & IS_POW2PS))
						return info;
				}
			} else {
				return info;
			}
		}
	}

	/*
	 * Treat other chips as errors ... we won't know the right page
	 * size (it might be binary) even when we can tell which density
	 * class is involved (legacy chip id scheme).
	 */
	printf("dataflash: JEDEC id %06x not handled\n", jedec);
	return ERR_PTR(-ENODEV);
}

/*
 * Detect and initialize DataFlash device, using JEDEC IDs on newer chips
 * or else the ID code embedded in the status bits:
 *
 *   Device      Density         ID code          #Pages PageSize  Offset
 *   AT45DB011B  1Mbit   (128K)  xx0011xx (0x0c)    512    264      9
 *   AT45DB021B  2Mbit   (256K)  xx0101xx (0x14)   1024    264      9
 *   AT45DB041B  4Mbit   (512K)  xx0111xx (0x1c)   2048    264      9
 *   AT45DB081B  8Mbit   (1M)    xx1001xx (0x24)   4096    264      9
 *   AT45DB0161B 16Mbit  (2M)    xx1011xx (0x2c)   4096    528     10
 *   AT45DB0321B 32Mbit  (4M)    xx1101xx (0x34)   8192    528     10
 *   AT45DB0642  64Mbit  (8M)    xx111xxx (0x3c)   8192   1056     11
 *   AT45DB1282  128Mbit (16M)   xx0100xx (0x10)  16384   1056     11
 */
static int spi_dataflash_probe(struct udevice *dev)
{
	struct spi_slave *spi = dev_get_parent_priv(dev);
	struct spi_flash *spi_flash;
	struct flash_info *info;
	int status;

	spi_flash = dev_get_uclass_priv(dev);
	spi_flash->spi = spi;
	spi_flash->dev = dev;

	status = spi_claim_bus(spi);
	if (status)
		return status;

	/*
	 * Try to detect dataflash by JEDEC ID.
	 * If it succeeds we know we have either a C or D part.
	 * D will support power of 2 pagesize option.
	 * Both support the security register, though with different
	 * write procedures.
	 */
	info = jedec_probe(spi_flash);
	if (IS_ERR(info))
		goto err_jedec_probe;
	if (info != NULL) {
		status = add_dataflash(dev, info->name, info->nr_pages,
				info->pagesize, info->pageoffset,
				(info->flags & SUP_POW2PS) ? 'd' : 'c');
		if (status < 0)
			goto err_status;
	}

       /*
	* Older chips support only legacy commands, identifing
	* capacity using bits in the status byte.
	*/
	status = dataflash_status(spi_flash);
	if (status <= 0 || status == 0xff) {
		printf("dataflash: read status error %d\n", status);
		if (status == 0 || status == 0xff)
			status = -ENODEV;
		goto err_jedec_probe;
	}

       /*
	* if there's a device there, assume it's dataflash.
	* board setup should have set spi->max_speed_max to
	* match f(car) for continuous reads, mode 0 or 3.
	*/
	switch (status & 0x3c) {
	case 0x0c:	/* 0 0 1 1 x x */
		status = add_dataflash(dev, "AT45DB011B", 512, 264, 9, 0);
		break;
	case 0x14:	/* 0 1 0 1 x x */
		status = add_dataflash(dev, "AT45DB021B", 1024, 264, 9, 0);
		break;
	case 0x1c:	/* 0 1 1 1 x x */
		status = add_dataflash(dev, "AT45DB041x", 2048, 264, 9, 0);
		break;
	case 0x24:	/* 1 0 0 1 x x */
		status = add_dataflash(dev, "AT45DB081B", 4096, 264, 9, 0);
		break;
	case 0x2c:	/* 1 0 1 1 x x */
		status = add_dataflash(dev, "AT45DB161x", 4096, 528, 10, 0);
		break;
	case 0x34:	/* 1 1 0 1 x x */
		status = add_dataflash(dev, "AT45DB321x", 8192, 528, 10, 0);
		break;
	case 0x38:	/* 1 1 1 x x x */
	case 0x3c:
		status = add_dataflash(dev, "AT45DB642x", 8192, 1056, 11, 0);
		break;
	/* obsolete AT45DB1282 not (yet?) supported */
	default:
		printf("dataflash: unsupported device (%x)\n", status & 0x3c);
		status = -ENODEV;
		goto err_status;
	}

	return status;

err_status:
	spi_free_slave(spi);
err_jedec_probe:
	spi_release_bus(spi);
	return status;
}

static const struct dm_spi_flash_ops spi_dataflash_ops = {
	.read = spi_dataflash_read,
	.write = spi_dataflash_write,
	.erase = spi_dataflash_erase,
};

static const struct udevice_id spi_dataflash_ids[] = {
	{ .compatible = "atmel,at45", },
	{ .compatible = "atmel,dataflash", },
	{ }
};

U_BOOT_DRIVER(spi_dataflash) = {
	.name		= "spi_dataflash",
	.id		= UCLASS_SPI_FLASH,
	.of_match	= spi_dataflash_ids,
	.probe		= spi_dataflash_probe,
	.priv_auto_alloc_size = sizeof(struct dataflash),
	.ops		= &spi_dataflash_ops,
};
