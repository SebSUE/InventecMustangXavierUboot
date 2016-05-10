/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <asm/io.h>
#include "cygnus_asiu_lcd.h"



/* LCD AXI sideband signal control */
void cygnus_asiu_lcd_axi_sb_ctrl(void)
{
	uint32_t reg_data;

	reg_data = readl(ASIU_TOP_LCD_AXI_SB_CTRL);
	reg_data &= 0xfeffffff;
	writel(reg_data, ASIU_TOP_LCD_AXI_SB_CTRL);
}

/* LCD ASIU LCD clock gate control */
void cygnus_asiu_lcd_clk_gate_ctrl(void)
{
	uint32_t reg_data;

	/* enable LCD clock gate */
	reg_data = readl(LCD_CLK_GATING_CTRL);

	reg_data |= (1 << LCD_CLK_GATE_EN);
	reg_data |= (1 << DSI_CLK_GATE_EN);

	writel(reg_data, LCD_CLK_GATING_CTRL);
}

/* LCD ASIU MIPI PLL power on configuration */
void cygnus_asiu_mipi_dsi_gen_pll_pwr_on_config(void)
{
	uint32_t pwr_on;

	pwr_on = readl(LCD_PLL_PWR_CONFIG);
	pwr_on |= (1 << LCD_PWRON);
	pwr_on |= (1 << LCD_PWRON_PLL);
	pwr_on |= (1 << LCD_PWRON_BG);
	pwr_on |= (1 << LCD_PWRON_LDO);
	pwr_on &= (~(1 << LCD_ISO_IN));

	writel(pwr_on, LCD_PLL_PWR_CONFIG);
}

/* LCD ASIU MIPI PLL power off configuration */
void cygnus_asiu_mipi_dsi_gen_pll_pwr_off_config(void)
{
	uint32_t pwr_off;

	pwr_off = readl(LCD_PLL_PWR_CONFIG);

	pwr_off &= ~(1 << LCD_PWRON);
	pwr_off &= ~(1 << LCD_PWRON_PLL);
	pwr_off &= ~(1 << LCD_PWRON_BG);
	pwr_off &= ~(1 << LCD_PWRON_LDO);
	pwr_off |= (1 << LCD_ISO_IN);

	writel(pwr_off, LCD_PLL_PWR_CONFIG);
}

/* LCD ASIU LCD controller soft reset */
void cygnus_asiu_lcd_sw_reset(void)
{
	uint32_t sw_rst;

	sw_rst = readl(ASIU_TOP_SW_RESET_CTRL);
	sw_rst &= ~(1 << ASIU_TOP_SW_RESET_CTRL__LCD_SW_RESET_N);
	writel(sw_rst, ASIU_TOP_SW_RESET_CTRL);

	udelay(100);

	sw_rst |= (1 << ASIU_TOP_SW_RESET_CTRL__LCD_SW_RESET_N);
	writel(sw_rst, ASIU_TOP_SW_RESET_CTRL);
}
