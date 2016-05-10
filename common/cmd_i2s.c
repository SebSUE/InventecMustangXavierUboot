/*
 * Start I2S test routine for the BCM958503k.
 */

#include <common.h>
#include <command.h>

int audio_i2s_test(void);

int do_i2s(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	return audio_i2s_test();
}


/**************************************************/
U_BOOT_CMD(i2s, 1, 0, do_i2s,
	   "I2S test", "");


