#ifndef _TOUCHSCREEN_APIS_H
#define _TOUCHSCREEN_APIS_H

// #include "../../../arch/arm/include/asm/arch-bcmcygnus/socregs.h"
#define TSCRegCtl1 0x180a6000
#define TSCRegCtl2 0x180a6004
#define TSCRegCtl2__average_data_R 8
#define TS_Interrupt_Thres 0x180a6008
/*
#define TS_Interrupt_Thres_BASE 0x008
#define TS_Interrupt_Thres_OFFSET 0x180a6008
#define TS_Interrupt_Thres__int_th_L 4
#define TS_Interrupt_Thres__int_th_R 0
#define TS_Interrupt_Thres__int_th_WIDTH 5
#define TS_Interrupt_Thres__int_th_RESETVALUE 0xc
#define TS_Interrupt_Thres__RESERVED_L 31
#define TS_Interrupt_Thres__RESERVED_R 5
#define TS_Interrupt_Thres_WIDTH 5
#define TS_Interrupt_Thres__WIDTH 5
#define TS_Interrupt_Thres_ALL_L 4
#define TS_Interrupt_Thres_ALL_R 0
#define TS_Interrupt_Thres__ALL_L 4
#define TS_Interrupt_Thres__ALL_R 0
#define TS_Interrupt_Thres_DATAMASK 0x0000001f
#define TS_Interrupt_Thres_RESETVALUE 0xc
*/
#define TS_Interrupt_Mask 0x180a600c
/*
#define TS_Interrupt_Mask_BASE 0x00c
#define TS_Interrupt_Mask_OFFSET 0x180a600c
#define TS_Interrupt_Mask__msk_L 7
#define TS_Interrupt_Mask__msk_R 0
#define TS_Interrupt_Mask__msk_WIDTH 8
#define TS_Interrupt_Mask__msk_RESETVALUE 0x00
#define TS_Interrupt_Mask__RESERVED_L 31
#define TS_Interrupt_Mask__RESERVED_R 8
#define TS_Interrupt_Mask_WIDTH 8
#define TS_Interrupt_Mask__WIDTH 8
#define TS_Interrupt_Mask_ALL_L 7
#define TS_Interrupt_Mask_ALL_R 0
#define TS_Interrupt_Mask__ALL_L 7
#define TS_Interrupt_Mask__ALL_R 0
#define TS_Interrupt_Mask_DATAMASK 0x000000ff
#define TS_Interrupt_Mask_RESETVALUE 0x0
*/
#define TS_Interrupt_Status 0x180a6010
/*
#define TS_Interrupt_Status_BASE 0x010
#define TS_Interrupt_Status_OFFSET 0x180a6010
#define TS_Interrupt_Status__adc_touch_detect_irq 7
#define TS_Interrupt_Status__adc_touch_detect_irq_WIDTH 1
#define TS_Interrupt_Status__adc_touch_detect_irq_RESETVALUE 0x0
#define TS_Interrupt_Status__adc_softdata_irq 6
#define TS_Interrupt_Status__adc_softdata_irq_WIDTH 1
#define TS_Interrupt_Status__adc_softdata_irq_RESETVALUE 0x0
#define TS_Interrupt_Status__adc_rdy_irq 5
#define TS_Interrupt_Status__adc_rdy_irq_WIDTH 1
#define TS_Interrupt_Status__adc_rdy_irq_RESETVALUE 0x0
#define TS_Interrupt_Status__adc_strt_irq 4
#define TS_Interrupt_Status__adc_strt_irq_WIDTH 1
#define TS_Interrupt_Status__adc_strt_irq_RESETVALUE 0x0
#define TS_Interrupt_Status__auxin_avail_irq 3
#define TS_Interrupt_Status__auxin_avail_irq_WIDTH 1
#define TS_Interrupt_Status__auxin_avail_irq_RESETVALUE 0x0
#define TS_Interrupt_Status__fifo_thresh_irq 2
#define TS_Interrupt_Status__fifo_thresh_irq_WIDTH 1
#define TS_Interrupt_Status__fifo_thresh_irq_RESETVALUE 0x0
#define TS_Interrupt_Status__fifo_ovfl_irq 1
#define TS_Interrupt_Status__fifo_ovfl_irq_WIDTH 1
#define TS_Interrupt_Status__fifo_ovfl_irq_RESETVALUE 0x0
#define TS_Interrupt_Status__set_penirq 0
#define TS_Interrupt_Status__set_penirq_WIDTH 1
#define TS_Interrupt_Status__set_penirq_RESETVALUE 0x0
#define TS_Interrupt_Status__RESERVED_L 31
#define TS_Interrupt_Status__RESERVED_R 8
#define TS_Interrupt_Status_WIDTH 8
#define TS_Interrupt_Status__WIDTH 8
#define TS_Interrupt_Status_ALL_L 7
#define TS_Interrupt_Status_ALL_R 0
#define TS_Interrupt_Status__ALL_L 7
#define TS_Interrupt_Status__ALL_R 0
#define TS_Interrupt_Status_DATAMASK 0x000000ff
#define TS_Interrupt_Status_RESETVALUE 0x0
*/
#define TS_Controller_Status 0x180a6014
/*
#define TS_Controller_Status_BASE 0x014
#define TS_Controller_Status_OFFSET 0x180a6014
#define TS_Controller_Status__fsm1_stat_L 15
#define TS_Controller_Status__fsm1_stat_R 12
#define TS_Controller_Status__fsm1_stat_WIDTH 4
#define TS_Controller_Status__fsm1_stat_RESETVALUE 0x0
#define TS_Controller_Status__fsm2_stat_L 10
#define TS_Controller_Status__fsm2_stat_R 8
#define TS_Controller_Status__fsm2_stat_WIDTH 3
#define TS_Controller_Status__fsm2_stat_RESETVALUE 0x0
#define TS_Controller_Status__adc_rdy 5
#define TS_Controller_Status__adc_rdy_WIDTH 1
#define TS_Controller_Status__adc_rdy_RESETVALUE 0x0
#define TS_Controller_Status__adc_strt 4
#define TS_Controller_Status__adc_strt_WIDTH 1
#define TS_Controller_Status__adc_strt_RESETVALUE 0x0
#define TS_Controller_Status__touch_det 2
#define TS_Controller_Status__touch_det_WIDTH 1
#define TS_Controller_Status__touch_det_RESETVALUE 0x0
#define TS_Controller_Status__fifo_empty 1
#define TS_Controller_Status__fifo_empty_WIDTH 1
#define TS_Controller_Status__fifo_empty_RESETVALUE 0x1
#define TS_Controller_Status__pen_down 0
#define TS_Controller_Status__pen_down_WIDTH 1
#define TS_Controller_Status__pen_down_RESETVALUE 0x0
#define TS_Controller_Status__RESERVED_0_L 31
#define TS_Controller_Status__RESERVED_0_R 16
#define TS_Controller_Status__RESERVED_1 11
#define TS_Controller_Status__RESERVED_2_L 7
#define TS_Controller_Status__RESERVED_2_R 6
#define TS_Controller_Status__RESERVED_3 3
#define TS_Controller_Status__RESERVED_L 31
#define TS_Controller_Status__RESERVED_R 16
#define TS_Controller_Status_WIDTH 16
#define TS_Controller_Status__WIDTH 16
#define TS_Controller_Status_ALL_L 15
#define TS_Controller_Status_ALL_R 0
#define TS_Controller_Status__ALL_L 15
#define TS_Controller_Status__ALL_R 0
#define TS_Controller_Status_DATAMASK 0x0000f737
#define TS_Controller_Status_RESETVALUE 0x2
*/
#define TS_FIFO_Data 0x180a6018
/*
#define TS_FIFO_Data_BASE 0x018
#define TS_FIFO_Data_OFFSET 0x180a6018
#define TS_FIFO_Data__y_L 31
#define TS_FIFO_Data__y_R 16
#define TS_FIFO_Data__y_WIDTH 16
#define TS_FIFO_Data__y_RESETVALUE 0xffff
#define TS_FIFO_Data__x_L 15
#define TS_FIFO_Data__x_R 0
#define TS_FIFO_Data__x_WIDTH 16
#define TS_FIFO_Data__x_RESETVALUE 0xffff
#define TS_FIFO_Data_WIDTH 32
#define TS_FIFO_Data__WIDTH 32
#define TS_FIFO_Data_ALL_L 31
#define TS_FIFO_Data_ALL_R 0
#define TS_FIFO_Data__ALL_L 31
#define TS_FIFO_Data__ALL_R 0
#define TS_FIFO_Data_DATAMASK 0xffffffff
#define TS_FIFO_Data_RESETVALUE 0xffffffff
*/
#define TS_Analog_Control 0x180a601c
/*
#define TS_Analog_Control_BASE 0x01c
#define TS_Analog_Control_OFFSET 0x180a601c
#define TS_Analog_Control__sel_inv_output_clk 25
#define TS_Analog_Control__sel_inv_output_clk_WIDTH 1
#define TS_Analog_Control__sel_inv_output_clk_RESETVALUE 0x0
#define TS_Analog_Control__ldo_cntl_L 24
#define TS_Analog_Control__ldo_cntl_R 15
#define TS_Analog_Control__ldo_cntl_WIDTH 10
#define TS_Analog_Control__ldo_cntl_RESETVALUE 0x0
#define TS_Analog_Control__cal_en 14
#define TS_Analog_Control__cal_en_WIDTH 1
#define TS_Analog_Control__cal_en_RESETVALUE 0x0
#define TS_Analog_Control__i_amux_L 13
*/
#define TS_Analog_Control__i_amux_R 11
/*
#define TS_Analog_Control__i_amux_WIDTH 3
#define TS_Analog_Control__i_amux_RESETVALUE 0x0
#define TS_Analog_Control__ckext_en 10
#define TS_Analog_Control__ckext_en_WIDTH 1
#define TS_Analog_Control__ckext_en_RESETVALUE 0x0
#define TS_Analog_Control__refout_en 9
#define TS_Analog_Control__refout_en_WIDTH 1
#define TS_Analog_Control__refout_en_RESETVALUE 0x0
#define TS_Analog_Control__testmode 8
#define TS_Analog_Control__testmode_WIDTH 1
#define TS_Analog_Control__testmode_RESETVALUE 0x0
#define TS_Analog_Control__bg_faststart_dis 7
#define TS_Analog_Control__bg_faststart_dis_WIDTH 1
#define TS_Analog_Control__bg_faststart_dis_RESETVALUE 0x0
#define TS_Analog_Control__bg_indp_bias 6
#define TS_Analog_Control__bg_indp_bias_WIDTH 1
#define TS_Analog_Control__bg_indp_bias_RESETVALUE 0x0
#define TS_Analog_Control__bg_altref 5
#define TS_Analog_Control__bg_altref_WIDTH 1
#define TS_Analog_Control__bg_altref_RESETVALUE 0x0
#define TS_Analog_Control__bg_ctl_L 4
#define TS_Analog_Control__bg_ctl_R 2
#define TS_Analog_Control__bg_ctl_WIDTH 3
#define TS_Analog_Control__bg_ctl_RESETVALUE 0x0
#define TS_Analog_Control__detect_vth_L 1
#define TS_Analog_Control__detect_vth_R 0
#define TS_Analog_Control__detect_vth_WIDTH 2
#define TS_Analog_Control__detect_vth_RESETVALUE 0x0
#define TS_Analog_Control__RESERVED_L 31
#define TS_Analog_Control__RESERVED_R 26
#define TS_Analog_Control_WIDTH 26
#define TS_Analog_Control__WIDTH 26
#define TS_Analog_Control_ALL_L 25
#define TS_Analog_Control_ALL_R 0
#define TS_Analog_Control__ALL_L 25
#define TS_Analog_Control__ALL_R 0
#define TS_Analog_Control_DATAMASK 0x03ffffff
#define TS_Analog_Control_RESETVALUE 0x0
*/
#define TS_Aux_Data 0x180a6020
/*
#define TS_Aux_Data_BASE 0x020
#define TS_Aux_Data_OFFSET 0x180a6020
#define TS_Aux_Data__auxin_dout_L 15
#define TS_Aux_Data__auxin_dout_R 0
#define TS_Aux_Data__auxin_dout_WIDTH 16
#define TS_Aux_Data__auxin_dout_RESETVALUE 0x0000
#define TS_Aux_Data__RESERVED_L 31
#define TS_Aux_Data__RESERVED_R 16
#define TS_Aux_Data_WIDTH 16
#define TS_Aux_Data__WIDTH 16
#define TS_Aux_Data_ALL_L 15
#define TS_Aux_Data_ALL_R 0
#define TS_Aux_Data__ALL_L 15
#define TS_Aux_Data__ALL_R 0
#define TS_Aux_Data_DATAMASK 0x0000ffff
#define TS_Aux_Data_RESETVALUE 0x0
*/
#define TS_Debounce_Cntr_Stat 0x180a6024
/*
#define TS_Debounce_Cntr_Stat_BASE 0x024
#define TS_Debounce_Cntr_Stat_OFFSET 0x180a6024
#define TS_Debounce_Cntr_Stat__debounce_cntr_L 12
#define TS_Debounce_Cntr_Stat__debounce_cntr_R 0
#define TS_Debounce_Cntr_Stat__debounce_cntr_WIDTH 13
#define TS_Debounce_Cntr_Stat__debounce_cntr_RESETVALUE 0x0
#define TS_Debounce_Cntr_Stat__RESERVED_L 31
#define TS_Debounce_Cntr_Stat__RESERVED_R 13
#define TS_Debounce_Cntr_Stat_WIDTH 13
#define TS_Debounce_Cntr_Stat__WIDTH 13
#define TS_Debounce_Cntr_Stat_ALL_L 12
#define TS_Debounce_Cntr_Stat_ALL_R 0
#define TS_Debounce_Cntr_Stat__ALL_L 12
#define TS_Debounce_Cntr_Stat__ALL_R 0
#define TS_Debounce_Cntr_Stat_DATAMASK 0x00001fff
#define TS_Debounce_Cntr_Stat_RESETVALUE 0x0
*/
#define TS_Scan_Cntr_Stat 0x180a6028
/*
#define TS_Scan_Cntr_Stat_BASE 0x028
#define TS_Scan_Cntr_Stat_OFFSET 0x180a6028
#define TS_Scan_Cntr_Stat__scan_cntr_stat_L 13
#define TS_Scan_Cntr_Stat__scan_cntr_stat_R 0
#define TS_Scan_Cntr_Stat__scan_cntr_stat_WIDTH 14
#define TS_Scan_Cntr_Stat__scan_cntr_stat_RESETVALUE 0x1
#define TS_Scan_Cntr_Stat__RESERVED_L 31
#define TS_Scan_Cntr_Stat__RESERVED_R 14
#define TS_Scan_Cntr_Stat_WIDTH 14
#define TS_Scan_Cntr_Stat__WIDTH 14
#define TS_Scan_Cntr_Stat_ALL_L 13
#define TS_Scan_Cntr_Stat_ALL_R 0
#define TS_Scan_Cntr_Stat__ALL_L 13
#define TS_Scan_Cntr_Stat__ALL_R 0
#define TS_Scan_Cntr_Stat_DATAMASK 0x00003fff
#define TS_Scan_Cntr_Stat_RESETVALUE 0x1
*/
#define TS_Rem_Cntr_Stat 0x180a602c
/*
#define TS_Rem_Cntr_Stat_BASE 0x02c
#define TS_Rem_Cntr_Stat_OFFSET 0x180a602c
#define TS_Rem_Cntr_Stat__touch_rem_cntr_L 7
#define TS_Rem_Cntr_Stat__touch_rem_cntr_R 0
#define TS_Rem_Cntr_Stat__touch_rem_cntr_WIDTH 8
#define TS_Rem_Cntr_Stat__touch_rem_cntr_RESETVALUE 0x00
#define TS_Rem_Cntr_Stat__RESERVED_L 31
#define TS_Rem_Cntr_Stat__RESERVED_R 8
#define TS_Rem_Cntr_Stat_WIDTH 8
#define TS_Rem_Cntr_Stat__WIDTH 8
#define TS_Rem_Cntr_Stat_ALL_L 7
#define TS_Rem_Cntr_Stat_ALL_R 0
#define TS_Rem_Cntr_Stat__ALL_L 7
#define TS_Rem_Cntr_Stat__ALL_R 0
#define TS_Rem_Cntr_Stat_DATAMASK 0x000000ff
#define TS_Rem_Cntr_Stat_RESETVALUE 0x0
*/
#define TS_Settling_Timer_Stat 0x180a6030
/*
#define TS_Settling_Timer_Stat_BASE 0x030
#define TS_Settling_Timer_Stat_OFFSET 0x180a6030
#define TS_Settling_Timer_Stat__settling_timer_L 9
#define TS_Settling_Timer_Stat__settling_timer_R 0
#define TS_Settling_Timer_Stat__settling_timer_WIDTH 10
#define TS_Settling_Timer_Stat__settling_timer_RESETVALUE 0x0
#define TS_Settling_Timer_Stat__RESERVED_L 31
#define TS_Settling_Timer_Stat__RESERVED_R 10
#define TS_Settling_Timer_Stat_WIDTH 10
#define TS_Settling_Timer_Stat__WIDTH 10
#define TS_Settling_Timer_Stat_ALL_L 9
#define TS_Settling_Timer_Stat_ALL_R 0
#define TS_Settling_Timer_Stat__ALL_L 9
#define TS_Settling_Timer_Stat__ALL_R 0
#define TS_Settling_Timer_Stat_DATAMASK 0x000003ff
#define TS_Settling_Timer_Stat_RESETVALUE 0x0
*/
#define TS_Spare_Reg 0x180a6034
/*
#define TS_Spare_Reg_BASE 0x034
#define TS_Spare_Reg_OFFSET 0x180a6034
#define TS_Spare_Reg__spare_reg_L 31
#define TS_Spare_Reg__spare_reg_R 0
#define TS_Spare_Reg__spare_reg_WIDTH 32
#define TS_Spare_Reg__spare_reg_RESETVALUE 0x0000ffff
#define TS_Spare_Reg_WIDTH 32
#define TS_Spare_Reg__WIDTH 32
#define TS_Spare_Reg_ALL_L 31
#define TS_Spare_Reg_ALL_R 0
#define TS_Spare_Reg__ALL_L 31
#define TS_Spare_Reg__ALL_R 0
#define TS_Spare_Reg_DATAMASK 0xffffffff
#define TS_Spare_Reg_RESETVALUE 0xffff
*/
#define TS_Soft_Bypass_Control 0x180a6038
/*
#define TS_Soft_Bypass_Control_BASE 0x038
#define TS_Soft_Bypass_Control_OFFSET 0x180a6038
#define TS_Soft_Bypass_Control__Start 5
#define TS_Soft_Bypass_Control__Start_WIDTH 1
#define TS_Soft_Bypass_Control__Start_RESETVALUE 0x0
#define TS_Soft_Bypass_Control__reserved 4
#define TS_Soft_Bypass_Control__reserved_WIDTH 1
#define TS_Soft_Bypass_Control__reserved_RESETVALUE 0x0
#define TS_Soft_Bypass_Control__xenable 3
#define TS_Soft_Bypass_Control__xenable_WIDTH 1
#define TS_Soft_Bypass_Control__xenable_RESETVALUE 0x0
#define TS_Soft_Bypass_Control__detect 2
#define TS_Soft_Bypass_Control__detect_WIDTH 1
#define TS_Soft_Bypass_Control__detect_RESETVALUE 0x0
#define TS_Soft_Bypass_Control__precharege 1
#define TS_Soft_Bypass_Control__precharege_WIDTH 1
#define TS_Soft_Bypass_Control__precharege_RESETVALUE 0x0
#define TS_Soft_Bypass_Control__soft_bypass_enable 0
#define TS_Soft_Bypass_Control__soft_bypass_enable_WIDTH 1
#define TS_Soft_Bypass_Control__soft_bypass_enable_RESETVALUE 0x0
#define TS_Soft_Bypass_Control__RESERVED_L 31
#define TS_Soft_Bypass_Control__RESERVED_R 6
#define TS_Soft_Bypass_Control_WIDTH 6
#define TS_Soft_Bypass_Control__WIDTH 6
#define TS_Soft_Bypass_Control_ALL_L 5
#define TS_Soft_Bypass_Control_ALL_R 0
#define TS_Soft_Bypass_Control__ALL_L 5
#define TS_Soft_Bypass_Control__ALL_R 0
#define TS_Soft_Bypass_Control_DATAMASK 0x0000003f
#define TS_Soft_Bypass_Control_RESETVALUE 0x0
*/
#define TS_Soft_Bypass_Data 0x180a603c
/*
#define TS_Soft_Bypass_Data_BASE 0x03c
#define TS_Soft_Bypass_Data_OFFSET 0x180a603c
#define TS_Soft_Bypass_Data__adc_output_L 9
#define TS_Soft_Bypass_Data__adc_output_R 0
#define TS_Soft_Bypass_Data__adc_output_WIDTH 10
#define TS_Soft_Bypass_Data__adc_output_RESETVALUE 0x0
#define TS_Soft_Bypass_Data__RESERVED_L 31
#define TS_Soft_Bypass_Data__RESERVED_R 10
#define TS_Soft_Bypass_Data_WIDTH 10
#define TS_Soft_Bypass_Data__WIDTH 10
#define TS_Soft_Bypass_Data_ALL_L 9
#define TS_Soft_Bypass_Data_ALL_R 0
#define TS_Soft_Bypass_Data__ALL_L 9
#define TS_Soft_Bypass_Data__ALL_R 0
#define TS_Soft_Bypass_Data_DATAMASK 0x000003ff
#define TS_Soft_Bypass_Data_RESETVALUE 0x0
*/
#define CRMU_CHIP_IO_PAD_CONTROL 0x0301d0bc

#define ASIU_TOP_CLK_GATING_CTRL 0x180aa024
#define ASIU_TOP_CLK_GATING_CTRL__ADC_CLK_GATE_EN 9
#define ASIU_TOP_SW_RESET_CTRL 0x180aa020
#define ASIU_TOP_SW_RESET_CTRL__ADC_SW_RESET_N 9

// #include "../halapi/reg_access.h"

// #include "cygnus_types.h"

void touch_screen_delay(uint32_t value);
void touch_screen_WRITE_REG_api(uint32_t addr,uint32_t data);
uint32_t touch_screen_READ_REG_api(uint32_t addr);
void touch_screen_REG_RD_WR_api(uint32_t addr ,uint32_t data)  ;
void touch_screen_LDO_POWER_UP_api(uint32_t dummy);
void touch_screen_ADC_POWER_UP_api(uint32_t dummy);
void touch_screen_BG_POWER_UP_api(uint32_t dummy);
void touch_screen_TSC_POWER_UP_api(uint32_t dummy);
void touch_screen_LDO_POWER_DOWN_api(uint32_t dummy);
void touch_screen_ADC_POWER_DOWN_api(uint32_t dummy);
void touch_screen_BG_POWER_DOWN_api(uint32_t dummy);
void touch_screen_TSC_POWER_DOWN_api(uint32_t adc_en);
void touch_screen_CONFIG_api(uint32_t timer_in, uint32_t avg_data, uint32_t mux_sel, uint32_t scan_en);
void touch_screen_INTR_MASK_api(uint32_t mask_in);
void touch_screen_AUXIN_INT_api(uint32_t fifo_threshold);
uint32_t check_tsc_auxin_data_api(uint32_t pd);
void touch_screen_xinput_check_api(uint32_t fifo_threshold);
uint32_t check_fifo_data_api(uint32_t mask_in, uint32_t fifo_threshold);
uint32_t read_fifo_api(uint32_t dummy);
void touch_screen_soft_bypass_api(uint32_t sw_input);
uint32_t touch_screen_soft_data_api(uint32_t intr_data);
void touch_screen_cntr_reg_read(uint32_t dummy);
 void touch_screen_sw_reset(void);
  void touch_screen_clk_gate_disable(void);
  void touch_screen_powerdown(void);
   void touch_screen_powerup(void);


#endif
