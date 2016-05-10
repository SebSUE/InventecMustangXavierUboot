/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <asm/io.h>
#include <spi.h>
#include <lcd.h>
#include <cygnus_pl111.h>

#define CRMU_IOMUX_CTRL5_ADDR     0x0301d0dc
#define IOMUX_GPIO88_SEL          0x3
#define IOMUX_GPIO88_SEL_MASK     0x7
#define IOMUX_GPIO88_SEL_SHIFT    8
#define IOMUX_GPIO92_SEL          0x3
#define IOMUX_GPIO92_SEL_MASK     0x7
#define IOMUX_GPIO92_SEL_SHIFT    4

#define ASIU_GP_DATA_OUT_2_ADDR   0x180a5404
#define ASIU_GP_OUT_EN_2_ADDR     0x180a5408
#define GPIO88_SHIFT              0
#define GPIO92_SHIFT              4

#define LCD_PL022_SPI_BUS 1
#define LCD_SPI_CS 0
#define MAX_SPI_SPEED 500000

/*
 * Wintek LCD Panel WD-F6448X-EFLWc
 */
struct clcd_config lcd_config = {
	.address =  (struct clcd_registers *)LCD_BASE_ADDRESS,
	.tim0 = 0x8B3B459C,
	.tim1 = 0x210509DF,
	.tim2 = 0x027F0004,
	.tim3 = 0x00010003,
	.cntl = 0x00000126
};

/* Supports 24 bpp but driver will operate in 8 bpp mode */
vidinfo_t panel_info = {
	.vl_col  = 640,
	.vl_row  = 480,
	.vl_bpix = LCD_COLOR8,    /* Bits per pixel */
	.cmap = (ushort *)LCD_BASE_ADDRESS + LCD_CMAP_OFFSET, /* color map */
	.priv = &lcd_config /* driver-specific data */
};

static int spi_config_panel(void)
{
	struct spi_slave *slave;

	uint16_t dout[] = {
		0x0203, 0x06c2, 0x0a11, 0x0e8c,
		0x128b, 0x1622, 0x1a00, 0x1e30,
		0x2208, 0x2640, 0x2a88, 0x2e88,
		0x3220, 0x3620, 0x8203, 0x86c2,
		0x8a11, 0x8e8c, 0x928b, 0x9622,
		0x9a00, 0x9e30, 0xa208, 0xa640,
		0xaa88, 0xae88, 0xb220, 0xb620
	};

	slave = spi_setup_slave(LCD_PL022_SPI_BUS, LCD_SPI_CS,
				MAX_SPI_SPEED, SPI_MODE_0);

	if (!slave) {
		error("PL022 SPI controller init failed\n");
		return -1;
	}

	if (spi_set_wordlen(slave, 16)) {
		spi_free_slave(slave);
		return -1;
	}

	if (spi_claim_bus(slave)) {
		spi_free_slave(slave);
		return -1;
	}
	spi_xfer(slave, sizeof(dout) * 8, dout, NULL,
		 SPI_XFER_BEGIN | SPI_XFER_END);
	spi_release_bus(slave);

	spi_free_slave(slave);

	return 0;
}

int clcd_panel_init(vidinfo_t *panel_info)
{
	uint32_t val;

	val = readl(CRMU_IOMUX_CTRL5_ADDR);
	val &= ~((IOMUX_GPIO88_SEL_MASK << IOMUX_GPIO88_SEL_SHIFT) |
		(IOMUX_GPIO92_SEL_MASK << IOMUX_GPIO92_SEL_SHIFT));
	val |= (IOMUX_GPIO88_SEL << IOMUX_GPIO88_SEL_SHIFT) |
		(IOMUX_GPIO92_SEL << IOMUX_GPIO92_SEL_SHIFT);
	writel(val, CRMU_IOMUX_CTRL5_ADDR);

	val = readl(ASIU_GP_OUT_EN_2_ADDR);
	val |= (1 << GPIO88_SHIFT) | (1 << GPIO92_SHIFT);
	writel(val, ASIU_GP_OUT_EN_2_ADDR);

	/* Power on and reset panel */
	val = readl(ASIU_GP_DATA_OUT_2_ADDR);
	val |= (1 << GPIO88_SHIFT) | (1 << GPIO92_SHIFT);
	writel(val, ASIU_GP_DATA_OUT_2_ADDR);

	spi_config_panel();

	return 0;
}
