#include <common.h>
#include <asm/io.h>
#include <post.h>
#include "kbd_apis.h"

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

_kbd_cfg kbd_config = {
	.kpcr	= ROW_WIDTH(4) | COLUMN_WIDTH(4) | KEYPAD_EN,
	.kpior	= ROW_OP_CTRL(0x0f),

	.kpemr0	= KPEMR(0, 0, 1) | KPEMR(0, 1, 1) | KPEMR(0, 2, 1) | KPEMR(0, 3, 1) |
			  KPEMR(1, 0, 1) | KPEMR(1, 1, 1) | KPEMR(1, 2, 1) | KPEMR(1, 3, 1),
	.kpemr1	= KPEMR(0, 0, 1) | KPEMR(0, 1, 1) | KPEMR(0, 2, 1) | KPEMR(0, 3, 1) |
			  KPEMR(1, 0, 1) | KPEMR(1, 1, 1) | KPEMR(1, 2, 1) | KPEMR(1, 3, 1),

	.kpemr2	= 0x00,
	.kpemr3	= 0x00,

	.kpimr0	= KPIMR_EN(0, 0) | KPIMR_EN(0, 1) | KPIMR_EN(0, 2) | KPIMR_EN(0, 3) |
			  KPIMR_EN(1, 0) | KPIMR_EN(1, 1) | KPIMR_EN(1, 2) | KPIMR_EN(1, 3) |
			  KPIMR_EN(2, 0) | KPIMR_EN(2, 1) | KPIMR_EN(2, 2) | KPIMR_EN(2, 3) |
			  KPIMR_EN(3, 0) | KPIMR_EN(3, 1) | KPIMR_EN(3, 2) | KPIMR_EN(3, 3),
	.kpimr1	= 0x00
};

static char key_map_0x0404[16] = {
	'D', 'C', 'B', 'A',
	'#', '9', '6', '3',
	'0', '8', '5', '2',
	'*', '7', '4', '1'};

static char key_map_0x0505[25] = {
	'.', ':', 'F', 'H', 'M',
	'A', '1', '2', '3', 'S',
	'P', '4', '5', '6', 'U',
	'C', '7', '8', '9', 'D',
	'H', '*', '0', '#', 'E'};

static char *key_map =
#ifdef CONFIG_DIAGS_KEYPAD
	TOKENPASTE2(key_map_, CONFIG_DIAGS_KEYPAD);
#else
	key_map_0x0404;
#endif

_kbd_reg *kbd_regs;

/* To read KBD registers */
uint32_t kbd_read_reg(uint32_t a)
{
	uint32_t d = readl(a);
#ifdef KBD_DEBUG_ENABLE
#ifdef KBD_LOG_ALL_REGACCESS
	post_log("lcd_read_reg - Address = 0x%X, Data = 0x%X\n", a, d);
#endif
#endif
	return d;
}

/* To write KBD registers */
void kbd_write_reg(uint32_t a, uint32_t d)
{
	writel(d, a);
#ifdef KBD_DEBUG_ENABLE
#ifdef KBD_LOG_ALL_REGACCESS
	post_log("lcd_write_reg - Address = 0x%X, Data = 0x%X\n", a, d);
#endif
#endif
}

/* KBD Clock gating */
void kbd_clk_gate(void)
{
	/*LCD clock Gate enabled*/
	uint32_t d = (kbd_read_reg(KBD_CLK_GATING_CTRL) | KBD_CLK_GATE_EN);
	kbd_write_reg(KBD_CLK_GATING_CTRL, d);
#ifdef KBD_DEBUG_ENABLE
	post_log("kbd_clk_gate -> 0x%X\n", kbd_read_reg(KBD_CLK_GATING_CTRL));
#endif
}

/* KBD Clock div */
void kbd_clk_div(uint32_t clk_div_en, uint32_t div_h, uint32_t div_l)
{
	kbd_write_reg(KBD_KEYPAD_CLK_DIV, (KBD_CLK_DIV_EN(clk_div_en) | KBD_CLK_DIV_HIGH(div_h) | KBD_CLK_DIV_LOW(div_h)));
#ifdef KBD_DEBUG_ENABLE
	post_log("kbd_clk_div -> 0x%X\n", (KBD_CLK_DIV_EN(clk_div_en) | KBD_CLK_DIV_HIGH(div_h) | KBD_CLK_DIV_LOW(div_h)));
#endif
}

void kbd_set_config(_kbd_cfg *cfg)
{
	kbd_config.kpcr		= cfg->kpcr;
	kbd_config.kpior	= cfg->kpior;
	kbd_config.kpemr0	= cfg->kpemr0;
	kbd_config.kpemr1	= cfg->kpemr1;
	kbd_config.kpemr2	= cfg->kpemr2;
	kbd_config.kpemr3	= cfg->kpemr3;
	kbd_config.kpimr0	= cfg->kpimr0;
	kbd_config.kpimr1	= cfg->kpimr1;
}

/* controller Init */
void kbd_ctrl_init(void)
{
	_kbd_reg *regs = (_kbd_reg *)KBD_BASE_ADDRESS;
	/* Board specific init*/
	/*kbd_board_specific_init();*/

	/*Disable Keypad ctrlr*/
	writel(kbd_config.kpcr & (~KEYPAD_EN), &regs->kpcr);
	/*writel(kbd_config.kpior, &regs->kpemr0);*/
	writel(kbd_config.kpemr0, &regs->kpemr0);
	writel(kbd_config.kpemr1, &regs->kpemr1);
	writel(kbd_config.kpemr2, &regs->kpemr2);
	writel(kbd_config.kpemr3, &regs->kpemr3);
	writel(kbd_config.kpimr0, &regs->kpimr0);
	writel(kbd_config.kpimr1, &regs->kpimr1);
	/*Enable Keypad ctrlr*/
	writel(kbd_config.kpcr | KEYPAD_EN, &regs->kpcr);

#ifdef KBD_DEBUG_ENABLE

#endif
}
/*void kbd_reset(void)
{
	uint32_t sw_rst = lcd_read_reg(ASIU_TOP_SW_RESET_CTRL);
#ifdef KBD_DEBUG_ENABLE
	post_log("rd ASIU_TOP_SW_RESET_CTRL = 0x%X\n",sw_rst);
#endif
	sw_rst = (sw_rst & ~(1 << ASIU_TOP_SW_RESET_CTRL__KEYPAD_SW_RESET_N));
	lcd_write_reg(ASIU_TOP_SW_RESET_CTRL,sw_rst);
#ifdef KBD_DEBUG_ENABLE
	post_log("wr ASIU_TOP_SW_RESET_CTRL = 0x%X\n",sw_rst);
	post_log("Delay...\n");
#endif
	{
		int i = 0;
		for(i = 0 ; i < 100 ; i++);
	}

	sw_rst = (sw_rst | (1 << ASIU_TOP_SW_RESET_CTRL__KEYPAD_SW_RESET_N));
#ifdef KBD_DEBUG_ENABLE
	post_log("wr ASIU_TOP_SW_RESET_CTRL = 0x%X\n",sw_rst);
#endif
	lcd_write_reg(ASIU_TOP_SW_RESET_CTRL,sw_rst);
}*/

/*Get the register position based on the row and col position , row and col numbering start from 0, first entry is (0,0)*/
uint32_t kbd_get_bitfield_pos(uint32_t row, uint32_t col)
{
	return (row*KBD_MAX_COL) + col;
}

char kbd_extract_key_press_int(void)
{
	_kbd_reg *regs = (_kbd_reg *)KBD_BASE_ADDRESS;
	uint32_t row = 0;
	uint32_t k_map_index = 0;
	uint32_t i , j;
	uint32_t k_int;

	k_int = readl(&regs->kpisr0);
	if (k_int) {
		for (i = 0; i < KBD_ROW_USED; i++) {
			row = ((k_int >> (i * 8)) & 0xFF);
			for (j = 0; j < KBD_COLUMN_USED; j++) {
				if (row & 0x01) {
					writel(0x1 << ((i * 8) + j), &regs->kpicr0);
					return key_map[k_map_index];
				}
				k_map_index++;
				row = row >> 0x01;
			}
		}
	}

	k_int = readl(&regs->kpisr1);
	if (k_int) {
		for (i = 0; i < KBD_ROW_USED; i++) {
			row = ((k_int >> (i * 8)) & 0xFF);
			for (j = 0; j < KBD_COLUMN_USED; j++) {
				if (row & 0x01) {
					writel(0x1 << ((i * 8) + j), &regs->kpicr1);
					return key_map[k_map_index+4*KBD_COLUMN_USED];
				}
				k_map_index++;
				row = row >> 0x01;
			}
		}
	}

	return KBD_NO_KEY_FOUND;
}

char kbd_extract_key_press_status(void)
{
	_kbd_reg *regs = (_kbd_reg *)KBD_BASE_ADDRESS;
	uint32_t row = 0;
	uint32_t k_map_index = 0;
	uint32_t i, j;
	uint32_t key_press = ((readl(&regs->kpcr) >> 1) & 0x01) ? 0 : 1;
	uint32_t k_stat;

	k_stat = readl(&regs->kpssr0);
	if (k_stat) {
		for (i = 0; i < KBD_ROW_USED; i++) {
			row = ((k_stat >> (i * 8)) & 0xFF);
			for (j = 0; j < KBD_COLUMN_USED; j++) {
				if ((row & 0x01) == key_press)
					return key_map[k_map_index];
				k_map_index++;
				row = row >> 0x01;
			}
		}
	}

	k_stat = readl(&regs->kpisr1);
	if (k_stat) {
		for (i = 0; i < KBD_ROW_USED; i++) {
			row = ((k_stat >> (i * 8)) & 0xFF) + 4;
			for (j = 0; j < KBD_COLUMN_USED; j++) {
				if ((row & 0x01) == key_press)
					return key_map[k_map_index+4*KBD_COLUMN_USED];
				k_map_index++;
				row = row >> 0x01;
			}
		}
	}

	return KBD_NO_KEY_FOUND;
}

uint32_t kbd_key_pressed_int(void)
{
	_kbd_reg *regs = (_kbd_reg *)KBD_BASE_ADDRESS;
	uint32_t k_int = readl(&regs->kpisr0);
	if (k_int)
		return KBD_KEY_DETECTED;
	return KBD_NO_KEY_DETECTED;
}

uint32_t kbd_key_pressed_stat(void)
{
	_kbd_reg *regs = (_kbd_reg *)KBD_BASE_ADDRESS;
	uint32_t k_stat = readl(&regs->kpssr0);
	if (k_stat)
		return KBD_KEY_DETECTED;
	return KBD_NO_KEY_DETECTED;
}

void kbd_board_specific_init(void)
{
	/*Clock gate configuration*/
	kbd_clk_gate();

	/* IOMUX Function select for Keypad Row and Column*/
#ifdef KBD_DEBUG_ENABLE
	post_log("CRMU_IOMUX_CTRL1 = 0x%X\n", kbd_read_reg(CRMU_IOMUX_CTRL1));
	post_log("CRMU_IOMUX_CTRL2 = 0x%X\n", kbd_read_reg(CRMU_IOMUX_CTRL2));
#endif
	kbd_write_reg(CRMU_IOMUX_CTRL1, ((1<<(4*KBD_ROW_USED))-1)/0xf);
	kbd_write_reg(CRMU_IOMUX_CTRL2, ((1<<(4*KBD_COLUMN_USED))-1)/0xf);
#ifdef KBD_DEBUG_ENABLE
	post_log("CRMU_IOMUX_CTRL1 = 0x%X\n", kbd_read_reg(CRMU_IOMUX_CTRL1));
	post_log("CRMU_IOMUX_CTRL2 = 0x%X\n", kbd_read_reg(CRMU_IOMUX_CTRL2));
#endif
	/*Clock divider configuration*/
#ifdef CONFIG_CYGNUS_EMULATION
	kbd_clk_div(1, 0x5, 0x5);
#else
	kbd_clk_div(1, 0x187, 0x187);
#endif
}

void kbd_reset(void)
{
	uint32_t sw_rst = keypad_read(ASIU_TOP_SW_RESET_CTRL);
	post_log("rd ASIU_TOP_SW_RESET_CTRL = 0x%X\n", sw_rst);
	sw_rst = (sw_rst & ~(1 << ASIU_TOP_SW_RESET_CTRL__KEYPAD_SW_RESET_N));
	keypad_write(sw_rst, ASIU_TOP_SW_RESET_CTRL);
	post_log("wr ASIU_TOP_SW_RESET_CTRL = 0x%X\n", sw_rst);
	post_log("Delay...\n");
	udelay(100);
	sw_rst = (sw_rst | (1 << ASIU_TOP_SW_RESET_CTRL__KEYPAD_SW_RESET_N));
	post_log("wr ASIU_TOP_SW_RESET_CTRL = 0x%X\n", sw_rst);

	keypad_write(sw_rst, ASIU_TOP_SW_RESET_CTRL);
}

uint32_t keypad_read(uint32_t x)
{
	uint32_t v;
	v = readl(x);
	return v;
}

uint32_t keypad_read_or(uint32_t x, uint32_t vv)
{
	uint32_t v;
	v = readl(x);
	v = v | vv;
	return v;
}

uint32_t keypad_read_and(uint32_t x, uint32_t vv)
{
	uint32_t v;
	v = readl(x);
	v = v & vv;
	return v;
}

void keypad_write(uint32_t v, uint32_t x)
{
	writel(v, x);
}

void keypad_write_or(uint32_t v, uint32_t x)
{
	uint32_t vv;
	vv = readl(x);
	vv = vv | v;
	writel(vv, x);
}

void keypad_write_and(uint32_t v, uint32_t x)
{
	uint32_t vv;
	vv = readl(x);
	vv = vv & v;
	writel(vv, x);
}

void keypad_init(void)
{
	uint32_t ctrl_reg, v;

	/*Keypad Soft Reset*/
	kbd_reset();

	/*Enable Clock Gating*/
	keypad_write_or(KEYPAD_CLK_GATE_EN, ASIU_TOP_CLK_GATING_CTRL);

	/*IOMUX Setting*/
	keypad_write_and(0x77770000, CRMU_IOMUX_CTRL1);
	keypad_write_and(0x77770000, CRMU_IOMUX_CTRL2);
	keypad_write_or(0x00001111, CRMU_IOMUX_CTRL1);
	keypad_write_or(0x00001111, CRMU_IOMUX_CTRL2);

	ctrl_reg =  3<<20;       /*Row*/
	ctrl_reg |= 3<<16;       /*Col*/
	ctrl_reg |= 1<<15;       /*StatFilEn*/
	ctrl_reg |= 5<<12;       /*StatFilType*/
	ctrl_reg |= 1<<11;       /*ColFilEn*/
	ctrl_reg |= 5<<8;       /*ColFilType*/
	ctrl_reg |= 0<<3;       /*IoMode*/
	ctrl_reg |= 0<<2;       /*SwapRc*/
	ctrl_reg |= 1<<1;       /*ScanMode*/
	ctrl_reg |= 1<<0;       /*Enable or disable*/

	keypad_write(ctrl_reg, KEYPAD_TOP_REGS_KPCR);

	keypad_write_or(0x0F000F00, KEYPAD_TOP_REGS_KPIOR);

	keypad_write(0x00aa00aa, KEYPAD_TOP_REGS_KPEMR0);      /*Edge setting*/
	keypad_write(0x00aa00aa, KEYPAD_TOP_REGS_KPEMR1);      /*Edge setting*/

	keypad_write(0, KEYPAD_TOP_REGS_KPIMR0);        /*Disable all interrupts*/
	keypad_write(0, KEYPAD_TOP_REGS_KPIMR1);        /*Disable all interrupts*/

	v = keypad_read(KEYPAD_TOP_REGS_KPICR0);
	keypad_write(v, KEYPAD_TOP_REGS_KPICR0);       /*Clear interrupt*/
	v = keypad_read(KEYPAD_TOP_REGS_KPICR1);
	keypad_write(v, KEYPAD_TOP_REGS_KPICR1);       /*Clear interrupt*/
}
