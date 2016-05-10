/*
 * =====================================================================================
 *       Filename:  lcd_apis.h
 *    Description:  C header file
 *        Version:  1.0
 *        Created:  28/Jan/2014
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Susanth
 *        Company:  Broadcom
 * =====================================================================================
 */
#ifndef _LCD_APIS_H
#define _LCD_APIS_H

#include <common.h>
#include "pwmtamper_apis.h"

#define LCDIMSC__MBERRORIM 4
#define LCDIMSC__VcompIM 3
#define LCDIMSC__LNBUIM 2
#define LCDIMSC__FUFIM 1
#define ASIU_TOP_LCD_AXI_SB_CTRL 0x180aa02c
#define ASIU_TOP_SW_RESET_CTRL 0x180aa020
#define ASIU_TOP_SW_RESET_CTRL__LCD_SW_RESET_N 4
#define ASIU_TOP_CLK_GATING_CTRL 0x180aa024
#define ASIU_TOP_CLK_GATING_CTRL__LCD_CLK_GATE_EN 4
#define ASIU_TOP_CLK_GATING_CTRL__MIPI_DSI_CLK_GATE_EN 3
#define CLCDPeriphID0_WIDTH 8
#define CLCDPeriphID1_WIDTH 8
#define CLCDPeriphID2_WIDTH 8
#define CLCDPCellID0_WIDTH 8
#define CLCDPCellID1_WIDTH 8
#define CLCDPCellID2_WIDTH 8
#define LCDIMSC_DATAMASK 0x0000001e
#define ClcdCrsrIMSC_DATAMASK 0x00000001
#define ClcdCrsrConfig_DATAMASK 0x00000003
#define ClcdCrsrCtrl_DATAMASK 0x00000031
#define ClcdCrsrPalette0_DATAMASK 0x00ffffff
#define ClcdCrsrXY_DATAMASK 0x0fff0fff
#define ClcdCrsrXY__CrsrY_L 25
#define ClcdCrsrXY__CrsrY_R 16
#define ClcdCrsrClip_DATAMASK 0x00003f3f
#define ClcdCrsrClip__CrsrClipY_L 13
#define ClcdCrsrClip__CrsrClipY_R 8
#define MIPI_DSI_GENPLL_CONTROL0 0x180a9800
#define MIPI_DSI_GENPLL_CONTROL0__GENPLL_KI_R 4
#define MIPI_DSI_GENPLL_CONTROL0__GENPLL_KA_R 7
#define MIPI_DSI_GENPLL_CONTROL0__GENPLL_KP_R 0
#define MIPI_DSI_GENPLL_CONTROL0__GENPLL_POST_RESETB 10
#define MIPI_DSI_GENPLL_CONTROL0__GENPLL_RESETB 11
#define MIPI_DSI_GENPLL_CONTROL1 0x180a9804
#define MIPI_DSI_GENPLL_CONTROL1__GENPLL_BYP_EN_CH_R 18
#define MIPI_DSI_GENPLL_CONTROL1__GENPLL_ENABLEB_CH_R 12
#define MIPI_DSI_GENPLL_CONTROL2 0x180a9808
#define MIPI_DSI_GENPLL_CONTROL3 0x180a980c
#define MIPI_DSI_GENPLL_CONTROL4 0x180a9810
#define MIPI_DSI_GENPLL_CONTROL4__GENPLL_NDIV_INT_R 20
#define MIPI_DSI_GENPLL_CONTROL4__GENPLL_NDIV_FRAC_R 0
#define MIPI_DSI_GENPLL_CONTROL5 0x180a9814
#define MIPI_DSI_GENPLL_CONTROL5__GENPLL_PDIV_R 0
#define MIPI_DSI_GENPLL_CONTROL6 0x180a9818
#define MIPI_DSI_GENPLL_CONTROL7 0x180a981c
#define MIPI_DSI_GENPLL_CONTROL8 0x180a9820
#define MIPI_DSI_GENPLL_CONTROL8__GENPLL_CH0_MDIV_R 0
#define MIPI_DSI_GENPLL_CONTROL8__GENPLL_CH1_MDIV_R 10
#define MIPI_DSI_GENPLL_CONTROL8__GENPLL_CH2_MDIV_R 20
#define MIPI_DSI_GENPLL_CONTROL9 0x180a9824
#define MIPI_DSI_GENPLL_STATUS 0x180a9828
#define MIPI_DSI_GENPLL_STATUS__GENPLL_LOCK 12
#define CRMU_PLL_AON_CTRL 0x0301c020
#define CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON 20
#define CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON_PLL 19
#define CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON_BG 18
#define CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON_LDO 17
#define CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_ISO_IN 16
#define CRMU_CHIP_IO_PAD_CONTROL 0x0301d0bc
#define CRMU_IOMUX_CTRL0 0x0301d0c8
#define CRMU_IOMUX_CTRL1 0x0301d0cc
#define CRMU_IOMUX_CTRL2 0x0301d0d0
#define CRMU_IOMUX_CTRL3 0x0301d0d4
#define CRMU_IOMUX_CTRL3__CORE_TO_IOMUX_PWM0_SEL_R 0
#define CRMU_IOMUX_CTRL4 0x0301d0d8
#define CRMU_IOMUX_CTRL5 0x0301d0dc
#define CRMU_IOMUX_CTRL6 0x0301d0e0
#define CRMU_IOMUX_CTRL7 0x0301d0e4

/* ############ LCD API Configuration defines for Diag and Debug Start ################*/
/* Enable / Disable LCD driver logs */
/*#define LCD_DEBUG_ENABLE*/
/* Enable Register read write log */
/*#define LCD_LOG_ALL_REGACCESS*/
/* Enable diag specific API */
/*#define LCD_DIAG_ENABLE*/
/* ############ LCD API Configuration defines for Diag and Debug End ################*/

#define LCD_BASE_ADDRESS 0x180a0000

/*
 * CLCD Controller Internal Register addresses
 */
typedef struct clcd_registers {
	uint32_t tim0;	/* 0x00 */
	uint32_t tim1;
	uint32_t tim2;
	uint32_t tim3;
	uint32_t ubase;	/* 0x10 */
	uint32_t lbase;
	uint32_t cntl;
	uint32_t imsc;
	uint32_t ris;	/* 0x20 */
	uint32_t mis;
	uint32_t icr;
	uint32_t ucurr;
	uint32_t lcurr;	/* 0x30 */
	uint32_t unused1[0x73];	/* 0x34..0x1ff */
	uint32_t palette[0x80];	/* 0x200..0x3ff */
	uint32_t unused2[0x100];
	uint32_t curimg[0x100];
	uint32_t curctr;
	uint32_t curcfg;
	uint32_t curpalett0;
	uint32_t curpalett1;
	uint32_t curXY;
	uint32_t curclip;
	uint32_t unused3[0x02];
	uint32_t curimsc;
	uint32_t curicr;
	uint32_t curris;
	uint32_t curmis;
	uint32_t unused4[0xec];
	uint32_t perID0;
	uint32_t perID1;
	uint32_t perID2;
	uint32_t perID3;
	uint32_t cellID0;
	uint32_t cellID1;
	uint32_t cellID2;
	uint32_t cellID3;
} _lcd_reg;

/* u-boot specific: information passed by the board file */
struct clcd_config {
	uint32_t tim0;
	uint32_t tim1;
	uint32_t tim2;
	uint32_t tim3;
	uint32_t cntl;
};

#define LCD_FAIL	0x0000ffff
#define LCD_PASS	0xffff0000

/*LCD PLL Configuration */
#define LCD_PLL_PDIV		4
#define LCD_PLL_NDIV_INT	80
#define LCD_PLL_NDIC_FRAC	0
#define LCD_PLL_M1DIV		19
#define LCD_PLL_M2DIV		19
#define LCD_PLL_M3DIV		19
#define LCD_PLL_GAIN_KI		3
#define LCD_PLL_GAIN_KA		0
#define LCD_PLL_GAIN_KP		6

/*lcd_mipi_dsi_gen_pll_config(1,0x00000073,0x00000000,0x00000008,0x0000001C,0x0000001C,3,0,6);*/
/* LCD Cursor Image length 32 * 256*/
#define LCD_CUR_IMG_RAM_SIZE 0x100 /*256 x 32 bits*/

/* LCD palette RAM size 32 * 128*/
#define LCD_PALETTE_RAM_SIZE 0x100 /*256 x 16 bit*/

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

/* Bit definitions for interrupt flags */
#define LCD_INTR_MBERROR	(1 << LCDIMSC__MBERRORIM)
#define LCD_INTR_VcompIM	(1 << LCDIMSC__VcompIM)
#define LCD_INTR_LNBUIM		(1 << LCDIMSC__LNBUIM)
#define LCD_INTR_FUFIM		(1 << LCDIMSC__FUFIM)

/* Clock gating register define*/
#define LCD_CLK_GATING_CTRL	ASIU_TOP_CLK_GATING_CTRL
#define LCD_CLK_GATE_EN		ASIU_TOP_CLK_GATING_CTRL__LCD_CLK_GATE_EN
#define DSI_CLK_GATE_EN		ASIU_TOP_CLK_GATING_CTRL__MIPI_DSI_CLK_GATE_EN

/* Cursor Settings */
#define LCD_CUR_XY_Y_SHIFT			ClcdCrsrXY__CrsrY_R
#define LCD_CUR_CLIP_XY_Y_SHIFT		ClcdCrsrClip__CrsrClipY_R

/* Interrupt register mask value */
#define LCD_MASK_INTR_REG			LCDIMSC_DATAMASK /*0x1E*/
#define LCD_MASK_CUR_INTR_REG		ClcdCrsrIMSC_DATAMASK /*0x00000001*/
#define LCD_MASK_CUR_CONFIG_REG		ClcdCrsrConfig_DATAMASK /*0x03*/
#define LCD_MASK_CUR_CONTRL_REG		ClcdCrsrCtrl_DATAMASK /*0x00000031*/
#define LCD_MASK_CUR_XY_REG			ClcdCrsrXY_DATAMASK /*0x0fff0fff*/
#define LCD_MASK_CUR_PALETTE		ClcdCrsrPalette0_DATAMASK /*ClcdCrsrPalette0_DATAMASK and ClcdCrsrPalette1_DATAMASK -> 0x00ffffff both have same value*/
#define LCD_MASK_CUR_CLIP_XY_REG    ClcdCrsrClip_DATAMASK /*0x00003f3f*/

/*PLL Configuration register for MIPI-DSI and LCD clock*/
#define LCD_PLL_PWR_CONFIG	CRMU_PLL_AON_CTRL
#define LCD_PWRON				CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON
#define LCD_PWRON_PLL 			CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON_PLL
#define LCD_PWRON_BG			CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON_BG
#define LCD_PWRON_LDO			CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_PWRON_LDO
#define LCD_ISO_IN				CRMU_PLL_AON_CTRL__ASIU_MIPI_GENPLL_ISO_IN

uint32_t lcd_read_reg(uint32_t a);
void lcd_write_reg(uint32_t a, uint32_t d);
void lcd_enable(void);
void lcd_reset(void);

void lcd_set_config(struct clcd_config *cfg);
uint32_t get_lcd_image_lbase(void);

uint32_t get_lcd_image_ubase(void);
void lcd_ctrl_init(void *lcdbase);
/* Palette programming */
uint32_t lcd_palette_programming(uint16_t *pal_array, uint32_t length);
/* Get Peripheral ID */
uint32_t lcd_get_peripheral_id(void);
/* Get PrimeCell ID */
uint32_t lcd_get_primecell_id(void);
/* Set LCD intr */
void lcd_intr_enable(uint32_t intr);
/* Clear LCD intr */
void lcd_intr_clear(uint32_t intr);
/* Get LCD Raw Intr Status */
uint32_t lcd_get_raw_intr(void);
/* Get LCD Masked Intr Status */
uint32_t lcd_get_masked_intr(void);
/* LCD Cursor configuration */
void lcd_cur_config(uint32_t curConfig);
/* LCD Cursor control */
void lcd_cur_control(uint32_t curControl);
/* LCD Cursor XY settings */
void lcd_set_cur_xy(uint32_t x, uint32_t y);
/* Cursor palette programming */
void lcd_cur_palette_programming(uint32_t *pal_array);
/* LCD CursorClip set XY */
void lcd_set_curclip_xy(uint32_t clipx, uint32_t clipy);
/* Set LCD cur intr */
void lcd_cur_intr_enable(uint32_t intr);
/* Clear LCD cur intr */
void lcd_cur_intr_clear(uint32_t intr);
/* Get LCD cur Raw Intr Status */
uint32_t lcd_cur_get_raw_intr(void);
/* Get LCD cur Masked Intr Status */
uint32_t lcd_cur_get_masked_intr(void);
/* Get LCD cur Image RAM Filling */
uint32_t lcd_cur_image_RAM_fill(uint32_t *img, uint32_t length);
uint32_t lcd_gen_pll_config(uint32_t p, uint32_t n_int, uint32_t n_frac, uint32_t m1div, uint32_t m2div, uint32_t m3div, uint32_t gain_ki, uint32_t gain_ka, uint32_t gain_kp);

void lcd_asiu_mipi_dsi_gen_pll_pwr_on_config(void);
/* LCD Clock gating */
void lcd_clk_gate(void);
uint32_t lcd_board_specific_init(void);
/*
 * To use this driver you need to provide the following in board files:
 *	a panel_info definition
 *	an lcd_enable function (can't define a weak default with current code)
 */

/* There is nothing to do with color registers, we use true color */
void lcd_setcolreg(ushort regno, ushort red, ushort green, ushort blue);

#endif
