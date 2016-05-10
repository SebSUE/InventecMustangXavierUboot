/*
 * =====================================================================================
 *       Filename:  lcd_apis.c
 *    Description:  C APIs file
 *        Version:  1.0
 *        Created:  28/Jan/2014
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Susanth
 *        Company:  Broadcom
 * =====================================================================================
 */
#include "lcd_apis.h"
#include <common.h>
#include <asm/io.h>
#include <lcd.h>
#include <post.h>

/* These variables are required by lcd.c -- although it sets them by itself */
int lcd_line_length;
int lcd_color_fg;
int lcd_color_bg;
void *lcd_base;
void *lcd_console_address;
short console_col;
short console_row;

struct clcd_config lcd_config = {
	.tim0 = 0x105010C,
	.tim1 = 0x101003F,
	.tim2 = 0x43f0252,
	.tim3 = 0x10002,
	.cntl = 0x1382f
};

/*struct clcd_registers *regs;*/
_lcd_reg *lcd_regs = (struct clcd_registers *)LCD_BASE_ADDRESS;

/* To read LCD registers */
uint32_t lcd_read_reg(uint32_t a)
{
	uint32_t d = readl(a);
#ifdef LCD_DEBUG_ENABLE
#ifdef LCD_LOG_ALL_REGACCESS
	post_log("lcd_read_reg - Address = 0x%X, Data = 0x%X\n", a, d);
#endif
#endif
	return d;
}

/* To write LCD registers */
void lcd_write_reg(uint32_t a, uint32_t d)
{
	writel(d, a);
#ifdef LCD_DEBUG_ENABLE
#ifdef LCD_LOG_ALL_REGACCESS
	post_log("lcd_write_reg - Address = 0x%X, Data = 0x%X\n", a, d);
#endif
#endif
}

void lcd_enable(void)
{
	writel((readl(&lcd_regs->cntl) | LCD_CNTL_LCDEN), &lcd_regs->cntl);
}

void lcd_reset(void)
{
	uint32_t sw_rst = lcd_read_reg(ASIU_TOP_SW_RESET_CTRL);
#ifdef LCD_DEBUG_ENABLE
	post_log("rd ASIU_TOP_SW_RESET_CTRL = 0x%X\n", sw_rst);
#endif
	sw_rst = (sw_rst & ~(1 << ASIU_TOP_SW_RESET_CTRL__LCD_SW_RESET_N));
	lcd_write_reg(ASIU_TOP_SW_RESET_CTRL, sw_rst);
#ifdef LCD_DEBUG_ENABLE
	post_log("wr ASIU_TOP_SW_RESET_CTRL = 0x%X\n", sw_rst);
	post_log("Delay...\n");
#endif
	udelay(100);

	sw_rst = (sw_rst | (1 << ASIU_TOP_SW_RESET_CTRL__LCD_SW_RESET_N));
#ifdef LCD_DEBUG_ENABLE
	post_log("wr ASIU_TOP_SW_RESET_CTRL = 0x%X\n", sw_rst);
#endif
	lcd_write_reg(ASIU_TOP_SW_RESET_CTRL, sw_rst);
}

void lcd_set_config(struct clcd_config *cfg)
{
	lcd_config.tim0 = cfg->tim0;
	lcd_config.tim1 = cfg->tim1;
	lcd_config.tim2 = cfg->tim2;
	lcd_config.tim3 = cfg->tim3;
	lcd_config.cntl = cfg->cntl;
}

uint32_t get_lcd_image_lbase(void)
{
	return readl(&lcd_regs->lbase);
}

uint32_t get_lcd_image_ubase(void)
{
	return readl(&lcd_regs->ubase);
}

/* controller Init */
void lcd_ctrl_init(void *lcdbase)
{
	/*struct clcd_config *config  = (struct clcd_config*) panel_info.priv; //to do for stdio*/
	/*lcd_regs = (struct clcd_registers*) panel_info.mmio; //to do for stdio*/
	lcd_board_specific_init();

	/* first control with disable */
	writel(lcd_config.cntl & ~LCD_CNTL_LCDEN, &lcd_regs->cntl);
	writel(lcd_config.tim0, &lcd_regs->tim0);
	writel(lcd_config.tim1, &lcd_regs->tim1);
	writel(lcd_config.tim2, &lcd_regs->tim2);
	writel(lcd_config.tim3, &lcd_regs->tim3);
	writel((uint32_t)lcdbase, &lcd_regs->lbase);
	writel((uint32_t)lcdbase, &lcd_regs->ubase);
	/* finally, enable */
	writel(lcd_config.cntl | LCD_CNTL_LCDEN, &lcd_regs->cntl);

#ifdef LCD_DEBUG_ENABLE

	post_log("lcd_ctrl_init - done\n");
	post_log("cnlt  - 0x%X\n", lcd_regs->cntl);
	post_log("tim0  - 0x%X\n", lcd_regs->tim0);
	post_log("tim1  - 0x%X\n", lcd_regs->tim1);
	post_log("tim2  - 0x%X\n", lcd_regs->tim2);
	post_log("tim3  - 0x%X\n", lcd_regs->tim3);
	post_log("ubase - 0x%X\n", lcd_regs->ubase);
#endif
}

/* Palette programming */
uint32_t lcd_palette_programming(uint16_t *pal_array, uint32_t length)
{
	uint32_t i = 0, a_index = 0;

	post_log("Palette base : 0x%X\n", readl(&lcd_regs->palette[0]));

	if (length > LCD_PALETTE_RAM_SIZE) {
#ifdef LCD_DEBUG_ENABLE
		post_log("lcd_palette_programming - length out of range\n");
#endif
		return LCD_FAIL;
	}
	post_log("Length : 0x%X\n", length);
	for (i = 0; i < (length); i += 2)
		writel(pal_array[i] | (pal_array[i+1] << 16), &lcd_regs->palette[a_index++]);

#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_palette_programming - done\n");
#endif
	return LCD_PASS;
}

/* Get Peripheral ID */
uint32_t lcd_get_peripheral_id(void)
{
	return readl(&lcd_regs->perID0) |
		(readl(&lcd_regs->perID1) << CLCDPeriphID0_WIDTH) |
		(readl(&lcd_regs->perID2) << (CLCDPeriphID0_WIDTH + CLCDPeriphID1_WIDTH)) |
		(readl(&lcd_regs->perID3) << (CLCDPeriphID0_WIDTH + CLCDPeriphID1_WIDTH + CLCDPeriphID2_WIDTH));
}

/* Get PrimeCell ID */
uint32_t lcd_get_primecell_id(void)
{
	return readl(&lcd_regs->cellID0) |
		(readl(&lcd_regs->cellID1) << CLCDPCellID0_WIDTH) |
		(readl(&lcd_regs->cellID2) << (CLCDPCellID0_WIDTH + CLCDPCellID1_WIDTH)) |
		(readl(&lcd_regs->cellID3) << (CLCDPCellID0_WIDTH + CLCDPCellID1_WIDTH + CLCDPCellID2_WIDTH));
}

/* Set LCD intr */
void lcd_intr_enable(uint32_t intr)
{
	writel(intr & LCD_MASK_INTR_REG, &lcd_regs->imsc);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_intr_enable -> 0x%X\n", readl(&lcd_regs->imsc));
#endif
}

/* Clear LCD intr */
void lcd_intr_clear(uint32_t intr)
{
	writel(intr & LCD_MASK_INTR_REG, &lcd_regs->icr);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_intr_clear -> 0x%X\n", (intr & LCD_MASK_INTR_REG));
#endif
}

/* Get LCD Raw Intr Status */
uint32_t lcd_get_raw_intr(void)
{
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_get_raw_intr -> 0x%X\n", readl(&lcd_regs->ris));
#endif
	return readl(&lcd_regs->ris);
}

/* Get LCD Masked Intr Status */
uint32_t lcd_get_masked_intr(void)
{
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_get_masked_intr -> 0x%X\n", readl(&lcd_regs->mis));
#endif
	return readl(&lcd_regs->mis);
}

/* LCD Cursor configuration */
void lcd_cur_config(uint32_t curConfig)
{
	writel(curConfig & LCD_MASK_CUR_CONFIG_REG, &lcd_regs->curcfg);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_config -> 0x%X\n", readl(&lcd_regs->curcfg));
#endif
}

/* LCD Cursor control */
void lcd_cur_control(uint32_t curControl)
{
	writel(curControl & LCD_MASK_CUR_CONTRL_REG, &lcd_regs->curctr);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_control -> 0x%X\n", readl(&lcd_regs->curctr));
#endif
}

/* LCD Cursor XY settings */
void lcd_set_cur_xy(uint32_t x, uint32_t y)
{
	writel((x | (y << LCD_CUR_XY_Y_SHIFT)) & LCD_MASK_CUR_XY_REG, &lcd_regs->curXY);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_set_cursor_xy -> 0x%X\n", readl(&lcd_regs->curXY));
#endif
}

/* Cursor palette programming */
void lcd_cur_palette_programming(uint32_t *pal_array)
{
	writel(pal_array[0] & LCD_MASK_CUR_PALETTE, &lcd_regs->curpalett0);
	writel(pal_array[1] & LCD_MASK_CUR_PALETTE, &lcd_regs->curpalett1);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_palette_programming - done\n");
#endif
}

/* LCD CursorClip set XY */
void lcd_set_curclip_xy(uint32_t clipx, uint32_t clipy)
{
	writel((clipx | (clipy << LCD_CUR_CLIP_XY_Y_SHIFT)) & LCD_MASK_CUR_CLIP_XY_REG, &lcd_regs->curclip);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_set_cursorclip_xy -> 0x%X\n", readl(&lcd_regs->curclip));
#endif
}

/* Set LCD cur intr */
void lcd_cur_intr_enable(uint32_t intr)
{
	writel(intr & LCD_MASK_CUR_INTR_REG, &lcd_regs->curimsc);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_intr_enable -> 0x%X\n", readl(&lcd_regs->curimsc));
#endif
}

/* Clear LCD cur intr */
void lcd_cur_intr_clear(uint32_t intr)
{
	writel(intr & LCD_MASK_CUR_INTR_REG, &lcd_regs->curicr);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_intr_clear -> 0x%X\n", (intr & LCD_MASK_CUR_INTR_REG));
#endif
}

/* Get LCD cur Raw Intr Status */
uint32_t lcd_cur_get_raw_intr(void)
{
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_get_raw_intr -> 0x%X\n", readl(&lcd_regs->curris));
#endif
	return readl(&lcd_regs->curris);
}

/* Get LCD cur Masked Intr Status */
uint32_t lcd_cur_get_masked_intr(void)
{
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_get_masked_intr -> 0x%X\n", readl(&lcd_regs->curmis));
#endif
	return readl(&lcd_regs->curmis);
}

/* Get LCD cur Image RAM Filling */
uint32_t lcd_cur_image_RAM_fill(uint32_t *img, uint32_t length)
{
	uint32_t a_index = 0x0;
	if (length > LCD_CUR_IMG_RAM_SIZE) {
#ifdef LCD_DEBUG_ENABLE
		post_log("lcd_cur_image_RAM_fill - length out of range\n");
#endif
		return LCD_FAIL;
	}

	for (a_index = 0x0; a_index < length; a_index++)
		writel(img[a_index], &lcd_regs->curimg[a_index]);

#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_cur_image_RAM_fill - done\n");
#endif

	return LCD_PASS;
}

uint32_t lcd_gen_pll_config(uint32_t p, uint32_t n_int, uint32_t n_frac, uint32_t m1div, uint32_t m2div, uint32_t m3div, uint32_t gain_ki, uint32_t gain_ka, uint32_t gain_kp)
{
	uint32_t timeout = 500;
	uint32_t reg_val = 0;
	uint32_t prev_ctrl0 = 0;

#ifdef MIPI_DSI_DEBUG_ENABLE
	post_log("mipi_dsi_gen_pll_config - p_div = 0x%X , n_div_int = 0x%X , n_div_frac = 0x%X , m1div = 0x%X , m2div = 0x%X , m3div = 0x%X , ki = 0x%X , ka = 0x%X , kp = 0x%X\n",
		p, n_int, n_frac, m1div, m2div, m3div, gain_ki, gaim_ka, gain, kp);
#endif

	/*PLL Control Input*/
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL6, 0x00000000); /*GENPLL_PLL_CTRL_H*/
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL7, 0x00880000); /*GENPLL_PLL_CTRL_L*/

	/*PDIV Value*/
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL5, (p << MIPI_DSI_GENPLL_CONTROL5__GENPLL_PDIV_R));

	/*NDIV Value*/
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL4, ((n_int << MIPI_DSI_GENPLL_CONTROL4__GENPLL_NDIV_INT_R) | (n_frac << MIPI_DSI_GENPLL_CONTROL4__GENPLL_NDIV_FRAC_R)));

	/* Configure LDO I/P Value - Default 0x5005
	 * Set KI, KA, and KP values - RMW
	 */
	reg_val = (lcd_read_reg(MIPI_DSI_GENPLL_CONTROL0) & ~(0x3FF));
	reg_val = (reg_val | (gain_ki << MIPI_DSI_GENPLL_CONTROL0__GENPLL_KI_R)
						| (gain_ka << MIPI_DSI_GENPLL_CONTROL0__GENPLL_KA_R)
						| (gain_kp << MIPI_DSI_GENPLL_CONTROL0__GENPLL_KP_R));
	prev_ctrl0 = reg_val;
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL0, reg_val);

	/*MDIV Channel 0 value //MDIV Channel 1 value //MDIV Channel 3 value*/
	reg_val = (m1div << MIPI_DSI_GENPLL_CONTROL8__GENPLL_CH0_MDIV_R) | (m2div << MIPI_DSI_GENPLL_CONTROL8__GENPLL_CH1_MDIV_R) | (m3div << MIPI_DSI_GENPLL_CONTROL8__GENPLL_CH2_MDIV_R);
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL8, reg_val);

	reg_val = (0 << MIPI_DSI_GENPLL_CONTROL1__GENPLL_BYP_EN_CH_R) | (0	<< MIPI_DSI_GENPLL_CONTROL1__GENPLL_ENABLEB_CH_R);
	lcd_write_reg (MIPI_DSI_GENPLL_CONTROL1, reg_val);

	/* Retain wr_data_gain from the previous write to wr_data_gain
	 * resetb & post_resetb Assertion
	 */
	reg_val = prev_ctrl0 | ((1 << MIPI_DSI_GENPLL_CONTROL0__GENPLL_POST_RESETB) | (1 << MIPI_DSI_GENPLL_CONTROL0__GENPLL_RESETB));
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL0, reg_val);

	udelay(600);

	reg_val = reg_val & (~((1<<MIPI_DSI_GENPLL_CONTROL0__GENPLL_POST_RESETB) | (1<<MIPI_DSI_GENPLL_CONTROL0__GENPLL_RESETB)));
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL0, reg_val);

	udelay(600);

	/*resetb de-assertion*/
	reg_val = reg_val | (1 << MIPI_DSI_GENPLL_CONTROL0__GENPLL_RESETB);
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL0, reg_val);

	udelay(600);

	/*post_resetb de-assertion*/
	reg_val = reg_val | (1 << MIPI_DSI_GENPLL_CONTROL0__GENPLL_POST_RESETB);
	lcd_write_reg(MIPI_DSI_GENPLL_CONTROL0, reg_val);

#ifdef MIPI_DSI_DEBUG_ENABLE
	post_log("Waiting For PLL Lock\n");
#endif

	udelay(600);

	do {
		reg_val = lcd_read_reg(MIPI_DSI_GENPLL_STATUS);
		if ((reg_val & (1 << MIPI_DSI_GENPLL_STATUS__GENPLL_LOCK))) {
#ifdef MIPI_DSI_DEBUG_ENABLE
			post_log("MIPI PLL Lock asserted\n");
#endif
			return LCD_PASS;
		}
	} while (timeout--);
#ifdef MIPI_DSI_DEBUG_ENABLE
	post_log("error : MIPI PLL Lock timeout\n");
#endif

	post_log("LCD PLL lock done");
	return LCD_FAIL;
}

void lcd_asiu_mipi_dsi_gen_pll_pwr_on_config(void)
{
	uint32_t pwr_on = lcd_read_reg(LCD_PLL_PWR_CONFIG);
	pwr_on = (pwr_on | (1 << LCD_PWRON) | (1 << LCD_PWRON_PLL) | (1 << LCD_PWRON_BG) | (1 << LCD_PWRON_LDO)) & (~(1 << LCD_ISO_IN));
	lcd_write_reg(LCD_PLL_PWR_CONFIG, pwr_on);
#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_asiu_mipi_dsi_gen_pll_pwr_on_config - 0x%X\n", lcd_read_reg(LCD_PLL_PWR_CONFIG));
#endif
}

/* LCD Clock gating */
void lcd_clk_gate(void)
{
	/*LCD clock Gate enabled*/
	uint32_t reg_data = lcd_read_reg(LCD_CLK_GATING_CTRL);
	reg_data = (reg_data | (1 << LCD_CLK_GATE_EN) | (1 << DSI_CLK_GATE_EN));
	lcd_write_reg(LCD_CLK_GATING_CTRL, reg_data);

#ifdef LCD_DEBUG_ENABLE
	post_log("lcd_clk_gate - ASIU_TOP_CLK_GATING_CTR -> 0x%X\n", reg_data);
#endif
}

uint32_t lcd_board_specific_init(void)
{
	uint32_t pwmctl_data = 0x3FFF0001;
	uint32_t status = LCD_PASS;
	/*Clock gate configuration*/
	lcd_clk_gate();

	writel(0, CRMU_CHIP_IO_PAD_CONTROL);

	/*PLL pwr on config*/
	lcd_asiu_mipi_dsi_gen_pll_pwr_on_config();

/*
#ifndef CONFIG_CYGNUS_EMULATION //No PLL config in Emulation
	//PLL Configuration
	status = lcd_gen_pll_config (LCD_PLL_PDIV, LCD_PLL_NDIV_INT, LCD_PLL_NDIC_FRAC, LCD_PLL_M1DIV, LCD_PLL_M2DIV, LCD_PLL_M3DIV, LCD_PLL_GAIN_KI, LCD_PLL_GAIN_KA, LCD_PLL_GAIN_KP);
#endif
*/


/*LCD Back light enable*/
	/* IOMUX Function select for Keypad Row and Column*/
uint32_t reg_val = lcd_read_reg(CRMU_IOMUX_CTRL3);
#ifdef LCD_DEBUG_ENABLE
	post_log("CRMU_IOMUX_CTRL3 = 0x%X\n", reg_val);
#endif
	reg_val &= ~(0x0F << CRMU_IOMUX_CTRL3__CORE_TO_IOMUX_PWM0_SEL_R);
	lcd_write_reg(CRMU_IOMUX_CTRL3, reg_val);
#ifdef LCD_DEBUG_ENABLE
	post_log("CRMU_IOMUX_CTRL3 = 0x%X\n", lcd_read_reg(CRMU_IOMUX_CTRL3));
#endif

	pwminit();
	setPwmCtl(0x3FFF0000);
	udelay(10000);
	setPrescale(0x0);
	udelay(10000);
	setPeriod(0x0, 0x7D0);
	udelay(10000);
	setHighPeriod(0x0, 0x3E8);
	udelay(10000);
	setPwmCtl(pwmctl_data);
return status;
}

/*
 * To use this driver you need to provide the following in board files:
 *	a panel_info definition
 *	an lcd_enable function (can't define a weak default with current code)
 */

/* There is nothing to do with color registers, we use true color */
void lcd_setcolreg(ushort regno, ushort red, ushort green, ushort blue)
{
	return;
}
