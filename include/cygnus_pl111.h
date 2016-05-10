/*
 *
 * Copyright 2015 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#ifndef _CYGNUS_PL111_H
#define _CYGNUS_PL111_H

#include <common.h>
#include <lcd.h>

#define LCD_BASE_ADDRESS            0x180a0000
#define LCD_CMAP_OFFSET             0x200

/*
 * CLCD Controller Internal Register addresses
 */
struct clcd_registers {
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
	uint32_t curxy;
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
};

/* u-boot specific: information passed by the board file */
struct clcd_config {
	struct clcd_registers *address;
	uint32_t tim0;
	uint32_t tim1;
	uint32_t tim2;
	uint32_t tim3;
	uint32_t cntl;
};

int clcd_panel_init(vidinfo_t *panel_info);
#endif /* _CYGNUS_PL111_H */
