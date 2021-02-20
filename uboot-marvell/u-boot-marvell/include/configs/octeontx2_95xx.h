/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:    GPL-2.0
 * https://spdx.org/licenses
 */


#ifndef __OCTEONTX2_95XX_H__
#define __OCTEONTX2_95XX_H__

/* Generic Interrupt Controller Definitions */
#define GICD_BASE			(0x801000000000)
#define GICR_BASE			(0x801000002000)

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		(0x1800000)	/* 24MHz */

#define CONFIG_SUPPORT_RAW_INITRD

/** Maximum size of image supported for bootm (and bootable FIT images) */
#define CONFIG_SYS_BOOTM_LEN		(256 << 20)

/** Memory base address */
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_TEXT_BASE

/** Stack starting address */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + 0xffff0)

/** Memory test starting address */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE

/** Memory test end address */
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0xf0000)

/** Heap size for U-Boot */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 64 * 1024 * 1024)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_SDRAM_BASE

#define CONFIG_NR_DRAM_BANKS		1

#define CONFIG_MISC_INIT_R

/**
 * Only allow the Ethernet MAC address environment variable to be
 * overwritten once.
 */
#define CONFIG_OVERWRITE_ETHADDR_ONCE

/** Reduce hashes printed out */
#define CONFIG_TFTP_TSIZE

/* BOOTP options */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_PXE
#define CONFIG_BOOTP_TFTP_SERVERIP

/***** SPI Defines *********/
#ifdef CONFIG_DM_SPI_FLASH
#define CONFIG_SF_DEFAULT_SPEED 12500000
#define CONFIG_SF_DEFAULT_MODE	0
#define CONFIG_SF_DEFAULT_BUS	0
#define CONFIG_SF_DEFAULT_CS	0
#endif

/** Extra environment settings */
#define CONFIG_EXTRA_ENV_SETTINGS	\
					"loadaddr=040080000\0"	\
					"autoload=0\0"

/** Environment defines */
#define CONFIG_ENV_SIZE			0x8000
#define CONFIG_ENV_OFFSET		0xf00000
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		0
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_SECT_SIZE		(64*1024)
#define CONFIG_ENV_SPI_MAX_HZ		12500000
#define CONFIG_ENV_SPI_MODE		0
#define CONFIG_ENV_SPI_BUS		0
#define CONFIG_ENV_SPI_CS		0
#endif

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		1024	/** Console I/O Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MAXARGS		64	/** max command args */

#define CONFIG_SYS_MMC_MAX_BLK_COUNT	8191

#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT		env_get("prompt")

#define CONFIG_MMC_SUPPORTS_TUNING
#define MMC_SUPPORTS_TUNING
#define CONFIG_CMD_BKOPS_ENABLE
#define CONFIG_SUPPORT_EMMC_RPMB

#define CONFIG_MMC_OCTEONTX
#ifndef CONFIG_BLK
# define CONFIG_BLK
#endif

#endif /* __OCTEONTX2_95XX_H__ */
