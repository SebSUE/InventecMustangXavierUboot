/*
 * Firmware update support - flags.
 *
 * Copyright (C) 2016 StreamUnlimited
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __FWUPDATE_FLAGS_H_
#define __FWUPDATE_FLAGS_H_

typedef enum {
	BS_Off = 0,
	BS_Normal,
	BS_DontUnplug,		/* flashing new FW, ... */
	BS_HardFailure,		/* cannot boot, ... */
	BS_BootingKernel,
} BoardState;


#endif /* __FWUPDATE_FLAGS_H_ */
