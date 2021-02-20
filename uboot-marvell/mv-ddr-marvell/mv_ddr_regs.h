/*******************************************************************************
Copyright (C) 2017 Marvell International Ltd.

This software file (the "File") is owned and distributed by Marvell
International Ltd. and/or its affiliates ("Marvell") under the following
alternative licensing terms.  Once you have made an election to distribute the
File under one of the following license alternatives, please (i) delete this
introductory statement regarding license alternatives, (ii) delete the three
license alternatives that you have not elected to use and (iii) preserve the
Marvell copyright notice above.

********************************************************************************
Marvell Commercial License Option

If you received this File from Marvell and you have entered into a commercial
license agreement (a "Commercial License") with Marvell, the File is licensed
to you under the terms of the applicable Commercial License.

********************************************************************************
Marvell GPL License Option

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 2 of the License, or any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

********************************************************************************
Marvell GNU General Public License FreeRTOS Exception

If you received this File from Marvell, you may opt to use, redistribute and/or
modify this File in accordance with the terms and conditions of the Lesser
General Public License Version 2.1 plus the following FreeRTOS exception.
An independent module is a module which is not derived from or based on
FreeRTOS.
Clause 1:
Linking FreeRTOS statically or dynamically with other modules is making a
combined work based on FreeRTOS. Thus, the terms and conditions of the GNU
General Public License cover the whole combination.
As a special exception, the copyright holder of FreeRTOS gives you permission
to link FreeRTOS with independent modules that communicate with FreeRTOS solely
through the FreeRTOS API interface, regardless of the license terms of these
independent modules, and to copy and distribute the resulting combined work
under terms of your choice, provided that:
1. Every copy of the combined work is accompanied by a written statement that
details to the recipient the version of FreeRTOS used and an offer by yourself
to provide the FreeRTOS source code (including any modifications you may have
made) should the recipient request it.
2. The combined work is not itself an RTOS, scheduler, kernel or related
product.
3. The independent modules add significant and primary functionality to
FreeRTOS and do not merely extend the existing functionality already present in
FreeRTOS.
Clause 2:
FreeRTOS may not be used for any competitive or comparative purpose, including
the publication of any form of run time or compile time metric, without the
express permission of Real Time Engineers Ltd. (this is the norm within the
industry and is intended to ensure information accuracy).

********************************************************************************
Marvell BSD License Option

If you received this File from Marvell, you may opt to use, redistribute and/or
modify this File under the following licensing terms.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.

	* Neither the name of Marvell nor the names of its contributors may be
	  used to endorse or promote products derived from this software without
	  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/

#ifndef _MV_DDR_REGS_H
#define _MV_DDR_REGS_H

#define GLOB_CTRL_STATUS_REG			0x1030
#define TRAINING_TRIGGER_OFFS			0
#define TRAINING_TRIGGER_MASK			0x1
#define TRAINING_TRIGGER_ENA			1
#define TRAINING_DONE_OFFS			1
#define TRAINING_DONE_MASK			0x1
#define TRAINING_DONE_DONE			1
#define TRAINING_DONE_NOT_DONE			0
#define TRAINING_RESULT_OFFS			2
#define TRAINING_RESULT_MASK			0x1
#define TRAINING_RESULT_PASS			0
#define TRAINING_RESULT_FAIL			1

#define GENERAL_TRAINING_OPCODE_REG		0x1034

#define OPCODE_REG0_BASE			0x1038
#define OPCODE_REG0_REG(obj)			(OPCODE_REG0_BASE + (obj) * 0x4)

#define OPCODE_REG1_BASE			0x10b0
#define OPCODE_REG1_REG(obj)			(OPCODE_REG1_BASE + (obj) * 0x4)

#define CAL_PHY_BASE				0x10c0
#define CAL_PHY_REG(obj)			(CAL_PHY_BASE + (obj) * 0x4)

#define WL_DONE_CNTR_REF_REG			0x10f8
#define ODPG_WR_RD_MODE_ENA_REG			0x10fc

#define SDRAM_CFG_REG				0x1400
#define REFRESH_OFFS				0
#define REFRESH_MASK				0x3fff
#define DRAM_TYPE_OFFS				14
#define DRAM_TYPE_MASK				0x1
#define BUS_IN_USE_OFFS				15
#define BUS_IN_USE_MASK				0x1
#define CPU_2DRAM_WR_BUFF_CUT_TH_OFFS		16
#define CPU_2DRAM_WR_BUFF_CUT_TH_MASK		0x1
#define REG_DIMM_OFFS				17
#define REG_DIMM_MASK				0x1
#define ECC_OFFS				18
#define ECC_MASK				0x1
#define IGNORE_ERRORS_OFFS			19
#define IGNORE_ERRORS_MASK			0x1
#define DRAM_TYPE_HIGH_OFFS			20
#define DRAM_TYPE_HIGH_MASK			0x1
#define SELF_REFRESH_MODE_OFFS			24
#define SELF_REFRESH_MODE_MASK			0x1
#define CPU_RD_PER_PROP_OFFS			25
#define CPU_RD_PER_PROP_MASK			0x1
#define DDR4_EMULATION_OFFS			26
#define DDR4_EMULATION_MASK			0x1
#define PHY_RF_RST_OFFS				27
#define PHY_RF_RST_MASK				0x1
#define PUP_RST_DIVIDER_OFFS			28
#define PUP_RST_DIVIDER_MASK			0x1
#define DATA_PUP_WR_RESET_OFFS			29
#define DATA_PUP_WR_RESET_MASK			0x1
#define DATA_PUP_RD_RESET_OFFS			30
#define DATA_PUP_RD_RESET_MASK			0x1
#define DATA_PUP_RD_RESET_ENA			0x0
#define DATA_PUP_RD_RESET_DIS			0x1
#define IO_BIST_OFFS				31
#define DATA_PUP_RD_RESET_MASK			0x1

#define DUNIT_CTRL_LOW_REG			0x1404

#define SDRAM_TIMING_LOW_REG			0x1408
#define SDRAM_TIMING_LOW_TRAS_OFFS		0
#define SDRAM_TIMING_LOW_TRAS_MASK		0xf
#define SDRAM_TIMING_LOW_TRCD_OFFS		4
#define SDRAM_TIMING_LOW_TRCD_MASK		0xf
#define SDRAM_TIMING_HIGH_TRCD_OFFS		22
#define SDRAM_TIMING_HIGH_TRCD_MASK		0x1
#define SDRAM_TIMING_LOW_TRP_OFFS		8
#define SDRAM_TIMING_LOW_TRP_MASK		0xf
#define SDRAM_TIMING_HIGH_TRP_OFFS		23
#define SDRAM_TIMING_HIGH_TRP_MASK		0x1
#define SDRAM_TIMING_LOW_TWR_OFFS		12
#define SDRAM_TIMING_LOW_TWR_MASK		0xf
#define SDRAM_TIMING_LOW_TWTR_OFFS		16
#define SDRAM_TIMING_LOW_TWTR_MASK		0xf
#define SDRAM_TIMING_LOW_TRAS_HIGH_OFFS		20
#define SDRAM_TIMING_LOW_TRAS_HIGH_MASK		0x3
#define SDRAM_TIMING_LOW_TRRD_OFFS		24
#define SDRAM_TIMING_LOW_TRRD_MASK		0xf
#define SDRAM_TIMING_LOW_TRTP_OFFS		28
#define SDRAM_TIMING_LOW_TRTP_MASK		0xf

#define SDRAM_TIMING_HIGH_REG			0x140c
#define SDRAM_TIMING_HIGH_TRFC_OFFS		0
#define SDRAM_TIMING_HIGH_TRFC_MASK		0x7f
#define SDRAM_TIMING_HIGH_TR2R_OFFS		7
#define SDRAM_TIMING_HIGH_TR2R_MASK		0x3
#define SDRAM_TIMING_HIGH_TR2W_W2R_OFFS		9
#define SDRAM_TIMING_HIGH_TR2W_W2R_MASK		0x3
#define SDRAM_TIMING_HIGH_TW2W_OFFS		11
#define SDRAM_TIMING_HIGH_TW2W_MASK		0x1f
#define SDRAM_TIMING_HIGH_TRFC_HIGH_OFFS	16
#define SDRAM_TIMING_HIGH_TRFC_HIGH_MASK	0x7
#define SDRAM_TIMING_HIGH_TR2R_HIGH_OFFS	19
#define SDRAM_TIMING_HIGH_TR2R_HIGH_MASK	0x7
#define SDRAM_TIMING_HIGH_TR2W_W2R_HIGH_OFFS	22
#define SDRAM_TIMING_HIGH_TR2W_W2R_HIGH_MASK	0x7
#define SDRAM_TIMING_HIGH_TMOD_OFFS		25
#define SDRAM_TIMING_HIGH_TMOD_MASK		0xf
#define SDRAM_TIMING_HIGH_TMOD_HIGH_OFFS	30
#define SDRAM_TIMING_HIGH_TMOD_HIGH_MASK	0x3

#define SDRAM_ADDR_CTRL_REG			0x1410
#define CS_STRUCT_BASE				0
#define CS_STRUCT_OFFS(cs)			(CS_STRUCT_BASE + (cs) * 4)
#define CS_STRUCT_MASK				0x3
#define CS_SIZE_BASE				2
#define CS_SIZE_OFFS(cs)			(CS_SIZE_BASE + (cs) * 4)
#define CS_SIZE_MASK				0x3
#define CS_SIZE_HIGH_BASE			20
#define CS_SIZE_HIGH_OFFS(cs)			(CS_SIZE_HIGH_BASE + (cs))
#define CS_SIZE_HIGH_MASK			0x1
#define T_FAW_OFFS				24
#define T_FAW_MASK				0x7f

#define SDRAM_OPEN_PAGES_CTRL_REG		0x1414

#define SDRAM_OP_REG				0x1418
#define SDRAM_OP_CMD_OFFS			0
#define SDRAM_OP_CMD_MASK			0x1f
#define SDRAM_OP_CMD_CS_BASE			8
#define SDRAM_OP_CMD_CS_OFFS(cs)		(SDRAM_OP_CMD_CS_BASE + (cs))
#define SDRAM_OP_CMD_CS_MASK			0x1
#define SDRAM_OP_CMD_ALL_CS_MASK		0xf
enum {
	CMD_NORMAL,
	CMD_PRECHARGE,
	CMD_REFRESH,
	CMD_DDR3_DDR4_MR0,
	CMD_DDR3_DDR4_MR1,
	CMD_NOP,
	CMD_RES_0X6,
	CMD_SELFREFRESH,
	CMD_DDR3_DDR4_MR2,
	CMD_DDR3_DDR4_MR3,
	CMD_ACT_PDE,
	CMD_PRE_PDE,
	CMD_ZQCL,
	CMD_ZQCS,
	CMD_CWA,
	CMD_RES_0XF,
	CMD_DDR4_MR4,
	CMD_DDR4_MR5,
	CMD_DDR4_MR6,
	DDR4_MPR_WR
};

#define DUNIT_CTRL_HIGH_REG			0x1424
#define CPU_INTERJECTION_ENA_OFFS		3
#define CPU_INTERJECTION_ENA_MASK		0x1
#define CPU_INTERJECTION_ENA_SPLIT_ENA		0
#define CPU_INTERJECTION_ENA_SPLIT_DIS		1

#define DDR_ODT_TIMING_LOW_REG			0x1428

#define DDR_TIMING_REG				0x142c
#define DDR_TIMING_TCCD_OFFS			18
#define DDR_TIMING_TCCD_MASK			0x7
#define DDR_TIMING_TPD_OFFS			0
#define DDR_TIMING_TPD_MASK			0xf
#define DDR_TIMING_TXPDLL_OFFS			4
#define DDR_TIMING_TXPDLL_MASK			0x1f

#define DDR_ODT_TIMING_HIGH_REG			0x147c

#define SDRAM_INIT_CTRL_REG			0x1480
#define DRAM_RESET_MASK_OFFS			1
#define DRAM_RESET_MASK_MASK			0x1
#define DRAM_RESET_MASK_NORMAL			0
#define DRAM_RESET_MASK_MASKED			1

#define SDRAM_ODT_CTRL_HIGH_REG			0x1498
#define DUNIT_ODT_CTRL_REG			0x149c
#define RD_BUFFER_SEL_REG			0x14a4
#define AXI_CTRL_REG				0x14a8
#define DUNIT_MMASK_REG				0x14b0

#define HORZ_SSTL_CAL_MACH_CTRL_REG		0x14c8
#define HORZ_POD_CAL_MACH_CTRL_REG		0x17c8
#define VERT_SSTL_CAL_MACH_CTRL_REG		0x1dc8
#define VERT_POD_CAL_MACH_CTRL_REG		0x1ec8

#define MAIN_PADS_CAL_MACH_CTRL_REG		0x14cc
#define DYN_PADS_CAL_ENABLE_OFFS		0
#define DYN_PADS_CAL_ENABLE_MASK		0x1
#define DYN_PADS_CAL_ENABLE_DIS			0
#define DYN_PADS_CAL_ENABLE_ENA			1
#define PADS_RECAL_OFFS				1
#define PADS_RECAL_MASK				0x1
#define DYN_PADS_CAL_BLOCK_OFFS			2
#define DYN_PADS_CAL_BLOCK_MASK			0x1
#define CAL_UPDATE_CTRL_OFFS			3
#define CAL_UPDATE_CTRL_MASK			0x3
#define CAL_UPDATE_CTRL_INT			1
#define CAL_UPDATE_CTRL_EXT			2
#define DYN_PADS_CAL_CNTR_OFFS			13
#define DYN_PADS_CAL_CNTR_MASK			0x3ffff
#define CAL_MACH_STATUS_OFFS			31
#define CAL_MACH_STATUS_MASK			0x1
#define CAL_MACH_BUSY				0
#define CAL_MACH_RDY				1

#define DRAM_DLL_TIMING_REG			0x14e0
#define DRAM_ZQ_INIT_TIMIMG_REG			0x14e4
#define DRAM_ZQ_TIMING_REG			0x14e8

#define DRAM_LONG_TIMING_REG			0x14ec
#define DDR4_TRRD_L_OFFS			0
#define DDR4_TRRD_L_MASK			0xf
#define DDR4_TWTR_L_OFFS			4
#define DDR4_TWTR_L_MASK			0xf

#define DDR_IO_REG				0x1524
#define DFS_REG					0x1528

#define RD_DATA_SMPL_DLYS_REG			0x1538
#define RD_SMPL_DLY_CS_BASE			0
#define RD_SMPL_DLY_CS_OFFS(cs)			(RD_SMPL_DLY_CS_BASE + (cs) * 8)
#define RD_SMPL_DLY_CS_MASK			0x1f

#define RD_DATA_RDY_DLYS_REG			0x153c
#define RD_RDY_DLY_CS_BASE			0
#define RD_RDY_DLY_CS_OFFS(cs)			(RD_RDY_DLY_CS_BASE + (cs) * 8)
#define RD_RDY_DLY_CS_MASK			0x1f

#define TRAINING_REG				0x15b0
#define TRN_START_OFFS				31
#define TRN_START_MASK				0x1
#define TRN_START_ENA				1
#define TRN_START_DIS				0

#define TRAINING_SW_1_REG			0x15b4

#define TRAINING_SW_2_REG			0x15b8
#define TRAINING_ECC_MUX_OFFS			1
#define TRAINING_ECC_MUX_MASK			0x1
#define TRAINING_ECC_MUX_DIS			0
#define TRAINING_ECC_MUX_ENA			1
#define TRAINING_SW_OVRD_OFFS			0
#define TRAINING_SW_OVRD_MASK			0x1
#define TRAINING_SW_OVRD_DIS			0
#define TRAINING_SW_OVRD_ENA			1

#define TRAINING_PATTERN_BASE_ADDR_REG		0x15bc
#define TRAINING_DBG_1_REG			0x15c0
#define TRAINING_DBG_2_REG			0x15c4

#define TRAINING_DBG_3_REG			0x15c8
#define TRN_DBG_RDY_INC_PH_2TO1_BASE		0
#define TRN_DBG_RDY_INC_PH_2TO1_OFFS(phase)	(TRN_DBG_RDY_INC_PH_2TO1_BASE + (phase) * 3)
#define TRN_DBG_RDY_INC_PH_2TO1_MASK		0x7

#define DDR3_RANK_CTRL_REG			0x15e0
#define CS_EXIST_BASE				0
#define CS_EXIST_OFFS(cs)			(CS_EXIST_BASE + (cs))
#define CS_EXIST_MASK				0x1

#define ZQC_CFG_REG				0x15e4
#define DRAM_PHY_CFG_REG			0x15ec
#define ODPG_CTRL_CTRL_REG			0x1600
#define ODPG_CTRL_AUTO_REFRESH_OFFS		21
#define ODPG_CTRL_AUTO_REFRESH_MASK		0x1
#define ODPG_CTRL_AUTO_REFRESH_DIS		1
#define ODPG_CTRL_AUTO_REFRESH_ENA		0

#define ODPG_DATA_CTRL_REG			0x1630
#define ODPG_WRBUF_WR_CTRL_OFFS			0
#define ODPG_WRBUF_WR_CTRL_MASK			0x1
#define ODPG_WRBUF_WR_CTRL_DIS			0
#define ODPG_WRBUF_WR_CTRL_ENA			1
#define ODPG_WRBUF_RD_CTRL_OFFS			1
#define ODPG_WRBUF_RD_CTRL_MASK			0x1
#define ODPG_WRBUF_RD_CTRL_DIS			0
#define ODPG_WRBUF_RD_CTRL_ENA			1
#define ODPG_DATA_CBDEL_OFFS			15
#define ODPG_DATA_CBDEL_MASK			0x3f
#define ODPG_MODE_OFFS				25
#define ODPG_MODE_MASK				0x1
#define ODPG_MODE_RX				0
#define ODPG_MODE_TX				1
#define ODPG_DATA_CS_OFFS			26
#define ODPG_DATA_CS_MASK			0x3
#define ODPG_DISABLE_OFFS			30
#define ODPG_DISABLE_MASK			0x1
#define ODPG_DISABLE_DIS			1
#define ODPG_ENABLE_OFFS			31
#define ODPG_ENABLE_MASK			0x1
#define ODPG_ENABLE_ENA				1

#define ODPG_DATA_BUFFER_OFFS_REG		0x1638
#define ODPG_DATA_BUFFER_SIZE_REG		0x163c
#define PHY_LOCK_STATUS_REG			0x1674

#define PHY_REG_FILE_ACCESS_REG			0x16a0
#define PRFA_DATA_OFFS				0
#define PRFA_DATA_MASK				0xffff
#define PRFA_REG_NUM_OFFS			16
#define PRFA_REG_NUM_MASK			0x3f
#define PRFA_PUP_NUM_OFFS			22
#define PRFA_PUP_NUM_MASK			0xf
#define PRFA_PUP_CTRL_DATA_OFFS			26
#define PRFA_PUP_CTRL_DATA_MASK			0x1
#define PRFA_PUP_BCAST_WR_ENA_OFFS		27
#define PRFA_PUP_BCAST_WR_ENA_MASK		0x1
#define PRFA_REG_NUM_HI_OFFS			28
#define PRFA_REG_NUM_HI_MASK			0x3
#define PRFA_TYPE_OFFS				30
#define PRFA_TYPE_MASK				0x1
#define PRFA_REQ_OFFS				31
#define PRFA_REQ_MASK				0x1
#define PRFA_REQ_DIS				0x0
#define PRFA_REQ_ENA				0x1

#define TRAINING_WL_REG				0x16ac

#define ODPG_DATA_WR_ADDR_REG			0x16b0
#define ODPG_DATA_WR_ACK_OFFS			0
#define ODPG_DATA_WR_ACK_MASK			0x7f
#define ODPG_DATA_WR_DATA_OFFS			8
#define ODPG_DATA_WR_DATA_MASK			0xff

#define ODPG_DATA_WR_DATA_HIGH_REG		0x16b4
#define ODPG_DATA_WR_DATA_LOW_REG		0x16b8
#define ODPG_DATA_RX_WORD_ERR_ADDR_REG		0x16bc
#define ODPG_DATA_RX_WORD_ERR_CNTR_REG		0x16c0
#define ODPG_DATA_RX_WORD_ERR_DATA_HIGH_REG	0x16c4
#define ODPG_DATA_RX_WORD_ERR_DATA_LOW_REG	0x16c8
#define ODPG_DATA_WR_DATA_ERR_REG		0x16cc

#define DUAL_DUNIT_CFG_REG			0x16d8
#define FC_SAMPLE_STAGES_OFFS			0
#define FC_SAMPLE_STAGES_MASK			0x7
#define SINGLE_CS_PIN_OFFS			3
#define SINGLE_CS_PIN_MASK			0x1
#define SINGLE_CS_ENA				1
#define TUNING_ACTIVE_SEL_OFFS			6
#define TUNING_ACTIVE_SEL_MASK			0x1
#define TUNING_ACTIVE_SEL_MC			0
#define TUNING_ACTIVE_SEL_TIP			1

#define WL_DQS_PATTERN_REG			0x16dc
#define ODPG_DONE_STATUS_REG			0x16fc
#define ODPG_DONE_STATUS_BIT_OFFS		0
#define ODPG_DONE_STATUS_BIT_MASK		0x1
#define ODPG_DONE_STATUS_BIT_CLR		0
#define ODPG_DONE_STATUS_BIT_SET		1

#define RESULT_CTRL_BASE			0x1830
#define BLOCK_STATUS_OFFS			25
#define BLOCK_STATUS_MASK			0x1
#define BLOCK_STATUS_LOCK			1
#define BLOCK_STATUS_NOT_LOCKED			0

#define MR0_REG					0x15d0
#define MR1_REG					0x15d4
#define MR2_REG					0x15d8
#define MR3_REG					0x15dc
#define MRS0_CMD				0x3
#define MRS1_CMD				0x4
#define MRS2_CMD				0x8
#define MRS3_CMD				0x9

#if defined(CONFIG_DDR4)
/* DDR4 MRS */
#define MRS4_CMD				0x10
#define MRS5_CMD				0x11
#define MRS6_CMD				0x12

/* DDR4 Registers */
#define DDR4_MR0_REG				0x1900
#define DDR4_MR1_REG				0x1904
#define DDR4_MR2_REG				0x1908
#define DDR4_MR3_REG				0x190c
#define DDR4_MPR_PS_OFFS			0
#define DDR4_MPR_PS_MASK			0x3
enum mv_ddr_mpr_ps { /* DDR4 MPR Page Selection */
	DDR4_MPR_PAGE0,
	DDR4_MPR_PAGE1,
	DDR4_MPR_PAGE2,
	DDR4_MPR_PAGE3
};
#define DDR4_MPR_OP_OFFS			2
#define DDR4_MPR_OP_MASK			0x1
enum mv_ddr_mpr_op { /* DDR4 MPR Operation */
	DDR4_MPR_OP_DIS, /* normal operation */
	DDR4_MPR_OP_ENA  /* dataflow from mpr */
};
#define DDR4_MPR_RF_OFFS			11
#define DDR4_MPR_RF_MASK			0x3
enum mv_ddr_mpr_rd_frmt { /* DDR4 MPR Read Format */
	DDR4_MPR_RF_SERIAL,
	DDR4_MPR_RF_PARALLEL,
	DDR4_MPR_RF_STAGGERED,
	DDR4_MPR_RF_RSVD_TEMP

};

#define DDR4_MR4_REG				0x1910
#define DDR4_RPT_OFFS				10
#define DDR4_RPT_MASK				0x1
enum { /* read preamble training mode */
	DDR4_RPT_DIS,
	DDR4_RPT_ENA
};

#define DDR4_MR5_REG				0x1914
#define DDR4_MR6_REG				0x1918
#define DDR4_MPR_WR_REG				0x19d0
#define DDR4_MPR_LOC_OFFS			8
#define DDR4_MPR_LOC_MASK			0x3
/*
 * MPR Location for MPR write and read accesses
 * MPR Location 0..3 within the selected page (page selection in MR3 [1:0] bits)
 */
enum {
	DDR4_MPR_LOC0,
	DDR4_MPR_LOC1,
	DDR4_MPR_LOC2,
	DDR4_MPR_LOC3
};
#endif /* CONFIG_DDR4 */

#define DRAM_PINS_MUX_REG			0x19d4
#define CTRL_PINS_MUX_OFFS			0
#define CTRL_PINS_MUX_MASK			0x3
enum {
	DUNIT_DDR3_ON_BOARD,
	DUNIT_DDR3_DIMM,
	DUNIT_DDR4_ON_BOARD,
	DUNIT_DDR4_DIMM
};

/* ddr phy registers */
#define WL_PHY_BASE				0x0
#define WL_PHY_REG(cs)				(WL_PHY_BASE + (cs) * 0x4)
#define WR_LVL_PH_SEL_OFFS			6
#define WR_LVL_PH_SEL_MASK			0x7
#define WR_LVL_PH_SEL_PHASE1			1
#define WR_LVL_REF_DLY_OFFS			0
#define WR_LVL_REF_DLY_MASK			0x1f
#define CTRL_CENTER_DLY_OFFS			10
#define CTRL_CENTER_DLY_MASK			0x1f
#define CTRL_CENTER_DLY_INV_OFFS		15
#define CTRL_CENTER_DLY_INV_MASK		0x1

#define CTX_PHY_BASE				0x1
#define CTX_PHY_REG(cs)				(CTX_PHY_BASE + (cs) * 0x4)

#define RL_PHY_BASE				0x2
#define RL_PHY_REG(cs)				(RL_PHY_BASE + (cs) * 0x4)
#define RL_REF_DLY_OFFS				0
#define RL_REF_DLY_MASK				0x1f
#define RL_PH_SEL_OFFS				6
#define RL_PH_SEL_MASK				0x7

#define CRX_PHY_BASE				0x3
#define CRX_PHY_REG(cs)				(CRX_PHY_BASE + (cs) * 0x4)

#define PHY_CTRL_PHY_REG			0x90
#define INV_PAD0_OFFS				2
#define INV_PAD1_OFFS				3
#define INV_PAD2_OFFS				4
#define INV_PAD3_OFFS				5
#define INV_PAD4_OFFS				6
#define INV_PAD5_OFFS				7
#define INV_PAD6_OFFS				8
#define INV_PAD7_OFFS				9
#define INV_PAD8_OFFS				10
#define INV_PAD9_OFFS				11
#define INV_PAD10_OFFS				12
#define INV_PAD_MASK				0x1
#define INVERT_PAD				1

#define ADLL_CFG0_PHY_REG			0x92
#define ADLL_CFG1_PHY_REG			0x93
#define ADLL_CFG2_PHY_REG			0x94
#define CMOS_CONFIG_PHY_REG			0xa2
#define PAD_ZRI_CAL_PHY_REG			0xa4
#define PAD_ODT_CAL_PHY_REG			0xa6
#define PAD_CFG_PHY_REG				0xa8
#define PAD_PRE_DISABLE_PHY_REG			0xa9
#define TEST_ADLL_PHY_REG			0xbf

#define VREF_PHY_BASE				0xd0
#define VREF_PHY_REG(cs, bit)			(VREF_PHY_BASE + (cs) * 12 + bit)
enum {
	DQSP_PAD = 4,
	DQSN_PAD
};

#define VREF_BCAST_PHY_BASE			0xdb
#define VREF_BCAST_PHY_REG(cs)			(VREF_BCAST_PHY_BASE + (cs) * 12)

#define PBS_TX_PHY_BASE				0x10
#define PBS_TX_PHY_REG(cs, bit)			(PBS_TX_PHY_BASE + (cs) * 0x10 + (bit))

#define PBS_TX_BCAST_PHY_BASE			0x1f
#define PBS_TX_BCAST_PHY_REG(cs)		(PBS_TX_BCAST_PHY_BASE + (cs) * 0x10)

#define PBS_RX_PHY_BASE				0x50
#define PBS_RX_PHY_REG(cs, bit)			(PBS_RX_PHY_BASE + (cs) * 0x10 + (bit))

#define PBS_RX_BCAST_PHY_BASE			0x5f
#define PBS_RX_BCAST_PHY_REG(cs)		(PBS_RX_BCAST_PHY_BASE + (cs) * 0x10)

#define RESULT_PHY_REG				0xc0
#define RESULT_PHY_RX_OFFS			5
#define RESULT_PHY_TX_OFFS			0


#endif /* _MV_DDR_REGS_H */
