/*
 * $Copyright Open Broadcom Corporation$
 */


#include <common.h>
#include <post.h>

#if CONFIG_POST & CONFIG_SYS_POST_SPDIF

# if !defined (CONFIG_CYGNUS)
#include <iproc_i2s.h>
#include <iproc_gsio.h>
#else
#include "../bcm/halapi/audio_apis.h"

#define cpu_rd_single(x, y) readl(x)
#define cpu_wr_single(x, y, z) writel(y, x)
 
#endif





#if defined (CONFIG_CYGNUS)
int audio_spdif_out_test(void)
{
	uint32_t *wr_addr;
    uint32_t *end_addr;
    uint32_t src_en,i,slave_present;
    uint32_t slave_address,speed_mode;
    uint32_t dte_fifo_contents[4];
    uint32_t control_byte1,control_byte2;
    uint32_t *stereo_interleaved_l,*stereo_interleaved_r;
    int smbus_status;
    uint32_t spdif_en = 1;

	int smbus_slave_address = 0x10;
	int control_reg;
	int control_byte;
	int cnt = 20;
	uint32_t read_val;

	extern uint32_t right_channel_samples[480];
	extern uint32_t left_channel_samples[480];


    stereo_interleaved_l = 0x67000000;
    for (i=0;i<480;i++)
    {
        *stereo_interleaved_l = left_channel_samples[i];
        stereo_interleaved_l++;
    }

    stereo_interleaved_l = 0x67000000;
	stereo_interleaved_r = 0x68000000;

	for (i=0;i<480;i++)
    {
        *stereo_interleaved_r = right_channel_samples[i];
        stereo_interleaved_r++;
    }

    stereo_interleaved_r = 0x68000000;


    //This test uses Stereo interleaved data to be played back from DDR
    //crmu_reset_related();
    audio_sw_reset();
    audio_io_mux_select();
    asiu_audio_clock_gating_disable();
	asiu_audio_pad_enable();
    //Configuring CRMU PLL CONTROL REGISTER
    asiu_audio_gen_pll_pwr_on(1);
    //user macro set to 48kHz clock
	asiu_audio_gen_pll_group_id_config(1,0x00000046,0x000C75FF,0x000000D8,4,0,8,2,0,0);
	post_log("Audio PLL configuration done \n");

    //Audio BF control source channel configuration
    post_log("Source Channel 3 under configuration \n");
    asiu_audio_fmm_bf_ctrl_source_cfg(spdif_en,1,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,1,0,0);
    post_log("Source Channel 3 configuration done \n");

	wr_addr = 0x6700077F;
    end_addr = 0x6700077F;//stereo_interleaved_data + 480;
    //post_log("Audio Write Address Programmed for ")
    //Audio I2S0 Source Buffer Address Configuration for Stereo Interleaved data
	//asiu_audio_set_source_buffer_addr(4,1,stereo_interleaved,wr_addr,stereo_interleaved,end_addr,0,0,0,0,0,0,0,0);
    cpu_wr_single(0x180AE590,0x67000000,4);
    cpu_wr_single(0x180AE594,0x6700077F,4);
    cpu_wr_single(0x180AE598,0x67000000,4);
    cpu_wr_single(0x180AE59C,0x6700077F,4);

    wr_addr = 0x68000EFF;
    end_addr = 0x68000EFF;

	cpu_wr_single(0x180AE5A8,0x68000000,4);
	cpu_wr_single(0x180AE5AC,0x6800077F,4);
    cpu_wr_single(0x180AE5B0,0x68000000,4);
    cpu_wr_single(0x180AE5B4,0x6800077F,4);

    post_log("Source Channel Ring Buffer Addresses Configured \n");

	//SPDIF related registers to be configured now
    //SPDIF OUTPUT STREAM CONFIG
    asiu_audio_spdif_stream_config(0,1,0,8,0,1,0,0,3);

    //SPDIF CTRL CONFIG
	asiu_audio_spdif_ctrl(0,0,0,0,0,0,0,1,0,0,8,1,0,0);

    //SPDIF CHANNEL STATUS CONFIGURATION for 48 KHz linear pcm audio
    asiu_audio_spdif_ch_status(0x00002000,0x00000000,0x00000000);

    //SPDIF FORMAT CONFIG
    asiu_audio_spdif_format_config(0,0,1,1,1);

    //SPDIF MCLK AND PLL CLOCK SELECT, MCLK,PLL
    asiu_audio_spdif_mclk_config(2,1);

    //SPDIF STREAM ENABLE
    asiu_audio_spdif_stream_config(1,1,0,8,0,1,0,0,3);

    //SOURCE FIFO ENABLE
    asiu_audio_fmm_bf_ctrl_source_cfg(spdif_en,1,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,1,1,0);

    //PLAY RUN
    asiu_audio_start_dma_read(1,1,0);

    post_log("End of test : check for 1 KHz audio on the left channel and 2 KHz audio on the right channel - audio_spdif_out_test \n");

#if 0
	
    cpu_wr_single(CRMU_IOMUX_CTRL6,0x03333333,4);
	read_val = cpu_rd_single(ASIU_GP_OUT_EN_3,4);
	read_val |= 0x40;
	cpu_wr_single(ASIU_GP_OUT_EN_3,read_val,4);
	read_val = cpu_rd_single(ASIU_GP_DATA_OUT_3,4);
	read_val &= ~(0x40);
	cpu_wr_single(ASIU_GP_DATA_OUT_3,read_val,4);
	add_delay(80000);
	read_val = cpu_rd_single(ASIU_GP_DATA_OUT_3,4);
	read_val |= (0x40);
	cpu_wr_single(ASIU_GP_DATA_OUT_3,read_val,4);


    smbus1_init(0);
    slave_present = smbus1_slave_read(smbus_slave_address, 0x7F);
	if (slave_present)
		printf("Id : 0x%x\n", slave_present & 0xFF);
	else
        printf("No Response!!!\n");

	control_reg = 0x4;
	control_byte = 0x88 | (1 << 31);

	smbus_status = smbus_write(smbus_slave_address,control_reg,control_byte);
	if (smbus_status) {
	    post_log("SMBUS TRANSACTION NOT SUCCESSFUL \n");
        return -1;
	}
	
	control_reg = 0x6;
	control_byte = 0xFF | (1 << 31);

	smbus_status = smbus_write(smbus_slave_address,control_reg,control_byte);
	if (smbus_status) {
	    post_log("SMBUS TRANSACTION NOT SUCCESSFUL \n");
        return -1;
	}
	
	control_reg = 0x7;
	control_byte = 0xFF | (1 << 31);

	smbus_status = smbus_write(smbus_slave_address,control_reg,control_byte);
	if (smbus_status) {
	    post_log("SMBUS TRANSACTION NOT SUCCESSFUL \n");
        return -1;
	}

	control_reg = 0x5;
	control_byte = 0x85 | (1 << 31);

	smbus_status = smbus_write(smbus_slave_address,control_reg,control_byte);
	if (smbus_status) {
	    post_log("SMBUS TRANSACTION NOT SUCCESSFUL \n");
        return -1;
	}

#if 1
	control_reg = 0x2;
	control_byte = 0x30 | (1 << 31);

	smbus_status = smbus_write(smbus_slave_address,control_reg,control_byte);
	if (smbus_status) {
	    post_log("SMBUS TRANSACTION NOT SUCCESSFUL \n");
        return -1;
	}
#endif

#if 0
	control_reg = 0x1;
	control_byte = 0x80 | (1 << 31);

	smbus_status = smbus_write(smbus_slave_address,control_reg,control_byte);
	if (smbus_status) {
	    post_log("SMBUS TRANSACTION NOT SUCCESSFUL \n");
        return -1;
	}
#endif

#if 1
	control_reg = 0x0;
	control_byte = 0x08 | (1 << 31);

	smbus_status = smbus_write(smbus_slave_address,control_reg,control_byte);
	if (smbus_status) {
	    post_log("SMBUS TRANSACTION NOT SUCCESSFUL \n");
        return -1;
	}
#endif
	
	while(1) {
	slave_present = smbus1_slave_read(smbus_slave_address, 0xB);
	if (slave_present)
		printf("Format detect : 0x%x\n", slave_present & 0xFF);
	else
        printf("No Response!!!\n");
    
	slave_present = smbus1_slave_read(smbus_slave_address, 0xC);
	if (slave_present)
		printf("Rx error status : 0x%x\n", slave_present & 0xFF);
	else
        printf("No Response!!!\n");
	
	slave_present = smbus1_slave_read(smbus_slave_address, 0x18);
	if (slave_present)
		printf("OMCLK/RMCLK Ratio : 0x%x\n", slave_present & 0xFF);
	else
        printf("No Response!!!\n");
	}

    return 0;
#endif

}
#endif

int spdif_post_test (int flags)
{
# if !defined (CONFIG_CYGNUS)
    int status = 0;
#if (defined(CONFIG_NS_PLUS))

    if ( post_get_board_diags_type() & HR_ER_BOARDS )
	{
		printf("\nThis diag is not supported on this platform\n ");
		return 2;
	}
#endif

    post_log("\nSPDIF diags starts  !!!");


    if ( (flags & POST_SEMI_AUTO) !=  POST_SEMI_AUTO )
    {
        post_log("\nSet strap pin: A2=0, A3=0x3, A4=0xf, A5=0x28, A6=0xf4", 0);
        post_getConfirmation("\nReady? (Y/N)");

    }
    else
    {
        post_set_strappins("A2 0%A3 3%A4 f%A5 38%A6 f4%");
    }



    if ( WM8750_SPI_Enable() != 0 )
    {
        post_log("\nSPI not present, can't configure WM8750\n");
        goto failed;
    }

    // Reset the codec
    WM8750_reset_codec();

    WM8750_poweron_channels();

    WM8750_configure_sigal_path();

    WM8750_configure_volume();

    WM8750_configure_format_mode();

    post_log("\nConnect an audio input device to the upper port of the J3301 and start an audio playback\n ");
    post_log(" Connect an audio output device to the top optical output port(J3208) with a SPDIF cable,\n ");
    post_getConfirmation("\nReady? (Y/N)");

    iproc_i2s_recording_start();


    post_log("\nAudio is recorded. start playback");

    //Disable the codec from playback recording data
    WM8750_disable_ADC_data();


    iproc_i2s_playback_recorded_audio();


    status = post_getUserResponse("\nSound output OK? (Y/N)");

    if ( -1 == status )
    {
       post_log("\nTest Failed !");
    }
    else
    {
       post_log("\nTest Passed !");
    }

    post_log("\n\nI2S test done\n");

    //I2S__w8955_post_test(0);
    return status;

failed:
    post_log("\nTest Failed !");
    return -1;
#else
	audio_spdif_out_test();
#endif
}




#endif /* CONFIG_POST & CONFIG_SYS_POST_SPDIF */
