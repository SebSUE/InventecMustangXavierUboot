/*
 * Copyright (C) 2015 Broadcom Corporation
 * Copyright (C) 2016 StreamUnlimited
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BCMIPROC_BBL_H_
#define __BCMIPROC_BBL_H_

/* BBL register offset */
#define REG_BBL_RTC_PER			0x00
#define REG_BBL_RTC_MATCH		0x04
#define REG_BBL_RTC_DIV			0x08
#define REG_BBL_RTC_SECOND		0x0C
#define REG_BBL_INTERRUPT_EN		0x10
#define REG_BBL_INTERRUPT_STAT		0x14
#define REG_BBL_INTERRUPT_CLR		0x18
#define REG_BBL_CONTROL			0x1C
#define REG_BBL_TAMPER_TIMESTAMP	0x20
#define REG_BBL_TAMPER_SRC_ENABLE	0x24
#define REG_BBL_TAMPER_SRC_STAT		0x28
#define REG_BBL_TAMPER_SRC_CLEAR	0x2C
#define REG_BBL_TAMPER_SRC_ENABLE_1	0x30
#define REG_BBL_TAMPER_SRC_STAT_1	0x34
#define REG_BBL_TAMPER_SRC_CLEAR_1	0x38
#define REG_BBL_GLITCH_CFG		0x3C
#define REG_BBL_EN_TAMPERIN		0x40
#define REG_BBL_EMESH_CONFIG		0x44
#define REG_BBL_EMESH_CONFIG_1		0x48
#define REG_BBL_MESH_CONFIG		0x4C
#define REG_BBL_LFSR_SEED		0x50
#define REG_BBL_INPUT_STATUS		0x54
#define REG_BBL_CONFIG			0x58
#define REG_BBL_CONFIG_1		0x5C
#define REG_BBL_TMON_CONFIG		0x60
#define REG_BBL_TMON_CONFIG_1		0x64
#define REG_BBL_STAT			0x68
#define REG_FMON_CNG			0x6C
#define REG_FMON_CNG_1			0x70
#define REG_FMON_CNG_2			0x74
#define REG_FMON_CNG_3			0x78
#define REG_FMON_CNT_VAL		0x7C
#define REG_PAD_CONFIG			0x80
#define REG_TAMPER_INPUT_PULL_UP	0x84
#define REG_PAD_PULL_HYST		0x88
#define REG_DBG_CONFIG			0x8C
#define REG_XTAL_CONFIG			0x90
#define REG_LDO_CONFIG1			0x94
#define REG_LDO_CONFIG2			0x98
#define REG_LDO_CONFIG3			0x9C
#define REG_LDO_STATUS			0xA0
#define REG_TAMPER_INP_TIMEBASE		0xA4
#define REG_FILTER_THREHOLD_CONFIG1	0xA8
#define REG_FILTER_THREHOLD_CONFIG2	0xAC
#define REG_BBL_RSVD			0xB0
#define REG_BBL_WR_BLOCK		0x1CC

/* SEC1280 Bank Register defines */
#define BBL_SEC0_MEM			0x200
#define BBL_SEC39_MEM			0x29C

int bbl_reg_write(u32 reg_addr, u32 value);
u32 bbl_reg_read(u32 reg_addr, u32 *data);
int bbl_init(void);

#endif /* __BCMIPROC_BBL_H_ */
