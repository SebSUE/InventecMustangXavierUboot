/*
 * Board specific defines, constants and structures.
 *
 * Copyright (C) 2016 StreamUnlimited
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BOARD_DEFINES_H_
#define _BOARD_DEFINES_H_

#include <common.h>
#include <fwupdate_flags.h>

/** Module version
 */
typedef enum {
	MV_unknown,
} ModuleVersion;

/*extern const char *ModuleVersionStrings[];*/

/** Carrier board type
 */
typedef enum {
	CBT_unknown,
	CBT_Voicebox,				///< SUE factory test board
} CarrierBoardType;

/*extern const char *CarrierBoardTypeStrings[];*/

struct BoardInfo {
	ModuleVersion moduleVersion;
	CarrierBoardType carrierBoardType;
};

extern const char *boardStateStrings[];

/* TODO add implementation, etc. */
ModuleVersion get_module_version(void);
CarrierBoardType get_carrier_board_type(void);
void board_detect(struct BoardInfo*);
void board_printf_info(const struct BoardInfo *);

#endif /* _BOARD_DEFINES_H_ */
