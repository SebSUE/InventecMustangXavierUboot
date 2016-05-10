#include <common.h>
#include <post.h>
#include <asm/io.h>

#if defined(CONFIG_CYGNUS)
#if CONFIG_POST & CONFIG_SYS_POST_AUDIO
#include "../bcm/halapi/audio_data.h"
#include "../bcm/halapi/audio_apis.h"

#define cpu_rd_single(x, y) readl(x)
#define cpu_wr_single(x, y, z) writel(y, x)

#define ADD_TEST(X)  { #X, X }
#define TEST_PASS  0
#define TEST_FAIL -1
#define TEST_SKIP  2

#define AUDIO_BASE_ADDRESS 0x180a0000

/* Default Register Value */

extern uint32_t right_channel_samples[480];
extern uint32_t left_channel_samples[480];
extern uint32_t stereo_interleaved_data[960];
extern uint32_t right_channel_samples_0_1[480];
extern uint32_t left_channel_samples_0_1[480];

/*extern uint32_t left_data[1574888];*/
/*extern uint32_t right_data[1574888];*/

typedef struct {
	char *func_name;
	int (*func)(void);
} FUNC_INFO;

int audio_i2s2_output_test_to_stereo_codec(void)
{
	uint32_t i, slave_present;
	uint32_t slave_address, speed_mode;
	uint32_t control_byte1, control_byte2;
	uint32_t *stereo_interleaved_l, *stereo_interleaved_r;
	int smbus_status;

	stereo_interleaved_l = (uint32_t *)0x67000000;
	for (i = 0; i < 480; i++) {
		*stereo_interleaved_l = left_channel_samples[i];
		stereo_interleaved_l++;
	}
	stereo_interleaved_l = (uint32_t *)0x67000000;

	stereo_interleaved_r = (uint32_t *)0x68000000;
	for (i = 0; i < 480; i++) {
		*stereo_interleaved_r = right_channel_samples[i];
		stereo_interleaved_r++;
	}
	stereo_interleaved_r = (uint32_t *)0x68000000;

	/* This test uses Stereo interleaved data to be played back from DDR*/
	/*crmu_reset_related();*/
	audio_sw_reset();
	audio_io_mux_select();
	asiu_audio_clock_gating_disable();
	asiu_audio_pad_enable();
	/*Configuring CRMU PLL CONTROL REGISTER*/
	asiu_audio_gen_pll_pwr_on(1);
	/*user macro set to 48kHz clock*/
	asiu_audio_gen_pll_group_id_config(1, 0x00000046, 0x000C75FF, 0x000000D8, 4, 0, 8, 2, 0, 0);
	post_log("Audio PLL configuration done\n");

	/*Audio BF control source channel configuration*/
	post_log("Source Channel 2 under configuration\n");
	asiu_audio_fmm_bf_ctrl_source_cfg(0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 4);
	post_log("Source Channel 2 configuration done\n");

	/*post_log("Audio Write Address Programmed for ")*/
	/*Audio I2S0 Source Buffer Address Configuration for Stereo Interleaved data*/
	/*asiu_audio_set_source_buffer_addr(4, 1, stereo_interleaved,wr_addr, stereo_interleaved,end_addr, 0, 0, 0, 0, 0, 0, 0, 0);*/
	cpu_wr_single(0x180AE560, 0x67000000, 4);
	cpu_wr_single(0x180AE564, 0x6700077F, 4);
	cpu_wr_single(0x180AE568, 0x67000000, 4);
	cpu_wr_single(0x180AE56C, 0x6700077F, 4);

	cpu_wr_single(0x180AE578, 0x68000000, 4);
	cpu_wr_single(0x180AE57C, 0x6800077F, 4);
	cpu_wr_single(0x180AE580, 0x68000000, 4);
	cpu_wr_single(0x180AE584, 0x6800077F, 4);

	post_log("Source Channel Ring Buffer Addresses Configured\n");

	/*I2S0 Stream output configuration*/
	asiu_audio_i2s_stream_config_samp_count(0, 1, 0, 8, 0, 1, 0, 0, 2, 4);

	asiu_audio_mclk_cfg(1, 2, 4);
	post_log("MCLK and PLLCLKSEL programming done\n");

	/*I2S2 Out Config*/
	asiu_audio_i2s_out_tx_config(1, 1, 0, 0, 1, 0, 24, 0, 0, 0, 2, 0, 0, 4);
	post_log("Done with I2S Out Configuration\n");

	/*I2S Legacy Enable*/
	asiu_audio_i2s_out_legacy_enable(1, 4);
	post_log("I2S port 2 Enabled\n");

	/*SFIFO Enable*/
	asiu_audio_fmm_bf_ctrl_source_cfg(0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 4);

	/*PLAY RUN*/
	asiu_audio_start_dma_read(0, 1, 4);
	post_log("Play Run Enabled\n");

	asiu_audio_i2s_stream_config_samp_count(1, 1, 0, 8, 0, 1, 0, 0, 2, 4);

	speed_mode = 0; /*400 KHz*/
	if (post_get_board_diags_type() == BCM958305K)
		smbus1_init(speed_mode);
	else
		smbus_init(speed_mode);

	post_log("Done with Smbus init\n");

	/*WM8750L Slave Address for the 2-wire interface*/
	slave_address = 0x1A;

	/*Check for SMBUS Slave Presence*/
	if (post_get_board_diags_type() == BCM958305K)
		slave_present = smbus1_slave_presence(slave_address);
	else
		slave_present = smbus_slave_presence(slave_address);

	if (slave_present == 0) {
		post_log("WM8750L Stereo Codec recognized as a Slave\n");
	} else {
		post_log("WM8750L Stereo Codec not recognized as a Slave\n");
		return -1;
	}

	/*program the stereo codec WM8750L - Power up the Stereo Codec - R25*/
	control_byte1 = 0x32;
	control_byte2 = 0xC0 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*program the stereo codec WM8750L - Power up the Stereo Codec - R26*/
	control_byte1 = 0x35;
	control_byte2 = 0xE0 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*program the stereo codec WM8750L - Power up the Stereo Codec - R24*/
	control_byte1 = 0x30;
	control_byte2 = 0x03 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*program the stereo codec WM8750L - DAC LEFT VOLUME REGISTER - R10*/
	control_byte1 = 0x15;
	control_byte2 = 0xff | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*program the stereo codec WM8750L - DAC RIGHT VOLUME REGISTER - R11*/
	control_byte1 = 0x17;
	control_byte2 = 0xff | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*program the DAC and ADC COntrol Register - R5*/
	control_byte1 = 0x0A;
	control_byte2 = 0x00 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*Program the Stereo Codec WM8750L - Left DAC to Left Mixer - R34*/
	control_byte1 = 0x45;
	control_byte2 = 0x50 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*Program the Stereo Codec WM8750L - Right DAC to Right Mixer - R37*/
	control_byte1 = 0x4B;
	control_byte2 = 0x50 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*Program the Stereo Codec WM8750L - LOUT1 Volume - R2*/
	control_byte1 = 0x05;
	control_byte2 = 0x79 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*Program the Stereo Codec WM8750L - ROUT1 Volume - R3*/
	control_byte1 = 0x07;
	control_byte2 = 0x79 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*Program the Stereo Codec WM8750L - R23*/
	control_byte1 = 0x2E;
	control_byte2 = 0xC0 | (1 << 31);
	if (post_get_board_diags_type() == BCM958305K)
		smbus_status = smbus_write(slave_address,control_byte1,control_byte2);
	else
		smbus_status = smbus_wm8750_write(slave_address,control_byte1,control_byte2);

	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	post_log("End of test : check for 1 KHz audio on the left channel and 2 KHz audio on the right channel - audio_i2s2_output_test\n");
	return 0;
}

int audio_i2s0_output_to_stereo_codec_test(void)
{
	uint32_t i;
	uint32_t smbus_slave_address;
	uint32_t gpio_read;
	int slave_present;
	uint32_t control_register;
	uint32_t control_byte;
	int smbus_status;
	uint32_t *stereo_interleaved_l, *stereo_interleaved_r;

	stereo_interleaved_l = (uint32_t *)0x67000000;
	for (i = 0; i < 480; i++) {
		*stereo_interleaved_l = left_channel_samples[i];
		stereo_interleaved_l++;
	}
	stereo_interleaved_l = (uint32_t *)0x67000000;

	stereo_interleaved_r = (uint32_t *)0x68000000;
	for (i = 0; i < 480; i++) {
		*stereo_interleaved_r = right_channel_samples[i];
		stereo_interleaved_r++;
	}
	stereo_interleaved_r = (uint32_t *)0x68000000;



	/*This test uses Stereo interleaved data to be played back from DDR*/
	/*crmu_reset_related();*/
	audio_sw_reset();
	audio_io_mux_select();
	asiu_audio_clock_gating_disable();

	smbus_init(0);
	smbus_slave_address = 0x18;
// [ADK] added here ----------------------------------------------------------
	/*Check for SMBUS Slave Presence*/
	slave_present = smbus_slave_presence(smbus_slave_address);
	if (slave_present == 0) {
		post_log("TI Stereo Codec in daughter card recognized as a Slave\n");
	} else {
		post_log("TI Stereo Codec in daughter card not recognized as a Slave\n");
		return -1;
	}

	/*start writing to the Stereo Codec registers - Page 0 chosen for the following register accesses*/
	control_register = 0x00;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/* Reset */
	control_register = 0x01;
	control_byte = 0x80 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

#if 0
// Test GPIO1 as a GPIO output pin, OK 
while(1) {
	/* Register 98*/
	control_register = 98;
	control_byte = 0x90 | (1 << 31); /**/
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
//		post_log("SMBUS TRANSACTION SUCCESSFUL - GPIO1 Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -GPIO1 Control  Register\n");
		return -1;
	}


	control_register = 98;
	control_byte = 0x91 | (1 << 31); /**/
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
//		post_log("SMBUS TRANSACTION SUCCESSFUL - GPIO1 Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -GPIO1 Control  Register\n");
		return -1;
	}
}
#endif // 0
	/* Register 98*/
	control_register = 98;
	control_byte = 0x28 | (1 << 31); /* GPIO1 - output clock mux with M=1, take clock from the CLKDIV ..*/
//	control_byte = 0x20 | (1 << 31); /* GPIO1 - output clock mux with M=1, take clock from the PLL_OUT*/
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - GPIO1 Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -GPIO1 Control  Register\n");
		return -1;
	}

// Step 1 from Caleb ..

	/* Register 102  CLKDIV from MCLK, PLL from GPIO2, N=2*/
	control_register = 102;
	control_byte = 0x52 | (1 << 31); /* 00010010b */
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Clock Generation Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Clock Generation Control Register\n");
		return -1;
	}

#if 0
	/* Register 3: PLL disable (CODEC_CLKIN=CLKDIV_OUT), Q=2*/
	control_register = 3;
	control_byte = 0x10 | (1 << 31); /* 00010xxxb */
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - PLL Programming Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -PLL Programming Control  Register\n");
		return -1;
	}

#endif

#if 0
	/* Register 99*/
	control_register = 99;
	control_byte = 0x80 | (1 << 31); /**/
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - GPIO2 Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -GPIO2 Control  Register\n");
		return -1;
	}

#endif // 0

// Step 2 from Caleb ..
	smbus_slave_address = 0x19;
	/*start writing to the Stereo Codec registers - Page 0 chosen for the following register accesses*/
	control_register = 0x00;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/* Reset */
	control_register = 0x01;
	control_byte = 0x80 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}
	/* Register 102  CLKDIV from MCLK, PLL from MCLK */
	control_register = 102;
	control_byte = 0x02 | (1 << 31); /* 00000010b */
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL 0x19 - Clock Generation Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Clock Generation Control Register\n");
		return -1;
	}
	
	smbus_slave_address = 0x1a;
	/*start writing to the Stereo Codec registers - Page 0 chosen for the following register accesses*/
	control_register = 0x00;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/* Reset */
	control_register = 0x01;
	control_byte = 0x80 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}
	/* Register 102  CLKDIV from MCLK, PLL from MCLK */
	control_register = 102;
	control_byte = 0x02 | (1 << 31); /* 00000010b */
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL  0x1a - Clock Generation Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Clock Generation Control Register\n");
		return -1;
	}

	smbus_slave_address = 0x1b;

	/*start writing to the Stereo Codec registers - Page 0 chosen for the following register accesses*/
	control_register = 0x00;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}
	/* Reset */
	control_register = 0x01;
	control_byte = 0x80 | (1 << 31);
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}
	/* Register 102  CLKDIV from MCLK, PLL from MCLK */
	control_register = 102;
	control_byte = 0x02 | (1 << 31); /* 00000010b */
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL  0x1b - Clock Generation Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Clock Generation Control Register\n");
		return -1;
	}

// Step 3 from Caleb ..
	smbus_slave_address = 0x18;
	/* Register 98*/
	control_register = 98;
	control_byte = 0x20 | (1 << 31); /**/
	smbus_status = smbus_wm8750_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - GPIO1 Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -GPIO1 Control  Register\n");
		return -1;
	}
// -----------------------------------------------------------------

	asiu_audio_pad_enable();

	/*Configuring CRMU PLL CONTROL REGISTER*/
	asiu_audio_gen_pll_pwr_on(1);

	/*user macro set to 48kHz clock*/
	asiu_audio_gen_pll_group_id_config(1, 0x00000046, 0x000C75FF, 0x000000D8, 4, 0, 8, 2, 0, 0);
	post_log("Audio PLL configuration done\n");

// goto mclk_cfg;


goto mclk_cfg;

	/*Audio BF control source channel configuration*/
	post_log("Source Channel 0 under configuration\n");
	asiu_audio_fmm_bf_ctrl_source_cfg(0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1);
	post_log("Source Channel 0 configuration done\n");

	/*post_log("Audio Write Address Programmed for ")*/
	/*Audio I2S0 Source Buffer Address Configuration for Stereo Interleaved data*/
	/*asiu_audio_set_source_buffer_addr(4, 1, stereo_interleaved,wr_addr, stereo_interleaved,end_addr, 0, 0, 0, 0, 0, 0, 0, 0);*/
	cpu_wr_single(0x180AE500, 0x67000000, 4);
	cpu_wr_single(0x180AE504, 0x6700077F, 4);
	cpu_wr_single(0x180AE508, 0x67000000, 4);
	cpu_wr_single(0x180AE50C, 0x6700077F, 4);

	cpu_wr_single(0x180AE518, 0x68000000, 4);
	cpu_wr_single(0x180AE51C, 0x6800077F, 4);
	cpu_wr_single(0x180AE520, 0x68000000, 4);
	cpu_wr_single(0x180AE524, 0x6800077F, 4);

	post_log("Source Channel Ring Buffer Addresses Configured\n");

	/*I2S0 Stream output configuration*/
	asiu_audio_i2s_stream_config_samp_count(0, 1, 0, 8, 0, 1, 0, 0, 0, 1);
	
mclk_cfg:
	asiu_audio_mclk_cfg(1, 2, 1);
	post_log("MCLK and PLLCLKSEL programming done\n");
	
return 0;

	/*I2S0 Out Config*/
	asiu_audio_i2s_out_tx_config(1, 1, 0, 0, 1, 0, 24, 0, 0, 0, 2, 0, 0, 1);
	post_log("Done with I2S Out Configuration\n");

	/*I2S Legacy Enable*/
	asiu_audio_i2s_out_legacy_enable(1, 1);
	post_log("I2S port 2 Enabled\n");

	/*SFIFO Enable*/
	asiu_audio_fmm_bf_ctrl_source_cfg(0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1);

	/*PLAY RUN*/
	asiu_audio_start_dma_read(0, 1, 1);
	post_log("Play Run Enabled\n");

	asiu_audio_i2s_stream_config_samp_count(1, 1, 0, 8, 0, 1, 0, 0, 0, 1);

	if ((post_get_board_diags_type() == BCM958300K) || (post_get_board_diags_type() == BCM958302K)) {
	/* send a reset to the TI stereo codec
	 * To Select SC0_DC_GPIOx_3P3, AON_GPIO1 and AON_GPIO0
	 * should be driven as '1' and '0'
	 */
	cpu_wr_single(GP_OUT_EN, 0x3, 4);
	cpu_wr_single(GP_DATA_OUT, 0x2, 4);
	post_log("AON_GPIO1 driven 1 and AON_GPIO0 driven 0\n");

	/* SC0_DC_GPIO1_3P3 is the reset to the TI stereo codec - Active low reset
	 * This signal has to be made '0', add some delay and then again made '1'
	 * This signal has to be driven from the ASIU GPIO registers
	 * GPIO1_3P3 corresponds to CHIP_GPIO_142 - GPIO - 0 to 23 are from IPROC
	 * GPIO1_3P3 corresponds to ASIU_GPIO_118
	 */
	cpu_wr_single(ASIU_GP_OUT_EN_3, 0x400000, 4);
	cpu_wr_single(ASIU_GP_DATA_OUT_3, 0x000000, 4);
	udelay(800000);
	cpu_wr_single(ASIU_GP_DATA_OUT_3, 0x400000, 4);
	post_log("TI stereo codec on the daughter card has been reset\n");

	/* External HeadSet Enable - SC0_DC_GPIO0_3P3
	 * This signal has to be driven as '1'
	 * This GPIO0_3P3 corresponds to ASIU_GPIO_117
	 */
	gpio_read = cpu_rd_single(ASIU_GP_OUT_EN_3, 4);
	gpio_read = gpio_read | 0x200000;
	cpu_wr_single(ASIU_GP_OUT_EN_3, gpio_read, 4);

	gpio_read = cpu_rd_single(ASIU_GP_DATA_OUT_3, 4);
	gpio_read = gpio_read | 0x200000;
	cpu_wr_single(ASIU_GP_DATA_OUT_3, gpio_read, 4);
	post_log("External Headset Enable has been asserted 1 - SC0_DC_GPIO0_3P3\n");

	/* HandsFree Speaker Enable - SC0_DC_GPIO2_3P3
	 * This signal has to be driven as '1'
	 * This GPIO2_3P3 corresponds to ASIU_GPIO_119
	 */
	gpio_read = cpu_rd_single(ASIU_GP_OUT_EN_3, 4);
	gpio_read = gpio_read | 0x800000;
	cpu_wr_single(ASIU_GP_OUT_EN_3, gpio_read, 4);

	gpio_read = cpu_rd_single(ASIU_GP_DATA_OUT_3, 4);
	gpio_read = gpio_read | 0x800000;
	cpu_wr_single(ASIU_GP_DATA_OUT_3, gpio_read, 4);
	post_log("Handsfree Speaker Enable has been asserted 1 - SC0_DC_GPIO2_3P3\n");

	} else if ((post_get_board_diags_type() == BCM958305K)) {

		//send a reset to the TI stereo codec 

		cpu_wr_single(CRMU_IOMUX_CTRL6,0x03333333,4);
		//SC0_DC_GPIO1_3P3 is the reset to the TI stereo codec - Active low reset
		//This signal has to be made '0' , add some delay and then again made '1'
		//This signal has to be driven from the ASIU GPIO registers
		//GPIO1_3P3 corresponds to CHIP_GPIO_142 - GPIO - 0 to 23 are from IPROC
		//GPIO1_3P3 corresponds to ASIU_GPIO_118
		cpu_wr_single(ASIU_GP_OUT_EN_3,0x80,4);
		cpu_wr_single(ASIU_GP_DATA_OUT_3,0x000000,4);
		udelay(800000);
		cpu_wr_single(ASIU_GP_DATA_OUT_3,0x80,4);
		post_log("TI stereo codec on the daughter card has been reset\n");

		//External HeadSet Enable - SC0_DC_GPIO0_3P3
		//This signal has to be driven as '1'
		//This GPIO0_3P3 corresponds to ASIU_GPIO_117
		gpio_read = cpu_rd_single(ASIU_GP_OUT_EN_3,4);
		gpio_read = gpio_read | 0x40;
		cpu_wr_single(ASIU_GP_OUT_EN_3,gpio_read,4);

		gpio_read = cpu_rd_single(ASIU_GP_DATA_OUT_3,4);
		gpio_read = gpio_read | 0x40;
		cpu_wr_single(ASIU_GP_DATA_OUT_3,gpio_read,4);
		post_log("External Headset Enable has been asserted 1 - SC0_DC_GPIO0_3P3 \n");

		////HandsFree Speaker Enable - SC0_DC_GPIO2_3P3
		//This signal has to be driven as '1'
		//This GPIO2_3P3 corresponds to ASIU_GPIO_119
		gpio_read = cpu_rd_single(ASIU_GP_OUT_EN_3,4);
		gpio_read = gpio_read | 0x100;
		cpu_wr_single(ASIU_GP_OUT_EN_3,gpio_read,4);

		gpio_read = cpu_rd_single(ASIU_GP_DATA_OUT_3,4);
		gpio_read = gpio_read | 0x100;
		cpu_wr_single(ASIU_GP_DATA_OUT_3,gpio_read,4);
		post_log("Handsfree Speaker Enable has been asserted 1 - SC0_DC_GPIO2_3P3 \n");

	} else if ((post_get_board_diags_type() == BCM911360K)) {
		/* send a reset to the TI stereo codec
		To Select I2S0_DUT_GPIOx, AON_GPIO1 and AON_GPIO0 should be driven as '0' and '0' */
		cpu_wr_single(GP_OUT_EN, 0x3, 4);
		cpu_wr_single(GP_DATA_OUT, 0x0, 4);/*0x2 is the value on combo and pos...0x0 is the value on VoIP*/
		post_log("AON_GPIO1 driven 0 and AON_GPIO0 driven 0\n");

		/* The below description is on Combo/POS
		   I2S0_DUT_GPIO6 is the reset to the TI stereo codec - Active low reset
		   This signal has to be made '0' , add some delay and then again made '1'
		   This signal has to be driven from the ASIU GPIO registers
		 */

		/* For the VoIP board
		   cpu_wr_single(ASIU_GP_OUT_EN_3,0x400000,4);
		   cpu_wr_single(ASIU_GP_DATA_OUT_3,0x000000,4);
		 */

		/* On the VOIP board - DUT_GPIO6 is the reset */
		cpu_wr_single(ChipcommonG_GP_OUT_EN, 0x40, 4);
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, 0x000000, 4);
		udelay(800000);
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, 0x40, 4);
		post_log("TI stereo codec on the daughter card has been reset\n");

		/* External HeadSet Enable - I2S0_DUT_GPIO5
		   This signal has to be driven as '1' */

		gpio_read = cpu_rd_single(ChipcommonG_GP_OUT_EN, 4);
		gpio_read = gpio_read | 0x20;
		cpu_wr_single(ChipcommonG_GP_OUT_EN, gpio_read, 4);

		gpio_read = cpu_rd_single(ChipcommonG_GP_DATA_OUT, 4);
		gpio_read = gpio_read | 0x20;
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, gpio_read, 4);
		post_log("External Headset Enable has been asserted 1 - I2S0_DUT_GPIO5\n");

		/* HandsFree Speaker Enable - I2S0_DUT_GPIO7
		   This signal has to be driven as '1' */

		gpio_read = cpu_rd_single(ChipcommonG_GP_OUT_EN, 4);
		gpio_read = gpio_read | 0x80;
		cpu_wr_single(ChipcommonG_GP_OUT_EN, gpio_read, 4);

		gpio_read = cpu_rd_single(ChipcommonG_GP_DATA_OUT, 4);
		gpio_read = gpio_read | 0x80;
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, gpio_read, 4);
		post_log("Handsfree Speaker Enable has been asserted 1 - I2S0_DUT_GPIO7\n");
	}

	/*Check for SMBUS Slave Presence*/
	slave_present = smbus1_slave_presence(smbus_slave_address);
	if (slave_present == 0) {
		post_log("TI Stereo Codec in daughter card recognized as a Slave\n");
	} else {
		post_log("TI Stereo Codec in daughter card not recognized as a Slave\n");
		return -1;
	}

	/*start writing to the Stereo Codec registers - Page 0 chosen for the following register accesses*/
	control_register = 0x00;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*page 0 - Register 7 - Codec Datapath Setup Register*/
	control_register = 0x07;
	control_byte = 0x0A | (1 << 31); /*00001010*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Codec Datapath register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - Codec Datapath register write not succesful\n");
		return -1;
	}

	/*this needs to be verified twice*/
	/*age 0 - register 9 - Audio Serial Interface Control Register B - program the audio data bit to be 24 bits and 256 clock mode*/
	control_register = 0x09;
	control_byte = 0x20 | (1 << 31); /*00101000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Audio Serial Interface Control Register B\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - Audio Serial Interface Control Register B\n");
		return -1;
	}

	/*page 0 - Register 14  - Make the Stereo Output driver - fully differential*/
	control_register = 0x0E;
	control_byte = 0x40 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Stereo Output driver - fully differential\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - Stereo Output driver - fully differential\n");
		return -1;
	}

	/*control_register = 0x10;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - right ADC Gain\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - right ADc Gain\n");
		return -1;
	}*/

	/*Page 0 - REgister 37 - Power up Left and right DACs*/
	control_register = 37;
	control_byte = 0xC0 | (1 << 31); /*11000000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - DACs Powered Up\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - DACs not powered up\n");
		return -1;
	}

	/*Page 0 - REgister 40 - VCM setting*/
	control_register = 40;
	control_byte = 0x80 | (1 << 31); /*11000000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - VCM Setting Done\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - VCM Setting Done\n");
		return -1;
	}

	/*Page 0 - REgister 41 - Route DAC to*/
	control_register = 41;
	control_byte = 0x50 | (1 << 31); /*01010000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - DAC Routing Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - DAC Routing Register\n");
		return -1;
	}

	/*page0 - Register 43 - Left DAC Volume Control REgister*/
	control_register = 43;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Left DAC Volume Control REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Left DAC Volume Control REgister\n");
		return -1;
	}

	/*page0 - Register 44 - Right DAC Volume Control REgister*/
	control_register = 44;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Right DAC Volume Control REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Right DAC Volume Control REgister\n");
		return -1;
	}

	/*Left LOP - Register 86*/
	control_register = 86;
	control_byte = 0x0B | (1 << 31); /*00001010*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Left LOPP Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Left LOP Volume Control Register\n");
		return -1;
	}

	/*Right LOM - Register 93*/
	control_register = 93;
	control_byte = 0x0B | (1 << 31); /*00001010*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Right LOM Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Right LOM Volume Control Register\n");
		return -1;
	}

	/*Just have an eye for the page 0 - REgister 102 - clock division register*/
	post_log("End of test : check for 1 KHz audio on the left channel and 2 KHz audio on the right channel - audio_i2s0_output_test\n");
	return TEST_PASS;
}

int audio_i2s1_output_to_stereo_codec_test(void)
{
	uint32_t i;
	uint32_t smbus_slave_address;
	uint32_t gpio_read;
	int slave_present;
	uint32_t control_register;
	uint32_t control_byte;
	int smbus_status;
	uint32_t *stereo_interleaved_l, *stereo_interleaved_r;

	stereo_interleaved_l = (uint32_t *)0x67000000;
	for (i = 0; i < 480; i++) {
		*stereo_interleaved_l = left_channel_samples[i];
		stereo_interleaved_l++;
	}
	stereo_interleaved_l = (uint32_t *)0x67000000;

	stereo_interleaved_r = (uint32_t *)0x68000000;
	for (i = 0; i < 480; i++) {
		*stereo_interleaved_r = right_channel_samples[i];
		stereo_interleaved_r++;
	}
	stereo_interleaved_r = (uint32_t *)0x68000000;

	/*This test uses Stereo interleaved data to be played back from DDR*/
	/*crmu_reset_related();*/
	audio_sw_reset();
	audio_io_mux_select();
	asiu_audio_clock_gating_disable();
	asiu_audio_pad_enable();
	/*Configuring CRMU PLL CONTROL REGISTER*/
	asiu_audio_gen_pll_pwr_on(1);
	/*user macro set to 48kHz clock*/
	asiu_audio_gen_pll_group_id_config(1, 0x00000046, 0x000C75FF, 0x000000D8, 4, 0, 8, 2, 0, 0);
	post_log("Audio PLL configuration done\n");
	smbus1_init(1);
	smbus_slave_address = 0x18;


	/*Audio BF control source channel configuration*/
	post_log("Source Channel 1 under configuration\n");
	asiu_audio_fmm_bf_ctrl_source_cfg(0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 2);
	post_log("Source Channel 1 configuration done\n");

	/* post_log("Audio Write Address Programmed for ")
	 * Audio I2S0 Source Buffer Address Configuration for Stereo
	 * Interleaved data
	 */
	/*asiu_audio_set_source_buffer_addr(4, 1, stereo_interleaved,wr_addr, stereo_interleaved,end_addr, 0, 0, 0, 0, 0, 0, 0, 0);*/
	cpu_wr_single(0x180AE530, 0x67000000, 4);
	cpu_wr_single(0x180AE534, 0x6700077F, 4);
	cpu_wr_single(0x180AE538, 0x67000000, 4);
	cpu_wr_single(0x180AE53C, 0x6700077F, 4);

	cpu_wr_single(0x180AE548, 0x68000000, 4);
	cpu_wr_single(0x180AE54C, 0x6800077F, 4);
	cpu_wr_single(0x180AE550, 0x68000000, 4);
	cpu_wr_single(0x180AE554, 0x6800077F, 4);

	post_log("Source Channel Ring Buffer Addresses Configured\n");

	/*I2S0 Stream output configuration*/
	asiu_audio_i2s_stream_config_samp_count(0, 1, 0, 8, 0, 1, 0, 0, 1, 2);

	asiu_audio_mclk_cfg(1, 2, 2);
	post_log("MCLK and PLLCLKSEL programming done\n");

	/*I2S2 Out Config*/
	asiu_audio_i2s_out_tx_config(1, 1, 0, 0, 1, 0, 24, 0, 0, 0, 2, 0, 0, 2);
	post_log("Done with I2S Out Configuration\n");

	/*I2S Legacy Enable*/
	asiu_audio_i2s_out_legacy_enable(1, 2);
	post_log("I2S port 1 Enabled\n");

	/*SFIFO Enable*/
	asiu_audio_fmm_bf_ctrl_source_cfg(0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2);

	/*PLAY RUN*/
	asiu_audio_start_dma_read(0, 1, 2);
	post_log("Play Run Enabled\n");

	asiu_audio_i2s_stream_config_samp_count(1, 1, 0, 8, 0, 1, 0, 0, 1, 2);

	if ((post_get_board_diags_type() == BCM958300K) || (post_get_board_diags_type() == BCM958302K)) {
	/*send a reset to the TI stereo codec
	 * To Select SC0_DC_GPIOx_3P3, AON_GPIO1 and AON_GPIO0
	 * should be driven as '1' and '0'
	 */
	cpu_wr_single(GP_OUT_EN, 0x3, 4);
	cpu_wr_single(GP_DATA_OUT, 0x1, 4);  /*modified by sathishs*/
	post_log("AON_GPIO1 driven 0 and AON_GPIO0 driven 1\n");

	cpu_wr_single(ASIU_GP_OUT_EN_3, 0x400000, 4);
	cpu_wr_single(ASIU_GP_DATA_OUT_3, 0x000000, 4);
	udelay(800000);
	cpu_wr_single(ASIU_GP_DATA_OUT_3, 0x400000, 4);
	post_log("TI stereo codec on the daughter card has been reset\n");

	/* External HeadSet Enable - SC0_DC_GPIO0_3P3
	 * This signal has to be driven as '1'
	 * This GPIO0_3P3 corresponds to ASIU_GPIO_117
	 */
	gpio_read = cpu_rd_single(ASIU_GP_OUT_EN_3, 4);
	gpio_read = gpio_read | 0x200000;
	cpu_wr_single(ASIU_GP_OUT_EN_3, gpio_read, 4);

	gpio_read = cpu_rd_single(ASIU_GP_DATA_OUT_3, 4);
	gpio_read = gpio_read | 0x200000;
	cpu_wr_single(ASIU_GP_DATA_OUT_3, gpio_read, 4);
	post_log("External Headset Enable has been asserted 1 - SC0_DC_GPIO0_3P3\n");

	/* HandsFree Speaker Enable - SC0_DC_GPIO2_3P3
	 * This signal has to be driven as '1'
	 * This GPIO2_3P3 corresponds to ASIU_GPIO_119
	 */
	gpio_read = cpu_rd_single(ASIU_GP_OUT_EN_3, 4);
	gpio_read = gpio_read | 0x800000;
	cpu_wr_single(ASIU_GP_OUT_EN_3, gpio_read, 4);

	gpio_read = cpu_rd_single(ASIU_GP_DATA_OUT_3, 4);
	gpio_read = gpio_read | 0x800000;
	cpu_wr_single(ASIU_GP_DATA_OUT_3, gpio_read, 4);
	post_log("Handsfree Speaker Enable has been asserted 1 - SC0_DC_GPIO2_3P3\n");

	} else if ((post_get_board_diags_type() == BCM958305K)) {

		cpu_wr_single(CRMU_IOMUX_CTRL2,0x0,4);

		//send a reset to the TI stereo codec

		/* The below description is on Combo/POS
		//I2S1_DUT_GPIO6 is the reset to the TI stereo codec - Active low reset
		//This signal has to be made '0' , add some delay and then again made '1'
		//This signal has to be driven from the ASIU GPIO registers
		 */

		//On the VOIP board - DUT_GPIO6 is the reset 
		cpu_wr_single(ChipcommonG_GP_OUT_EN,0x400000,4);
		cpu_wr_single(ChipcommonG_GP_DATA_OUT,0x000000,4);
		udelay(800000);
		cpu_wr_single(ChipcommonG_GP_DATA_OUT,0x400000,4);
		post_log("TI stereo codec on the daughter card has been reset\n");

		//External HeadSet Enable - I2S1_DUT_GPIO5
		//This signal has to be driven as '1'

		gpio_read = cpu_rd_single(ChipcommonG_GP_OUT_EN,4);
		gpio_read = gpio_read | 0x200000;
		cpu_wr_single(ChipcommonG_GP_OUT_EN,gpio_read,4);

		gpio_read = cpu_rd_single(ChipcommonG_GP_DATA_OUT,4);
		gpio_read = gpio_read | 0x200000;
		cpu_wr_single(ChipcommonG_GP_DATA_OUT,gpio_read,4);
		post_log("External Headset Enable has been asserted 1 - I2S1_DUT_GPIO5 \n");

		////HandsFree Speaker Enable - I2S1_DUT_GPIO7
		//This signal has to be driven as '1'

		gpio_read = cpu_rd_single(ChipcommonG_GP_OUT_EN,4);
		gpio_read = gpio_read | 0x2000;
		cpu_wr_single(ChipcommonG_GP_OUT_EN,gpio_read,4);

		gpio_read = cpu_rd_single(ChipcommonG_GP_DATA_OUT,4);
		gpio_read = gpio_read | 0x2000;
		cpu_wr_single(ChipcommonG_GP_DATA_OUT,gpio_read,4);
		post_log("Handsfree Speaker Enable has been asserted 1 - I2S1_DUT_GPIO7 \n");

	} else if ((post_get_board_diags_type() == BCM911360K)) {
		/* send a reset to the TI stereo codec
		   To Select I2S1_DUT_GPIOx, AON_GPIO1 and AON_GPIO0 should be driven as '1' and '0' */
		cpu_wr_single(GP_OUT_EN, 0x3, 4);
		cpu_wr_single(GP_DATA_OUT, 0x2, 4);   /* 0x2 is the value on combo and pos...0x2 is the value on VoIP */
		post_log("AON_GPIO1 driven 1 and AON_GPIO0 driven 0\n");

		/* The below description is on Combo/POS
		   I2S1_DUT_GPIO6 is the reset to the TI stereo codec - Active low reset
		   This signal has to be made '0' , add some delay and then again made '1'
		   This signal has to be driven from the ASIU GPIO registers
		 */

		/* On the VOIP board - DUT_GPIO6 is the reset */
		cpu_wr_single(ChipcommonG_GP_OUT_EN, 0x40, 4);
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, 0x000000, 4);
		udelay(800000);
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, 0x40, 4);
		post_log("TI stereo codec on the daughter card has been reset\n");

		/* External HeadSet Enable - I2S1_DUT_GPIO5
		   This signal has to be driven as '1' */

		gpio_read = cpu_rd_single(ChipcommonG_GP_OUT_EN, 4);
		gpio_read = gpio_read | 0x20;
		cpu_wr_single(ChipcommonG_GP_OUT_EN, gpio_read, 4);

		gpio_read = cpu_rd_single(ChipcommonG_GP_DATA_OUT, 4);
		gpio_read = gpio_read | 0x20;
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, gpio_read, 4);
		post_log("External Headset Enable has been asserted 1 - I2S1_DUT_GPIO5\n");

		/* HandsFree Speaker Enable - I2S1_DUT_GPIO7
		   This signal has to be driven as '1' */

		gpio_read = cpu_rd_single(ChipcommonG_GP_OUT_EN, 4);
		gpio_read = gpio_read | 0x80;
		cpu_wr_single(ChipcommonG_GP_OUT_EN, gpio_read, 4);

		gpio_read = cpu_rd_single(ChipcommonG_GP_DATA_OUT, 4);
		gpio_read = gpio_read | 0x80;
		cpu_wr_single(ChipcommonG_GP_DATA_OUT, gpio_read, 4);
		post_log("Handsfree Speaker Enable has been asserted 1 - I2S1_DUT_GPIO7\n");
	}

	/*Check for SMBUS Slave Presence*/
	slave_present = smbus1_slave_presence(smbus_slave_address);
	if (slave_present == 0) {
		post_log("TI Stereo Codec in daughter card recognized as a Slave\n");
	} else {
		post_log("TI Stereo Codec in daughter card not recognized as a Slave\n");
		return -1;
	}

	/*start writing to the Stereo Codec registers - Page 0 chosen for the following register accesses*/
	control_register = 0x00;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Page Select REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL\n");
		return -1;
	}

	/*page 0 - Register 7 - Codec Datapath Setup Register*/
	control_register = 0x07;
	control_byte = 0x0A | (1 << 31); /*00001010*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Codec Datapath register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - Codec Datapath register write not succesful\n");
		return -1;
	}

	/* this needs to be verified twice
	 * page 0 - register 9 - Audio Serial Interface Control Register B
	 * - program the audio data bit to be 24 bits and 256 clock mode
	 */
	control_register = 0x09;
	control_byte = 0x20 | (1 << 31); /*00101000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Audio Serial Interface Control Register B\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - Audio Serial Interface Control Register B\n");
		return -1;
	}

	/*page 0 - Register 14  - Make the Stereo Output driver - fully differential*/
	control_register = 0x0E;
	control_byte = 0x40 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Stereo Output driver - fully differential\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - Stereo Output driver - fully differential\n");
		return -1;
	}

	/*control_register = 0x10;
	control_byte = 0x00 | (1 << 31);
		smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - right ADC Gain\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - right ADc Gain\n");
		return -1;
	}*/

	/*Page 0 - REgister 37 - Power up Left and right DACs*/
	control_register = 37;
	control_byte = 0xC0 | (1 << 31); /*11000000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - DACs Powered Up\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - DACs not powered up\n");
		return -1;
	}

	/*Page 0 - REgister 40 - VCM setting*/
	control_register = 40;
	control_byte = 0x80 | (1 << 31); /*11000000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - VCM Setting Done\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - VCM Setting Done\n");
		return -1;
	}

	/*Page 0 - REgister 41 - Route DAC to*/
	control_register = 41;
	control_byte = 0x50 | (1 << 31); /*01010000*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - DAC Routing Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL - DAC Routing Register\n");
		return -1;
	}

	/*page0 - Register 43 - Left DAC Volume Control REgister*/
	control_register = 43;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Left DAC Volume Control REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Left DAC Volume Control REgister\n");
		return -1;
	}

	/*page0 - Register 44 - Right DAC Volume Control REgister*/
	control_register = 44;
	control_byte = 0x00 | (1 << 31);
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Right DAC Volume Control REgister\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Right DAC Volume Control REgister\n");
		return -1;
	}

	/*Left LOP - Register 86*/
	control_register = 86;
	control_byte = 0x0B | (1 << 31); /*00001010*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Left LOPP Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Left LOP Volume Control Register\n");
		return -1;
	}

	/*Right LOM - Register 93*/
	control_register = 93;
	control_byte = 0x0B | (1 << 31); /*00001010*/
	smbus_status = smbus_write(smbus_slave_address, control_register, control_byte);
	if (smbus_status == 0) {
		post_log("SMBUS TRANSACTION SUCCESSFUL - Right LOM Control Register\n");
	} else {
		post_log("SMBUS TRANSACTION NOT SUCCESSFUL -Right LOM Volume Control Register\n");
		return -1;
	}

	post_log("End of test : check for 1 KHz audio on the left channel and 2 KHz audio on the right channel - audio_i2s1_output_test\n");
	return TEST_PASS;
}

int audio_i2s_test(void)
{
	int i2s =0, status = 0;

/*
	post_log("\n\nSelect I2S interface: 0 - I2S0; 1 - I2S1 2 - I2S2\n");
	i2s = post_getUserInput("I2S i/f? (0/1/2) : ");
*/
	if (i2s == 0)
		status = audio_i2s0_output_to_stereo_codec_test();
#if 0
	if (i2s == 1)
		status = audio_i2s1_output_to_stereo_codec_test();

	if (i2s == 2)
		status = audio_i2s2_output_test_to_stereo_codec();
#endif
	return status;
}

/******* Internal test function start ********/
int AUDIO_post_test(int flags)
{

	/*
	lcd_clk_gate();
	lcd_asiu_mipi_dsi_gen_pll_pwr_on_config();
	lcd_ctrl_init(0x61000000,&config);
	*/

#if 1
	int status = TEST_PASS;
	if (TEST_FAIL == audio_i2s_test())
		status = TEST_FAIL;
	return status;
#else
	uint32_t test_no;
	int loop_count = 0, total_tests = 0, total_reg_tests = 0;
	int status = 0, autorun_status = 0, pass = 0, fail = 0, skip = 0;

	FUNC_INFO function[] = {
		ADD_TEST(audio_soft_reset_test),
		ADD_TEST(audio_dte_i2s0_bitclk_test),
		ADD_TEST(audio_dte_i2s1_bitclk_test),
		ADD_TEST(audio_dte_i2s2_bitclk_test),
		ADD_TEST(audio_dte_i2s0_wordclk_test),
		ADD_TEST(audio_dte_i2s1_wordclk_test),
		ADD_TEST(audio_dte_i2s2_wordclk_test),
		ADD_TEST(audio_dte_external_input_test_11),
		ADD_TEST(audio_dte_external_input_test_12),
		ADD_TEST(audio_dte_external_input_test_13),
		ADD_TEST(audio_dte_external_input_test_14),
		ADD_TEST(audio_dte_i2s0_bitclk_interrupt_test),
		ADD_TEST(audio_i2s0_output_test),
		ADD_TEST(audio_i2s1_output_test),
		ADD_TEST(audio_i2s2_output_test),
		ADD_TEST(audio_spdif_out_test),
		ADD_TEST(asiu_audio_sch0_internal_loopback_test_ddr),
		ADD_TEST(asiu_audio_sch0_internal_loopback_test_sram),
		ADD_TEST(asiu_audio_sch1_internal_loopback_test_ddr),
		ADD_TEST(asiu_audio_sch1_internal_loopback_test_sram),
		ADD_TEST(asiu_audio_sch2_internal_loopback_test_ddr),
		ADD_TEST(asiu_audio_sch2_internal_loopback_test_sram),
		ADD_TEST(audio_i2s2_output_test_to_stereo_codec),
		ADD_TEST(audio_i2s2_output_test_to_stereo_codec_tdm_mode),
		ADD_TEST(audio_i2s0_output_to_stereo_codec_test),
		ADD_TEST(audio_i2s0_output_to_stereo_codec_test_tdm_mode),
		ADD_TEST(audio_i2s1_output_to_stereo_codec_test),
		ADD_TEST(audio_i2s1_output_to_stereo_codec_test_tdm_mode),
		ADD_TEST(audio_i2s2_in_master_test),
		ADD_TEST(audio_i2s1_in_master_test),
		ADD_TEST(audio_i2s0_in_master_test),
		ADD_TEST(audio_i2s2_in_slave_test),
		ADD_TEST(audio_i2s1_in_slave_test),
		ADD_TEST(audio_i2s0_in_slave_test),
		ADD_TEST(audio_i2s2_in_i2s0_out_test),
		ADD_TEST(audio_i2s2_output_test_slave_mode),
		ADD_TEST(audio_i2s1_output_test_slave_mode),
		ADD_TEST(audio_i2s0_output_test_slave_mode)
	};

	FUNC_INFO regression_func[] = {
		ADD_TEST(audio_soft_reset_test),
		ADD_TEST(audio_dte_i2s0_bitclk_test),
		ADD_TEST(audio_dte_i2s1_bitclk_test),
		ADD_TEST(audio_dte_i2s2_bitclk_test),
		ADD_TEST(audio_dte_i2s0_wordclk_test),
		ADD_TEST(audio_dte_i2s1_wordclk_test),
		ADD_TEST(audio_dte_i2s2_wordclk_test),
	};

	total_tests = (sizeof(function) / sizeof(function[0]));
	total_reg_tests = (sizeof(regression_func) / sizeof(regression_func[0]));

	if (flags & POST_REGRESSION) {
		for (loop_count = 0; loop_count  < total_reg_tests; loop_count++) {
			post_log("\nExecuting test %2d - %s\n", (loop_count + 1), regression_func[loop_count].func_name);

			status = (*regression_func[loop_count].func)();
			switch (status) {
			case TEST_PASS:
				t_result->t_pass++;
				break;
			case TEST_FAIL:
				t_result->t_fail++;
				break;
			case TEST_SKIP:
				t_result->t_skip++;
				break;
			}
		}
		return 0;
	} else {
		post_log("Number of test : %d", total_tests);
		do {
			for (loop_count = 0; loop_count  < total_tests; loop_count++)
				post_log("\n%2d %s", (loop_count + 1), function[loop_count].func_name);
			post_log("\n%2d run all tests", (total_tests + 1));
			post_log("\n%2d exit\n", (total_tests + 2));
			test_no = post_getUserInput ("\nEnter Test Number : ");

			if (test_no > (total_tests + 2)) {
				post_log("ERROR - invalid entry\n");
			} else {
				if (test_no == (total_tests + 1)) {
					autorun_status = 0;
					pass = 0;
					fail = 0;
					skip = 0;
					for (loop_count = 0; loop_count  < total_tests; loop_count++) {
						post_log("\nExecuting test no : %2d. %s\n", (loop_count + 1), function[loop_count].func_name);
						autorun_status = (*function[loop_count].func)();
						switch (autorun_status) {
						case TEST_PASS:
							pass++;
							break;
						case TEST_FAIL:
							fail++;
							break;
						case TEST_SKIP:
							skip++;
							break;
						}
					}
					post_log("Report TOTAL Test => %d - PASS => %d - FAIL => %d - SKIP => %d\n", (pass + fail + skip), pass, fail, skip);
					if (fail)
						status = -1;
				} else if (test_no < (total_tests + 1)) {
					if (TEST_FAIL == (*function[test_no - 1].func)())
						status = -1;
				}
			}
		} while (test_no != (total_tests + 2));
		return status;
	}
	return 0;
#endif
}
#endif
#endif
