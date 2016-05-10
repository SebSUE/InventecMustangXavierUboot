#include <common.h>
#include <asm/io.h>
#include <post.h>
#include "audio_apis.h"

#define cpu_rd_single(x, y) readl(x)
#define cpu_wr_single(x, y, z) writel(y, x)

void asiu_dte_iomux_select_external_inputs(void)
{
	uint32_t val;
	val = 0x02200000;
	cpu_wr_single(CRMU_IOMUX_CTRL1, val, 4);
	post_log("CRMU IOMUX CTRL1 Register programmed with %08X\n", val);
	val = 0x22000000;
	cpu_wr_single(CRMU_IOMUX_CTRL2, val, 4);
	post_log("CRMU IOMUX CTRL2 Register programmed with %08X\n", val);
}

void audio_sw_reset(void)
{
	cpu_wr_single(CRMU_CHIP_IO_PAD_CONTROL, 0, 4);
	post_log("ASIU SOFT RESET INITIATED\n");
	/*ASIU SW RESET*/
	cpu_wr_single(ASIU_TOP_SW_RESET_CTRL, 0x00000000, 4);
	cpu_wr_single(ASIU_TOP_SW_RESET_CTRL, 0x000003ff, 4);
	udelay(1000);
	post_log("ASIU SOFT RESET DONE\n");
}

void crmu_reset_related(void)
{
	cpu_wr_single(CRMU_RESET_CTRL, 0xf, 4);
	cpu_wr_single(CRMU_SW_POR_RESET_CTRL, 0x1, 4);
	cpu_wr_single(CRMU_CLOCK_GATE_CTRL, 0x1, 4);
}

void audio_io_mux_select(void)
{
	/*IOMUX PROGRAMMING FOR I2S0*/
	cpu_wr_single(CRMU_IOMUX_CTRL0, 0x22222, 4);

	/*IOMUX PROGRAMMING FOR I2S1 and I2S2 AND SPDIF*/
	cpu_wr_single(CRMU_IOMUX_CTRL3, 0x01100000, 4);
	cpu_wr_single(SMART_CARD_FCB_SEL, 0x00000011, 4);

	post_log("IOMUX PROGRAMMING Done properly for Audio - I2S0 I2S1 I2S2 and SPDIF\n");
}

void asiu_audio_pad_enable(void)
{
	cpu_wr_single(CRMU_CHIP_IO_PAD_CONTROL, 0, 4);
	cpu_wr_single(ASIU_TOP_PAD_CTRL_0, 0x00000001, 4);

	post_log("ASIU AUDIO PAD AND SEROUT ENABLES DONE\n");
}

void asiu_audio_clock_gating_disable(void)
{
	cpu_wr_single(ASIU_TOP_CLK_GATING_CTRL, 0x00000002, 4);
	post_log("Clock Gating Disabled for the ASIU - Audio block\n");

	/*default Group ID configurations also*/
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP0_REG, 0x0, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP1_REG, 0x1, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP2_REG, 0x2, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP3_REG, 0x3, 4);
}

void asiu_audio_spdif_stream_config(
	uint32_t ena,
	uint32_t channel_grouping,
	uint32_t group_id,
	uint32_t stream_bit_resolution,
	uint32_t wait_for_valid,
	uint32_t ignore_first_underflow,
	uint32_t init_sm,
	uint32_t ins_inval,
	uint32_t fci_id)
{
	uint32_t wr_data = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : SPDIF Stream Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	wr_data = (
			ena                           <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__ENA  |
			channel_grouping              <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__CHANNEL_GROUPING_R|
			group_id                      <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__GROUP_ID_R|
			stream_bit_resolution         <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__STREAM_BIT_RESOLUTION_R|
			wait_for_valid                <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__WAIT_FOR_VALID|
			ignore_first_underflow        <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__IGNORE_FIRST_UNDERFLOW|
			init_sm                       <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__INIT_SM|
			ins_inval                     <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__INS_INVAL|
			3                             <<  AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG__FCI_ID_R
			);
	post_log("AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG= %0x\n", wr_data);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_STREAM_CFG_0_REG, wr_data, 4);
}

/*--------------------------- SPDIF Control function -----------------------------------*/
void asiu_audio_spdif_ctrl(
	uint32_t dither_value,
	uint32_t wait_pcm_to_comp,
	uint32_t length_code,
	uint32_t overwrite_data,
	uint32_t comp_or_linear,
	uint32_t flush_on_uflow,
	uint32_t insert_on_uflow,
	uint32_t insert_when_disa,
	uint32_t spdif_bypass,
	uint32_t cp_toggle_rate,
	uint32_t dither_width,
	uint32_t dither_ena,
	uint32_t hold_cstat,
	uint32_t validity)
{
	uint32_t wr_data = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : SPDIF Control\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	wr_data = (
			dither_value		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__DITHER_VALUE|
			wait_pcm_to_comp		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__WAIT_PCM_TO_COMP_R|
			length_code		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__LENGTH_CODE|
			overwrite_data		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__OVERWRITE_DATA|
			comp_or_linear		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__COMP_OR_LINEAR|
			flush_on_uflow		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__FLUSH_ON_UFLOW|
			insert_on_uflow		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__INSERT_ON_UFLOW|
			insert_when_disa		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__INSERT_WHEN_DISA|
			spdif_bypass		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__SPDIF_BYPASS|
			cp_toggle_rate		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__CP_TOGGLE_RATE_R|
			dither_width		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__DITHER_WIDTH_R|
			dither_ena		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__DITHER_ENA|
			hold_cstat		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__HOLD_CSTAT|
			validity			<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG__VALIDITY
			);
	post_log("AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG= %0x\n", wr_data);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CTRL_REG, wr_data, 4);
}

/*--------------------------- SPDIF Channel Status function -----------------------------------*/
void asiu_audio_spdif_ch_status(
	uint32_t ch_status_0,
	uint32_t ch_status_1,
	uint32_t ch_status_2)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : SPDIF Channel Status\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	post_log("AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CHANSTAT_0_REG= %0x\n", ch_status_0);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CHANSTAT_0_REG, ch_status_0, 4);

	post_log("AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CHANSTAT_1_REG= %0x\n", ch_status_1);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CHANSTAT_1_REG, ch_status_1, 4);

	post_log("AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CHANSTAT_2_REG= %0x\n", ch_status_2);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_CHANSTAT_2_REG, ch_status_2, 4);
}

/*--------------------------- SPDIF Ramp Burst -----------------------------------*/

void asiu_audio_spdif_ramp_burst(
	uint32_t stepsize,
	uint32_t rampup_steps,
	uint32_t stop_bit,
	uint32_t burst_type,
	uint32_t rep_period)
{
	uint32_t wr_data = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : SPDIF Ramp Burst\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	wr_data = (
			stepsize		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_RAMP_BURST_REG__STEPSIZE_R|
			rampup_steps	<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_RAMP_BURST_REG__RAMPUP_STEPS_R|
			stop_bit		<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_RAMP_BURST_REG__STOP_BIT|
			burst_type	<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_RAMP_BURST_REG__BURST_TYPE|
			rep_period	<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_RAMP_BURST_REG__REP_PERIOD_R
			);
	post_log("AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_RAMP_BURST_REG= %0x\n", wr_data);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_RAMP_BURST_REG, wr_data, 4);
}

/*--------------------------- SPDIF Format Config -----------------------------------*/
void asiu_audio_spdif_format_config(
	uint32_t lr_select,
	uint32_t limit_to_16_bits,
	uint32_t pream_pol,
	uint32_t data_enable,
	uint32_t clock_enable)
{
	uint32_t wr_data = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : SPDIF Format Config\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	wr_data = (
			lr_select               <<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_FORMAT_CFG_REG__LR_SELECT_R|
			limit_to_16_bits	<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_FORMAT_CFG_REG__LIMIT_TO_16_BITS|
			pream_pol               <<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_FORMAT_CFG_REG__PREAM_POL|
			data_enable	<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_FORMAT_CFG_REG__DATA_ENABLE|
			clock_enable	<<  AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_FORMAT_CFG_REG__CLOCK_ENABLE
			);

	post_log("AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_FORMAT_CFG_REG= %0x\n", wr_data);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_SPDIF_FORMAT_CFG_REG, wr_data, 4);
}

/*--------------------------- SPDIF MCLK Config -----------------------------------*/
void asiu_audio_spdif_mclk_config(
	uint32_t mclk_rate,
	uint32_t pllclksel)
{
	uint32_t wr_data = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : SPDIF MCLK Config\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	wr_data = (
			mclk_rate               <<  AUD_FMM_IOP_OUT_SPDIF_0_MCLK_CFG_0_REG__MCLK_RATE_R|
			pllclksel	<<  AUD_FMM_IOP_OUT_SPDIF_0_MCLK_CFG_0_REG__PLLCLKSEL_R
			);

	post_log("AUD_FMM_IOP_OUT_SPDIF_0_MCLK_CFG_0_REG= %0x\n", wr_data);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_MCLK_CFG_0_REG, wr_data, 4);
}

/*--------------------------- SPDIF Sample Count Config -----------------------------------*/
void asiu_audio_spdif_sample_count(
	uint32_t spdif_count_start,
	uint32_t spdif_count_clear)
{
	uint32_t wr_data = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : SPDIF Sample Count Config\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	wr_data = (
			spdif_count_start               <<  AUD_FMM_IOP_OUT_SPDIF_0_SAMP_COUNT_CTRL_REG__COUNT_START |
			spdif_count_clear	<<  AUD_FMM_IOP_OUT_SPDIF_0_SAMP_COUNT_CTRL_REG__COUNT_CLEAR
			);

	post_log("AUD_FMM_IOP_OUT_SPDIF_0_SAMP_COUNT_CTRL_REG= %0x\n", wr_data);
	cpu_wr_single(AUD_FMM_IOP_OUT_SPDIF_0_SAMP_COUNT_CTRL_REG, wr_data, 4);
}

/*--------------------------- I2S In Configuration function -----------------------------------*/
void asiu_audio_i2s_in_rx_config(
	/*Capture Stream*/
	uint32_t cap_ena,
	uint32_t cap_group_id,
	uint32_t ignore_first_overflow ,
	/*I2S In*/
	uint32_t lrck_polarity,
	uint32_t sclk_polarity,
	uint32_t data_alignment,
	uint32_t data_justification,
	uint32_t bits_per_sample,
	uint32_t bits_per_slot,
	uint32_t valid_slot,
	uint32_t slave_mode,
	uint32_t tdm_mode,
	uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S In Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*Capture Stream*/
	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_0_CAP_STREAM_CFG_0_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_0_CAP_STREAM_CFG_0_REG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				ignore_first_overflow  <<  AUD_FMM_IOP_IN_I2S_0_CAP_STREAM_CFG_0_REG__IGNORE_FIRST_OVERFLOW  |
				cap_group_id           <<  AUD_FMM_IOP_IN_I2S_0_CAP_STREAM_CFG_0_REG__CAP_GROUP_ID_R  |
				cap_ena                <<  AUD_FMM_IOP_IN_I2S_0_CAP_STREAM_CFG_0_REG__CAP_ENA
				);
		post_log("AUD_FMM_IOP_IN_I2S_0_CAP_STREAM_CFG_0_REG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_0_CAP_STREAM_CFG_0_REG, wr_data, 4);

		rd_data = 0;
		wr_data = 0;
		mask = 0;
	}
	/*I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_1_CAP_STREAM_CFG_0_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_1_CAP_STREAM_CFG_0_REG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				ignore_first_overflow  <<  AUD_FMM_IOP_IN_I2S_1_CAP_STREAM_CFG_0_REG__IGNORE_FIRST_OVERFLOW  |
				cap_group_id           <<  AUD_FMM_IOP_IN_I2S_1_CAP_STREAM_CFG_0_REG__CAP_GROUP_ID_R  |
				cap_ena                <<  AUD_FMM_IOP_IN_I2S_1_CAP_STREAM_CFG_0_REG__CAP_ENA
				);
		post_log("AUD_FMM_IOP_IN_I2S_1_CAP_STREAM_CFG_0_REG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_1_CAP_STREAM_CFG_0_REG, wr_data, 4);

		rd_data = 0;
		wr_data = 0;
		mask = 0;
	}
	/*I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_2_CAP_STREAM_CFG_0_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_2_CAP_STREAM_CFG_0_REG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				ignore_first_overflow  <<  AUD_FMM_IOP_IN_I2S_2_CAP_STREAM_CFG_0_REG__IGNORE_FIRST_OVERFLOW  |
				cap_group_id           <<  AUD_FMM_IOP_IN_I2S_2_CAP_STREAM_CFG_0_REG__CAP_GROUP_ID_R  |
				cap_ena                <<  AUD_FMM_IOP_IN_I2S_2_CAP_STREAM_CFG_0_REG__CAP_ENA
				);
		post_log("AUD_FMM_IOP_IN_I2S_2_CAP_STREAM_CFG_0_REG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_2_CAP_STREAM_CFG_0_REG, wr_data, 4);

		rd_data = 0;
		wr_data = 0;
		mask = 0;
	}

	/*I2S In Config*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				lrck_polarity       <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__LRCK_POLARITY  |
				sclk_polarity       <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__SCLK_POLARITY  |
				data_alignment      <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__DATA_ALIGNMENT  |
				data_justification  <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__DATA_JUSTIFICATION  |
				bits_per_sample     <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__BITS_PER_SAMPLE_R  |
				bits_per_slot       <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__BITS_PER_SLOT |
				valid_slot          <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__VALID_SLOT_R |
				slave_mode          <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__SLAVE_MODE |
				tdm_mode            <<  AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG__TDM_MODE
				);

		post_log("AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_0_I2S_IN_CFG_REG, wr_data, 4);
	}
	/*I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				lrck_polarity       <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__LRCK_POLARITY  |
				sclk_polarity       <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__SCLK_POLARITY  |
				data_alignment      <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__DATA_ALIGNMENT  |
				data_justification  <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__DATA_JUSTIFICATION  |
				bits_per_sample     <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__BITS_PER_SAMPLE_R  |
				bits_per_slot       <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__BITS_PER_SLOT |
				valid_slot          <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__VALID_SLOT_R |
				slave_mode          <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__SLAVE_MODE |
				tdm_mode            <<  AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG__TDM_MODE
				);

		post_log("AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_1_I2S_IN_CFG_REG, wr_data, 4);
	}
	/*I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				lrck_polarity       <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__LRCK_POLARITY  |
				sclk_polarity       <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__SCLK_POLARITY  |
				data_alignment      <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__DATA_ALIGNMENT  |
				data_justification  <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__DATA_JUSTIFICATION  |
				bits_per_sample     <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__BITS_PER_SAMPLE_R  |
				bits_per_slot       <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__BITS_PER_SLOT |
				valid_slot          <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__VALID_SLOT_R |
				slave_mode          <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__SLAVE_MODE |
				tdm_mode            <<  AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG__TDM_MODE
				);

		post_log("AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_2_I2S_IN_CFG_REG, wr_data, 4);
	}
}

/*--------------------------- I2S Destination Buffer Configuration function -----------------------------------*/
void asiu_audio_fmm_bf_ctrl_dest_cfg(
	uint32_t process_seq_id_valid,
	uint32_t blocked_access_disable,
	uint32_t process_id_high,
	uint32_t dma_block_cnt,
	uint32_t reverse_endian,
	uint32_t capture_mode,
	uint32_t fci_cap_id,
	uint32_t not_pause_when_full,
	uint32_t source_fifo_id,
	uint32_t input_frm_sourcefifo,
	uint32_t capture_to_sourcefifo,
	uint32_t play_from_capture,
	uint32_t destfifo_size_double,
	uint32_t buffer_pair_enable,
	uint32_t capture_enable,
	uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S0 Dest Buffer Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_DESTCH_CFG0_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_DESTCH_CFG0_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
		process_seq_id_valid      << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__PROCESS_SEQ_ID_VALID |
		blocked_access_disable    << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__BLOCKED_ACCESS_DISABLE |
		process_id_high           << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__PROCESS_ID_HIGH |
		dma_block_cnt             << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__DMA_BLOCK_CNT_R |
		reverse_endian            << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__REVERSE_ENDIAN |
		capture_mode              << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__CAPTURE_MODE |
		/*fci_cap_id                << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__FCI_CAP_ID_R |*/
		0x180                     << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__FCI_CAP_ID_R |
		not_pause_when_full       << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__NOT_PAUSE_WHEN_FULL |
		source_fifo_id            << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__SOURCE_FIFO_ID_R |
		input_frm_sourcefifo      << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__INPUT_FRM_SOURCEFIFO|
		capture_to_sourcefifo     << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__CAPTURE_TO_SOURCEFIFO|
		play_from_capture         << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__PLAY_FROM_CAPTURE|
		destfifo_size_double      << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__DESTFIFO_SIZE_DOUBLE|
		buffer_pair_enable        << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__BUFFER_PAIR_ENABLE |
		capture_enable            << AUD_FMM_BF_CTRL_DESTCH_CFG0_REG__CAPTURE_ENABLE);
		post_log("AUD_FMM_BF_CTRL_DESTCH_CFG0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_CFG0_REG, wr_data, 4);
	}

	/*I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_DESTCH_CFG1_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_DESTCH_CFG1_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			process_seq_id_valid      << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__PROCESS_SEQ_ID_VALID |
			blocked_access_disable    << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__BLOCKED_ACCESS_DISABLE |
			process_id_high           << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__PROCESS_ID_HIGH |
			dma_block_cnt             << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__DMA_BLOCK_CNT_R |
			reverse_endian            << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__REVERSE_ENDIAN |
			capture_mode              << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__CAPTURE_MODE |
			/*fci_cap_id                << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__FCI_CAP_ID_R |*/
			0x181                     << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__FCI_CAP_ID_R |
			not_pause_when_full       << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__NOT_PAUSE_WHEN_FULL |
			source_fifo_id            << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__SOURCE_FIFO_ID_R |
			input_frm_sourcefifo      << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__INPUT_FRM_SOURCEFIFO|
			capture_to_sourcefifo     << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__CAPTURE_TO_SOURCEFIFO|
			play_from_capture         << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__PLAY_FROM_CAPTURE|
			destfifo_size_double      << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__DESTFIFO_SIZE_DOUBLE|
			buffer_pair_enable        << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__BUFFER_PAIR_ENABLE |
			capture_enable            << AUD_FMM_BF_CTRL_DESTCH_CFG1_REG__CAPTURE_ENABLE);
		post_log("AUD_FMM_BF_CTRL_DESTCH_CFG1= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_CFG1_REG, wr_data, 4);
	}

	/*I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {

		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_DESTCH_CFG2_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_DESTCH_CFG2_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			process_seq_id_valid      << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__PROCESS_SEQ_ID_VALID |
			blocked_access_disable    << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__BLOCKED_ACCESS_DISABLE |
			process_id_high           << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__PROCESS_ID_HIGH |
			dma_block_cnt             << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__DMA_BLOCK_CNT_R |
			reverse_endian            << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__REVERSE_ENDIAN |
			capture_mode              << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__CAPTURE_MODE |
			/*fci_cap_id                << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__FCI_CAP_ID_R |*/
			0x182                     << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__FCI_CAP_ID_R |
			not_pause_when_full       << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__NOT_PAUSE_WHEN_FULL |
			source_fifo_id            << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__SOURCE_FIFO_ID_R |
			input_frm_sourcefifo      << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__INPUT_FRM_SOURCEFIFO|
			capture_to_sourcefifo     << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__CAPTURE_TO_SOURCEFIFO|
			play_from_capture         << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__PLAY_FROM_CAPTURE|
			destfifo_size_double      << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__DESTFIFO_SIZE_DOUBLE|
			buffer_pair_enable        << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__BUFFER_PAIR_ENABLE |
			capture_enable            << AUD_FMM_BF_CTRL_DESTCH_CFG2_REG__CAPTURE_ENABLE);
		post_log("AUD_FMM_BF_CTRL_DESTCH_CFG2= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_CFG2_REG, wr_data, 4);
	}
 }

/*--------------------------- I2S OUT Configuration function -----------------------------------*/
void asiu_audio_i2s_out_tx_config(
	uint32_t clock_enable,
	uint32_t data_enable,
	uint32_t lrck_polarity,
	uint32_t sclk_polarity,
	uint32_t data_alignment,
	uint32_t data_justification,
	uint32_t bits_per_sample,
	uint32_t bits_per_slot,
	uint32_t valid_slot,
	uint32_t fsync_width,
	uint32_t sclk_per_1fs,
	uint32_t slave_mode,
	uint32_t tdm_mode,
	uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S OUT Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {

		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				clock_enable        <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__CLOCK_ENABLE  |
				data_enable         <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__DATA_ENABLE  |
				lrck_polarity       <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__LRCK_POLARITY  |
				sclk_polarity       <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__SCLK_POLARITY  |
				data_alignment      <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__DATA_ALIGNMENT  |
				data_justification  <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__DATA_JUSTIFICATION  |
				bits_per_sample     <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__BITS_PER_SAMPLE_R  |
				bits_per_slot       <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__BITS_PER_SLOT |
				valid_slot          <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__VALID_SLOT_R |
				fsync_width         <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__FSYNC_WIDTH_R |
				sclk_per_1fs        <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__SCLKS_PER_1FS_DIV32_R |
				slave_mode          <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__SLAVE_MODE |
				tdm_mode            <<  AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG__TDM_MODE
				);
		post_log("AUD_FMM_IOP_OUT_I2S_0_I2S_CFG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_0_I2S_CFG_REG, wr_data, 4);
	}

	/*I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				clock_enable        <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__CLOCK_ENABLE  |
				data_enable         <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__DATA_ENABLE  |
				lrck_polarity       <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__LRCK_POLARITY  |
				sclk_polarity       <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__SCLK_POLARITY  |
				data_alignment      <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__DATA_ALIGNMENT  |
				data_justification  <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__DATA_JUSTIFICATION  |
				bits_per_sample     <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__BITS_PER_SAMPLE_R  |
				bits_per_slot       <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__BITS_PER_SLOT |
				valid_slot          <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__VALID_SLOT_R |
				fsync_width         <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__FSYNC_WIDTH_R |
				sclk_per_1fs        <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__SCLKS_PER_1FS_DIV32_R |
				slave_mode          <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__SLAVE_MODE |
				tdm_mode            <<  AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG__TDM_MODE
				);
		post_log("AUD_FMM_IOP_OUT_I2S_1_I2S_CFG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_1_I2S_CFG_REG, wr_data, 4);
	}

	/*I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                    |
				clock_enable        <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__CLOCK_ENABLE  |
				data_enable         <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__DATA_ENABLE  |
				lrck_polarity       <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__LRCK_POLARITY  |
				sclk_polarity       <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__SCLK_POLARITY  |
				data_alignment      <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__DATA_ALIGNMENT  |
				data_justification  <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__DATA_JUSTIFICATION  |
				bits_per_sample     <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__BITS_PER_SAMPLE_R  |
				bits_per_slot       <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__BITS_PER_SLOT |
				valid_slot          <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__VALID_SLOT_R |
				fsync_width         <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__FSYNC_WIDTH_R |
				sclk_per_1fs        <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__SCLKS_PER_1FS_DIV32_R |
				slave_mode          <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__SLAVE_MODE |
				tdm_mode            <<  AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG__TDM_MODE
				);
		post_log("AUD_FMM_IOP_OUT_I2S_2_I2S_CFG= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_2_I2S_CFG_REG, wr_data, 4);
	}
}

/*--------------------------- I2S Stream Configuration function -----------------------------------*/
void asiu_audio_i2s_stream_config_samp_count(
	uint32_t ena,
	uint32_t channel_grouping,
	uint32_t group_id,
	uint32_t stream_bit_resolution,
	uint32_t wait_for_valid,
	uint32_t ignore_first_underflow,
	uint32_t init_sm,
	uint32_t ins_inval,
	uint32_t fci_id,
	uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S Out Stream Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S 0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (/*wr_data                       |*/
				ena                           <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__ENA  |
				channel_grouping              <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__CHANNEL_GROUPING_R |
				group_id                      <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__GROUP_ID_R |
				stream_bit_resolution         <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__STREAM_BIT_RESOLUTION_R |
				wait_for_valid                <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__WAIT_FOR_VALID|
				ignore_first_underflow        <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__IGNORE_FIRST_UNDERFLOW|
				init_sm                       <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__INIT_SM |
				ins_inval                     <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__INS_INVAL|
				/*fci_id                        <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__FCI_ID_R*/
				0                        <<  AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG__FCI_ID_R);
		post_log("AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_0_STREAM_CFG_0_REG, wr_data, 4);
	}

	/*I2S 1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (/*wr_data                       |*/
				ena                           <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__ENA  |
				channel_grouping              <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__CHANNEL_GROUPING_R |
				group_id                      <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__GROUP_ID_R |
				stream_bit_resolution         <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__STREAM_BIT_RESOLUTION_R |
				wait_for_valid                <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__WAIT_FOR_VALID|
				ignore_first_underflow        <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__IGNORE_FIRST_UNDERFLOW|
				init_sm                       <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__INIT_SM |
				ins_inval                     <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__INS_INVAL|
				/*fci_id                        <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__FCI_ID_R*/
				1                        <<  AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG__FCI_ID_R
				);
		post_log("AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_1_STREAM_CFG_0_REG, wr_data, 4);
	}

	/*I2S 2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (/*wr_data                       |*/
				ena                           <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__ENA  |
				channel_grouping              <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__CHANNEL_GROUPING_R |
				group_id                      <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__GROUP_ID_R |
				stream_bit_resolution         <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__STREAM_BIT_RESOLUTION_R |
				wait_for_valid                <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__WAIT_FOR_VALID|
				ignore_first_underflow        <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__IGNORE_FIRST_UNDERFLOW|
				init_sm                       <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__INIT_SM |
				ins_inval                     <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__INS_INVAL|
				/*fci_id                        <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__FCI_ID_R*/
				2                        <<  AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG__FCI_ID_R
				);
		post_log("AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_2_STREAM_CFG_0_REG, wr_data, 4);
	}
}

/*--------------------------- I2S Source Buffer Configuration function -----------------------------------*/
void asiu_audio_fmm_bf_ctrl_source_cfg(
	uint32_t spdif_en,
	uint32_t process_seq_id_valid,
	uint32_t blocked_access_disable,
	uint32_t process_id_high,
	uint32_t dma_block_cnt,
	uint32_t reverse_endian,
	uint32_t bit_resolution,
	uint32_t shared_sbuf_id,
	uint32_t share_sbuf,
	uint32_t sfifo_start_halffull,
	uint32_t dma_read_disable,
	uint32_t sample_repeat_enable,
	uint32_t not_pause_when_empty,
	uint32_t start_selection,
	uint32_t retain_fci_tag,
	uint32_t sourcefifo_size_double,
	uint32_t lr_data_ctrl,
	uint32_t sample_ch_mode,
	uint32_t buffer_pair_enable,
	uint32_t sourcefifo_enable,
	uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S Source Buffer Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			process_seq_id_valid          << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__PROCESS_SEQ_ID_VALID|
			blocked_access_disable        << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__BLOCKED_ACCESS_DISABLE|
			process_id_high               << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__PROCESS_ID_HIGH|
			dma_block_cnt                 << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__DMA_BLOCK_CNT_R |
			reverse_endian                << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__REVERSE_ENDIAN|
			bit_resolution                << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__BIT_RESOLUTION_R |
			shared_sbuf_id                << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__SHARED_SBUF_ID_R |
			share_sbuf                    << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__SHARE_SBUF |
			sfifo_start_halffull          << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__SFIFO_START_HALFFULL |
			dma_read_disable              << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__DMA_READ_DISABLE |
			sample_repeat_enable          << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__SAMPLE_REPEAT_ENABLE |
			not_pause_when_empty          << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__NOT_PAUSE_WHEN_EMPTY |
			start_selection               << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__START_SELECTION |
			retain_fci_tag                << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__RETAIN_FCI_TAG |
			sourcefifo_size_double        << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__SOURCEFIFO_SIZE_DOUBLE |
			lr_data_ctrl                  << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__LR_DATA_CTRL_R |
			sample_ch_mode                << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__SAMPLE_CH_MODE |
			buffer_pair_enable            << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__BUFFER_PAIR_ENABLE |
			sourcefifo_enable             << AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG__SOURCEFIFO_ENABLE);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CFG0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CFG0_REG, wr_data, 4);
	}

	/*I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			process_seq_id_valid          << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__PROCESS_SEQ_ID_VALID|
			blocked_access_disable        << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__BLOCKED_ACCESS_DISABLE|
			process_id_high               << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__PROCESS_ID_HIGH|
			dma_block_cnt                 << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__DMA_BLOCK_CNT_R |
			reverse_endian                << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__REVERSE_ENDIAN|
			bit_resolution                << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__BIT_RESOLUTION_R |
			shared_sbuf_id                << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__SHARED_SBUF_ID_R |
			share_sbuf                    << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__SHARE_SBUF |
			sfifo_start_halffull          << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__SFIFO_START_HALFFULL |
			dma_read_disable              << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__DMA_READ_DISABLE |
			sample_repeat_enable          << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__SAMPLE_REPEAT_ENABLE |
			not_pause_when_empty          << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__NOT_PAUSE_WHEN_EMPTY |
			start_selection               << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__START_SELECTION |
			retain_fci_tag                << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__RETAIN_FCI_TAG |
			sourcefifo_size_double        << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__SOURCEFIFO_SIZE_DOUBLE |
			lr_data_ctrl                  << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__LR_DATA_CTRL_R |
			sample_ch_mode                << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__SAMPLE_CH_MODE |
			buffer_pair_enable            << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__BUFFER_PAIR_ENABLE |
			sourcefifo_enable             << AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG__SOURCEFIFO_ENABLE);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CFG1= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CFG1_REG, wr_data, 4);
	}

	/*I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			process_seq_id_valid          << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__PROCESS_SEQ_ID_VALID|
			blocked_access_disable        << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__BLOCKED_ACCESS_DISABLE|
			process_id_high               << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__PROCESS_ID_HIGH|
			dma_block_cnt                 << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__DMA_BLOCK_CNT_R |
			reverse_endian                << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__REVERSE_ENDIAN|
			bit_resolution                << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__BIT_RESOLUTION_R |
			shared_sbuf_id                << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__SHARED_SBUF_ID_R |
			share_sbuf                    << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__SHARE_SBUF |
			sfifo_start_halffull          << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__SFIFO_START_HALFFULL |
			dma_read_disable              << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__DMA_READ_DISABLE |
			sample_repeat_enable          << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__SAMPLE_REPEAT_ENABLE |
			not_pause_when_empty          << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__NOT_PAUSE_WHEN_EMPTY |
			start_selection               << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__START_SELECTION |
			retain_fci_tag                << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__RETAIN_FCI_TAG |
			sourcefifo_size_double        << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__SOURCEFIFO_SIZE_DOUBLE |
			lr_data_ctrl                  << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__LR_DATA_CTRL_R |
			sample_ch_mode                << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__SAMPLE_CH_MODE |
			buffer_pair_enable            << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__BUFFER_PAIR_ENABLE |
			sourcefifo_enable             << AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG__SOURCEFIFO_ENABLE);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CFG2= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG, wr_data, 4);
	}

	/*SPDIF Enable*/
	if (spdif_en) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;

		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CFG2_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			process_seq_id_valid          << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__PROCESS_SEQ_ID_VALID|
			blocked_access_disable        << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__BLOCKED_ACCESS_DISABLE|
			process_id_high               << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__PROCESS_ID_HIGH|
			dma_block_cnt                 << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__DMA_BLOCK_CNT_R |
			reverse_endian                << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__REVERSE_ENDIAN|
			bit_resolution                << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__BIT_RESOLUTION_R |
			shared_sbuf_id                << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__SHARED_SBUF_ID_R |
			share_sbuf                    << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__SHARE_SBUF |
			sfifo_start_halffull          << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__SFIFO_START_HALFFULL |
			dma_read_disable              << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__DMA_READ_DISABLE |
			sample_repeat_enable          << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__SAMPLE_REPEAT_ENABLE |
			not_pause_when_empty          << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__NOT_PAUSE_WHEN_EMPTY |
			start_selection               << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__START_SELECTION |
			retain_fci_tag                << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__RETAIN_FCI_TAG |
			sourcefifo_size_double        << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__SOURCEFIFO_SIZE_DOUBLE |
			lr_data_ctrl                  << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__LR_DATA_CTRL_R |
			sample_ch_mode                << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__SAMPLE_CH_MODE |
			buffer_pair_enable            << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__BUFFER_PAIR_ENABLE |
			sourcefifo_enable             << AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG__SOURCEFIFO_ENABLE);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CFG3= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CFG3_REG, wr_data, 4);
	}
}

/*---------------------------  I2S Source Buffer  Address Set function -----------------------------------*/
void asiu_audio_set_source_buffer_addr(
	uint32_t i2s_port_num,
	uint32_t single_buffer_en,
	uint32_t first_ringbuf_rdaddr,
	uint32_t first_ringbuf_wraddr,
	uint32_t first_ringbuf_base_addr,
	uint32_t first_ringbuf_end_address,
	uint32_t first_ringbuf_start_wrpnt,
	uint32_t second_ringbuf_rdaddr,
	uint32_t second_ringbuf_wraddr,
	uint32_t second_ringbuf_base_addr,
	uint32_t second_ringbuf_end_address,
	uint32_t second_ringbuf_start_wrpnt,
	uint32_t audio_address_difference,
	uint32_t spdif_en)
{
	uint32_t mi_valid_wr_data = 0x00000000;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S Source Buffer Address Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*For I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		if (single_buffer_en == 0) {
			/*Buffer 0*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);
			/*Buffer 1*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_RDADDR_REG, second_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_WRADDR_REG, second_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_BASEADDR_REG, second_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_ENDADDR_REG, second_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_MI_VALID_REG, mi_valid_wr_data, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_START_WRPOINT_REG, second_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_MI_VALID= %0x\n", mi_valid_wr_data);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_RDADDR= %0x\n", second_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_WRADDR= %0x\n", second_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_BASEADDR= %0x\n", second_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_ENDADDR= %0x\n", second_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_START_WRPOINT= %0x\n", second_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_MI_VALID= %0x\n", mi_valid_wr_data);
		} else {
			/*Buffer 0*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_MI_VALID= %0x\n", mi_valid_wr_data);
		}
	}

	/*For I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		if (single_buffer_en == 0) {
			/*Buffer 2*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_RDADDR_REG, (first_ringbuf_rdaddr | audio_address_difference), 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_WRADDR_REG, (first_ringbuf_wraddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_BASEADDR_REG, (first_ringbuf_base_addr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_ENDADDR_REG, (first_ringbuf_end_address | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_MI_VALID_REG, mi_valid_wr_data , 4);
			/*cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT_REG, (first_ringbuf_start_wrpnt | audio_address_difference) , 4);*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT_REG, (first_ringbuf_start_wrpnt) , 4);
			/*Buffer 3*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_RDADDR_REG, (second_ringbuf_rdaddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_WRADDR_REG, (second_ringbuf_wraddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_BASEADDR_REG, (second_ringbuf_base_addr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_ENDADDR_REG, (second_ringbuf_end_address | audio_address_difference), 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_MI_VALID_REG, mi_valid_wr_data , 4);
			/*cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_START_WRPOINT_REG, (second_ringbuf_start_wrpnt| audio_address_difference) , 4);*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_START_WRPOINT_REG, (second_ringbuf_start_wrpnt) , 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_RDADDR= %0x\n", (first_ringbuf_rdaddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_WRADDR= %0x\n", (first_ringbuf_wraddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_BASEADDR= %0x\n", (first_ringbuf_base_addr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_ENDADDR= %0x\n", (first_ringbuf_end_address | audio_address_difference));
			/*post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt | audio_address_difference));*/
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_MI_VALID= %0x\n", mi_valid_wr_data);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_RDADDR= %0x\n", (second_ringbuf_rdaddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_WRADDR= %0x\n", (second_ringbuf_wraddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_BASEADDR= %0x\n", (second_ringbuf_base_addr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_ENDADDR= %0x\n", (second_ringbuf_end_address | audio_address_difference));
			/*post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_START_WRPOINT= %0x\n", (second_ringbuf_start_wrpnt | audio_address_difference));*/
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_START_WRPOINT= %0x\n", (second_ringbuf_start_wrpnt));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_MI_VALID= %0x\n", mi_valid_wr_data);
		} else {
			/*Buffer 2*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_RDADDR_REG, (first_ringbuf_rdaddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_WRADDR_REG, (first_ringbuf_wraddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_BASEADDR_REG, (first_ringbuf_base_addr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_ENDADDR_REG, (first_ringbuf_end_address | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_MI_VALID_REG, mi_valid_wr_data , 4);
			/*cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT_REG, (first_ringbuf_start_wrpnt | audio_address_difference) , 4);*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT_REG, (first_ringbuf_start_wrpnt) , 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_RDADDR= %0x\n", (first_ringbuf_rdaddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_WRADDR= %0x\n", (first_ringbuf_wraddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_BASEADDR= %0x\n", (first_ringbuf_base_addr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_ENDADDR= %0x\n", (first_ringbuf_end_address | audio_address_difference));
			/*post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt | audio_address_difference));*/
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt));
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_MI_VALID= %0x\n", mi_valid_wr_data);
		}
	}

	/*For I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		first_ringbuf_rdaddr = first_ringbuf_rdaddr | (audio_address_difference << 1);
		first_ringbuf_wraddr = first_ringbuf_wraddr | (audio_address_difference << 1);
		first_ringbuf_base_addr = first_ringbuf_base_addr | (audio_address_difference << 1);
		first_ringbuf_end_address = first_ringbuf_end_address | (audio_address_difference << 1);
		second_ringbuf_rdaddr = second_ringbuf_rdaddr | (audio_address_difference << 1);
		second_ringbuf_wraddr = second_ringbuf_wraddr | (audio_address_difference << 1);
		second_ringbuf_base_addr = second_ringbuf_base_addr | (audio_address_difference << 1);
		second_ringbuf_end_address = second_ringbuf_end_address | (audio_address_difference << 1);

		if (single_buffer_en == 0) {
			/*Buffer 4*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_ENDADDR_REG, first_ringbuf_end_address, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_START_WRPOINT_REG, first_ringbuf_start_wrpnt, 4);
			/*Buffer 5*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_RDADDR_REG, second_ringbuf_rdaddr, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_WRADDR_REG, second_ringbuf_wraddr, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_BASEADDR_REG, second_ringbuf_base_addr, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_ENDADDR_REG, second_ringbuf_end_address, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_START_WRPOINT_REG, second_ringbuf_start_wrpnt, 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_MI_VALID= %0x\n", mi_valid_wr_data);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_RDADDR= %0x\n", second_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_WRADDR= %0x\n", second_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_BASEADDR= %0x\n", second_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_ENDADDR= %0x\n", second_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_START_WRPOINT= %0x\n", second_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_MI_VALID= %0x\n", mi_valid_wr_data);
		} else {
			/*Buffer 4*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_RDADDR_REG, first_ringbuf_rdaddr, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_WRADDR_REG, first_ringbuf_wraddr, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_BASEADDR_REG, first_ringbuf_base_addr, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_ENDADDR_REG, first_ringbuf_end_address, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_START_WRPOINT_REG, first_ringbuf_start_wrpnt, 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_MI_VALID= %0x\n", mi_valid_wr_data);
		}
	}

	/*For SPDIF*/
	if (spdif_en) {
		if (single_buffer_en == 0) {
			/*Buffer 0*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);
			/*Buffer 1*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_RDADDR_REG, second_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_WRADDR_REG, second_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_BASEADDR_REG, second_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_ENDADDR_REG, second_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_MI_VALID_REG, mi_valid_wr_data, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_START_WRPOINT_REG, second_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_MI_VALID= %0x\n", mi_valid_wr_data);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_RDADDR= %0x\n", second_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_WRADDR= %0x\n", second_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_BASEADDR= %0x\n", second_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_ENDADDR= %0x\n", second_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_START_WRPOINT= %0x\n", second_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_MI_VALID= %0x\n", mi_valid_wr_data);

			/*Buffer 0*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_RDADDR_REG, first_ringbuf_rdaddr | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_WRADDR_REG, first_ringbuf_wraddr | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_BASEADDR_REG, first_ringbuf_base_addr | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_ENDADDR_REG, first_ringbuf_end_address | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);
			/*Buffer 1*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_RDADDR_REG, second_ringbuf_rdaddr | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_WRADDR_REG, second_ringbuf_wraddr | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_BASEADDR_REG, second_ringbuf_base_addr | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_ENDADDR_REG, second_ringbuf_end_address | audio_address_difference , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_MI_VALID_REG, mi_valid_wr_data, 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_START_WRPOINT_REG, second_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_RDADDR= %0x\n", first_ringbuf_rdaddr | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_WRADDR= %0x\n", first_ringbuf_wraddr | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_BASEADDR= %0x\n", first_ringbuf_base_addr | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_ENDADDR= %0x\n", first_ringbuf_end_address | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_MI_VALID= %0x\n", mi_valid_wr_data);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_RDADDR= %0x\n", second_ringbuf_rdaddr | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_WRADDR= %0x\n", second_ringbuf_wraddr | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_BASEADDR= %0x\n", second_ringbuf_base_addr | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_ENDADDR= %0x\n", second_ringbuf_end_address | audio_address_difference);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_START_WRPOINT= %0x\n", second_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_7_MI_VALID= %0x\n", mi_valid_wr_data);
		} else {
			/*Buffer 0*/
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_MI_VALID_REG, mi_valid_wr_data , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_6_MI_VALID= %0x\n", mi_valid_wr_data);
		}
	}
}

/*---------------------------  I2S Destination Buffer  Address Set function -----------------------------------*/
void asiu_audio_set_dest_buffer_addr(
	uint32_t i2s_port_num,
	uint32_t single_buffer_en,
	uint32_t first_ringbuf_rdaddr,
	uint32_t first_ringbuf_wraddr,
	uint32_t first_ringbuf_base_addr,
	uint32_t first_ringbuf_end_address,
	uint32_t first_ringbuf_start_wrpnt,
	uint32_t second_ringbuf_rdaddr,
	uint32_t second_ringbuf_wraddr,
	uint32_t second_ringbuf_base_addr,
	uint32_t second_ringbuf_end_address,
	uint32_t second_ringbuf_start_wrpnt,
	uint32_t audio_address_difference)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S Destination Buffer Address Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*For I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		if (single_buffer_en == 0) {
			/*Buffer 0*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);
			/*Buffer 1*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_RDADDR_REG, second_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_WRADDR_REG, second_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_BASEADDR_REG, second_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_ENDADDR_REG, second_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_START_WRPOINT_REG, second_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_RDADDR= %0x\n", second_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_WRADDR= %0x\n", second_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_BASEADDR= %0x\n", second_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_ENDADDR= %0x\n", second_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_1_START_WRPOINT= %0x\n", second_ringbuf_start_wrpnt);
		} else {
			/*Buffer 0*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_0_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
		}
	}

	/*For I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		if (single_buffer_en == 0) {
			/*Buffer 2*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_RDADDR_REG, (first_ringbuf_rdaddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_WRADDR_REG, (first_ringbuf_wraddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_BASEADDR_REG, (first_ringbuf_base_addr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_ENDADDR_REG, (first_ringbuf_end_address | audio_address_difference) , 4);
			/*cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_START_WRPOINT_REG, (first_ringbuf_start_wrpnt | audio_address_difference) , 4);*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_START_WRPOINT_REG, (first_ringbuf_start_wrpnt) , 4);
			/*Buffer 3*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_RDADDR_REG, (second_ringbuf_rdaddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_WRADDR_REG, (second_ringbuf_wraddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_BASEADDR_REG, (second_ringbuf_base_addr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_ENDADDR_REG, (second_ringbuf_end_address | audio_address_difference) , 4);
			/*cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_START_WRPOINT_REG, (second_ringbuf_start_wrpnt| audio_address_difference) , 4);*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_START_WRPOINT_REG, (second_ringbuf_start_wrpnt) , 4);

			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_RDADDR= %0x\n", (first_ringbuf_rdaddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_WRADDR= %0x\n", (first_ringbuf_wraddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_BASEADDR= %0x\n", (first_ringbuf_base_addr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_ENDADDR= %0x\n", (first_ringbuf_end_address | audio_address_difference));
			/*post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt | audio_address_difference));*/
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_RDADDR= %0x\n", (second_ringbuf_rdaddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_WRADDR= %0x\n", (second_ringbuf_wraddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_BASEADDR= %0x\n", (second_ringbuf_base_addr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_ENDADDR= %0x\n", (second_ringbuf_end_address | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_3_START_WRPOINT= %0x\n", (second_ringbuf_start_wrpnt));
		} else {
			/*Buffer 2*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_RDADDR_REG, (first_ringbuf_rdaddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_WRADDR_REG, (first_ringbuf_wraddr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_BASEADDR_REG, (first_ringbuf_base_addr | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_ENDADDR_REG, (first_ringbuf_end_address | audio_address_difference) , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_START_WRPOINT_REG, (first_ringbuf_start_wrpnt | audio_address_difference) , 4);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_RDADDR= %0x\n", (first_ringbuf_rdaddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_WRADDR= %0x\n", (first_ringbuf_wraddr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_BASEADDR= %0x\n", (first_ringbuf_base_addr | audio_address_difference));
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_ENDADDR= %0x\n", (first_ringbuf_start_wrpnt | audio_address_difference));
			/*post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt | audio_address_difference));*/
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_2_START_WRPOINT= %0x\n", (first_ringbuf_start_wrpnt));
		}
	}

	/*For I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		first_ringbuf_rdaddr = first_ringbuf_rdaddr | (audio_address_difference << 2);
		first_ringbuf_wraddr = first_ringbuf_wraddr | (audio_address_difference << 2);
		first_ringbuf_base_addr = first_ringbuf_base_addr | (audio_address_difference << 2);
		first_ringbuf_end_address = first_ringbuf_end_address | (audio_address_difference << 2);
		/*first_ringbuf_start_wrpnt = first_ringbuf_start_wrpnt | (audio_address_difference << 1);*/
		second_ringbuf_rdaddr = second_ringbuf_rdaddr | (audio_address_difference << 2);
		second_ringbuf_wraddr = second_ringbuf_wraddr | (audio_address_difference << 2);
		second_ringbuf_base_addr = second_ringbuf_base_addr | (audio_address_difference << 2);
		second_ringbuf_end_address = second_ringbuf_end_address | (audio_address_difference << 2);
		/*first_ringbuf_start_wrpnt = first_ringbuf_start_wrpnt | (audio_address_difference << 1);*/

		if (single_buffer_en == 0) {
			/*Buffer 4*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);
			/*Buffer 5*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_RDADDR_REG, second_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_WRADDR_REG, second_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_BASEADDR_REG, second_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_ENDADDR_REG, second_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_START_WRPOINT_REG, second_ringbuf_start_wrpnt , 4);

			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_RDADDR= %0x\n", second_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_WRADDR= %0x\n", second_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_BASEADDR= %0x\n", second_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_ENDADDR= %0x\n", second_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_5_START_WRPOINT= %0x\n", second_ringbuf_start_wrpnt);
		} else {
			/*Buffer 4*/
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_RDADDR_REG, first_ringbuf_rdaddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_WRADDR_REG, first_ringbuf_wraddr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_BASEADDR_REG, first_ringbuf_base_addr , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_ENDADDR_REG, first_ringbuf_end_address , 4);
			cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_START_WRPOINT_REG, first_ringbuf_start_wrpnt , 4);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_RDADDR= %0x\n", first_ringbuf_rdaddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_WRADDR= %0x\n", first_ringbuf_wraddr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_BASEADDR= %0x\n", first_ringbuf_base_addr);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_ENDADDR= %0x\n", first_ringbuf_end_address);
			post_log("AUD_FMM_BF_CTRL_DESTCH_RINGBUF_4_START_WRPOINT= %0x\n", first_ringbuf_start_wrpnt);
		}
	}
}

/*--------------------------- Start DMA Read Enable function -----------------------------------*/
void asiu_audio_start_dma_read(uint32_t spdif_en, uint32_t play_run, uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : Start DMA Read\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL0_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CTRL0_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			play_run          << AUD_FMM_BF_CTRL_SOURCECH_CTRL0_REG__PLAY_RUN);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CTRL0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL0_REG, wr_data, 4);
	}

	/*I2S1*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL1_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CTRL1_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			play_run          << AUD_FMM_BF_CTRL_SOURCECH_CTRL1_REG__PLAY_RUN);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CTRL1= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL1_REG, wr_data, 4);
	}

	/*I2S2*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL2_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CTRL2_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			play_run          << AUD_FMM_BF_CTRL_SOURCECH_CTRL2_REG__PLAY_RUN);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CTRL2= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL2_REG, wr_data, 4);
	}

	/*SPDIF*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (spdif_en) {
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL3_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_SOURCECH_CTRL2_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			play_run          << AUD_FMM_BF_CTRL_SOURCECH_CTRL3_REG__PLAY_RUN);
		post_log("AUD_FMM_BF_CTRL_SOURCECH_CTRL3= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_CTRL3_REG, wr_data, 4);
	}
}

/*--------------------------- Start Capture Enable function -----------------------------------*/
void asiu_audio_start_capture_inputs(uint32_t capture_run , uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : Start Capturing I2S Inputs\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_DESTCH_CTRL0_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_DESTCH_CTRL0_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			capture_run          << AUD_FMM_BF_CTRL_DESTCH_CTRL0_REG__CAPTURE_RUN);
		post_log("AUD_FMM_BF_CTRL_DESTCH_CTRL0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_CTRL0_REG, wr_data, 4);
	}

	/*I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_DESTCH_CTRL1_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_DESTCH_CTRL1_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			capture_run          << AUD_FMM_BF_CTRL_DESTCH_CTRL1_REG__CAPTURE_RUN);
		post_log("AUD_FMM_BF_CTRL_DESTCH_CTRL1= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_CTRL1_REG, wr_data, 4);
	}

	/*I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = 0;
		wr_data = 0;
		mask = 0;
		rd_data = cpu_rd_single(AUD_FMM_BF_CTRL_DESTCH_CTRL2_REG, 4);
		/*mask = AUD_FMM_BF_CTRL_DESTCH_CTRL2_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			capture_run          << AUD_FMM_BF_CTRL_DESTCH_CTRL2_REG__CAPTURE_RUN);
		post_log("AUD_FMM_BF_CTRL_DESTCH_CTRL2= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_DESTCH_CTRL2_REG, wr_data, 4);
	}
}

/*--------------------------- I2S Out Legacy Enable function -----------------------------------*/
void asiu_audio_i2s_out_legacy_enable(uint32_t legacy_en, uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S Out Legacy Enable\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_0_I2S_LEGACY_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_0_I2S_LEGACY_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			legacy_en          << AUD_FMM_IOP_OUT_I2S_0_I2S_LEGACY_REG__EN_LEGACY_I2S);
		post_log("AUD_FMM_IOP_OUT_I2S_0_I2S_LEGACY= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_0_I2S_LEGACY_REG, wr_data, 4);
	}

	/*I2S1*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_1_I2S_LEGACY_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_1_I2S_LEGACY_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			legacy_en          << AUD_FMM_IOP_OUT_I2S_1_I2S_LEGACY_REG__EN_LEGACY_I2S);
		post_log("AUD_FMM_IOP_OUT_I2S_1_I2S_LEGACY= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_1_I2S_LEGACY_REG, wr_data, 4);
	}

	/*I2S2*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_2_I2S_LEGACY_REG, 4);
		/*mask = AUD_FMM_IOP_OUT_I2S_2_I2S_LEGACY_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			legacy_en          << AUD_FMM_IOP_OUT_I2S_2_I2S_LEGACY_REG__EN_LEGACY_I2S);
		post_log("AUD_FMM_IOP_OUT_I2S_2_I2S_LEGACY= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_2_I2S_LEGACY_REG, wr_data, 4);
	}
}

/*--------------------------- I2S Out Sample Count -----------------------------------*/
void asiu_audio_i2s_out_sample_count(uint32_t count_start, uint32_t count_clear, uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S Out Sample Count Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_0_SAMP_COUNT_CTRL_REG, 4);
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			count_start          << AUD_FMM_IOP_OUT_I2S_0_SAMP_COUNT_CTRL_REG__COUNT_START |
			count_clear          << AUD_FMM_IOP_OUT_I2S_0_SAMP_COUNT_CTRL_REG__COUNT_CLEAR);
		post_log("AUD_FMM_IOP_OUT_I2S_0_SAMP_COUNT_CTRL= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_0_SAMP_COUNT_CTRL_REG, wr_data, 4);
	}

	/*I2S1*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_1_SAMP_COUNT_CTRL_REG, 4);
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			count_start          << AUD_FMM_IOP_OUT_I2S_1_SAMP_COUNT_CTRL_REG__COUNT_START |
			count_clear          << AUD_FMM_IOP_OUT_I2S_1_SAMP_COUNT_CTRL_REG__COUNT_CLEAR);
		post_log("AUD_FMM_IOP_OUT_I2S_1_SAMP_COUNT_CTRL= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_1_SAMP_COUNT_CTRL_REG, wr_data, 4);
	}

	/*I2S2*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_OUT_I2S_2_SAMP_COUNT_CTRL_REG, 4);
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			count_start          << AUD_FMM_IOP_OUT_I2S_2_SAMP_COUNT_CTRL_REG__COUNT_START |
			count_clear          << AUD_FMM_IOP_OUT_I2S_2_SAMP_COUNT_CTRL_REG__COUNT_CLEAR);
		post_log("AUD_FMM_IOP_OUT_I2S_2_SAMP_COUNT_CTRL= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_2_SAMP_COUNT_CTRL_REG, wr_data, 4);
	}
}

/*--------------------------- I2S In Sample Count -----------------------------------*/
void asiu_audio_i2s_in_sample_count(uint32_t in_count_start, uint32_t in_count_clear, uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S In Sample Count Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_0_SAMP_COUNT_CTRL_REG, 4);
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			in_count_start          << AUD_FMM_IOP_IN_I2S_0_SAMP_COUNT_CTRL_REG__COUNT_START |
			in_count_clear          << AUD_FMM_IOP_IN_I2S_0_SAMP_COUNT_CTRL_REG__COUNT_CLEAR);
		post_log("AUD_FMM_IOP_IN_I2S_0_SAMP_COUNT_CTRL= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_0_SAMP_COUNT_CTRL_REG, wr_data, 4);
	}

	/*I2S1*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_1_SAMP_COUNT_CTRL_REG, 4);
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			in_count_start          << AUD_FMM_IOP_IN_I2S_1_SAMP_COUNT_CTRL_REG__COUNT_START |
			in_count_clear          << AUD_FMM_IOP_IN_I2S_1_SAMP_COUNT_CTRL_REG__COUNT_CLEAR);
		post_log("AUD_FMM_IOP_IN_I2S_1_SAMP_COUNT_CTRL= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_1_SAMP_COUNT_CTRL_REG, wr_data, 4);
	}

	/*I2S2*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_2_SAMP_COUNT_CTRL_REG, 4);
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			in_count_start          << AUD_FMM_IOP_IN_I2S_2_SAMP_COUNT_CTRL_REG__COUNT_START |
			in_count_clear          << AUD_FMM_IOP_IN_I2S_2_SAMP_COUNT_CTRL_REG__COUNT_CLEAR);
		post_log("AUD_FMM_IOP_IN_I2S_2_SAMP_COUNT_CTRL= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_2_SAMP_COUNT_CTRL_REG, wr_data, 4);
	}
}

/*--------------------------- I2S In Legacy Enable function -----------------------------------*/
void asiu_audio_i2s_in_legacy_enable(uint32_t legacy_en, uint32_t i2s_port_num)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : I2S In Legacy Enable\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_0_I2S_LEGACY_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_0_I2S_LEGACY_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			legacy_en          << AUD_FMM_IOP_IN_I2S_0_I2S_LEGACY_REG__EN_LEGACY_I2S);
		post_log("AUD_FMM_IOP_IN_I2S_0_I2S_LEGACY= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_0_I2S_LEGACY_REG, wr_data, 4);
	}

	/*I2S1*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_1_I2S_LEGACY_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_1_I2S_LEGACY_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			legacy_en          << AUD_FMM_IOP_IN_I2S_1_I2S_LEGACY_REG__EN_LEGACY_I2S);
		post_log("AUD_FMM_IOP_IN_I2S_1_I2S_LEGACY= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_1_I2S_LEGACY_REG, wr_data, 4);
	}

	/*I2S2*/
	rd_data = 0;
	wr_data = 0;
	mask = 0;
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		rd_data = cpu_rd_single(AUD_FMM_IOP_IN_I2S_2_I2S_LEGACY_REG, 4);
		/*mask = AUD_FMM_IOP_IN_I2S_2_I2S_LEGACY_REG_DATAMASK;*/
		mask = 0x00000000;
		wr_data = rd_data & mask;

		wr_data = (wr_data                       |
			legacy_en          << AUD_FMM_IOP_IN_I2S_2_I2S_LEGACY_REG__EN_LEGACY_I2S);
		post_log("AUD_FMM_IOP_IN_I2S_2_I2S_LEGACY= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_IN_I2S_2_I2S_LEGACY_REG, wr_data, 4);
	}
}

/*--------------------------- PLL Clock Select function -----------------------------------*/
void asiu_audio_mclk_cfg(uint32_t pllclksel, uint32_t mclk_rate, uint32_t i2s_port_num)
{
	uint32_t wr_data = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : Mclk PLL Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S0*/
	if (i2s_port_num == 1 || i2s_port_num == 7) {
		wr_data = (wr_data                       |
			mclk_rate << AUD_FMM_IOP_OUT_I2S_0_MCLK_CFG_0_REG__MCLK_RATE_R |
			pllclksel << AUD_FMM_IOP_OUT_I2S_0_MCLK_CFG_0_REG__PLLCLKSEL_R);
		post_log("AUD_FMM_IOP_OUT_I2S_0_MCLK_CFG_0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_0_MCLK_CFG_0_REG, wr_data, 4);
	}

	wr_data = 0;

	/*I2S1*/
	if (i2s_port_num == 2 || i2s_port_num == 7) {
		wr_data = (wr_data |
			mclk_rate << AUD_FMM_IOP_OUT_I2S_1_MCLK_CFG_0_REG__MCLK_RATE_R |
			pllclksel << AUD_FMM_IOP_OUT_I2S_1_MCLK_CFG_0_REG__PLLCLKSEL_R);
		post_log("AUD_FMM_IOP_OUT_I2S_1_MCLK_CFG_0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_1_MCLK_CFG_0_REG, wr_data, 4);
	}

	wr_data = 0;

	/*I2S2*/
	if (i2s_port_num == 4 || i2s_port_num == 7) {
		wr_data = (wr_data |
			mclk_rate << AUD_FMM_IOP_OUT_I2S_2_MCLK_CFG_0_REG__MCLK_RATE_R |
			pllclksel << AUD_FMM_IOP_OUT_I2S_2_MCLK_CFG_0_REG__PLLCLKSEL_R);
		post_log("AUD_FMM_IOP_OUT_I2S_2_MCLK_CFG_0= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_IOP_OUT_I2S_2_MCLK_CFG_0_REG, wr_data, 4);
	}
}

/*---------------------------  Gen PLL Configuration function -----------------------------------*/
void asiu_audio_gen_pll_pwr_on(uint32_t crmu_pll_pwr_on)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : CRMU Control Reigster Write -- Audio PLL Power On\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*post_log("Before Change: CRMU_PLL_AON_CTRL= %0x\n", wr_data);*/
	if (crmu_pll_pwr_on == 1) {
		cpu_wr_single(CRMU_PLL_AON_CTRL, ((cpu_rd_single(CRMU_PLL_AON_CTRL, 4) & 0xfffffbff) | 0x00000400) , 4);
		udelay(10000);
		cpu_wr_single(CRMU_PLL_AON_CTRL, ((cpu_rd_single(CRMU_PLL_AON_CTRL, 4) & 0xfffff5ff) | 0x00000a00) , 4);
		udelay(10000);
		cpu_wr_single(CRMU_PLL_AON_CTRL, ((cpu_rd_single(CRMU_PLL_AON_CTRL, 4) & 0xfffffeff)) , 4);
		post_log("CRMU_PLL_AON_CTRL= %0x\n", cpu_rd_single(CRMU_PLL_AON_CTRL, 4));
		udelay(2000);
	}
}

/*---------------------------  Gen PLL Configuration function -----------------------------------*/
void asiu_audio_gen_pll_group_id_config(uint32_t pdiv, uint32_t ndiv_int, uint32_t ndiv_frac, uint32_t mdiv, uint32_t gain_ki, uint32_t gain_ka, uint32_t gain_kp, uint32_t user_macro, uint32_t i2s_port_num, uint32_t audio_ext_test_clock_en)
{
	uint32_t wr_data_user = 0;
	uint32_t wr_data_clk_gate = 0;
	uint32_t wr_data = 0;
	uint32_t reset_val = 0;
	uint32_t rd_data = 0;
	uint32_t timeout = 120;
	uint32_t bypass_disable = 0;
	uint32_t mask = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API :  Gen. PLL Configuration\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*Enable AXI clock*/
	rd_data = cpu_rd_single(ASIU_TOP_CLK_GATING_CTRL, 4);
	wr_data_clk_gate = (rd_data |
					1 << ASIU_TOP_CLK_GATING_CTRL__AUD_CLK_GATE_EN);
	cpu_wr_single(ASIU_TOP_CLK_GATING_CTRL, wr_data_clk_gate , 4);

	if (audio_ext_test_clock_en != 1) {
		/*User Macro select*/
		wr_data_user = (wr_data_user                       |
					user_macro          << AUD_FMM_IOP_PLL_0_MACRO_REG__MACRO_SELECT_R
				);
		post_log("AUD_FMM_IOP_PLL_0_MACRO= %0x\n", wr_data_user);
		cpu_wr_single(AUD_FMM_IOP_PLL_0_MACRO_REG, wr_data_user, 4);

		/*Bypass Disable Ch0*/
		rd_data = cpu_rd_single(AUD_FMM_IOP_PLL_0_USER_MDIV_Ch0_REG, 4);
		mask = AUD_FMM_IOP_PLL_0_USER_MDIV_Ch0_REG_DATAMASK;
		bypass_disable = rd_data & mask & 0xfffffcff;

		post_log("AUD_FMM_IOP_PLL_0_USER_MDIV_Ch0= %0x\n", bypass_disable);
		cpu_wr_single(AUD_FMM_IOP_PLL_0_USER_MDIV_Ch0_REG, bypass_disable , 4);

		bypass_disable = 0;
		mask = 0;

		/*Bypass Disable Ch1*/
		rd_data = cpu_rd_single(AUD_FMM_IOP_PLL_0_USER_MDIV_Ch1_REG, 4);
		mask = AUD_FMM_IOP_PLL_0_USER_MDIV_Ch1_REG_DATAMASK;
		bypass_disable = rd_data & mask & 0xfffffcff;
		post_log("AUD_FMM_IOP_PLL_0_USER_MDIV_Ch1= %0x\n", bypass_disable);
		cpu_wr_single(AUD_FMM_IOP_PLL_0_USER_MDIV_Ch1_REG, bypass_disable , 4);

		bypass_disable = 0;
		mask = 0;

		/*Bypass Disable Ch2*/
		rd_data = cpu_rd_single(AUD_FMM_IOP_PLL_0_USER_MDIV_Ch2_REG, 4);
		mask = AUD_FMM_IOP_PLL_0_USER_MDIV_Ch2_REG_DATAMASK;
		bypass_disable = rd_data & mask & 0xfffffcff;
		post_log("AUD_FMM_IOP_PLL_0_USER_MDIV_Ch2= %0x\n", bypass_disable);
		cpu_wr_single(AUD_FMM_IOP_PLL_0_USER_MDIV_Ch2_REG, bypass_disable , 4);

		/*resetb & post_resetb Assertion*/
		reset_val = (reset_val                       |
					1          << AUD_FMM_IOP_PLL_0_RESET_REG__RESETD |
					1          << AUD_FMM_IOP_PLL_0_RESET_REG__RESETA
					);
		post_log("AUD_FMM_IOP_PLL_0_RESET= %0x\n", reset_val);
		cpu_wr_single(AUD_FMM_IOP_PLL_0_RESET_REG, reset_val, 4);

		udelay(1000);

		reset_val = 0;
		/*resetb de-assertion*/
		reset_val = (reset_val                       |
					1          << AUD_FMM_IOP_PLL_0_RESET_REG__RESETD |
					0          << AUD_FMM_IOP_PLL_0_RESET_REG__RESETA
					);
		post_log("AUD_FMM_IOP_PLL_0_RESET= %0x\n", reset_val);
		cpu_wr_single(AUD_FMM_IOP_PLL_0_RESET_REG, reset_val, 4);

		udelay(1000);

		reset_val = 0;
		/*post_resetb de-assertion*/
		reset_val = (reset_val                       |
					0          << AUD_FMM_IOP_PLL_0_RESET_REG__RESETD |
					0          << AUD_FMM_IOP_PLL_0_RESET_REG__RESETA
					);
		post_log("AUD_FMM_IOP_PLL_0_RESET= %0x\n", reset_val);
		cpu_wr_single(AUD_FMM_IOP_PLL_0_RESET_REG, reset_val, 4);

		post_log("\nWaiting For PLL Lock\n");
		do {
			rd_data = cpu_rd_single(AUD_FMM_IOP_PLL_0_LOCK_STATUS_REG, 4);
			timeout--;
			/*post_log("timeout = %d",timeout);*/
		} while ((rd_data  != 0x00000001) & (timeout > 0));

		if (rd_data  == ((0x00000001 & timeout) != 0))
			post_log("\nPLL Lock Is Asserted\n");
		else if (timeout == 0)
			post_log("\nERROR: PLL Lock Timeout\n");
	} else {
		post_log("AUDIO TEST MODE : PLL Configuration Disabled From Scenario\n");
	}

	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP0_REG, 0x00000000, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP1_REG, 0x00000001, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP2_REG, 0x00000002, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP3_REG, 0x00000003, 4);

	/*Enable Audio As Secure Master*/
	rd_data = cpu_rd_single(ASIU_TOP_AUD_AXI_SB_CTRL, 4);
	wr_data = rd_data &  0xfc0fffff;
	cpu_wr_single(ASIU_TOP_AUD_AXI_SB_CTRL, wr_data , 4);
	post_log("ASIU_TOP_AUD_AXI_SB_CTRL= %0x\n", wr_data);
}

/*  --------------------------- function ----------------------------------- */
void asiu_audio_WRITE_REG_api(uint32_t addr, uint32_t data)
{
	cpu_wr_single(addr, data, 4);
	post_log(" API-WRITE REG - Address = %0x  Data = %0x\n", addr, data);
}

/*  --------------------------- function ----------------------------------- */
uint32_t asiu_audio_READ_REG_api(uint32_t addr)
{
	uint32_t rd_data;
	rd_data = cpu_rd_single(addr, 4);
	post_log(" API-READ REG - Address = %0x  Data = %0x\n", addr, rd_data);
	return rd_data;
}

/*--------------------------- Source FIFOinterrupt Set function -----------------------------------*/
void audio_source_fifo_intr_set(uint32_t i2s_port_number)
{
	uint32_t wr_data = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : Source FIFOinterrupt\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S 0,1,2*/
	if (i2s_port_number == 7) {
		wr_data = (wr_data                       |
		1   << AUD_FMM_BF_ESR_ESR0_MASK_CLEAR_REG__SOURCE_FIFO_0_UNDERFLOW |
		1   << AUD_FMM_BF_ESR_ESR0_MASK_CLEAR_REG__SOURCE_FIFO_1_UNDERFLOW |
		1   << AUD_FMM_BF_ESR_ESR0_MASK_CLEAR_REG__SOURCE_FIFO_2_UNDERFLOW |
		0   << AUD_FMM_BF_ESR_ESR0_MASK_CLEAR_REG__SOURCE_FIFO_3_UNDERFLOW);

		post_log("AUD_FMM_BF_ESR_ESR0_MASK_CLEAR= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_ESR_ESR0_MASK_CLEAR_REG, wr_data, 4);

		wr_data = 0;

		wr_data = (wr_data                       |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF0 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF1 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF2 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF3 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF4 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_IOP);

		post_log("AUD_INTH_R5F_MASK_CLEAR= %0x\n", wr_data);
		cpu_wr_single(AUD_INTH_R5F_MASK_CLEAR_REG, wr_data, 4);
	}
}

/*--------------------------- Source Ring Bufferinterrupt Set function -----------------------------------*/
void audio_source_rbf_intr_set(uint32_t i2s_port_number)
{
	uint32_t wr_data = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : Source Ring Bufferinterrupt\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S 0,1,2*/
	if (i2s_port_number == 7) {
		wr_data = (wr_data                       |
		1   << AUD_FMM_BF_ESR_ESR1_MASK_CLEAR_REG__SOURCE_RINGBUF_0_UNDERFLOW |
		1   << AUD_FMM_BF_ESR_ESR1_MASK_CLEAR_REG__SOURCE_RINGBUF_1_UNDERFLOW |
		1   << AUD_FMM_BF_ESR_ESR1_MASK_CLEAR_REG__SOURCE_RINGBUF_2_UNDERFLOW |
		0   << AUD_FMM_BF_ESR_ESR1_MASK_CLEAR_REG__SOURCE_RINGBUF_3_UNDERFLOW);

		post_log("AUD_FMM_BF_ESR_ESR1_MASK_CLEAR= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_ESR_ESR1_MASK_CLEAR_REG, wr_data, 4);

		wr_data = 0;

		wr_data = (wr_data                       |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF0 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF1 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF2 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF3 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF4 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_IOP);

		post_log("AUD_INTH_R5F_MASK_CLEAR= %0x\n", wr_data);
		cpu_wr_single(AUD_INTH_R5F_MASK_CLEAR_REG, wr_data, 4);
	}
}

/*--------------------------- Source Ring Buffer Freemarkinterrupt -----------------------------------*/
void audio_source_rbf_freemark_intr_set(uint32_t i2s_port_number, uint32_t freemark_bytes)
{
	uint32_t wr_data = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : Source Ring Buffer Freemarkinterrupt\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	/*I2S 0,1,2*/
	if (i2s_port_number == 7) {

		post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_FREE_MARk = %0x\n", freemark_bytes);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_0_FREE_MARK_REG, freemark_bytes, 4);

		post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_FREE_MARk = %0x\n", freemark_bytes);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_1_FREE_MARK_REG, freemark_bytes, 4);

		post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_FREE_MARk = %0x\n", freemark_bytes);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_2_FREE_MARK_REG, freemark_bytes, 4);

		post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_FREE_MARk = %0x\n", freemark_bytes);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_3_FREE_MARK_REG, freemark_bytes, 4);

		post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_FREE_MARk = %0x\n", freemark_bytes);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_4_FREE_MARK_REG, freemark_bytes, 4);

		post_log("AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_FREE_MARk = %0x\n", freemark_bytes);
		cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_RINGBUF_5_FREE_MARK_REG, freemark_bytes, 4);

		wr_data = 0x00000007;

		post_log("AUD_FMM_BF_CTRL_REARM_FREE_MARk = %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_CTRL_REARM_FREE_MARK_REG, wr_data , 4);

		wr_data = 0;

		wr_data = (wr_data                       |
		1   << AUD_FMM_BF_ESR_ESR3_MASK_CLEAR_REG__SOURCE_RINGBUF_0_EXCEED_FREEMARK |
		1   << AUD_FMM_BF_ESR_ESR3_MASK_CLEAR_REG__SOURCE_RINGBUF_1_EXCEED_FREEMARK |
		1   << AUD_FMM_BF_ESR_ESR3_MASK_CLEAR_REG__SOURCE_RINGBUF_2_EXCEED_FREEMARK |
		1   << AUD_FMM_BF_ESR_ESR3_MASK_CLEAR_REG__SOURCE_RINGBUF_3_EXCEED_FREEMARK);

		post_log("AUD_FMM_BF_ESR_ESR3_MASK_CLEAR= %0x\n", wr_data);
		cpu_wr_single(AUD_FMM_BF_ESR_ESR3_MASK_CLEAR_REG, wr_data, 4);

		wr_data = 0;

		wr_data = (wr_data                       |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF0 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF1 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF2 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF3 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_BF4 |
		1   << AUD_INTH_R5F_MASK_CLEAR_REG__AUD_IOP);

		post_log("AUD_INTH_R5F_MASK_CLEAR= %0x\n", wr_data);
		cpu_wr_single(AUD_INTH_R5F_MASK_CLEAR_REG, wr_data, 4);
	}
}

/* To write DTE BLOCK registers */
void dte_register_write(uint32_t a, uint32_t d)
{
	cpu_wr_single(a, d, 4);
#ifdef AUDIO_DTE_DEBUG_ENABLE
#ifdef AUDIO_DTE_LOG_ALL_REGACCESS
	post_log("DTE_WRITE_REGISTER - Address = 0x%X, Data = 0x%X\n", a, d);
#endif
#endif
}

/* To read DTE BLOCK registers */
uint32_t dte_register_read(uint32_t a)
{
	uint32_t d = cpu_rd_single(a, 4);
#ifdef AUDIO_DTE_DEBUG_ENABLE
#ifdef AUDIO_DTE_LOG_ALL_REGACCESS
	post_log("DTE_READ_REGISTER - Address = 0x%X, Read Data = 0x%X\n", a, d);
#endif
#endif
	return d;
}

void asiu_audio_dte_lts_src_en(uint32_t src_en_data, uint32_t audio_dte_both_edge_en)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE LTS SOURCE ENABLE\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	uint32_t wr_data = 0;
	uint32_t rd_data = 0;
	uint32_t rd_status;

	/*uint32_t csr_data = 0xffffffff;*/
	/*post_log("EAV_DTE_DTE_LTS_CSR= %0x\n", csr_data);*/
	/*cpu_wr_single(EAV_DTE_DTE_LTS_CSR, csr_data, 4);*/

	rd_status = cpu_rd_single(EAV_DTE_DTE_LTS_CSR_REG, 4);
	post_log("EAV_DTE_DTE_LTS_CSR_REG= %0x\n", rd_status);
	if ((rd_status & 0x00000010) == 0) {
		post_log("DTE FIFO IS NOT EMPTY !!!!!!!\n");
		do {
			rd_data = cpu_rd_single(EAV_DTE_DTE_LTS_FIFO_REG, 4);
			rd_status = cpu_rd_single(EAV_DTE_DTE_LTS_CSR_REG, 4);
			rd_data = cpu_rd_single(EAV_DTE_DTE_LTS_FIFO_REG, 4);
		} while ((rd_status & 0x00000010) == 0) ;
	}

	post_log("EAV_DTE_DTE_LTS_SRC_EN_REG= %0x\n", src_en_data);
	cpu_wr_single(EAV_DTE_DTE_LTS_SRC_EN_REG, src_en_data, 4);

	if (audio_dte_both_edge_en) {
		rd_data = cpu_rd_single(ASIU_TOP_MISC, 4);
		wr_data = (rd_data & 0xf1ffffff) | 0x07000000;
		cpu_wr_single(ASIU_TOP_MISC, wr_data, 4);
		post_log("ASIU_TOP_MISC= %0x\n", wr_data);
	}
 }

/*  --------------------------- DTE 54 DIV  Value ----------------------------------- */
void asiu_audio_dte_lts_div_54(uint32_t i2s0_bit_clk_div, uint32_t i2s1_bit_clk_div)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE : I2S0 & 1 BIT CLOCK DIV VALUE\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	uint32_t wr_data = 0;

	wr_data = (wr_data |
			i2s0_bit_clk_div << 16 |
			i2s1_bit_clk_div << 0);

	post_log("EAV_DTE_DTE_LTS_DIV_54_REG= %0x\n", wr_data);
	cpu_wr_single(EAV_DTE_DTE_LTS_DIV_54_REG, wr_data, 4);
}

/*  --------------------------- DTE 76 DIV  Value ----------------------------------- */
void asiu_audio_dte_lts_div_76(uint32_t i2s2_bit_clk_div, uint32_t ws_i2s0_clk_div)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE : I2S2 BIT & I2S0 LR CLOCK DIV VALUE\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	uint32_t wr_data = 0;

	wr_data = (wr_data |
		i2s2_bit_clk_div << 16 |
		ws_i2s0_clk_div << 0);

	post_log("EAV_DTE_DTE_LTS_DIV_76_REG= %0x\n", wr_data);
	cpu_wr_single(EAV_DTE_DTE_LTS_DIV_76_REG, wr_data, 4);
}

/*  --------------------------- DTE 98 DIV  Value ----------------------------------- */
void asiu_audio_dte_lts_div_98(uint32_t ws_i2s1_clk_div, uint32_t ws_i2s2_clk_div)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE : I2S1 & 2 LR CLOCK DIV VALUE\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	uint32_t wr_data = 0;

	wr_data = (wr_data |
		ws_i2s1_clk_div << 16 |
		ws_i2s2_clk_div << 0);

	post_log("EAV_DTE_DTE_LTS_DIV_98_REG= %0x\n", wr_data);
	cpu_wr_single(EAV_DTE_DTE_LTS_DIV_98_REG, wr_data, 4);
}

/*  --------------------------- DTE 1110 DIV  Value ----------------------------------- */
void asiu_audio_dte_lts_div_1110(uint32_t lts_ext_1_div, uint32_t lts_ext_2_div)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE : EXT. 1 & 2 DIV VALUE\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	uint32_t wr_data = 0;

	wr_data = (wr_data |
		lts_ext_1_div << 16 |
		lts_ext_2_div << 0);

	post_log("EAV_DTE_DTE_LTS_DIV_1110_REG= %0x\n", wr_data);
	cpu_wr_single(EAV_DTE_DTE_LTS_DIV_1110_REG, wr_data, 4);
}

/*  --------------------------- DTE 1312 DIV  Value ----------------------------------- */
 void asiu_audio_dte_lts_div_1312(uint32_t lts_ext_3_div, uint32_t lts_ext_4_div)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE : EXT. 3 & 4 DIV VALUE\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	uint32_t wr_data = 0;

	wr_data = (wr_data|
		lts_ext_3_div << 16 |
		lts_ext_4_div << 0);

	post_log("EAV_DTE_DTE_LTS_DIV_1312_REG= %0x\n", wr_data);
	cpu_wr_single(EAV_DTE_DTE_LTS_DIV_1312_REG, wr_data, 4);
}

/*  --------------------------- DTE 1514 DIV  Value ----------------------------------- */
void asiu_audio_dte_lts_div_1514(uint32_t lts_ext_5_div, uint32_t lts_ext_6_div)
{
	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE : EXT. 5 & 6 DIV VALUE\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	uint32_t wr_data = 0;

	wr_data = (wr_data |
		lts_ext_5_div << 16 |
		lts_ext_6_div << 0);

	post_log("EAV_DTE_DTE_LTS_DIV_14_REG= %0x\n", wr_data);
	cpu_wr_single(EAV_DTE_DTE_LTS_DIV_14_REG, wr_data, 4);
}

/*  --------------------------- DTEinterrupt SOI  Value ----------------------------------- */
void asiu_audio_dte_intr_config(uint32_t soi, uint32_t interval_length)
{
	uint32_t rd_data = 0;
	uint32_t wr_data = 0;

	post_log("--------------------------------------------------------------------------------------------------------\n");
	post_log("AUDIO C API : DTE :intERRUPT CONFIGURATION\n");
	post_log("--------------------------------------------------------------------------------------------------------\n");

	post_log("EAV_DTE_DTE_ILEN= %0x\n", interval_length);
	cpu_wr_single(EAV_DTE_DTE_ILEN_REG, interval_length, 4);

	rd_data = cpu_rd_single(EAV_DTE_DTE_NCO_TIME_REG, 4);

	/* Multiplication of 16 is done with sum2 read data
	 * As per the Figure "Figure 3 2 LTNCO Block Diagram",
	 * The local time is left shift 4 of sum2 register
	 */

	wr_data = (rd_data*16) + soi;
	cpu_wr_single(EAV_DTE_DTE_NEXT_SOI_REG, wr_data, 4);
	post_log("EAV_DTE_DTE_NEXT_SOi = %0x\n", wr_data);
}

uint32_t iomux_select_i2s_0_if_en_interface(void)
{
	uint32_t rdata, wdata, wdata_tmp, mask;

	rdata = cpu_rd_single(CRMU_IOMUX_CTRL3, 4);
	mask = 0xff0fffff;
	wdata_tmp = 0x00100000;
	wdata = (rdata & mask) | (wdata_tmp);
	cpu_wr_single(CRMU_IOMUX_CTRL3, wdata, 4);

	rdata = cpu_rd_single(SMART_CARD_FCB_SEL, 4);
	post_log("CRMU_IOMUX_CTRL3: %08X\n", rdata);

	mask = 0xfffffff0;
	wdata_tmp = 0x00000001;
	wdata = (rdata & mask) | (wdata_tmp);
	cpu_wr_single(SMART_CARD_FCB_SEL, wdata, 4);
	rdata = cpu_rd_single(SMART_CARD_FCB_SEL, 4);
	post_log("SMART_CARD_FCB: %08X\n", rdata);

	return 0;
}

int asiu_test_captured_samples(uint32_t *src_addr, uint32_t *dest_addr, uint32_t samples)
{
	uint32_t i;
	uint32_t error;
	error = 0;
	for (i = 0; i < samples; i++) {
		if (*src_addr != *dest_addr)
			error++;
	}

	if (error == 0) {
		post_log("Test PASSED\n");
		return 0;
	} else {
		post_log("Test Failed\n");
		return -1;
	}
}

void smbus_init(uint32_t speed_mode)
{
	uint32_t data;

	data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Config, 4);
	post_log("ChipcommonG_SMBus0_SMBus_Config = 0x%08X\n", data);
	data |= (1 << ChipcommonG_SMBus0_SMBus_Config__SMB_EN);
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Config, data, 4);
	data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Config, 4);
	post_log("ChipcommonG_SMBus0_SMBus_Config = 0x%08X\n", data);

	data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Config, 4);
	data |= (1 << ChipcommonG_SMBus0_SMBus_Config__RESET);
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Config, data, 4);
	data &= ~(1 << ChipcommonG_SMBus0_SMBus_Config__RESET);
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Config, data, 4);

	if (speed_mode) {
		data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Timing_Config, 4);
		data |= (1 << ChipcommonG_SMBus0_SMBus_Timing_Config__MODE_400);
		cpu_wr_single(ChipcommonG_SMBus0_SMBus_Timing_Config, data, 4);
		post_log("Configuring SMBUS in 400KHz mode\n");
	} else {
		data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Timing_Config, 4);
		data &= ~(1 << ChipcommonG_SMBus0_SMBus_Timing_Config__MODE_400);
		cpu_wr_single(ChipcommonG_SMBus0_SMBus_Timing_Config, data, 4);
		post_log("Configuring SMBUS in 100KHz mode\n");
	}
}

void smbus1_init(uint32_t speed_mode)
{
	uint32_t data;

	data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Config, 4);
	post_log("ChipcommonG_SMBus1_SMBus_Config = 0x%08X\n", data);
	data |= (1 << ChipcommonG_SMBus1_SMBus_Config__SMB_EN);
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Config, data, 4);
	data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Config, 4);
	post_log("ChipcommonG_SMBus1_SMBus_Config = 0x%08X\n", data);

	data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Config, 4);
	data |= (1 << ChipcommonG_SMBus1_SMBus_Config__RESET);
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Config, data, 4);
	data &= ~(1 << ChipcommonG_SMBus1_SMBus_Config__RESET);
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Config, data, 4);

	if (speed_mode) {
		data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Timing_Config, 4);
		data |= (1 << ChipcommonG_SMBus1_SMBus_Timing_Config__MODE_400);
		cpu_wr_single(ChipcommonG_SMBus1_SMBus_Timing_Config, data, 4);
		post_log("Configuring SMBUS in 400KHz mode\n");
	} else {
		data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Timing_Config, 4);
		data &= ~(1 << ChipcommonG_SMBus1_SMBus_Timing_Config__MODE_400);
		cpu_wr_single(ChipcommonG_SMBus1_SMBus_Timing_Config, data, 4);
		post_log("Configuring SMBUS in 100KHz mode\n");
	}
}

int smbus_slave_presence(uint32_t slave_address)
{
	uint32_t data;
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Master_Data_Write, (slave_address<<1), 4);
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Master_Command, ((0 << ChipcommonG_SMBus0_SMBus_Master_Command__SMBUS_PROTOCOL_R) | (1<<31)), 4);
	do {
		data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Master_Command, 4);
		data &= (1 << 31);
	} while (data);
	data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Master_Command, 4);
	data = (data >> 25) & 0x7;

	if (data == 0) {
		post_log("Slave present with address: 0x%08x\n", slave_address);
		/*return 0;*/
	} else {
		post_log("Error status for write tranasction - : 0x%08x - Addr : 0x%08X\n", data, slave_address);
		/*return -1;*/
	}
	return 0;
}

int smbus1_slave_presence(uint32_t slave_address)
{
	uint32_t data;
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Master_Data_Write, (slave_address<<1), 4);
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Master_Command, ((0 << ChipcommonG_SMBus1_SMBus_Master_Command__SMBUS_PROTOCOL_R) | (1<<31)), 4);
	do {
		data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Master_Command, 4);
		data &= (1 << 31);
	} while (data);
	data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Master_Command, 4);
	data = (data >> 25) & 0x7;

	if (data == 0) {
		post_log("Slave present with address: 0x%08x\n", slave_address);
		return 0;
	} else {
		post_log("Error status for write tranasction - : 0x%08x - Addr : 0x%08X\n", data, slave_address);
		return -1;
	}
}

int smbus_wm8750_write(uint32_t slave_addr, uint32_t control_byte1, uint32_t control_byte2)
{
	uint32_t data;
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Master_Data_Write, ((slave_addr << 1) | 0), 4);  /*write to WM8750L*/
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Master_Data_Write, control_byte1, 4);
	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Master_Data_Write, control_byte2, 4);

	cpu_wr_single(ChipcommonG_SMBus0_SMBus_Master_Command, ((0x7 << ChipcommonG_SMBus0_SMBus_Master_Command__SMBUS_PROTOCOL_R) | (1<<31)), 4);

	/*dummy read*/
	data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Master_Command, 4);
	do {
		data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Master_Command, 4);
		data &= (1 << 31);
	} while (data);

	/*check for smb_errors*/
	data = cpu_rd_single(ChipcommonG_SMBus0_SMBus_Master_Command, 4);
	data = (data >> 25) & 0x7;
	if (data != 0) {
		post_log("\n\rError occured ...!!!\n\r");
		return -1;
	} else {
//		post_log("SMBUS TRANSACTION PASSED FOR SLAVE ADDRESS : %08X WITH CONTROL BYTE 1 : %08X AND CONTROL BYTE 2 : %08X\n", slave_addr, control_byte1, control_byte2);
		return 0;
	}
}

int smbus_write(uint32_t slave_addr, uint32_t control_byte1, uint32_t control_byte2)
{
	uint32_t data;
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Master_Data_Write, ((slave_addr << 1) | 0), 4);  /*write to WM8750L*/
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Master_Data_Write, control_byte1, 4);
	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Master_Data_Write, control_byte2, 4);

	cpu_wr_single(ChipcommonG_SMBus1_SMBus_Master_Command, ((0x7 << ChipcommonG_SMBus1_SMBus_Master_Command__SMBUS_PROTOCOL_R) | (1<<31)), 4);

	/*dummy read*/
	data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Master_Command, 4);
	do {
		data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Master_Command, 4);
		data &= (1 << 31);
	} while (data);

	/*check for smb_errors*/
	data = cpu_rd_single(ChipcommonG_SMBus1_SMBus_Master_Command, 4);
	data = (data >> 25) & 0x7;
	if (data != 0) {
		post_log("\n\rError occured ...!!!\n\r");
		return -1;
	} else {
		post_log("SMBUS TRANSACTION PASSED FOR SLAVE ADDRESS : %08X WITH CONTROL BYTE 1 : %08X AND CONTROL BYTE 2 : %08X\n", slave_addr, control_byte1, control_byte2);
		return 0;
	}
}

void mustang_audio_sw_reset(void)
{
	cpu_wr_single(CRMU_CHIP_IO_PAD_CONTROL, 0, 4);
	/*ASIU SW RESET*/
	cpu_wr_single(ASIU_TOP_SW_RESET_CTRL, 0x00000000, 4);
	cpu_wr_single(ASIU_TOP_SW_RESET_CTRL, 0x000003ff, 4);
	udelay(1000);
}

void mustang_audio_io_mux_select(void)
{
	uint32_t data;
	/*IOMUX PROGRAMMING FOR I2Sx*/
//	cpu_wr_single(CRMU_IOMUX_CTRL0, 0x22222, 4);

	/*IOMUX PROGRAMMING FOR I2S1 and I2S2 AND SPDIF*/
	data = cpu_rd_single(CRMU_IOMUX_CTRL3, 4);
	data &=~(0x00700000);
	data |= 0x00100000;
	cpu_wr_single(CRMU_IOMUX_CTRL3, data, 4);
	cpu_wr_single(SMART_CARD_FCB_SEL, 0x00000001, 4);

}
void mustang_audio_clock_gating_disable(void)
{
	cpu_wr_single(ASIU_TOP_CLK_GATING_CTRL, 0x00000002, 4);

	/*default Group ID configurations also*/
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP0_REG, 0x0, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP1_REG, 0x1, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP2_REG, 0x2, 4);
	cpu_wr_single(AUD_FMM_BF_CTRL_SOURCECH_GRP3_REG, 0x3, 4);
}

void mustang_audio_pad_enable(void)
{
	uint32_t data;
//	cpu_wr_single(CRMU_CHIP_IO_PAD_CONTROL, 0, 4);
	data = cpu_rd_single(ASIU_TOP_PAD_CTRL_0, 4);
	data |= 0x1;
	cpu_wr_single(ASIU_TOP_PAD_CTRL_0, data, 4);
}

void mustang_audio_gen_pll_pwr_on(uint32_t crmu_pll_pwr_on)
{
	if (crmu_pll_pwr_on == 1) {
		cpu_wr_single(CRMU_PLL_AON_CTRL, ((cpu_rd_single(CRMU_PLL_AON_CTRL, 4) & 0xfffffbff) | 0x00000400) , 4);
		udelay(10000);
		cpu_wr_single(CRMU_PLL_AON_CTRL, ((cpu_rd_single(CRMU_PLL_AON_CTRL, 4) & 0xfffff5ff) | 0x00000a00) , 4);
		udelay(10000);
		cpu_wr_single(CRMU_PLL_AON_CTRL, ((cpu_rd_single(CRMU_PLL_AON_CTRL, 4) & 0xfffffeff)) , 4);
		post_log("CRMU_PLL_AON_CTRL= %0x\n", cpu_rd_single(CRMU_PLL_AON_CTRL, 4));
		udelay(2000);
	}
}

