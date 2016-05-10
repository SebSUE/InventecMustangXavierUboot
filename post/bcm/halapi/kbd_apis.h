/*
 * =====================================================================================
 *       Filename:  kbd_apis.h
 *    Description:  C header file
 *        Version:  1.0
 *        Created:  28/feb/2014
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Susanth
 *        Company:  Broadcom
 * =====================================================================================
 */
#ifndef _KBD_APIS_H
#define _KBD_APIS_H

/* ############ LCD API Configuration defines for Diag and Debug Start ################*/
/* Enable / Disable LCD driver logs */
#define KBD_DEBUG_ENABLE
/* Enable Register read write log */
/* Enable diag specific API */
#define KBD_DIAG_ENABLE

#define KEYPAD_CLK_GATE_EN  (1<<7)

#define KEYPAD_SW_RESET_N   (1<<7)
/* ############ LCD API Configuration defines for Diag and Debug End ################*/

#define CRMU_ASIU_KEYPAD_CLK_DIV 0x0301d048
#define CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_ENABLE 31
#define CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_LOW_L 9
#define CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_LOW_R 0
#define CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_HIGH_L 25
#define CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_HIGH_R 16
#define ASIU_TOP_SW_RESET_CTRL 0x180aa020
#define ASIU_TOP_SW_RESET_CTRL__KEYPAD_SW_RESET_N 7
#define ASIU_TOP_CLK_GATING_CTRL 0x180aa024
#define ASIU_TOP_CLK_GATING_CTRL__KEYPAD_CLK_GATE_EN 7
#define CRMU_IOMUX_CTRL0 0x0301d0c8
#define CRMU_IOMUX_CTRL1 0x0301d0cc
#define CRMU_IOMUX_CTRL2 0x0301d0d0
#define CRMU_IOMUX_CTRL3 0x0301d0d4
#define CRMU_IOMUX_CTRL4 0x0301d0d8
#define CRMU_IOMUX_CTRL5 0x0301d0dc
#define CRMU_IOMUX_CTRL6 0x0301d0e0
#define CRMU_IOMUX_CTRL7 0x0301d0e4
#define KEYPAD_TOP_REGS_KPEMR0 0x180ac090
#define KEYPAD_TOP_REGS_KPEMR1 0x180ac094
#define KEYPAD_TOP_REGS_KPEMR2 0x180ac098
#define KEYPAD_TOP_REGS_KPEMR3 0x180ac09c
#define KEYPAD_TOP_REGS_KPIMR0 0x180ac0b0
#define KEYPAD_TOP_REGS_KPIMR1 0x180ac0b4
#define KEYPAD_TOP_REGS_KPICR0 0x180ac0b8
#define KEYPAD_TOP_REGS_KPICR1 0x180ac0bc
#define KEYPAD_TOP_REGS_KPSSR0 0x180ac0a0
#define KEYPAD_TOP_REGS_KPSSR1 0x180ac0a4
#define KEYPAD_TOP_REGS_KPIOR 0x180ac084
#define KEYPAD_TOP_REGS_KPIOR__RowOContrl_L 31
#define KEYPAD_TOP_REGS_KPIOR__RowOContrl_R 24
#define KEYPAD_TOP_REGS_KPISR0 0x180ac0c0
#define KEYPAD_TOP_REGS_KPISR1 0x180ac0c4
#define KEYPAD_TOP_REGS_KPIOR__RowIOData_L 15
#define KEYPAD_TOP_REGS_KPIOR__RowIOData_R 8
#define KEYPAD_TOP_REGS_KPCR 0x180ac080

/*
 * CLCD Controller Internal Register addresses
 */
typedef struct kbd_registers {
	uint32_t kpcr;
	uint32_t kpior;
	uint32_t reserved1[2];
	uint32_t kpemr0;
	uint32_t kpemr1;
	uint32_t kpemr2;
	uint32_t kpemr3;
	uint32_t kpssr0;
	uint32_t kpssr1;
	uint32_t reserved2[2];
	uint32_t kpimr0;
	uint32_t kpimr1;
	uint32_t kpicr0;
	uint32_t kpicr1;
	uint32_t kpisr0;
	uint32_t kpisr1;
	uint32_t kpbdctr;
} _kbd_reg;

typedef struct kbd_config {
	uint32_t kpcr;
	uint32_t kpior;
	uint32_t kpemr0;
	uint32_t kpemr1;
	uint32_t kpemr2;
	uint32_t kpemr3;
	uint32_t kpimr0;
	uint32_t kpimr1;
} _kbd_cfg;

/*KEYPAD Base Register */
#define KBD_BASE_ADDRESS KEYPAD_TOP_REGS_KPCR
#define KBD_MAX_ROW		0x08
#define KBD_MAX_COL		0x08

/*Used Row and Column config*/
#ifdef CONFIG_DIAGS_KEYPAD
#define KBD_ROW_USED	((CONFIG_DIAGS_KEYPAD >> 8) & 0xff)
#define KBD_COLUMN_USED	((CONFIG_DIAGS_KEYPAD >> 0) & 0xff)
#else
#define KBD_ROW_USED	0x04
#define KBD_COLUMN_USED	0x04
#endif

/*Key pad config*/
#define ROW_WIDTH(x)			((x-1) << 20)
#define COLUMN_WIDTH(x)			((x-1) << 16)
#define STATUS_FILTER_ENABLE	(1 << 15)
#define STATUS_FILTER_TYP(x)	(x << 12)
#define COLUMN_FILTER_EN		(1 << 11)
#define COLUMN_FILTER_TYPE(x)	(x << 8)
#define SOFT					(1 << 3)
#define SWAP_ROW_COL			(1 << 2)
#define MODE_PULL_UP			(1 << 1)
#define KEYPAD_EN				(1 << 0)

#define ROW_OP_CTRL(x)	(x << 24)
#define COL_OP_CTRL(x)	(x << 16)
#define ROW_IO_DATA(x)	(x << 8)
#define COL_IO_DATA(x)	(x << 0)

#define KBD_NO_KEY_FOUND    0xFF
#define KBD_KEY_DETECTED    0xAB
#define KBD_NO_KEY_DETECTED 0xCD

#define KPEMR(x, y, z) (z << (2*(x*KBD_MAX_COL) + 2*y))
#define KPIMR_EN(x, y) (1 << ((x*KBD_MAX_COL) + y))

/* Clock gating register define*/
#define KBD_CLK_GATING_CTRL ASIU_TOP_CLK_GATING_CTRL
#define KBD_CLK_GATE_EN (1 << ASIU_TOP_CLK_GATING_CTRL__KEYPAD_CLK_GATE_EN)

/* Keypad CLK div */
#define KBD_KEYPAD_CLK_DIV	CRMU_ASIU_KEYPAD_CLK_DIV
#define KBD_CLK_DIV_HIGH(x)	(x << CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_LOW_R)
#define KBD_CLK_DIV_LOW(x)	(x << CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_HIGH_R)
#define KBD_CLK_DIV_EN(x)	(x << CRMU_ASIU_KEYPAD_CLK_DIV__CLK_DIV_ENABLE)

/* Keyboard APIs*/
uint32_t kbd_read_reg(uint32_t a);
void kbd_write_reg(uint32_t a, uint32_t d);
void kbd_clk_gate(void);
void kbd_clk_div(uint32_t clk_div_en, uint32_t div_h, uint32_t div_l);
void kbd_set_config(_kbd_cfg *cfg);
void kbd_ctrl_init(void);
void kbd_reset(void);
uint32_t kbd_get_bitfield_pos(uint32_t row, uint32_t col);
char kbd_extract_key_press_int(void);
char kbd_extract_key_press_status(void);
uint32_t kbd_key_pressed_int(void);
uint32_t kbd_key_pressed_stat(void);
void kbd_board_specific_init(void);

uint32_t keypad_read(uint32_t x);
uint32_t keypad_read_or(uint32_t x, uint32_t vv);
uint32_t keypad_read_and(uint32_t x, uint32_t vv);
void keypad_write(uint32_t v, uint32_t x);
void keypad_write_or(uint32_t v, uint32_t x);
void keypad_write_and(uint32_t v, uint32_t x);
void keypad_init(void);
void kbd_reset(void);

#endif
