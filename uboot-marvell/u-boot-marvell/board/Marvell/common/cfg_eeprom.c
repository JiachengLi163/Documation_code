// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018 Marvell International Ltd.
 */

#include <common.h>
#include <mvebu/cfg_eeprom.h>
#include <mmc.h>

struct eeprom_struct board_config_val = CFG_DEFAULT_VALUE;
struct config_types_info config_types_info[] = MV_EEPROM_CONFIG_INFO;
int eeprom_initialized = -1;
int g_board_hw_info = -1;
int g_mmc_info_blk_addr = 100; // max 1024 blk
int g_mmc_index = 1; // mmc device index
int g_blk_length = 2;
int g_blk_size = 512; // bytes
uint8_t g_temp_buffer[1024]; // g_blk_length * g_blk_size

static char hw_info_param_list[][HW_INFO_MAX_NAME_LEN] = {
	"pcb_slm",
	"pcb_rev",
	"eco_rev",
	"pcb_sn",
	"ethaddr",
	"eth1addr",
	"eth2addr",
	"eth3addr",
	"eth4addr",
	"eth5addr",
	"eth6addr",
	"eth7addr",
	"eth8addr",
	"eth9addr"
};

static int hw_info_param_num = (sizeof(hw_info_param_list) /
				sizeof(hw_info_param_list[0]));

static u32 cfg_eeprom_checksum8(u8 *start, u32 len)
{
	u32 sum = 0;
	u8 *startp = start;

	do {
		sum += *startp;
		startp++;
		len--;
	} while (len > 0);
	return sum;
}

/* cfg_eeprom_get_config_type
 * config_info input pointer receive the mapping of the
 * required field in the local struct
 */
static bool cfg_eeprom_get_config_type(enum mv_config_type_id id,
				       struct config_types_info *config_info)
{
	int i;

	/* verify existence of requested config type, pull its data */
	for (i = 0; i < MV_CONFIG_TYPE_MAX_OPTION ; i++)
		if (config_types_info[i].config_id == id) {
			*config_info = config_types_info[i];
			return true;
		}
	pr_err("requested MV_CONFIG_TYPE_ID was not found (%d)\n", id);

	return false;
}

/* cfg_eeprom_save - write the local struct to EEPROM */
void cfg_eeprom_save(int length)
{
	if ('E' != board_config_val.bootdev[0] && 'S' != board_config_val.bootdev[0]
			&& 'U' != board_config_val.bootdev[0] && 'N' != board_config_val.bootdev[0])
		return;

	/* write local struct with fdt blob to EEPROM */
	memset(g_temp_buffer, 0, sizeof(g_temp_buffer));
	board_config_val.checksum = cfg_eeprom_checksum8((u8 *)&board_config_val.pattern,
								 EEPROM_STRUCT_SIZE - 4);
	memcpy(g_temp_buffer, &board_config_val, EEPROM_STRUCT_SIZE);
	blk_write_devnum(IF_TYPE_MMC, g_mmc_index, g_mmc_info_blk_addr, g_blk_length,
			&board_config_val);
	/* reset g_board_id so it will get board ID from EEPROM again */
	g_board_hw_info = -1;
}

/* cfg_eeprom_get_board_config - return the whole board config
 * It is assumed the cfg_eeprom_init must be called prior to this routine,
 * otherwise static default configuration will be used.
 */
struct eeprom_struct *cfg_eeprom_get_board_config(void)
{
	return &board_config_val;
}

/* cfg_eeprom_get_hw_info_str - copy hw_info from cfg_emmc to destination */
void cfg_eeprom_get_hw_info_str(uchar *hw_info_str)
{
	int len;

	// must wait kernel init the record info, can read the info
	char ch = board_config_val.bootdev[0];
	if ('E' != ch && 'S' != ch && 'U' != ch && 'N' != ch)
		return;

	/* hw_info isn't initialized, need to read hw_info from EEPROM */
	if (g_board_hw_info == -1) {
		/* read hw_info config from EMMC */
		// 1 blk = 512byte, read 2 blk
		blk_read_devnum(IF_TYPE_MMC, g_mmc_index, g_mmc_info_blk_addr,
				g_blk_length, g_temp_buffer);
		memcpy(&board_config_val, g_temp_buffer, EEPROM_STRUCT_SIZE);
	}
	len = strlen((const char *)board_config_val.man_info.hw_info);
	if (len >= MVEBU_HW_INFO_LEN)
		len = MVEBU_HW_INFO_LEN - 1;

	memcpy(hw_info_str, board_config_val.man_info.hw_info, len);
}

/* cfg_eeprom_set_hw_info_str - copy hw_info sting to cfg_emmc module
 * It is assumed the cfg_eeprom_init must be called prior to this routine,
 * otherwise static default configuration will be used.
 */
void cfg_eeprom_set_hw_info_str(uchar *hw_info_str)
{
	int len;
	struct config_types_info config_info;

	/* read hw_info config from EEPROM */
	if (!cfg_eeprom_get_config_type(MV_CONFIG_HW_INFO, &config_info)) {
		pr_err("Could not find MV_CONFIG_hw_info\n");
		return;
	}

	len = strlen((const char *)hw_info_str);
	if (len >= config_info.byte_cnt)
		len = config_info.byte_cnt - 1;

	/* need to set all value to 0 at first for later string operation */
	memset(board_config_val.man_info.hw_info, 0, config_info.byte_cnt);
	memcpy(board_config_val.man_info.hw_info, hw_info_str, len);
}

/* cfg_eeprom_skip_space - skip the space character */
static char *cfg_eeprom_skip_space(char *buf)
{
	while ((buf[0] == ' ' || buf[0] == '\t'))
		++buf;
	return buf;
}

/*
 * cfg_eeprom_parse_hw_info
 * - parse the hw_info from string to name/value pairs
 */
int cfg_eeprom_parse_hw_info(struct hw_info_data_struct *hw_info_data_array)
{
	int count;
	char *name;
	char *value;
	int len;
	uchar hw_info_str[MVEBU_HW_INFO_LEN];

	/* need to set all to 0 for later string operation */
	memset(hw_info_str, 0, sizeof(hw_info_str));

	cfg_eeprom_get_hw_info_str(hw_info_str);
	name = (char *)hw_info_str;
	name = cfg_eeprom_skip_space(name);
	/* return 0 in case the string is empty */
	if (!name)
		return 0;

	for (count = 0; name; count++) {
		value = strchr(name, '=');

		if (!value)
			return count;

		*value = '\0';
		len = strlen(name);
		memcpy(hw_info_data_array[count].name, name, len);
		hw_info_data_array[count].name[len] = '\0';
		value++;

		name = strchr(value, ' ');
		if (!name)
			return ++count;

		*name = '\0';
		len = strlen(value);
		memcpy(hw_info_data_array[count].value, value, len);
		hw_info_data_array[count].value[len] = '\0';
		name = cfg_eeprom_skip_space(name + 1);
	}
	count++;

	return count;
}

/* cfg_eeprom_validate_name - check parameter's name is valid or not
 * valid - return 0
 * invalid - return -1
 */
int cfg_eeprom_validate_name(char *name)
{
	int idx;

	for (idx = 0; idx < hw_info_param_num; idx++) {
		if (strcmp(name, hw_info_param_list[idx]) == 0)
			return 0;
	}

	return -1;
}

/* cfg_eeprom_parse_env - parse the env from env to name/value pairs */
int cfg_eeprom_parse_env(struct hw_info_data_struct *data_array,
			 int size)
{
	int param_num = 0;
	int idx;
	int len;
	char *name;
	char *value;

	/* need to memset to 0 for later string operation */
	memset(data_array, 0, size);
	for (idx = 0; idx < hw_info_param_num; idx++) {
		name = hw_info_param_list[idx];
		value = env_get(name);

		if (!value) {
			printf("miss %s in env, please set it at first\n",
			       hw_info_param_list[idx]);
			continue;
		}

		len = strlen(name);
		if (len > HW_INFO_MAX_NAME_LEN)
			len  = HW_INFO_MAX_NAME_LEN;
		memcpy(data_array[param_num].name, name, len);
		len = strlen(value);
		if (len > HW_INFO_MAX_NAME_LEN)
			len  = HW_INFO_MAX_NAME_LEN;
		memcpy(data_array[param_num].value, value, len);

		param_num++;
	}

	return param_num;
}

struct mmc *get_mmc_device(int dev, bool force_init)
{
	struct mmc *mmc;
	mmc = find_mmc_device(dev);
	if (!mmc) {
		printf("no mmc device at slot %x\n", dev);
		return NULL;
	}

	if (force_init)
		mmc->has_init = 0;
	if (mmc_init(mmc))
		return NULL;
	return mmc;
}

/*
 * cfg_eeprom_init - initialize FDT configuration struct
 * The EEPROM FDT is used if 1) the checksum is valid, 2) the system
 * is not in recovery mode, 3) validation_counter < AUTO_RECOVERY_RETRY_TIMES
 * Otherwise the default FDT is used.
 */
int cfg_eeprom_init(void)
{
	struct eeprom_struct eeprom_buffer;
	u32 calculate_checksum;
	bool found_emmc_part = false;
	struct mmc *mmc;

	if (1 == eeprom_initialized)
		return 0;
	for (g_mmc_index = 0; g_mmc_index < 2; g_mmc_index++) {
		mmc = get_mmc_device(g_mmc_index, false);
		if (mmc->version && IS_MMC(mmc)) {
			found_emmc_part = true;
			break;
		}
	}
	if (!found_emmc_part)
		return -1;

	// 1 blk = 512byte, read 2 blk
	blk_read_devnum(IF_TYPE_MMC, g_mmc_index, g_mmc_info_blk_addr,
			g_blk_length, g_temp_buffer);

	memcpy(&eeprom_buffer, g_temp_buffer, EEPROM_STRUCT_SIZE);
	printf("info: checksum: %x bootdev: %s\n", eeprom_buffer.checksum,
			eeprom_buffer.bootdev);
	/* check if pattern in EEPROM is invalid */
	if (eeprom_buffer.pattern != board_config_val.pattern) {
		printf("EEPROM configuration pattern not detected.\n");
		goto init_done;
	}

	/* calculate checksum */
	calculate_checksum = cfg_eeprom_checksum8((u8*)&eeprom_buffer.pattern,
						  EEPROM_STRUCT_SIZE - 4);
	if (calculate_checksum == eeprom_buffer.checksum) {
		/* update board_config_val struct with read from EEPROM */
		board_config_val = eeprom_buffer;
		printf("hw_info ok!\n");
	}

init_done:
	eeprom_initialized = 1;
	return 0;
}

