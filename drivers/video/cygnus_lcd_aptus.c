/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#include <common.h>
#include <asm/io.h>
#include <lcd.h>
#include <cygnus_pl111.h>

/*
 * Aptus Display Panel CRD050TN02-40NM01
 */
struct clcd_config lcd_config = {
	.address = (struct clcd_registers *)LCD_BASE_ADDRESS,
	.tim0 = 0x57272FC8,
	.tim1 = 0x200D09DF,
	.tim2 = 0x031F0008,
	.tim3 = 0x00010002,
	.cntl = 0x0001302A
};

/* Supports 24 bpp but driver will operate in 32 bpp mode */
vidinfo_t panel_info = {
	.vl_col  = 800,
	.vl_row  = 480,
	.vl_bpix = 5,	/* Bits per pixel, 2^5 = 32 */
	.cmap = (ushort *)LCD_BASE_ADDRESS + LCD_CMAP_OFFSET, /* color map */
	.priv = &lcd_config /* driver-specific data */
};

int clcd_panel_init(vidinfo_t *panel_info)
{
	return 0;
}
