/*
 * Copyright 2014 Broadcom Corporation.
 * Copyright (C) 2016 StreamUnlimited
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <config.h>
#include <asm/system.h>
#ifdef CONFIG_IPROC_SDHCI
#include <mmc.h>
#include <sdhci.h>
#include <asm/iproc-common/iproc_sdhci.h>
#endif
#include <broadcom/reset_reason_api.h>
#include <asm/persistent_info.h>
#include <bcmiproc_bbl.h>

#ifdef CONFIG_RAW_BMP_LOGO
#include <bmp_raw.h>
#endif

#include <fwupdate_flags.h>
#include "board.h"


DECLARE_GLOBAL_DATA_PTR;

static struct BoardInfo __attribute__((section (".data"))) board = {
	.moduleVersion = MV_unknown,
	.carrierBoardType = CBT_unknown,
};

static BoardState boardState = BS_Off;
struct StreamBoardInterface *board_interface = NULL;

__weak unsigned long get_ramdump_reason(void)
{
	return 0;
}

__weak void clear_ramdump_reason(unsigned long val)
{
	/* Do nothing */
}

__weak int get_reset_reason(void)
{
	return RESET_REASON_UNKNOWN;
}

/*
 * board_init - early hardware init
 */
int board_init(void)
{
	/*
	 * Address of boot parameters passed to kernel
	 * Use default offset 0x100
	 */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	//bbl_init();


	run_command("ramdump", 0);



	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef CONFIG_CMD_SFU_PARSER
	if (RESET_REASON_POWER_ON == get_reset_reason()) {
		printf("Power ON detected, set BootCounter to 0\n");
		fwupdate_setBootCount(0);
		fwupdate_setUpdateFlag(0);
		fwupdate_setFailFlag(0);
		saveenv();
	}

	if (0 > fwupdate_init()) {
		printf("ERROR: fwupdate_init() call failed!\n");
	}
#endif /* CONFIG_CMD_SFU_PARSER */
}
#endif /* CONFIG_BOARD_LATE_INIT */

/*
 * dram_init - sets u-boot's idea of sdram size
 */
int dram_init(void)
{
#if defined(CONFIG_RESERVED_SEC_MEM)
	gd->ram_size = get_ram_size((long int *)CONFIG_SYS_SDRAM_BASE,
			(CONFIG_SYS_SDRAM_SIZE - CONFIG_RESERVED_SEC_MEM));
#else
	gd->ram_size = get_ram_size((long int *)CONFIG_SYS_SDRAM_BASE,
				    CONFIG_SYS_SDRAM_SIZE);
#endif
	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = gd->ram_size;
}

#ifdef CONFIG_BCM_SF2_ETH
int board_eth_init(bd_t *bis)
{
	int rc = -1;
	printf("Registering BCM sf2 eth\n");
	rc = bcm_sf2_eth_register(0);
	return rc;
}
#endif

#ifdef CONFIG_BCMIPROC_ETH
int board_eth_init(bd_t *bis)
{
	int rc = -1;
	printf("Registering eth\n");
	rc = bcmiproc_eth_register(0);
	return rc;
}
#endif

#ifdef CONFIG_IPROC_SDHCI
int board_mmc_init(bd_t *bis)
{
	int ret;

	/* Register SDIO0 */
	ret = iproc_sdhci_init(0, 0);
	if (ret)
		return ret;

	/* Register SDIO1 */
	ret = iproc_sdhci_init(1, 0);
	return ret;
}
#endif

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
void smp_set_core_boot_addr(unsigned long addr, int corenr)
{
}

void smp_kick_all_cpus(void)
{
}

void smp_waitloop(unsigned previous_address)
{
}
#endif

#ifdef CONFIG_SPLASH_SCREEN
#ifdef CONFIG_RAW_BMP_LOGO
int splash_screen_prepare(void)
{
	char bmp_location[10];
	snprintf(bmp_location, 8, "%08X", bmp_data);
	printf("bmp %p\n", bmp_data);
	setenv("splashimage", bmp_location);
	setenv("splashpos", "m,m");
	return 0;
}
#endif
#endif

#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
	char reason_str[20];

	printf("reset reason: %d\n", get_reset_reason());
	snprintf(reason_str, 20, "reset_reason=%d", get_reset_reason());
	setenv("reset_args", reason_str);

	return 0;
}
#endif

/* Handling board state change.
 * Should be handled individually for each board (LED indication, ...)
 */
void on_board_state_changed(BoardState state)
{
	if ((board_interface != NULL) && (board_interface->on_board_state_changed != NULL))
		board_interface->on_board_state_changed(&board, state);
}

void set_board_state(BoardState state)
{
	if (state == get_board_state() && state != 111)
		return;

	boardState = state;
	on_board_state_changed(state);
}

BoardState get_board_state(void)
{
	return boardState;
}

/* change/get board status */
static int do_bstate(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 1 || argc > 2) {
		printf ("Wrong arguments.\nUsage: %s\n", cmdtp->help);
		return 1;
	}

	if (argc == 1) {
		printf("Current board state: '%s'\n", boardStateStrings[get_board_state()]);


		return 0;
	}

	if (strncmp(argv[1], "normal", 5) == 0) {
		set_board_state(BS_Normal);
	} else if (strncmp(argv[1], "dontunplug", 10) == 0) {
		set_board_state(BS_DontUnplug);
	} else if (strncmp(argv[1], "hardfailure", 11) == 0) {
		set_board_state(BS_HardFailure);
	} else if (strncmp(argv[1], "booting", 7) == 0) {
		set_board_state(BS_BootingKernel);
	} else {
		printf ("Invalid parameter: '%s'\n", argv[1]);
		printf ("Usage: %s\n", cmdtp->help);
		return 1;
	}

	return 0;
}

U_BOOT_CMD(
	bstate, 2, 1,  do_bstate,
	"set/get board state",
	"[state]\n" \
	"    state: board state to set\n"\
	"            normal - initial state, running\n"\
	"        dontunplug - do not power off or unplug device (FW update, ...)\n"\
	"       hardfailure - non recoverable error state\n"\
	"           booting - booting kernel\n"
);


#ifdef CONFIG_BOOTCOUNT_LIMIT
void bootcount_store(ulong a)
{
	int status = fwupdate_setBootCount((uint32_t)a);
	
	if (0 != status)
		printf("ERROR: fwupdate_setBootCount() failed!\n");
	else
		printf("BOOTCOUNT is %ld\n", a);
}

ulong bootcount_load(void)
{
	uint8_t bootcount = 0xFF;

	int status = fwupdate_getBootCount(&bootcount);

	if (0 != status)
		printf("ERROR: getBootCount() failed!\n");
	return bootcount;
}
#endif /* CONFIG_BOOTCOUNT_LIMIT */
