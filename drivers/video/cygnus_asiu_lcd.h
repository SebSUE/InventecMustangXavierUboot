/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#ifndef _CYGNUS_ASIU_LCD_H
#define _CYGNUS_ASIU_LCD_H

#define ASIU_TOP_LCD_AXI_SB_CTRL                        0x180aa02c

#define ASIU_TOP_SW_RESET_CTRL                          0x180aa020
#define ASIU_TOP_SW_RESET_CTRL__LCD_SW_RESET_N          4

/* PLL Configure register for MIPI-DSI and LCD clock */
#define LCD_PLL_PWR_CONFIG    0x0301c020
#define LCD_PWRON             20
#define LCD_PWRON_PLL         19
#define LCD_PWRON_BG          18
#define LCD_PWRON_LDO         17
#define LCD_ISO_IN            16

#define LCD_CLK_GATING_CTRL   0x180aa024

/* Bit definitions for control register */
#define LCD_CNTL_LCDEN		(1 << 0)
#define LCD_CNTL_BPP1		(0 << 1)
#define LCD_CNTL_BPP2		(1 << 1)
#define LCD_CNTL_BPP4		(2 << 1)
#define LCD_CNTL_BPP8		(3 << 1)
#define LCD_CNTL_BPP16		(4 << 1) /*1555*/
#define LCD_CNTL_BPP24		(5 << 1)
#define LCD_CNTL_BPP16_565	(6 << 1)
#define LCD_CNTL_BPP12_444	(7 << 1)

#define LCD_CNTL_LCDBW			(1 << 4)
#define LCD_CNTL_LCDTFT			(1 << 5)
#define LCD_CNTL_LCDMONO8		(1 << 6)
#define LCD_CNTL_LCDDUAL		(1 << 7)
#define LCD_CNTL_BGR			(1 << 8)
#define LCD_CNTL_BEBO			(1 << 9)
#define LCD_CNTL_BEPO			(1 << 10)
#define LCD_CNTL_LCDPWR			(1 << 11)
#define LCD_CNTL_LCDVCOMP(x)	((x) << 12)
#define LCD_CNTL_LDMAFIFOTIME	(1 << 15)
#define LCD_CNTL_WATERMARK		(1 << 16)

#define CLCDPeriphID0_WIDTH 8
#define CLCDPeriphID1_WIDTH 8
#define CLCDPeriphID2_WIDTH 8
#define CLCDPCellID0_WIDTH 8
#define CLCDPCellID1_WIDTH 8
#define CLCDPCellID2_WIDTH 8
#define ASIU_TOP_CLK_GATING_CTRL__LCD_CLK_GATE_EN 4
#define ASIU_TOP_CLK_GATING_CTRL__MIPI_DSI_CLK_GATE_EN 3

#define LCD_FAIL	0x0000ffff
#define LCD_PASS	0xffff0000

/* LCD Cursor Image length 32 * 256*/
#define LCD_CUR_IMG_RAM_SIZE 0x100 /*256 x 32 bits*/

/* LCD palette RAM size 32 * 128*/
#define LCD_PALETTE_RAM_SIZE 0x100 /*256 x 16 bit*/

/* Clock gating register define*/
#define LCD_CLK_GATE_EN		ASIU_TOP_CLK_GATING_CTRL__LCD_CLK_GATE_EN
#define DSI_CLK_GATE_EN		ASIU_TOP_CLK_GATING_CTRL__MIPI_DSI_CLK_GATE_EN

void cygnus_asiu_mipi_dsi_gen_pll_pwr_on_config(void);
void cygnus_asiu_mipi_dsi_gen_pll_pwr_off_config(void);
void cygnus_asiu_lcd_clk_gate_ctrl(void);
void cygnus_asiu_lcd_axi_sb_ctrl(void);
void cygnus_asiu_lcd_sw_reset(void);

/* Get Peripheral ID */
uint32_t lcd_get_peripheral_id(void);
/* Get PrimeCell ID */
uint32_t lcd_get_primecell_id(void);
void lcd_ctrl_init(void *lcdbase);
void lcd_enable(void);
/* Palette programming */
uint32_t lcd_palette_programming(uint16_t *pal_array, uint32_t length);
void lcd_board_specific_init(void);


#endif /* _CYGNUS_ASIU_LCD_H */
