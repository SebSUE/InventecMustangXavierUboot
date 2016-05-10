#include <common.h>
#include <post.h>
#include "touchscreen_apis.h"

#define cpu_rd_single(x, y) readl(x)
#define cpu_wr_single(x, y, z) writel(y, x)

 void touch_screen_delay(uint32_t value)
 {
    uint32_t i;
	for(i=0;i<value;i++);
 }


 void touch_screen_powerdown(void)
 {
   	touch_screen_LDO_POWER_DOWN_api(0);
	touch_screen_delay(1000);
	touch_screen_ADC_POWER_DOWN_api(0);
	touch_screen_delay(1000);
	touch_screen_BG_POWER_DOWN_api(0);
	touch_screen_delay(1000);
	touch_screen_TSC_POWER_DOWN_api(1);
	touch_screen_delay(1000);
 }

 void touch_screen_powerup(void)
 {

	touch_screen_LDO_POWER_UP_api(0);
	touch_screen_delay(1000);
	touch_screen_ADC_POWER_UP_api(0);
	touch_screen_delay(1000);
	touch_screen_TSC_POWER_UP_api(0);
	touch_screen_delay(1000);
 }

 void touch_screen_clk_gate_disable(void)
 {
   uint32_t value;
   value = touch_screen_READ_REG_api(ASIU_TOP_CLK_GATING_CTRL); //cpu_rd_single(ASIU_TOP_CLK_GATING_CTRL,4);
   value = value | (1 << ASIU_TOP_CLK_GATING_CTRL__ADC_CLK_GATE_EN);
   touch_screen_WRITE_REG_api(ASIU_TOP_CLK_GATING_CTRL,value); //cpu_wr_single(ASIU_TOP_CLK_GATING_CTRL,value,4);
 }

 void touch_screen_sw_reset(void)
 {
   uint32_t value;
   value = touch_screen_READ_REG_api(ASIU_TOP_SW_RESET_CTRL);
   value = value & ~(1 << ASIU_TOP_SW_RESET_CTRL__ADC_SW_RESET_N);
   touch_screen_WRITE_REG_api(ASIU_TOP_SW_RESET_CTRL,value);


   value = touch_screen_READ_REG_api(ASIU_TOP_SW_RESET_CTRL);
   value = value | (1 << ASIU_TOP_SW_RESET_CTRL__ADC_SW_RESET_N);
   touch_screen_WRITE_REG_api(ASIU_TOP_SW_RESET_CTRL,value);
 }

 /*  --------------------------- function ----------------------------------- */
 extern int post_log (char *format, ...);
void touch_screen_WRITE_REG_api(uint32_t addr,uint32_t data)
{
      cpu_wr_single(addr,data,4);
      post_log(" API-WRITE REG - Address = %0x  Data = %0x\n",addr,data);
}

/*  --------------------------- function ----------------------------------- */

uint32_t touch_screen_READ_REG_api(uint32_t addr)

{
      uint32_t rd_data;
      rd_data=cpu_rd_single(addr,4);
      post_log(" API-READ REG - Address = %0x  Data = %0x\n",addr,rd_data);
      return(rd_data);
}

/*  --------------------------- function ----------------------------------- */
void touch_screen_REG_RD_WR_api(uint32_t addr ,uint32_t data)
{
  uint32_t rd_data[16];
  uint32_t i;
  uint32_t j=0;
 

      for(i=0;i<0x1c;i=i+4)
      {
      rd_data[j]=cpu_rd_single((addr+i),4);
      post_log(" API-READ REGISTER  - Address = %0x  Data = %0x\n",(addr+i),rd_data[j]);
      j++;
      }

//WRITE THE REGS

      for(i=0;i<0x1c;i=i+4)
      {
       cpu_wr_single((addr+i),data,4);
      post_log(" API-WRITE REGISTER  - Address = %0x  Data = %0x\n",(addr+i),data);
      }

//READ THE REGS AGAIN

     j=0;
     for(i=0;i<0x1c;i=i+4)
      {
      rd_data[j]=cpu_rd_single((addr+i),4);
      post_log(" API-READ REGISTER  - Address = %0x  Data = %0x\n",(addr+i),rd_data[j]);
      j++;
      }

}

/*  ------------ funtion to take care of the Power up sequence ------------------ */
void touch_screen_LDO_POWER_UP_api(uint32_t dummy)
{

  uint32_t rd_data;
  uint32_t wr_data;

  //Configuring the TSC registers

  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  wr_data = (rd_data & 0xFFFFFFDF);                 // Turn On LDO
  cpu_wr_single(TSCRegCtl2,wr_data,4);
  post_log(" TSCRegCtl2 WR DATA:= %0x for LDO power up\n",wr_data);
}

void touch_screen_ADC_POWER_UP_api(uint32_t dummy)
{

	uint32_t rd_data;
	  uint32_t wr_data;

  /*touch_screen_delay(20000);*/
  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  wr_data= (rd_data & 0xFFFFFFEF);
  cpu_wr_single(TSCRegCtl2,wr_data,4);              // TURN ON ADC_PWRUP
  post_log(" TSCRegCtl2 WR DATA:= %0x for ADC power up\n",wr_data);
}

void touch_screen_BG_POWER_UP_api(uint32_t dummy)
{

	uint32_t rd_data;
	  uint32_t wr_data;

  /*touch_screen_delay(20000);*/
  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  wr_data= (rd_data & 0xFFFFFFF7);
  cpu_wr_single(TSCRegCtl2,wr_data,4);              // TURN ON BG_PWRUP
  post_log(" TSCRegCtl2 WR DATA:= %0x for BG power up\n",wr_data);
}

void touch_screen_TSC_POWER_UP_api(uint32_t dummy)
{

	uint32_t rd_data;
	  uint32_t wr_data;

  /*touch_screen_delay(20000);*/
  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  wr_data= (rd_data & 0xFFFFFFFB);
  cpu_wr_single(TSCRegCtl2,wr_data,4);              // TURN ON TSC
  post_log(" TSCRegCtl2 WR DATA:= %0x for TSC power up\n",wr_data);

  post_log(" ***  POWER UP SEQUENCE COMPLETED ***\n");
}

/*  ------------ funtion to Power down LDO ------------------ */
void touch_screen_LDO_POWER_DOWN_api(uint32_t dummy)
{

  uint32_t rd_data;
  uint32_t wr_data;

  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  wr_data = (rd_data | 0x00000020);                 // Turn Off LDO
  cpu_wr_single(TSCRegCtl2,wr_data,4);
  post_log(" TSCRegCtl2 WR DATA:= %0x to power down LDO\n",wr_data);

  post_log(" ***  LDO POWER DOWN COMPLETED ***\n");
}

/*  ------------ funtion to Power down ADC ------------------ */
void touch_screen_ADC_POWER_DOWN_api(uint32_t dummy)
{

  uint32_t rd_data;
  uint32_t wr_data;

  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  wr_data = (rd_data | 0x00000010);                 // Turn Off ADC
  cpu_wr_single(TSCRegCtl2,wr_data,4);
  post_log(" TSCRegCtl2 WR DATA:= %0x to power down ADC\n",wr_data);

  post_log(" ***  ADC POWER DOWN COMPLETED ***\n");
}

/*  ------------ funtion to Power down BG ------------------ */
void touch_screen_BG_POWER_DOWN_api(uint32_t dummy)
{

  uint32_t rd_data;
  uint32_t wr_data;

  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  wr_data = (rd_data | 0x00000008);                 // Turn Off BG
  cpu_wr_single(TSCRegCtl2,wr_data,4);
  post_log(" TSCRegCtl2 WR DATA:= %0x to power down BG\n",wr_data);

  post_log(" ***  BG POWER DOWN COMPLETED ***\n");
}

/*  ------------ funtion to Power down TSC ------------------ */
void touch_screen_TSC_POWER_DOWN_api(uint32_t adc_en)
{

  uint32_t rd_data;
  uint32_t wr_data;

  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  if(adc_en)
  {
    wr_data = (rd_data | 0x00010004);
  }
  else
  {
    wr_data = (rd_data | 0x00000004);                 // Turn Off TSC
  }
  cpu_wr_single(TSCRegCtl2,wr_data,4);
  post_log(" TSCRegCtl2 WR DATA:= %0x to power down BG\n",wr_data);

  post_log(" ***  TSC POWER DOWN COMPLETED ***\n");
}

/*  ------------ funtion to Configure the Timers and start ADC ------------------ */


void touch_screen_CONFIG_api(uint32_t timer_in, uint32_t avg_data, uint32_t mux_sel, uint32_t scan_en)
{

  uint32_t rd_data;
  uint32_t wr_data = 0;

  wr_data = (mux_sel << TS_Analog_Control__i_amux_R);
  post_log(" Analog control WR DATA:= %0x\n",wr_data);
  cpu_wr_single(TS_Analog_Control, wr_data,4);

  post_log(" TSCRegCtl1 WD DATA:= %0x\n",timer_in);
  cpu_wr_single(TSCRegCtl1,timer_in,4);           // setting the timers

  rd_data= cpu_rd_single(TSCRegCtl2,4);

  avg_data = avg_data << TSCRegCtl2__average_data_R;
  wr_data = (rd_data & 0xFFFFF0FF) | avg_data;                   // configuring the avg data value
  post_log(" TSCRegCtl2 WR DATA:= %0x after adding avg data\n",wr_data);
  cpu_wr_single(TSCRegCtl2,wr_data,4);

  rd_data= cpu_rd_single(TSCRegCtl2,4);
  post_log(" TSCRegCtl2 RD DATA:= %0x\n",rd_data);

  if(scan_en)
  {
    wr_data = (rd_data | 0x00010001);                 // Turn On ADC
  }
  else
  {
    wr_data = ((rd_data & 0xFFFFFFFE) | 0x00010000);                 // Turn On ADC, disable scan_en
  }
  cpu_wr_single(TSCRegCtl2,wr_data,4);
  post_log(" TSCRegCtl2 WR DATA:= %0x to trigger the ADC\n",wr_data);

  post_log(" ***  TSC CONFIG and TRIGGER COMPLETED ***\n");
}

/*  ------------ funtion to Configure the Interrupt Masks ------------------ */
void touch_screen_INTR_MASK_api(uint32_t mask_in)
{

  post_log(" TS_Interrupt_Mask:= %0x\n",mask_in);
  cpu_wr_single(TS_Interrupt_Mask,mask_in,4);           // setting the interrupt mask values

  post_log(" ***  TSC INTERRUPT MASK CONFIGURED ***\n");
}

/*  --------------------------- function ----------------------------------- */
void touch_screen_AUXIN_INT_api(uint32_t fifo_threshold)
{

//Configuring the TSC registers

     cpu_wr_single(TS_Interrupt_Thres,fifo_threshold,4); //setting the FIFO thresholdvalue as 3
     post_log(" ***  AUXIN INPUT AND INTERRUPT CHECK TEST STARTED ***\n");
}

uint32_t check_tsc_auxin_data_api(uint32_t pd)
{

 uint32_t rd_data;

 uint32_t poll_data= 0x00000010; // For adc_strt

     post_log(" READ INTR STATUS \n");

     rd_data=cpu_rd_single(TS_Interrupt_Status,4);

     while ((rd_data & poll_data) == 0 )
     {
       rd_data=cpu_rd_single(TS_Interrupt_Status,4); // wait until ADC start bit is set
       //post_log(" AUXIN INTR STATUS:= %0x\n",rd_data);
     }
     post_log(" ADC Start Interrupt Generated := %0x\n",rd_data);

     poll_data = 0x00000020; // For adc_rdy

     if (!pd)
     {
        while ((rd_data & poll_data) == 0 )
        {
          rd_data=cpu_rd_single(TS_Interrupt_Status,4); // wait until AC ready bit is set
          //post_log(" AUXIN INTR STATUS:= %0x\n",rd_data);
        }
        post_log(" ADC Ready Interrupt Generated := %0x\n",rd_data);

        poll_data = 0x00000008; // For aux_in_avail

        while ((rd_data & poll_data) == 0 )
        {
          rd_data=cpu_rd_single(TS_Interrupt_Status,4); // wait until Aux data is available
          //post_log(" AUXIN INTR STATUS:= %0x\n",rd_data);
        }
        post_log(" AUX Data Available := %0x\n",rd_data);
     }
     else
     {
        touch_screen_delay(2000000);
        post_log(" POWER DOWN APPLIED\n");
        rd_data=cpu_rd_single(TS_Interrupt_Status,4);
        post_log("Interrupt Status := %0x\n",rd_data);
     }

     rd_data=cpu_rd_single(TS_Aux_Data,4);
     post_log(" AUXIN DATA:= %0x\n",rd_data);
     return(rd_data);
}


/*  --------------------------- function ----------------------------------- */
void touch_screen_xinput_check_api(uint32_t fifo_threshold)
{

     // Configuring the TSC registers
     cpu_wr_single(TS_Interrupt_Thres,fifo_threshold,4);
     post_log(" *** X/Y SCAN INPUT Testcase started ***\n");

}

uint32_t check_fifo_data_api(uint32_t mask_in, uint32_t fifo_threshold)
{
 uint32_t rd_data;
 
   rd_data=cpu_rd_single(TS_Interrupt_Status,4);
   if(fifo_threshold != 0x1F)
   {
     while ((rd_data & 0x4) == 0 )
     {
       rd_data=cpu_rd_single(TS_Interrupt_Status,4);  // Wait till FIFO Threshold is reached
     }
     post_log(" INTR STATUS:= %0x\n",rd_data);
   }
   else
   {
     while ((rd_data & 0x2) == 0 )
     {
       rd_data=cpu_rd_single(TS_Interrupt_Status,4);  // Wait till FIFO Overload is reached
     }
     post_log(" INTR STATUS:= %0x\n",rd_data);
   }

     rd_data=cpu_rd_single(TS_FIFO_Data,4);
     post_log(" FIFO DATA:= %0x\n",rd_data);
     return(rd_data);
}

uint32_t read_fifo_api(uint32_t dummy)
{
 uint32_t rd_data;


   rd_data=cpu_rd_single(TS_Interrupt_Status,4);
   while ((rd_data & 0x8) == 0 )
   {
     rd_data=cpu_rd_single(TS_Interrupt_Status,4);  // Wait till FIFO Threshold is reached
   }
   post_log(" INTR STATUS:= %0x\n",rd_data);
   touch_screen_delay(20000);
   rd_data=cpu_rd_single(TS_FIFO_Data,4);
   post_log(" FIFO DATA:= %0x\n",rd_data);
   return(rd_data);
}

void touch_screen_soft_bypass_api(uint32_t sw_input)
{

   // Configuring Software bypass register
   post_log(" Soft Bypass WR DATA:= %0x\n",sw_input);
   cpu_wr_single(TS_Soft_Bypass_Control,sw_input,4);
}

uint32_t touch_screen_soft_data_api(uint32_t intr_data)
{
   uint32_t rd_data;
  

   post_log(" INTR DATA:= %0x\n",intr_data);
   rd_data=cpu_rd_single(TS_Interrupt_Status,4);
   while ((rd_data & intr_data) == 0 )
   {
     rd_data=cpu_rd_single(TS_Interrupt_Status,4);  // Wait till Soft data register is written
   }
   post_log(" INTR STATUS:= %0x\n",rd_data);

   touch_screen_delay(20000);
   rd_data=cpu_rd_single(TS_Soft_Bypass_Data,4);
   post_log(" Soft Bypass Data:= %0x\n",rd_data);
   return(rd_data);
}

void touch_screen_cntr_reg_read(uint32_t dummy)
{
   uint32_t rd_data;

   rd_data=cpu_rd_single(TS_Debounce_Cntr_Stat,4);
   post_log("Debounce Counter Status:= %0x\n",rd_data);

   rd_data=cpu_rd_single(TS_Scan_Cntr_Stat,4);
   post_log("Scan Counter Status:= %0x\n",rd_data);

   rd_data=cpu_rd_single(TS_Rem_Cntr_Stat,4);
   post_log("Remembrance Counter Status:= %0x\n",rd_data);

   rd_data=cpu_rd_single(TS_Settling_Timer_Stat,4);
   post_log("Settling Timer Status:= %0x\n",rd_data);

   rd_data=cpu_rd_single(TS_Spare_Reg,4);
   post_log("Spare register:= %0x\n",rd_data);
}

