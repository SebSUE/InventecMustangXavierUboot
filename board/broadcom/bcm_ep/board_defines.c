#include "board_defines.h"

const char *boardStateStrings[] = {
	[BS_Off]		= "unknown",
	[BS_Normal]		= "normal",
	[BS_DontUnplug]		= "dont unplug/flashing new fw",
	[BS_HardFailure]	= "hard failure/cannot boot",
	[BS_BootingKernel]	= "booting kernel",
};

