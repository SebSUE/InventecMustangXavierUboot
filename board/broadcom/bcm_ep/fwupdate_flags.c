/*
 * Firmware Update Flags board specific implementation.
 *
 * Copyright (C) 2016 StreamUnlimited
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/io.h>
#include "board.h"

/* offset indexes in RTC scratch0 register for each flag */
#define UPDATE_FLAG_INDEX		0
#define FAIL_FLAG_INDEX			2
#define BOOT_COUNT_FLAG_INDEX		3

#define SFUPDATE_CNT_MASK		(0x3fffffff)
#define SFUPDATE_CNT_SHIFT		(0)
#define SFUPDATE_FAIL_MASK		(0x40000000)
#define SFUPDATE_FAIL_SHIFT		(30)
#define SFUPDATE_UPDATE_MASK		(0x80000000)
#define SFUPDATE_UPDATE_SHIFT		(31)

#define REG_SFUPDATE			BBL_SEC0_MEM


/*
 * Modify selected flag by its index.
 */
static int flag_write( uint8_t index, uint8_t data)
{
	uint32_t val = 0;
	assert(index <= 3);

	val = getenv_ulong("sfuflags", 10, 0);

	switch (index) {
	case UPDATE_FLAG_INDEX:
		val = (val & ~SFUPDATE_UPDATE_MASK) | ((data << SFUPDATE_UPDATE_SHIFT) & SFUPDATE_UPDATE_MASK);
		break;
	case FAIL_FLAG_INDEX:
		val = (val & ~SFUPDATE_FAIL_MASK) | ((data << SFUPDATE_FAIL_SHIFT) & SFUPDATE_FAIL_MASK);
		break;
	case BOOT_COUNT_FLAG_INDEX:
		val = (val & ~SFUPDATE_CNT_MASK) | ((data << SFUPDATE_CNT_SHIFT) & SFUPDATE_CNT_MASK);
		break;
	default:
		return 1;
	}

	setenv_ulong("sfuflags", val);

	return 0;
}

/*
 * Read selected flag by its index.
 */
static int flag_read(uint8_t index, uint8_t *data)
{
	uint32_t valb = 0;
	assert(index <= 3);

	valb = getenv_ulong("sfuflags", 10, 0);

	switch (index) {
	case UPDATE_FLAG_INDEX:
		*data = (valb & SFUPDATE_UPDATE_MASK) ? 1 : 0;
		break;
	case FAIL_FLAG_INDEX:
		*data = (valb & SFUPDATE_FAIL_MASK) ? 1 : 0;
		break;
	case BOOT_COUNT_FLAG_INDEX:
		*data = valb & SFUPDATE_CNT_MASK;
		break;
	default:
		return 1;
	}

	return 0;
}

int fwupdate_getUpdateFlag(uint8_t *pUpdateFlag)
{
	return flag_read(UPDATE_FLAG_INDEX, pUpdateFlag);
}

int fwupdate_setUpdateFlag(uint8_t updateFlag)
{
	return flag_write(UPDATE_FLAG_INDEX, updateFlag);
}

int fwupdate_getFailFlag(uint8_t* pFailFlag)
{
	return flag_read(FAIL_FLAG_INDEX, pFailFlag);
}

int fwupdate_setFailFlag(uint8_t failFlag)
{
	return flag_write(FAIL_FLAG_INDEX, failFlag);
}

int fwupdate_getBootCount(uint8_t* pBootCnt)
{
	return flag_read(BOOT_COUNT_FLAG_INDEX, pBootCnt);
}

int fwupdate_setBootCount(uint8_t bootCnt)
{
	return flag_write(BOOT_COUNT_FLAG_INDEX, bootCnt);
}

int fwupdate_getUsbUpdateReq(void)
{
	if (board_interface != NULL && board_interface->do_usb_update != NULL)
		return board_interface->do_usb_update();
	//panic("Not supported board!");
	return 0;
}
