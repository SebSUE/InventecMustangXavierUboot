/*
 * Copyright (C) 2015 Broadcom Corporation
 * Copyright (C) 2016 StreamUnlimited
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/io.h>
#include <linux/compat.h>
#include <bcmiproc_bbl.h>

/*
 * High and Low temperature set for
 * TMON tamper detection
 * ADC 0x2A7 = 85C
 * ADC 0x36B = -10C
 */
#define TEMPERATURE_HIGH		0x2A7
#define TEMPERATURE_LOW			0x36B
/*
 * To enable-disbale all tamper sources,
 * int SRC and SRC1 registers
 */
#define ALL_SRC				0xffffffff
#define ALL_SRC1			0x3f
/* Bit to reset mesh logic */
#define EMESH_CONFIG			BIT(25)
/* Bit to clear emesh fault detector logic */
#define EMESH_CLR			BIT(10)
/*
 * n[8:1] are outputs, p[8:1] are inputs,
 * and dynamic mode enabled.
 */
#define EMESH_CONFIG_ENABLE_DYMMODE	0x180ffff
/*
 * Enable pull up on Emesh output
 * and select emesh filter period
 */
#define EMESH_CONFIG1			0x3e8
#define EMESH_CONFIG1_FILTER		(0x14<<2)
/* Enable internal mesh and dynamic mode */
#define MESH_CONFIG_TAPS_FAULT		0xE0
/* Enable Glitch filter on emesh pins */
#define EMESH_GLITCH_FILTER		(0x1fe << 18)
/*
 * Tamper input enable for digital
 * tamper input pairs
 */
#define TAMPERIN_INP_EN			0x1
/* Glitch filter for PN tamper */
#define GLITCH_FILTER_EN		BIT(18)
/* Power down fmon, do auto calibration */
#define FMON_CNG_AUTOCALIB		0x02000c30
/* Calibration data bit start */
#define FMON_CAL_REG			0x10
/* FMON calibration stat */
#define FMON_CALIB_STAT			0xA
/* To power on-off fmon */
#define FMON_CNG1_POWER_ON		0xA4

/* Tamper sources in SRC and SRC1 */
#define TAMPER_N0			BIT(0)
#define TAMPER_N1			BIT(1)
#define TAMPER_P0			BIT(9)
#define TAMPER_P1			BIT(10)
#define TAMPER_EMESH_F			BIT(18)
#define TAMPER_EMESH_O			BIT(19)
#define TAMPER_IMESH			BIT(20)
#define TAMPER_TMON_LOW			BIT(21)
#define TAMPER_TMON_HIGH		BIT(22)
#define TAMPER_FMON_LOW			BIT(23)
#define TAMPER_FMON_HIGH		BIT(24)
#define TAMPER_VOL			BIT(25)
#define TAMPER_1_VOL			BIT(2)
#define TAMPER_COIN_CELL		BIT(3)

/* BBL SPRU Write/Read */
#define BBL_RAW_TAMPER_INTR_EN		BIT(8)
#define BBL_TWARN_INTR_EN		BIT(7)
#define BBL_TAMPERN_INTR_EN		BIT(6)
#define BBL_TAMPERIN_P_INTR_EN0		BIT(5)
#define BBL_TAMPERIN_P_INTR_EN1		BIT(4)
#define BBL_TAMPERIN_N_INTR_EN0		BIT(3)
#define BBL_TAMPERIN_N_INTR_EN1		BIT(2)

#define BBL_REG_TAMPER_INTR    (BBL_RAW_TAMPER_INTR_EN |\
		BBL_TAMPERN_INTR_EN |\
		BBL_TAMPERIN_P_INTR_EN0 |\
		BBL_TAMPERIN_P_INTR_EN1 |\
		BBL_TAMPERIN_N_INTR_EN0 |\
		BBL_TAMPERIN_N_INTR_EN1)

#define IREG_BBL_RTC_PER		0x00000000
#define IREG_BBL_RTC_MATCH		0x00000004
#define IREG_BBL_RTC_DIV		0x00000008
#define IREG_BBL_RTC_SECOND		0x0000000C
#define IREG_BBL_INTERRUPT_EN		0x00000010
#define IREG_BBL_INTERRUPT_STAT		0x00000014
#define IREG_BBL_INTERRUPT_CLR		0x00000018
#define IREG_BBL_CONTROL		0x0000001C

/* Period supported by BBL */
#define BBL_PER_125ms			0x00000001
#define BBL_PER_250ms			0x00000002
#define BBL_PER_500ms			0x00000004
#define BBL_PER_1s			0x00000008
#define BBL_PER_2s			0x00000010
#define BBL_PER_4s			0x00000020
#define BBL_PER_8s			0x00000040
#define BBL_PER_16s			0x00000080
#define BBL_PER_32s			0x00000100
#define BBL_PER_64s			0x00000200
#define BBL_PER_128s			0x00000400
#define BBL_PER_256s			0x00000800

#define RTC_REG_ACC_DONE		BIT(0)
#define RTC_REG_RTC_STOP		BIT(0)
#define RTC_REG_PERIO_INTR		BIT(0)
#define RTC_REG_ALARM_INTR		BIT(1)
#define RTC_IND_SOFT_RST_N		BIT(10)
#define RTC_REG_WR_CMD			BIT(11)
#define RTC_REG_RD_CMD			BIT(12)

/* used */

/*
 * Fmon wait time for calibrationa and
 * power on
 */
#define FMON_WAIT			50
/* Bit to reset FMON */
#define FMON_CNG1_OUT_RESET		(1 << 3)
/* Bit to power in tmon */
#define TMON_CNG_PWRDN			(1 << 1)
/* Offset for high and low temperature setting */
#define TMON_DELTA_LIMIT		0x1
#define TMON_MAX_LIMIT			0xB
/* Pad config bits for PN tamper */
#define PAD_CONFIG_UNATT_MODE		(1 << 25)
#define PAD_CONFIG_IND			(1 << 24)
/* Tmaper and interrupt bits for P0N0 */
#define TAMPER_SRC_P0N0			(1 << 0)
#define INTR_P0N0			(1 << 3)
/* EMESH clear delay */
#define EMESH_CLR_UDELAY		10
/* Reset cycle delay */
#define RESET_CYCLE_UDELAY		200
/* RTC Seconds shift right */
#define RTC_SEC_SHIFT_RT		7

/* Mask */
#define FMON_CALIB_MASK			0xff
#define TMON_TEMP_MASK			0xfffffffc
#define SPRU_BBL_REG_MASK		0x3FFF
#define RTC_SEC_MASK			0x3FFFFF
#define RTC_TM_TOSEC_MASK		0xFFFF
#define ADC_VAL				0x3ff

/* BBL SPRU Write/Read */
#define BBL_REG_ACC_DONE		(1 << 0)
#define BBL_IND_SOFT_RST_N		(1 << 10)
#define BBL_REG_WR_CMD			(1 << 11)
#define BBL_REG_RD_CMD			(1 << 12)

#define CRMU_ISO_PDBBL			(1 << 16)
#define CRMU_ISO_PDBBL_TAMPER		(1 << 24)

#define CRMU_ISO_CELL_CONTROL		0x0301c038

#define CRMU_AUTH_CODE_PWD		0x12345678
#define CRMU_AUTH_CODE_PWD_RST		0x99999999
#define CRMU_AUTH_CODE_PWD_CLR		0x0

#define CRMU_BBL_AUTH_CODE		0x03024c74
#define CRMU_BBL_AUTH_CHECK		0x03024c78
#define CRMU_SPRU_SOURCE_SEL_STAT	0x0301c040

#define SPRU_BBL_WDATA			0x03026000
#define SPRU_BBL_CMD			0x03026004
#define SPRU_BBL_STATUS			0x03026008
#define SPRU_BBL_RDATA			0x0302600c


/*
 * Timeout when waiting on register
 * reads or writes
 */
#define REG_TIMEOUT_MICROSECONDS	250

/*
 * SPRU Source Select status
 * 0 - SPRU is powered by AON power
 * 1 - SPRU is powerd by battery
 */
#define CRMU_SPRU_SOURCE_SEL_AON	0

/* SEC1280 Bank Register defines */
#define BBL_SEC0_MEM			0x200
#define BBL_SEC39_MEM			0x29C

/* Time to wait for BBL done status */
#define BBL_STATUS_TIMEOUT		1000


static int wait_acc_done(void)
{
	u32 reg_val;
	int timeout = BBL_STATUS_TIMEOUT;

	reg_val = readl(SPRU_BBL_STATUS);
	while (!(reg_val & BBL_REG_ACC_DONE)) {
		if (--timeout == 0) {
			return -EIO;
		}
		udelay(1);
		reg_val = readl(SPRU_BBL_STATUS);
	}
	return 0;
}

int bbl_reg_write(u32 reg_addr, u32 value)
{
	int ret;
	u32 cmd;

	writel(value, SPRU_BBL_WDATA);
	cmd = (reg_addr & SPRU_BBL_REG_MASK) | BBL_REG_WR_CMD | BBL_IND_SOFT_RST_N; /* Write command */
	writel(cmd, SPRU_BBL_CMD);
	ret = wait_acc_done();
	if (ret < 0)
		printf("BBL: reg write to 0x%x failed!\n", reg_addr);

	return ret;
}

int bbl_reg_read(u32 reg_addr, u32 *data)
{
	int ret;
	u32 cmd;

	cmd = (reg_addr & SPRU_BBL_REG_MASK) | BBL_REG_RD_CMD | BBL_IND_SOFT_RST_N; /* Read command */
	writel(cmd, SPRU_BBL_CMD);
	ret = wait_acc_done();
	if (ret < 0)
		printf("BBL: reg read to 0x%x failed!\n", reg_addr);
	else
		*data = readl(SPRU_BBL_RDATA);
	return ret;
}

int bbl_init(void)
{
	int ret;
	u32 reg_val, data;
	int timeout = REG_TIMEOUT_MICROSECONDS;

	printf("RTC: BBL init\n");

	/*
	 * Check SPRU Source Select status
	 * 0 - SPRU is powered by AON power
	 * 1 - SPRU is powerd by battery
	 */
	reg_val = readl(CRMU_SPRU_SOURCE_SEL_STAT);
	while (reg_val != CRMU_SPRU_SOURCE_SEL_AON) {
		if (--timeout == 0) {
			printf("RTC: BBL AON power not available\n");
			return -ENODEV;
		}
		udelay(1);
		reg_val = readl(CRMU_SPRU_SOURCE_SEL_STAT);
	}

	/* Wait for reset cycle */
	writel(0, SPRU_BBL_CMD);
	udelay(RESET_CYCLE_UDELAY);
	writel(BBL_IND_SOFT_RST_N, SPRU_BBL_CMD);

	/* remove BBL related isolation from CRMU */
	reg_val = readl(CRMU_ISO_CELL_CONTROL);
	reg_val &= ~(CRMU_ISO_PDBBL | CRMU_ISO_PDBBL_TAMPER);
	writel(reg_val, CRMU_ISO_CELL_CONTROL);

	/* program CRMU auth_code resister */
	writel(CRMU_AUTH_CODE_PWD, CRMU_BBL_AUTH_CODE);
	/*
	 * program CRMU auth_code_check register
	 * auth_code must equal to auth_code_check
	 */
	writel(CRMU_AUTH_CODE_PWD, CRMU_BBL_AUTH_CHECK);
	/*
	 * Need to set ind_unattended_mode (bit (25)
	 * and ind (bit 24) of PAD_CONFIG to zero
	 * in Cygnus B0, as the POR value is kept "1" to reduce
	 * the power consumption.
	 */
	ret = bbl_reg_read(REG_PAD_CONFIG, &data);
	if (ret < 0)
		goto err;
	ret = bbl_reg_write(REG_PAD_CONFIG, data & ~(PAD_CONFIG_UNATT_MODE | PAD_CONFIG_IND));
	if (ret < 0)
		goto err;
	/*
	 * Clearing the tamper stat and
	 * interrupt stat.
	 * To clear the bit, need to write a '1'
	 * to register and then clear the same.
	 */
	ret = bbl_reg_read(REG_BBL_TAMPER_SRC_CLEAR, &data);
	if (ret < 0)
		goto err;
	ret = bbl_reg_write(REG_BBL_TAMPER_SRC_CLEAR,
		data | TAMPER_SRC_P0N0);
	if (ret < 0)
		goto err;
	ret = bbl_reg_write(REG_BBL_TAMPER_SRC_CLEAR,
		data & ~(TAMPER_SRC_P0N0));
	if (ret < 0)
		goto err;
	ret = bbl_reg_write(REG_BBL_INTERRUPT_CLR, INTR_P0N0);
	if (ret < 0)
		goto err;
	ret = bbl_reg_write(REG_BBL_INTERRUPT_CLR, ~(INTR_P0N0));
	if (ret < 0)
		goto err;

	ret = bbl_reg_read(REG_BBL_TAMPER_SRC_STAT, &data);
	if (ret < 0)
		goto err;
	printf("BBL_TAMPER_SRC_STAT=%x\n", data);
	ret = bbl_reg_read(REG_BBL_INTERRUPT_STAT, &data);
	if (ret < 0)
		goto err;
	printf("BBL_INTERRUPT_STAT=%x\n", data);

err:
	return ret;
}
