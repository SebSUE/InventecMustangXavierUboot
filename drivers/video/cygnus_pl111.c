/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <asm/io.h>
#include <lcd.h>
#include <cygnus_pl111.h>
#include "cygnus_asiu_pwm.h"
#include "cygnus_asiu_lcd.h"
#include <post.h>

#define CRMU_CHIP_IO_PAD_CONTROL    0x0301d0bc
#define LCD_BASE_ADDRESS            0x180a0000
#define LCD_CMAP_OFFSET             0x200

static struct clcd_config *lcd_config;
static struct clcd_registers *lcd_regs;

void lcd_board_specific_init(void)
{
	/* clock gate configuration to the LCD block */
	cygnus_asiu_lcd_clk_gate_ctrl();

	/* PLL power on configuration */
	cygnus_asiu_mipi_dsi_gen_pll_pwr_on_config();
}

/* Enable and power on the LCD controller */
void lcd_enable(void)
{
	uint32_t reg;

	reg = readl(&lcd_regs->cntl);

	reg |= LCD_CNTL_LCDEN;
	writel(reg, &lcd_regs->cntl);

	mdelay(20);

	reg |= LCD_CNTL_LCDPWR;
	writel(reg, &lcd_regs->cntl);

	/* Enable the LCD backlight after enable to minimize flicker */
	cygnus_asiu_pwm_bl_enable();
}

/* Disable and power off the LCD controller then disable clocks. */
void lcd_disable(void)
{
	uint32_t reg;

	reg = readl(&lcd_regs->cntl);

	/* Power off LCD and wait 20 ms. */
	if (reg & LCD_CNTL_LCDPWR) {
		reg &= ~LCD_CNTL_LCDPWR;
		writel(reg, &lcd_regs->cntl);

		mdelay(20);
	}

	/* Disable LCD. */
	if (reg & LCD_CNTL_LCDEN) {
		reg &= ~LCD_CNTL_LCDEN;
		writel(reg, &lcd_regs->cntl);
	}

	/* Turn off clocks. */
	cygnus_asiu_mipi_dsi_gen_pll_pwr_off_config();
}

/* LCD controller initialization */
void lcd_ctrl_init(void *lcdbase)
{
	lcd_config = (struct clcd_config *)panel_info.priv;
	lcd_regs = (struct clcd_registers *)lcd_config->address;

	lcd_board_specific_init();
	clcd_panel_init(&panel_info);

	/*
	 * ensure that LCD controller is disabled prior to
	 * configuring the LCD panel timing registers
	 */
	writel(lcd_config->cntl, &lcd_regs->cntl);

	/* configure the LCD panel timing registers */
	writel(lcd_config->tim0, &lcd_regs->tim0);
	writel(lcd_config->tim1, &lcd_regs->tim1);
	writel(lcd_config->tim2, &lcd_regs->tim2);
	writel(lcd_config->tim3, &lcd_regs->tim3);

	/* configure the frame buffer address */
	writel((uint32_t)lcdbase, &lcd_regs->lbase);
	writel((uint32_t)lcdbase, &lcd_regs->ubase);
}

/*
 * Disable and power off LCD, turn off clocks and backlight, then reset the
 * block to ensure it's not in an unexpected state when the kernel is loaded.
 */
int lcd_ctrl_deinit(void)
{
	lcd_disable();
	cygnus_asiu_pwm_bl_disable();
	cygnus_asiu_lcd_sw_reset();

	return 0;
}

/* Set color registers in the color map */
void lcd_setcolreg(ushort regno, ushort red, ushort green, ushort blue)
{
	ushort bgr555;
	uint32_t reg;
	uint32_t mask;
	uint32_t index;

	/* keep the five high order bits of each color */
	red   = (red >> 3) & 0x001f;
	green = (green >> 3) & 0x001f;
	blue  = (blue >> 3) & 0x001f;

	/* LCD controller palette entry is in bgr555 format */
	bgr555 = (red << 10) | (green << 5) | (blue);

	/*
	 * avoid non-aligned access by reading a 32-bit
	 * register then overlaying the 16-bit color entry
	 */

	index = ((regno + 2) / 2) - 1;
	reg = readl(&lcd_regs->palette[index]);

	mask = bgr555;

	if (regno & 0x1) {
		mask <<= 16;
		reg &= 0x0000FFFF;
	} else {
		reg &= 0xFFFF0000;
	}

	reg |= mask;

	writel(reg, &lcd_regs->palette[index]);
}

/* Palette programming */
uint32_t lcd_palette_programming(uint16_t *pal_array, uint32_t length)
{
	uint32_t i = 0, a_index = 0;

	debug("Palette base : 0x%X\n", readl(&lcd_regs->palette[0]));

	if (length > LCD_PALETTE_RAM_SIZE) {
		printf("lcd_palette_programming - length out of range\n");
		return LCD_FAIL;
	}
	debug("Length : 0x%X\n", length);
	for (i = 0; i < (length); i += 2)
		writel(pal_array[i] | (pal_array[i+1] << 16),
		       &lcd_regs->palette[a_index++]);

	debug("lcd_palette_programming - done\n");
	return LCD_PASS;
}

/* Get Peripheral ID */
uint32_t lcd_get_peripheral_id(void)
{
	return readl(&lcd_regs->perID0) |
		(readl(&lcd_regs->perID1) << CLCDPeriphID0_WIDTH) |
		(readl(&lcd_regs->perID2) << (CLCDPeriphID0_WIDTH +
						CLCDPeriphID1_WIDTH)) |
		(readl(&lcd_regs->perID3) << (CLCDPeriphID0_WIDTH +
			CLCDPeriphID1_WIDTH + CLCDPeriphID2_WIDTH));
}

/* Get PrimeCell ID */
uint32_t lcd_get_primecell_id(void)
{
	return readl(&lcd_regs->cellID0) |
		(readl(&lcd_regs->cellID1) << CLCDPCellID0_WIDTH) |
		(readl(&lcd_regs->cellID2) << (CLCDPCellID0_WIDTH +
					CLCDPCellID1_WIDTH)) |
		(readl(&lcd_regs->cellID3) << (CLCDPCellID0_WIDTH +
			CLCDPCellID1_WIDTH + CLCDPCellID2_WIDTH));
}

