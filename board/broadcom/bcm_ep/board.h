/*
 * Board information header.
 *
 * Copyright (C) 2016 StreamUnlimited
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BOARD_H_
#define __BOARD_H_

#include <common.h>
#include <cpsw.h>
#include <fwupdate_flags.h>
#include "board_defines.h"


struct StreamBoardInterface {
	void (*init) (const struct BoardInfo *board);
	void (*late_init) (const struct BoardInfo *board);
	void (*cpsw_eth_init) (const struct BoardInfo *board, struct cpsw_platform_data *cpsw_data);
	void (*on_board_state_changed) (const struct BoardInfo *board, BoardState state);
	/* return 0, usb update should not start, return 1, usb update should start immediately  */
	int (*do_usb_update) (void);

	/* flags used during pinmux setup - fill in according to peripherals used */
	int has_rmii1;
	int has_rgmii2;
	int has_mmc1;
};

void set_board_interface(struct StreamBoardInterface * interface);
/*void set_board_state(BoardState state);*/
BoardState get_board_state(void);

/* Handling board state change.
 * Should be handled individually for each board (LED indication, ...)
 */
void on_board_state_changed(BoardState state);

extern struct StreamBoardInterface *board_interface;


#endif	/* __BOARD_H_ */
