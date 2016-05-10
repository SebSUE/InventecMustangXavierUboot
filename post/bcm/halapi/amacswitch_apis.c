#include <common.h>
#include "amacswitch_apis.h"
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
//#include <math.h>

#define MMU 0
#define MAG_MODE 1
#define BYPASS_MODE 0

#define OPCODE_WRITE 1
#define OPCODE_READ  2

void ccg_mdio_init(void) {
	 uint32_t byp;
	 uint32_t ext;
	 uint32_t pre;
	 uint32_t mdcdiv;
	 uint32_t data;

	 data = rd(ChipcommonG_MII_Management_Control);
	 post_log("ChipcommonG_MII_Management_Control = 0x%08X\n", data);

	 byp = 0;
	 ext = 0;
	 pre = 1;
	 mdcdiv = 62;

	 data = 0;
	 data |= (mdcdiv << ChipcommonG_MII_Management_Control__MDCDIV_R);
	 data |= (pre    << ChipcommonG_MII_Management_Control__PRE);
	 data |= (ext    << ChipcommonG_MII_Management_Control__EXT);
	 data |= (byp    << ChipcommonG_MII_Management_Control__BYP);

	 wr(ChipcommonG_MII_Management_Control, data);
	 data = rd(ChipcommonG_MII_Management_Control);
	 post_log("ChipcommonG_MII_Management_Control = 0x%08X\n", data);
}

void ccg_mdio_init_ext(void) {
	 uint32_t byp;
	 uint32_t ext;
	 uint32_t pre;
	 uint32_t mdcdiv;
	 uint32_t data;

	 data = rd(ChipcommonG_MII_Management_Control);
	 post_log("ChipcommonG_MII_Management_Control = 0x%08X\n", data);

	 byp = 0;
	 ext = 1;
	 pre = 1;
	 mdcdiv = 62;

	 data = 0;
	 data |= (mdcdiv << ChipcommonG_MII_Management_Control__MDCDIV_R);
	 data |= (pre    << ChipcommonG_MII_Management_Control__PRE);
	 data |= (ext    << ChipcommonG_MII_Management_Control__EXT);
	 data |= (byp    << ChipcommonG_MII_Management_Control__BYP);

	 wr(ChipcommonG_MII_Management_Control, data);
	 data = rd(ChipcommonG_MII_Management_Control);
	 post_log("ChipcommonG_MII_Management_Control = 0x%08X\n", data);
}

void ccg_mdio_read(uint32_t phy_addr, uint32_t reg_addr, uint32_t *rd_data) {
	 uint32_t data;

	 if(phy_addr & ~0x1FF) {
		post_log("Illegal value of PHY Address - 0x%08X\n", phy_addr);
	 }
	 if(reg_addr & ~0x1FF) {
		post_log("Illegal value of PHY Address - 0x%08X\n", reg_addr);
	 }

	 while (rd(ChipcommonG_MII_Management_Control) & (1 << ChipcommonG_MII_Management_Control__BSY));

	 data  = 0;
	 data |= (2           << ChipcommonG_MII_Management_Command_Data__TA_R);
	 data |= (reg_addr    << ChipcommonG_MII_Management_Command_Data__RA_R);
	 data |= (phy_addr    << ChipcommonG_MII_Management_Command_Data__PA_R);
	 data |= (OPCODE_READ << ChipcommonG_MII_Management_Command_Data__OP_R);
	 data |= (1           << ChipcommonG_MII_Management_Command_Data__SB_R);
	 wr(ChipcommonG_MII_Management_Command_Data, data);

	 while (rd(ChipcommonG_MII_Management_Control) & (1 << ChipcommonG_MII_Management_Control__BSY));
	 *rd_data = rd(ChipcommonG_MII_Management_Command_Data) & 0xFFFF;
}

void ccg_mdio_write(uint32_t phy_addr, uint32_t reg_addr, uint32_t *wr_data) {
	 uint32_t data;

	 if(phy_addr & ~0x1FF) {
		post_log("Illegal value of PHY Address - 0x%08X\n", phy_addr);
	 }
	 if(reg_addr & ~0x1FF) {
		post_log("Illegal value of PHY Address - 0x%08X\n", reg_addr);
	 }
	 if(*wr_data & ~0xFFFF) {
		post_log("Illegal value of write data - 0x%08X\n", wr_data);
	 }

	 while (rd(ChipcommonG_MII_Management_Control) & (1 << ChipcommonG_MII_Management_Control__BSY));

	 data  = 0;
	 data |= (2            << ChipcommonG_MII_Management_Command_Data__TA_R);
	 data |= (reg_addr     << ChipcommonG_MII_Management_Command_Data__RA_R);
	 data |= (phy_addr     << ChipcommonG_MII_Management_Command_Data__PA_R);
	 data |= (OPCODE_WRITE << ChipcommonG_MII_Management_Command_Data__OP_R);
	 data |= (1            << ChipcommonG_MII_Management_Command_Data__SB_R);
	 data |= *wr_data;
	 wr(ChipcommonG_MII_Management_Command_Data, data);

	 while (rd(ChipcommonG_MII_Management_Control) & (1 << ChipcommonG_MII_Management_Control__BSY));
}

void gphy_reg_read(uint32_t phy_addr, uint32_t reg_addr, uint32_t *rd_data) {
	 uint32_t data, addr;

	 //data = reg_addr & 0xFFF0;
	 //ccg_mdio_write(phy_addr, 0x1F, &data);
	 //addr = reg_addr & 0xF;
	 //ccg_mdio_read (phy_addr, addr, rd_data);
	 //post_log("GPHY Reg Read  : PHY : 0x%0X - REG : 0x%04X - DATA : 0x%04X\n", phy_addr, reg_addr, *rd_data);

	 addr = reg_addr;
	 ccg_mdio_read (phy_addr, addr, rd_data);
	 post_log("GPHY Reg Read  : PHY : 0x%0X - REG : 0x%04X - DATA : 0x%04X\n", phy_addr, reg_addr, *rd_data);
}

void gphy_reg_write(uint32_t phy_addr, uint32_t reg_addr, uint32_t *wr_data) {
	 uint32_t data, addr;

	 //data = reg_addr & 0xFFF0;
	 //ccg_mdio_write(phy_addr, 0x1F, &data);
	 //addr = reg_addr & 0xF;
	 //ccg_mdio_write (phy_addr, addr, wr_data);
	 //post_log("GPHY Reg Write : PHY : 0x%0X - REG : 0x%04X - DATA : 0x%04X\n", phy_addr, reg_addr, *wr_data);

	 addr = reg_addr;
	 ccg_mdio_write (phy_addr, addr, wr_data);
	 post_log("GPHY Reg Write : PHY : 0x%0X - REG : 0x%04X - DATA : 0x%04X\n", phy_addr, reg_addr, *wr_data);
}

/*void switch_bypass_mode(const uint8_t amac_id)
{
	uint32_t readvalue;
	//Switch Bypass Mode
					readvalue = cpu_rd_single((SWITCH_GLOBAL_CONFIG + (0x1000 * amac_id)), 4);
					readvalue = readvalue | (1 << 13); //Bypass Switch wrapper
					cpu_wr_single((SWITCH_GLOBAL_CONFIG + (0x1000 * amac_id)), 0x2001, 4);
					post_log("\t\t [INFO] SW IN BYPASS MODE %d \n", amac_id);
}*/
void amac_init(const uint8_t amac_id,uint32_t bypass_mode)
{
	uint32_t rdata32,readvalue;
	post_log("\t\t [INFO] Initializing AMAC %d \n", amac_id);

	// Assert reset
	cpu_wr_single((AMAC_IDM0_IDM_RESET_CONTROL + (0x1000 * amac_id)), 0x1,4);
	// Deassert reset
	cpu_wr_single((AMAC_IDM0_IDM_RESET_CONTROL + (0x1000 * amac_id)), 0x0,4);

	 if (bypass_mode == 1)
	 {
		 //switch_bypass_mode(amac_id);
	 }

	cpu_wr_single((UNIMAC0_MAC_0 + (0x1000 * amac_id)), 0xE2C25FAE, 4);
	cpu_wr_single((UNIMAC0_MAC_1 + (0x1000 * amac_id)), 0x0000A513, 4);

	// Programmed outer tag to value 8100
	cpu_wr_single((UNIMAC0_TAG_0 + (0x1000 * amac_id)), 0x00018100, 4);
	// Programmed inner tag to value 9100
	cpu_wr_single((UNIMAC0_TAG_1 + (0x1000 * amac_id)), 0x00019100, 4);

	// Configuring Tx_ipg_length to 12 bytes
	cpu_wr_single((UNIMAC0_TX_IPG_LENGTH + (0x1000 * amac_id)),0x0000000c, 4);

	//  Configuring Unimac_frm_length register //Max 12K Frame len
	cpu_wr_single((UNIMAC0_FRM_LENGTH + (0x1000 * amac_id)), 0x00003000, 4);

	//  Configuring Unimac_ipg_hd_bkp_cntl register
	cpu_wr_single((UNIMAC0_IPG_HD_BKP_CNTL + (0x1000 * amac_id)), 0x00000014, 4);

	//cpu_wr_single((GMAC0_TXQCONTROL + (0x1000 * amac_id)), 0x000001f4, 4);
	cpu_wr_single((GMAC0_TXQCONTROL + (0x1000 * amac_id)), 0x384, 4); //To avoid Tx underrun for more than 3k packet size
	cpu_wr_single((GMAC0_INTRCVLAZY + (0x1000 * amac_id)), 0x0, 4);

	cpu_wr_single((GMAC0_INTMASK    + (0x1000 * amac_id)), 0x0101C000, 4);
	//cpu_wr_single((UNIMAC0_MAC_MODE+ (0x1000 * amac_id)), 0x3e, 4);
	//cpu_wr_single((GMAC0_GMACIDLE_CNT_TH+ (0x1000 * amac_id)), 0xFFFF, 4);
#ifdef MMU
	//cpu_wr_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 0x421B03BF, 4);
	//cpu_wr_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 0xc21B03BB, 4);
	//cpu_wr_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 0xDEFB03BB, 4);
	cpu_wr_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 0xDEFB0385, 4);
	post_log("\t\t [INFO] Enabled cache from Master AMAC\n");
	readvalue = cpu_rd_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 4);
	post_log("\t\t AMAC_IDM0_IO_CONTROL_DIRECT = %#x\n",readvalue);


#else
	  //cpu_wr_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 0x2B, 4);
	  //Nishant
	   readvalue = cpu_rd_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 4);
	   readvalue = readvalue & (0xffffffbf); //To enable 125Hz clock.
	   cpu_wr_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), readvalue, 4);
	   readvalue = cpu_rd_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 4);
	   post_log("\t\t AMAC_IDM0_IO_CONTROL_DIRECT = %#x\n",readvalue);
	   post_log("\t\t [INFO] Disabled cache from Master AMAC\n");

	   //cpu_wr_single((AMAC_IDM0_IO_CONTROL_DIRECT + (0x1000 * amac_id)), 0x421B03BF, 4);
	  // post_log("\t\t [INFO] Enabled cache from Master AMAC\n");
#endif


}

uint32_t unimac_config(const uint8_t amac_id,uint8_t eth_speed,uint8_t rx_pause_ignore,uint8_t hd_ena,uint8_t mac_loop_con,uint8_t ena_ext_config,uint8_t rmt_loop_ena,uint8_t tx_pause_ignore ){

uint32_t cmd_cfg;

uint8_t TX_ENA      =1;
uint8_t RX_ENA      =1;
uint8_t ETH_SPEED   =eth_speed;
uint8_t PROMIS_EN   =1;
uint8_t PAD_EN      =0;
uint8_t CRC_FWD     =1;
uint8_t PAUSE_FWD   =0;
uint8_t RX_PAUSE_IGNORE     =rx_pause_ignore;
uint8_t TX_ADDR_INS         =0;
uint8_t HD_ENA              =hd_ena;
uint8_t RX_LOW_LATENCY_EN   =0;
uint8_t OVERFLOW_EN         =0;
uint8_t FCS_CORRUPT_URUN_EN =1;
uint8_t LCL_LOOP_ENA        =0;
uint8_t MAC_LOOP_CON        =mac_loop_con;
uint8_t SW_OVERRIDE_TX      =1;
uint8_t SW_OVERRIDE_RX      =1;
uint8_t EN_INTERNAL_TX_CRS  =0;
uint8_t ENA_EXT_CONFIG      =ena_ext_config;
uint8_t CNTL_FRM_ENA        =0;
uint8_t NO_LGTH_CHECK       =1;
uint8_t RMT_LOOP_ENA        =rmt_loop_ena;
uint8_t RX_ERR_DISC         =0;
uint8_t TX_PAUSE_IGNORE     =tx_pause_ignore;
uint8_t OOB_EFC_EN          =0;
uint8_t RUNT_FILTER_DIS     =0;

		 cmd_cfg =(((RUNT_FILTER_DIS&0x1)<<30)|\
					((OOB_EFC_EN&0x1)<<29)|\
					((TX_PAUSE_IGNORE&0x1)<<28)|\
					((RX_ERR_DISC&0x1)<<26)|\
					((RMT_LOOP_ENA&0x1)<<25)|\
					((NO_LGTH_CHECK&0x1)<<24)|\
					((CNTL_FRM_ENA&0x1)<<23)|\
					((ENA_EXT_CONFIG&0x1)<<22)|\
					((EN_INTERNAL_TX_CRS&0x1)<<21)|\
					((SW_OVERRIDE_RX&0x1)<<18)|\
					((SW_OVERRIDE_TX&0x1)<<17)|\
					((MAC_LOOP_CON&0x1)<<16)|\
					((LCL_LOOP_ENA&0x1)<<15)|\
					((FCS_CORRUPT_URUN_EN&0x1)<<14)|\
					((OVERFLOW_EN &0x1)<<12)|\
					((RX_LOW_LATENCY_EN&0x1)<<11)|\
					((HD_ENA&0x1)<<10)|\
					((TX_ADDR_INS&0x1)<<9)|\
					((RX_PAUSE_IGNORE&0x1)<<8)|\
					((PAUSE_FWD&0x1)<<7)|\
					((CRC_FWD&0x1)<<6)|\
					((PAD_EN&0x1)<<5)|\
					((PROMIS_EN&0x1)<<4)|\
					((ETH_SPEED&0x3)<<2)|\
					((RX_ENA&0x1)<<1)|\
					((TX_ENA&0x1)<<0));

	//  Assert soft reset for UNIMAC
	//cpu_wr_single((UNIMAC0_COMMAND_CONFIG + (0x1000 * amac_id)),  (cmd_cfg | 0x00002000), 4);
	//  De-Assert soft reset for UNIMAC
	cpu_wr_single((UNIMAC0_COMMAND_CONFIG + (0x1000 * amac_id)), cmd_cfg , 4);

	post_log("UNIMAC0_COMMAND_CONFIG = %x\n", cpu_rd_single((UNIMAC0_COMMAND_CONFIG+(0x1000 * amac_id)), 4));

	return 0;
 }


//Assemble Tx pkt in memory
void write_pkt_to_memory(uint8_t* packet, uint32_t frame_length, uint32_t start_address)
{
   uint8_t data;
   uint32_t index;
   uint8_t rno;
   uint8_t *ddr_byte;

   rno=0;
   ddr_byte = (uint8_t *) start_address;
   post_log("\t [INFO] Assembling Tx packet in memory at address 0x%x with frame_size %d \n", start_address,frame_length);

  //post_log("DDR address : %0x pkt length : %0x,\n", start_address,frame_length);
   //Perform 8-bit writes to DDR
   for (index = 0; index < frame_length; index++) {
		   //data = (uint8_t) *(packet + index);
		   //rno=(rand()%50);
		   if (index < 256){
					data = (uint8_t)((packet[index])+rno);
		   }else{
				   data = (uint8_t)((index%128)+rno);
				   //if(index == 3000){
				   //        data = (uint8_t)(0xff);
				  // }
		   }
		   //post_log("DDR address : %0x pkt dat : %0x, index :%0x\n", (start_address + index), data, index);

		   *(ddr_byte + index) = data;
		   //cpu_wr_single((start_address + index), data, BIT8);
		   //post_log("Done with writing data to memory \n");
   }
}

//Initialize TX DMA packet buffer
void amac_init_tx_dma(const uint8_t amac_id, uint64_t tx_desc_table_base_addr)
{
uint64_t xmt_ctl_tmp_data;
		post_log("\t [INFO] Initializing TX DMA %d \n", amac_id);

		cpu_wr_single((GMAC0_XMTADDR_LOW_0  + (0x1000 * amac_id)),  (uint32_t)(tx_desc_table_base_addr & 0xffffffff), 4);
		cpu_wr_single((GMAC0_XMTADDR_HIGH_0  + (0x1000 * amac_id)), (uint32_t)((tx_desc_table_base_addr >> 32) & 0xffffffff), 4);
		//cpu_wr_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), 0x001C0801, 4);//Suggested fr CYG but not wrking
		cpu_wr_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), 0x00040801, 4);//KT2
		//cpu_wr_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), 0x000C0841, 4);
		// updating xmt dma burst length field
		xmt_ctl_tmp_data = cpu_rd_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), 4);
		post_log("\t [INFO] GMAC0_XMTCONTROL_0 %x \n", xmt_ctl_tmp_data);
		//xmt_ctl_tmp_data = (xmt_ctl_tmp_data | 0x000C0000);
		//xmt_ctl_tmp_data = (xmt_ctl_tmp_data | 0x012C0000); //PRFETCHCHCTL & PREFETCHCHTHRST SET
		xmt_ctl_tmp_data = (xmt_ctl_tmp_data | 0x034C0000); //PRFETCHCHCTL & PREFETCHCHTHRST SET



		 post_log("\t [INFO] GMAC0_XMTCONTROL_0 %x \n", xmt_ctl_tmp_data);
		 cpu_wr_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), xmt_ctl_tmp_data, 4);//KT2

}

void amac_get_tx_status(const uint8_t amac_id)
{
		 uint32_t gmac_xmtstatus, status;

		post_log("[INFO] Reading TX DMA status for AMAC%d\n", amac_id);

		status = cpu_rd_single((GMAC0_INTSTATUS + (0x1000 * amac_id)), 4);
		post_log("GMAC_INTSTATUS                   = 0x%x\n", status);

		gmac_xmtstatus = cpu_rd_single((GMAC0_XMTSTATUS0_0 + (0x1000 * amac_id)), 4);

		status = GET_IPROC_REG_FIELD(GMAC0_XMTSTATUS0_0__XMTSTATE_0, gmac_xmtstatus, 4);
		post_log("GMAC_XMTSTATUS0_0__XMTSTATE_0    = %d\n", status);

		status = GET_IPROC_REG_FIELD(GMAC0_XMTSTATUS0_0__CURRDSCR_0, gmac_xmtstatus, 4);
		post_log("GMAC_XMTSTATUS0_0__CURRDSCR_0    = 0x%x\n", status);

		gmac_xmtstatus = cpu_rd_single((GMAC0_XMTSTATUS1_0 + (0x1000 * amac_id)), 4);

		//GMAC0_XMTSTATUS1_0__ACTIVEDESCR_0
		status = GET_IPROC_REG_FIELD(GMAC0_XMTSTATUS1_0__ACTIVEDESCR_0, gmac_xmtstatus, 4);
		post_log("GMAC_XMTSTATUS1_0__ACTIVEDESCR_0 = 0x%x\n", status);

		status = GET_IPROC_REG_FIELD(GMAC0_XMTSTATUS1_0__XMTERR_0, gmac_xmtstatus, 4);
		post_log("GMAC_XMTSTATUS1_0__XMTERR_0      = %d\n", status);
}

//Build TX DMA pkt info
void amac_build_tx_dma_pkt_info(const uint8_t amac_id, uint64_t pkt_addr, uint32_t frame_length)
{
		amac_tx_pkt_info_t* amac_tx_pkt_info_ptr;

		amac_tx_pkt_info_ptr -> frameLen        =  frame_length;
		amac_tx_pkt_info_ptr -> pktAddrLow      = (uint32_t)(pkt_addr & 0xffffffff);
		amac_tx_pkt_info_ptr -> pktAddrHigh     = (uint32_t)((pkt_addr >> 32) & 0xffffffff);
}

//Set up TX DMA descriptor table entry for a packet in DDR
uint32_t amac_config_tx_pkt_descriptor(const uint8_t amac_id, uint32_t amac_tx_pkt_desc_base, uint32_t amac_tx_pkt_desc_tbl_depth, uint64_t pkt_start_address, uint16_t frame_length,uint32_t desc_index)
{
	   amac_pkt_descriptor_t* amac_tx_pkt_desc_addr;
	   uint32_t current_desc_tbl_index = desc_index;
	   uint32_t amac_tx_pkt_desc_nxt_addr;
	   uint8_t amac_loop_idx, num_loops;

	   //amac_tx_pkt_desc_tbl_depth       = ((amac_id == 0) ? (AMAC0_TX_PKT_DESCRIPTOR_TABLE_DEPTH) : ((amac_id == 1) ? AMAC1_TX_PKT_DESCRIPTOR_TABLE_DEPTH : AMAC2_TX_PKT_DESCRIPTOR_TABLE_DEPTH));

	   //if(desc_index==0){
			   post_log("Setting up TX DMA descriptor table at 0x%08x \n",amac_tx_pkt_desc_base);
	   //}

	   if (frame_length % AMAC_PKT_BUFFER_SIZE_IN_BYTES) {
				num_loops = ((uint8_t)(frame_length / AMAC_PKT_BUFFER_SIZE_IN_BYTES) + 1);
	   } else {
				num_loops = (uint8_t) (frame_length / AMAC_PKT_BUFFER_SIZE_IN_BYTES);
	   }

	   amac_tx_pkt_desc_addr            = (amac_pkt_descriptor_t*)(amac_tx_pkt_desc_base);

	   //post_log("[INFO] AMAC %d TX_DMA_DESCRIPTOR_TABLE[%d] @ 0x%08x \n", amac_id, current_desc_tbl_index, amac_tx_pkt_desc_addr);

	   for (amac_loop_idx = num_loops; amac_loop_idx > 0; amac_loop_idx--) {
				amac_tx_pkt_desc_addr -> reserved_0_19           = 0;
				amac_tx_pkt_desc_addr -> flags                   = AMAC_PKT_DESC_FLAGS__CRC_APPEND; //AMAC_PKT_DESC_FLAGS__CRC_FORWARD,
				amac_tx_pkt_desc_addr -> intOnComp               = 0; //Raise interrupt on DMA transfer completion
				amac_tx_pkt_desc_addr -> bufCount                = (frame_length-4); //This field contains the length, in bytes, of the data buffer associated with this descriptor
				amac_tx_pkt_desc_addr -> reserved_47_47          = 0;
				amac_tx_pkt_desc_addr -> addrExt                 = 0; //Address Extension for PCI
				amac_tx_pkt_desc_addr -> reserved_50_50          = 1; //Not sure!
				amac_tx_pkt_desc_addr -> reserved_51_63          = 0; //This field should be 0. Else the decriptor protocol error will be reported

				if (current_desc_tbl_index < (amac_tx_pkt_desc_tbl_depth - 1)) {
						amac_tx_pkt_desc_addr -> eot                     = 0;
				} else {
						amac_tx_pkt_desc_addr -> eot                     = 1;
				}
				post_log("amac_tx_pkt_desc_addr -> eot = %d \n", amac_tx_pkt_desc_addr -> eot);

				if (amac_loop_idx == num_loops) {
						amac_tx_pkt_desc_addr -> sof                     = 1; //When set to 1, this field indicates that the current descriptor is the first one in the current frame
				} else {
						amac_tx_pkt_desc_addr -> sof                     = 0; //When set to 1, this field indicates that the current descriptor is the first one in the current frame
				}

				if (amac_loop_idx == 1) {
						amac_tx_pkt_desc_addr -> eof                     = 1; //When set to 1, this field inDESCRIPTOR_TABLE current descriptor is the last one in the current frame
				} else {
						amac_tx_pkt_desc_addr -> eof                     = 0; //When set to 1, this field indicates that the current descriptor is the last one in the current frame
				}

				amac_tx_pkt_desc_addr -> dataBufPtrLow           = (uint32_t)(pkt_start_address & 0xffffffff);
				amac_tx_pkt_desc_addr -> dataBufPtrHigh          = (uint32_t)((pkt_start_address >> 32) & 0xffffffff);

				amac_tx_pkt_desc_addr++;
	   }
	   amac_tx_pkt_desc_nxt_addr=(uint32_t)amac_tx_pkt_desc_addr;

	   return amac_tx_pkt_desc_nxt_addr;
}

//Enable TX DMA
void amac_enable_tx_dma(const uint8_t amac_id, uint32_t amac_tx_pkt_desc_addr_last_entry)
{
	post_log("\t [INFO] Enabling TX DMA for AMAC %d \n", amac_id);
	post_log("\t Last TxDesc Offset=0x%x \n", (amac_tx_pkt_desc_addr_last_entry & 0x1fff));

	cpu_wr_single((GMAC0_XMTPTR_0 + (0x1000 * amac_id)), (amac_tx_pkt_desc_addr_last_entry & 0x1fff), 4);
}

//Initialize RX DMA packet buffer
void amac_init_rx_dma(const uint8_t amac_id, uint64_t amac_rx_pkt_desc_base, uint16_t amac_rx_pkt_desc_tbl_depth, uint64_t amac_rx_pkt_buffer_base, uint16_t amac_rx_buffer_size_in_bytes)
{
	  amac_pkt_descriptor_t* amac_rx_pkt_desc_addr;
	  uint64_t buffer_address;
	  uint16_t index;
	  uint32_t rdata32;

	  post_log("\t [INFO] Initializng RX DMA for AMAC %d \n", amac_id);

	  cpu_wr_single((GMAC0_RXQCONTROL  + (0x1000 * amac_id)), 0x06000001,4);
	  cpu_wr_single((GMAC0_RCVADDR_LOW + (0x1000 * amac_id)),  (uint32_t)(amac_rx_pkt_desc_base & 0xffffffff), 4);//31:0
	  cpu_wr_single((GMAC0_RCVADDR_HIGH + (0x1000 * amac_id)), (uint32_t)((amac_rx_pkt_desc_base >> 32) & 0xffffffff), 4);//31:0
	  //cpu_wr_single((GMAC0_RCVCONTROL  + (0x1000 * amac_id)), 0x1080809, 4);//31:0// No prefetch
	  //cpu_wr_single((GMAC0_RCVCONTROL  + (0x1000 * amac_id)), 0x01280809, 4);//31:0// wITH prefetch
	  cpu_wr_single((GMAC0_RCVCONTROL  + (0x1000 * amac_id)), 0x03480809, 4);//31:0// wITH prefetch


	  rdata32=cpu_rd_single((GMAC0_RXQCONTROL  + (0x1000 * amac_id)),4);
	  post_log("GMAC0_RXQCONTROL = 0x%x\n",rdata32);
	  rdata32=cpu_rd_single((GMAC0_RCVADDR_LOW  + (0x1000 * amac_id)),4);
	  post_log("GMAC0_RCVADDR_LOW = 0x%x\n",rdata32);
	  rdata32=cpu_rd_single((GMAC0_RCVADDR_HIGH  + (0x1000 * amac_id)),4);
	  post_log("GMAC0_RCVADDR_HIGH = 0x%x\n",rdata32);
	  rdata32 = cpu_rd_single((GMAC0_RCVCONTROL  + (0x1000 * amac_id)),4);
	  post_log("GMAC0_RCVCONTROL = 0x%x\n",rdata32);


	  //Construct the RX DMA descriptor table
	  amac_rx_pkt_desc_addr            = (amac_pkt_descriptor_t*)(amac_rx_pkt_desc_base);
	  post_log("\t Setting up RX DMA descriptor table at 0x%08x \n", amac_rx_pkt_desc_addr);

	  for (index = 0; index < amac_rx_pkt_desc_tbl_depth; index++) {
		post_log("[INFO] Setting up RX DMA descriptor table[%d] at 0x%08x \n",index,amac_rx_pkt_desc_addr);
		buffer_address = amac_rx_pkt_buffer_base + (amac_rx_buffer_size_in_bytes * index);
		amac_rx_pkt_desc_addr -> reserved_0_19           = 0;
		amac_rx_pkt_desc_addr -> flags                   = AMAC_PKT_DESC_FLAGS__CRC_FORWARD;
		amac_rx_pkt_desc_addr -> intOnComp               = 0; //Raise interrupt on DMA transfer completion
		amac_rx_pkt_desc_addr -> bufCount                = amac_rx_buffer_size_in_bytes; //This field contains the length, in bytes, of the data buffer
		amac_rx_pkt_desc_addr -> reserved_47_47          = 0;
		amac_rx_pkt_desc_addr -> addrExt                 = 0; //Address Extension for PCI
		amac_rx_pkt_desc_addr -> reserved_50_50          = 1; //Not sure!
		amac_rx_pkt_desc_addr -> reserved_51_63          = 0; //This field should be 0. Else the decriptor protocol error will be reported

		if (index < amac_rx_pkt_desc_tbl_depth - 1) {
		amac_rx_pkt_desc_addr -> eot                     = 0;
		} else {
		amac_rx_pkt_desc_addr -> eot                     = 1;
		}

		amac_rx_pkt_desc_addr -> sof                     = 0; //NA for RX PKT descriptor
		amac_rx_pkt_desc_addr -> eof                     = 0; //NA for RX PKT descriptor
		amac_rx_pkt_desc_addr -> dataBufPtrLow           = (uint32_t)(buffer_address & 0xffffffff);
		amac_rx_pkt_desc_addr -> dataBufPtrHigh          = (uint32_t)((buffer_address >> 32) & 0xffffffff);

		amac_rx_pkt_desc_addr++;
	  }
}

void amac_get_rx_status(const uint8_t amac_id)
{
		uint32_t gmac_rcvstatus, status;

		post_log("[INFO] Reading RX DMA status for AMAC%d\n", amac_id);

		status = cpu_rd_single((GMAC0_INTSTATUS + (0x1000 * amac_id)), 4);
		post_log("GMAC_INTSTATUS               = 0x%x\n", status);

		gmac_rcvstatus = cpu_rd_single((GMAC0_RCVSTATUS0 + (0x1000 * amac_id)), 4);

		status = GET_IPROC_REG_FIELD(GMAC0_RCVSTATUS0__RCVSTATE, gmac_rcvstatus, 4);
		post_log("GMAC_RCVSTATUS0__RCVSTATE    = %d\n", status);

		status = GET_IPROC_REG_FIELD(GMAC0_RCVSTATUS0__CURRDSCR, gmac_rcvstatus, 4);
		post_log("GMAC_RCVSTATUS0__CURRDSCR    = 0x%x\n", status);

		gmac_rcvstatus = cpu_rd_single((GMAC0_RCVSTATUS1 + (0x1000 * amac_id)), 4);

		status = GET_IPROC_REG_FIELD(GMAC0_RCVSTATUS1__RCVERR, gmac_rcvstatus, 4);
		post_log("GMAC_RCVSTATUS1__RCVERR      = 0x%x\n", status);

		status = GET_IPROC_REG_FIELD(GMAC0_RCVSTATUS1__ACTIVEDESCR, gmac_rcvstatus, 4);
		post_log("GMAC_RCVSTATUS1__ACTIVEDESCR = 0x%x\n", status);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This is done in real time - needs to be real fast!
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t amac_update_rcvptr(const uint8_t amac_id, uint32_t nextDescrOffsetAddr)
{
   uint32_t rdata32;

   post_log("\t [INFO] Enabling RX DMA for AMAC %d\n",amac_id);
   post_log("\t Last RxDesc Entry = 0x%08x \n", nextDescrOffsetAddr);

   cpu_wr_single((GMAC0_RCVPTR + (0x1000 * amac_id)),( nextDescrOffsetAddr & 0x1fff), 4);

   return nextDescrOffsetAddr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

amac_pkt_descriptor_t* amac_get_rx_pkt_descriptor_ptr(uint64_t amac_rx_pkt_desc_base, uint32_t rx_descr_offset)
{
   amac_pkt_descriptor_t* amac_rx_pkt_desc_ptr;

   amac_rx_pkt_desc_ptr   = (amac_pkt_descriptor_t*) (amac_rx_pkt_desc_base + (AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES * rx_descr_offset));
   return amac_rx_pkt_desc_ptr;
}

void amac_display_rx_pkt (uint64_t amac_rx_pkt_desc_base, uint32_t rx_descr_offset)
{
		amac_pkt_descriptor_t* amac_rx_pkt_desc_addr;
		amac_rx_pkt_info_t* amac_rx_pkt_info_ptr;
		uint32_t buffer_address_low, buffer_address_high, index, frame_ptr;

		amac_rx_pkt_desc_addr   = (amac_pkt_descriptor_t*) (amac_rx_pkt_desc_base + (AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES * rx_descr_offset));

	   // post_log(" amac_rx_pkt_desc_addr=0x%08x", amac_rx_pkt_desc_addr);

		buffer_address_low      = (uint32_t) (amac_rx_pkt_desc_addr -> dataBufPtrLow);
		buffer_address_high     = (uint32_t) (amac_rx_pkt_desc_addr -> dataBufPtrHigh);

		//Assuming DDR is 32-bit access
		amac_rx_pkt_info_ptr    = (amac_rx_pkt_info_t*) buffer_address_low;

		post_log(" Packet[%03d]=", rx_descr_offset);
		frame_ptr = (uint32_t)(buffer_address_low + 4);
		//printmem(frame_ptr, (uint32_t) amac_rx_pkt_info_ptr -> frameLen);

		//post_log("BuffAddrHigh= 0x%x BuffAddrLow= 0x%x \n", buffer_address_high, buffer_address_low);

		post_log(" FrmLen=%d\n ", (amac_rx_pkt_info_ptr -> frameLen));
		post_log("PktType=%d\n", (amac_rx_pkt_info_ptr -> pktType));
		post_log("VLANTagDetected=%d\n ", (amac_rx_pkt_info_ptr -> vlanTagDetected));
		post_log("CRCError=%d\n ", (amac_rx_pkt_info_ptr -> crcError));
		post_log("OvrSz=%d\n", (amac_rx_pkt_info_ptr -> overSize));
		post_log(" CTFHit=%d\n ", (amac_rx_pkt_info_ptr -> ctfHit));
		post_log("CTFErr=%d\n", (amac_rx_pkt_info_ptr -> ctfError));
		post_log("PktOverflow=%d\n ", (amac_rx_pkt_info_ptr -> pktOverflow));
		post_log("DscrCntr=%d\n", (amac_rx_pkt_info_ptr -> dscrCntr));
		post_log("DataType=%d\n", (amac_rx_pkt_info_ptr -> dataType));
}

void amac_set_config(const uint8_t amac_id, uint32_t loopback, uint32_t cfgValue)
{
		uint32_t value, readValue, field_startoffset, field_width, field_mask;

		if (loopback == MAC_LOOPBACK) {
				post_log("\t [INFO] Enabling Mac loopback\n");
				field_startoffset = 15;
				field_width = 1;
				field_mask = (uint32_t) ~(((1 << field_width) - 1) << field_startoffset);

				value = cfgValue;
				readValue = cpu_rd_single((UNIMAC0_COMMAND_CONFIG + (0x1000 * amac_id)), 4);

				readValue = readValue | (1 << 16); //Enable TX to phy
				cpu_wr_single((UNIMAC0_COMMAND_CONFIG + (0x1000 * amac_id)), (uint32_t)((readValue & field_mask) | (value << field_startoffset)), 4);
				readValue = cpu_rd_single((UNIMAC0_COMMAND_CONFIG + (0x1000 * amac_id)), 4);
				post_log("UNIMAC0_COMMAND_CONFIG = 0x%08x \n", readValue);
		}
}

void amac_getrxstats(const uint8_t amac_id)
{
   uint32_t readValue;

   post_log("RX Statistics for AMAC%d :\n", amac_id);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_GD_OCTETS_LO + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_GD_OCTETS_LO   = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_GD_OCTETS_HI + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_GD_OCTETS_HI   = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_GD_PKTS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_GD_PKTS        = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_OCTETS_LO + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_ALL_OCTETS_LO  = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_OCTETS_HI + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_ALL_OCTETS_HI  = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_ALL_PKTS       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_BRDCAST + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_BRDCAST        = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_MULT + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_MULT           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_64 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_64             = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_65_127 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_65_127         = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_128_255 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_128_255        = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_256_511 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_256_511        = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_512_1023 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_512_1023       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_1024_1522 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_1024_1522      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_1523_2047 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_1523_2047      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_2048_4095 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_2048_4095      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_4096_8191 + (0x1000 * amac_id)), 4);
   post_log("MAC0_RX_4096_8191       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_8192_MAX + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_8192_MAX       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_JAB + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_JAB            = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_OVR + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_OVR            = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_FRAG + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_FRAG           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_DROP + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_DROP           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_CRC_ALIGN + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_CRC_ALIGN      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_UND + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_UND            = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_CRC + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_CRC            = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALIGN + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_ALIGN          = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_SYM + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_SYM            = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_PAUS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_PAUS           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RX_CNTRL + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RX_CNTRL          = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RXSACHANGES + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RXSACHANGES       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_RXUNICASTPKTS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_RXUNICASTPKTS     = %d \n", readValue);

   post_log("\n");
}

void amac_gettxrxstats(const uint8_t amac_id)
{
   uint32_t readValue;

   post_log("TX Statistics for AMAC %d\n", amac_id);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_LO + (0x1000 * amac_id)), 4);
   //readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_LO + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_GD_OCTETS_LO  = %d\n", readValue);
   //post_log("GMAC0_RX_GD_OCTETS_LO  = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_HI + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_GD_OCTETS_HI  = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_PKTS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_GD_PKTS       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_OCTETS_LO + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_ALL_OCTETS_LO = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_OCTETS_HI + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_ALL_OCTETS_HI = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_ALL_PKTS      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_BRDCAST + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_BRDCAST       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_MULT + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_MULT          = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_64 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_64            = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_65_127 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_65_127        = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_128_255 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_128_255       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_256_511 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_256_511       = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_512_1023 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_512_1023      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1024_1522 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_1024_1522     = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1523_2047 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_1523_2047     = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_2048_4095 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_2048_4095     = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_4096_8191 + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_4096_8191     = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_8192_MAX + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_8192_MAX      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_JAB + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_JAB           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_OVER + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_OVER          = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_FRAG + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_FRAG          = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_UNDERRUN + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_UNDERRUN      = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_COL + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_COL           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1_COL + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_1_COL         = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_M_COL + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_M_COL         = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_EX_COL + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_EX_COL        = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_LATE + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_LATE          = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_DEF + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_DEF           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_CRS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_CRS           = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_PAUS + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TX_PAUS          = %d\n", readValue);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TXUNICASTPKT + (0x1000 * amac_id)), 4);
   post_log("GMAC0_TXUNICASTPKT     = %d \n", readValue);

   post_log("RX Statistics for AMAC %d\n", amac_id);

   readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_LO + (0x1000 * amac_id)), 4);
	  //readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_LO + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_GD_OCTETS_LO  = %d\n", readValue);
	  //post_log("GMAC0_RX_GD_OCTETS_LO  = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_HI + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_GD_OCTETS_HI  = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_PKTS + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_GD_PKTS       = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_OCTETS_LO + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_ALL_OCTETS_LO = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_OCTETS_HI + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_ALL_OCTETS_HI = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_ALL_PKTS      = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_BRDCAST + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_BRDCAST       = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_MULT + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_MULT          = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_64 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_64            = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_65_127 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_65_127        = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_128_255 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_128_255       = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_256_511 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_256_511       = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_512_1023 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_512_1023      = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1024_1522 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_1024_1522     = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1523_2047 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_1523_2047     = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_2048_4095 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_2048_4095     = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_4096_8191 + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_4096_8191     = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_8192_MAX + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_8192_MAX      = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_JAB + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_JAB           = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_OVER + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_OVER          = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_FRAG + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_FRAG          = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_UNDERRUN + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_UNDERRUN      = %d\n", readValue);

	  /*readValue = (uint32_t) cpu_rd_single((GMAC0_TX_COL + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_COL           = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1_COL + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_1_COL         = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_M_COL + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_M_COL         = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_EX_COL + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_EX_COL        = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_LATE + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_LATE          = %d\n", readValue);*/

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_DEF + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_DEF           = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_CRS + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_CRS           = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TX_PAUS + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RX_PAUS          = %d\n", readValue);

	  readValue = (uint32_t) cpu_rd_single((GMAC0_TXUNICASTPKT + (0x1000 * amac_id)), 4);
	  post_log("GMAC0_RXUNICASTPKT     = %d \n", readValue);


	post_log("\n");

}

/*char** parseConfigParams(char* paramValueString, char* delimiter)
{
   char** paramValuePairs;
   uint32_t index=0;

   while((paramValuePairs[index] = strtok(paramValueString, delimiter)) != NULL) {
	  paramValuePairs[index] = strtok(NULL, delimiter);
	  index++;
   }
   return paramValuePairs;
}*/

/*void printmem(uint32_t address, uint32_t num_bytes)
{
   uint32_t addr, index, i;
   uint8_t value, charIdx;
   char* printString = NULL;
   char* str = NULL;
   char* charStr = NULL;

   str = (char *) malloc(100);
   charStr = (char *) malloc(20);
   printString = (char *) malloc(100);

   addr = address;
   for (index = 0; index < num_bytes; index++) {

		//Address
		if ((index % 16) == 0) {
				//Start with a newline
				sprintf(str, " %08x : ", addr);
				strcpy(printString, str);
				charIdx = 0;
		}

		 //8 byte separator '-'
		if ((index % 8) == 0 && (index % 16) != 0) {
				sprintf(str, "- ");
				strcat(printString, str);
				//strcat(charStr, (char*)' ');
				charStr[charIdx] = ' ';
				charIdx++;
		}

		 //byte value
		value = (uint8_t)cpu_rd_single(addr, BIT8);
		sprintf(str, "%02x ", value);
		strcat(printString, str);
		if (isprint(value)) {
				charStr[charIdx] = (char)value;
		} else {
				charStr[charIdx] = '.';
		}
		charIdx++;

		//Print address, 16 data bytes and char values
		if ((index % 16) == 15) {
		   charStr[charIdx] = '\0';
		   post_log("%s", printString);
		   post_log("  %s", charStr);
		}
		//Next byte address
		addr++;
   }


   free(str);
   free(charStr);
   free(printString);
}*/

uint32_t cmpmem(uint32_t block1_start_addr, uint32_t block2_start_addr, uint32_t block_size_in_bytes)
{
   uint32_t index, flag = 0;
   uint32_t dataScr,dataDst;
   uint8_t *src;
   uint8_t *dst;
   src = (uint8_t *)block1_start_addr;
   dst = (uint8_t *)block2_start_addr;
   post_log("Scr_addr=0x%x\t Dst_addr=0x%x\t PktData_sz=%d\n",block1_start_addr,block2_start_addr,block_size_in_bytes);
   for (index = 0; index < block_size_in_bytes; index++) {
		  dataScr= *(src + index);//cpu_rd_single((block1_start_addr + index), BIT8);
		  dataDst= *(dst + index);//cpu_rd_single((block2_start_addr + index), BIT8);
	  if (dataScr!=dataDst) {
			  post_log("Data at %#x is %#x but expected %#x\n",(block2_start_addr + index),dataDst,dataScr);
		 flag = 1;
		 break;
	  }
   }

   return flag;
}



/*void amac_tests(uint8_t* command_string)
{
   char* command;
   uint8_t amac_id;
//amac_id = (uint8_t) getVal(command_string, 2));
   amac_id = getVal(command_string, 2);

//   command = (char*)getCmd(command_string, 3);
   command = (char*)getCmd(command_string, 3);

   post_log("id is %d and command is %s\n",amac_id,command);
   if (strcasecmp(command, "init") == 0) {
		amac_init(amac_id);
   } else if (strcasecmp(command, "inittxdma") == 0) {
		 amac_init_tx_dma(amac_id, getVal(command_string, 4));
   } else if (strcasecmp(command, "enabletxdma") == 0) {
		 amac_enable_tx_dma(amac_id, getVal(command_string, 4));
   } else if (strcasecmp(command, "initrxdma") == 0) {
		 amac_init_rx_dma(amac_id, (uint64_t)getVal(command_string, 4), (uint16_t)getVal(command_string, 5), (uint64_t)getVal(command_string, 6), (uint16_t)getVal(command_string, 7));
   } else if (strcasecmp(command, "updaterxdma") == 0) {
		 amac_update_rcvptr(amac_id, (uint32_t)getVal(command_string, 4));
   }  else if (strcasecmp(command, "disprxdmapktinfo") == 0) {
		amac_display_rx_pkt(getVal(command_string, 4), getVal(command_string, 5));
   } else if (strcasecmp(command, "rxstats") == 0) {
		 amac_getrxstats(amac_id);
   } else if (strcasecmp(command, "txstats") == 0) {
		 amac_gettxstats(amac_id);
   } else if (strcasecmp(command, "test1") == 0) {
		//amac_test1();
   } else if (strcasecmp(command, "printmem") == 0) {
		printmem((uint32_t) getVal(command_string, 4), (uint32_t) getVal(command_string, 5));
   } else if (strcasecmp(command, "set") == 0) {
	  amac_set_config(amac_id, (char*)getCmd(command_string, 4), getVal(command_string, 5));
   }


}*/

void amac_gettxstats(const uint8_t amac_id)
{
   uint32_t readValue;

   post_log("TX Statistics for AMAC %d\n", amac_id);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_GD_OCTETS_LO + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_GD_OCTETS_LO  = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_GD_OCTETS_HI + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_GD_OCTETS_HI  = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_GD_PKTS + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_GD_PKTS       = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_ALL_OCTETS_LO + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_ALL_OCTETS_LO = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_ALL_OCTETS_HI + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_ALL_OCTETS_HI = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_ALL_PKTS      = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_BRDCAST + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_BRDCAST       = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_MULT + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_MULT          = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_64 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_64            = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_65_127 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_65_127        = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_128_255 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_128_255       = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_256_511 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_256_511       = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_512_1023 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_512_1023      = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_1024_1522 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_1024_1522     = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_1523_2047 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_1523_2047     = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_2048_4095 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_2048_4095     = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_4096_8191 + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_4096_8191     = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_8192_MAX + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_8192_MAX      = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_JAB + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_JAB           = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_OVER + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_OVER          = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_FRAG + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_FRAG          = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_UNDERRUN + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_UNDERRUN      = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_COL + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_COL           = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_1_COL + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_1_COL         = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_M_COL + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_M_COL         = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_EX_COL + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_EX_COL        = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_LATE + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_LATE          = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_DEF + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_DEF           = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_CRS + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_CRS           = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_PAUS + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TX_PAUS          = %d\n", readValue);

   readValue = (uint32_t) CPU_READ_SINGLE((GMAC0_TXUNICASTPKT + (0x1000 * amac_id)), BIT32);
   post_log("GMAC0_TXUNICASTPKT     = %d \n", readValue);

	post_log("\n");

}


void suspend_tx(const uint8_t amac_id){
		uint32_t rdata32;
		rdata32=cpu_rd_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)),4);
		rdata32=(rdata32 | 0x2);//suspend tx
		cpu_wr_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), rdata32, 4);
		rdata32=cpu_rd_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)),4);
		post_log("TX Disabled GMAC0_XMTCONTROL_0 = 0x%x\n",rdata32);
}


void disable_tx(const uint8_t amac_id){
		uint32_t rdata32;
		rdata32=cpu_rd_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)),4);
		rdata32=(rdata32 & 0xFFFFFFFE);//disable TX
		cpu_wr_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), rdata32, 4);
		rdata32=cpu_rd_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)),4);
		post_log("TX Disabled GMAC0_XMTCONTROL_0 = 0x%x\n",rdata32);
}


void enable_tx(const uint8_t amac_id){
		uint32_t rdata32;
		rdata32=cpu_rd_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)),4);
		rdata32=(rdata32 | 0x00000001);//enable TX
		cpu_wr_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)), rdata32, 4);
		rdata32=cpu_rd_single((GMAC0_XMTCONTROL_0 + (0x1000 * amac_id)),4);
		post_log("TX Enabled GMAC0_XMTCONTROL_0 = 0x%x\n",rdata32);
}

void disable_rx(const uint8_t amac_id){
		uint32_t rdata32;
		rdata32=cpu_rd_single((GMAC0_RCVCONTROL + (0x1000 * amac_id)),4);
		rdata32=(rdata32 & 0xFFFFFFFE);//disable T=RX
		cpu_wr_single((GMAC0_RCVCONTROL + (0x1000 * amac_id)), rdata32, 4);
		rdata32=cpu_rd_single((GMAC0_RCVCONTROL + (0x1000 * amac_id)),4);
		post_log("RX Disabled GMAC0_RCVCONTROL = 0x%x\n",rdata32);
}


void enable_rx(const uint8_t amac_id){
		uint32_t rdata32;
		rdata32=cpu_rd_single((GMAC0_RCVCONTROL + (0x1000 * amac_id)),4);
		rdata32=(rdata32 | 0x00000001);//enable TX
		cpu_wr_single((GMAC0_RCVCONTROL + (0x1000 * amac_id)), rdata32, 4);
		rdata32=cpu_rd_single((GMAC0_RCVCONTROL + (0x1000 * amac_id)),4);
		post_log("RX Enabled GMAC0_RCVCONTROL = 0x%x\n",rdata32);
}

uint8_t tx_done(const uint8_t amac_id){
		uint32_t tx_status,tx_state;
		uint8_t done_status=0;
		tx_status = cpu_rd_single((GMAC0_XMTSTATUS0_0 + (0x1000 * amac_id)), 4);
		tx_state = GET_IPROC_REG_FIELD(GMAC0_XMTSTATUS0_0__XMTSTATE_0,tx_status, 4);
		//post_log("Tx status = %x\n",tx_state);
		if(tx_state==0x2){
				//done_status=1;
				post_log("Tx Done\n");
		}
		else if(tx_state==0x0){
				post_log("Tx Disabled\n");
		}
		else if(tx_state==0x1){
				post_log("Tx Active\n");
				done_status=1;
		}
		else if(tx_state==0x3){
				post_log("Tx stopped\n");
		}
		else if(tx_state==0x4){
				post_log("Tx Suspend pending\n");
		}
		else{
				post_log("Undefined Status\n");
		}

		return done_status;
}

uint8_t rx_done(const uint8_t amac_id){
		uint32_t rx_status,rx_state;
		uint8_t done_status=0;
		rx_status = cpu_rd_single((GMAC0_RCVSTATUS0 + (0x1000 * amac_id)), 4);
		rx_state = GET_IPROC_REG_FIELD(GMAC0_RCVSTATUS0__RCVSTATE,rx_status, 4);

		if(rx_state==0x2){
				//done_status=1;
				post_log("Rx Done\n");
		}
		else if(rx_state==0x0){
				post_log("Rx Disabled\n");
		}
		else if(rx_state==0x1){
				post_log("Rx Active\n");
		}
		else if(rx_state==0x3){
				post_log("Rx stopped\n");
		}
		else if(rx_state==0x4){
				post_log("Rx Suspend pending\n");
		}
		else{
				post_log("Undefined Status\n");
		}

		return done_status;
}

uint32_t checker_tpkt2rpkt(const uint8_t amac_id) {
   //Checks tx packets to rx packets
   uint32_t txpkts,rxpkts,diff;

   txpkts = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)), 4);
   rxpkts = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)), 4);
   diff = txpkts - rxpkts;

   post_log("Tx ALL packet = %d\n",txpkts);
   post_log("Rx ALL packet = %d\n",rxpkts);


   if ((diff != 0) ||(txpkts==0) ) {
	  post_log("TXALLPKTS - RXALLPKTS = %d\n",diff);
	  return TEST_FAIL;
   } else {
	  post_log("PASS: TXALLPKT == RXALLPKTS\n");
	  return TEST_PASS;
   }
}

uint32_t checker_toct2roct(const uint8_t amac_id) {
   //Checks tx packets to rx packets
   uint32_t txocts,rxocts,diff;

   txocts = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_OCTETS_LO + (0x1000 * amac_id)), 4);
   rxocts = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_OCTETS_LO + (0x1000 * amac_id)), 4);
   diff = txocts - rxocts;

   post_log("Tx ALL Octets = %d\n",txocts);
   post_log("Rx ALL Octets = %d\n",rxocts);


   if ((diff != 0) ||(txocts==0) ) {
	  post_log("TXALLOCTETS - RXALLOCTETS = %d\n",diff);
	  return TEST_FAIL;
   } else {
	  post_log("PASS: TXALLOCTETS == RXALLOCTETS\n");
	  return TEST_PASS;
   }
}

uint32_t checker_t2r(const uint8_t amac_id,uint32_t addr1,uint32_t addr2) {
   //Checks tx packets to rx packets
   uint32_t r1,r2,diff;

   r1 = (uint32_t) RdReg(addr1 + (0x1000 * amac_id));
   r2 = (uint32_t) RdReg(addr2 + (0x1000 * amac_id));
   diff = r1-r2;

   if (diff != 0) {
	  post_log("TX and RX Data Mismatch = %d\n",diff);
	  return TEST_FAIL;
   } else {
	  post_log("PASS: TX and RX Data Match\n");
	  return TEST_PASS;
   }
}

uint32_t checker_zero(const uint8_t amac_id) {
   //Checks Error are not Flagged
   uint32_t tx,tx1,tx2,tx3,tx4,tx5,tx6,tx7,tx8,tx9,tx10;
   uint32_t rx,rx1,rx2,rx3,rx4,rx5,rx6,rx7,rx8,rx9,rx10,rx11;

   tx1 = (uint32_t) RdReg(GMAC0_TX_JAB + (0x1000 * amac_id));
   tx2 = (uint32_t) RdReg(GMAC0_TX_OVER + (0x1000 * amac_id));
   tx3 = (uint32_t) RdReg(GMAC0_TX_FRAG + (0x1000 * amac_id));
   tx4 = (uint32_t) RdReg(GMAC0_TX_UNDERRUN + (0x1000 * amac_id));
   tx5 = (uint32_t) RdReg(GMAC0_TX_COL + (0x1000 * amac_id));
   tx6 = (uint32_t) RdReg(GMAC0_TX_1_COL + (0x1000 * amac_id));
   tx7 = (uint32_t) RdReg(GMAC0_TX_M_COL + (0x1000 * amac_id));
   tx8 = (uint32_t) RdReg(GMAC0_TX_EX_COL + (0x1000 * amac_id));
   tx9 = (uint32_t) RdReg(GMAC0_TX_LATE + (0x1000 * amac_id));
   tx10 = (uint32_t) RdReg(GMAC0_TX_DEF + (0x1000 * amac_id));

   rx1 = (uint32_t) RdReg(GMAC0_RX_JAB + (0x1000 * amac_id));
   rx2 = (uint32_t) RdReg(GMAC0_RX_OVR + (0x1000 * amac_id));
   rx3 = (uint32_t) RdReg(GMAC0_RX_FRAG + (0x1000 * amac_id));
   rx4 = (uint32_t) RdReg(GMAC0_RX_DROP + (0x1000 * amac_id));
   rx5 = (uint32_t) RdReg(GMAC0_RX_CRC_ALIGN + (0x1000 * amac_id));
   rx6 = (uint32_t) RdReg(GMAC0_RX_UND + (0x1000 * amac_id));
   rx7 = (uint32_t) RdReg(GMAC0_RX_CRC + (0x1000 * amac_id));
   rx8 = (uint32_t) RdReg(GMAC0_RX_ALIGN + (0x1000 * amac_id));
   rx9 = (uint32_t) RdReg(GMAC0_RX_SYM + (0x1000 * amac_id));
   rx10 = (uint32_t) RdReg(GMAC0_RX_PAUS + (0x1000 * amac_id));
   rx11 = (uint32_t) RdReg(GMAC0_RX_CNTRL + (0x1000 * amac_id));

   tx=(tx1| tx2| tx3| tx4| tx5| tx6| tx7| tx8| tx9| tx10);
   rx=(rx1| rx2| rx3| rx4| rx5| rx6| rx7| rx8| rx9| rx10| rx11);

   if ((tx | rx)!=0 ){
		if(tx!=0){
			post_log("TX Error Register Flagged\n");}
		if(rx!=0){
			post_log("RX Error Register Flagged\n");}
		return TEST_FAIL;
   } else {
	  post_log("PASS: No Transmission Error\n");
	  return TEST_PASS;
   }
}



uint32_t checker_all(const uint8_t amac_id){
		uint32_t result1,result2;
		result1=checker_tpkt2rpkt(amac_id);
		result2=checker_toct2roct(amac_id);
		if((result1==TEST_PASS)&&(result2==TEST_PASS)){
				return TEST_PASS;
		}
		else{return TEST_FAIL;}
}


void counter_reset(const uint8_t amac_id){
		uint32_t rdata32,readValue;
		rdata32=cpu_rd_single((GMAC0_DEVCONTROL + (0x1000 * amac_id)),4);
		rdata32=(rdata32 | 0x000000010);//enable counter read reset
		cpu_wr_single((GMAC0_DEVCONTROL + (0x1000 * amac_id)), rdata32, 4);
		rdata32=cpu_rd_single((GMAC0_DEVCONTROL + (0x1000 * amac_id)),4);
		post_log("COUNTER RESET GMAC0_DEVCONTROL = 0x%x\n",rdata32);
		//Read Rx Counter registers
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_GD_OCTETS_LO + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_GD_OCTETS_HI + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_GD_PKTS + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_OCTETS_LO + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_OCTETS_HI + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_BRDCAST + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_MULT + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_64 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_65_127 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_128_255 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_256_511 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_512_1023 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_1024_1522 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_1523_2047 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_2048_4095 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_4096_8191 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_8192_MAX + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_JAB + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_OVR + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_FRAG + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_DROP + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_CRC_ALIGN + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_UND + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_CRC + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_ALIGN + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_SYM + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_PAUS + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RX_CNTRL + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RXSACHANGES + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_RXUNICASTPKTS + (0x1000 * amac_id)), 4);
		//Read Tx Counters
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_LO + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_OCTETS_HI + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_GD_PKTS + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_OCTETS_LO + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_OCTETS_HI + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_BRDCAST + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_MULT + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_64 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_65_127 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_128_255 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_256_511 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_512_1023 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1024_1522 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1523_2047 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_2048_4095 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_4096_8191 + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_8192_MAX + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_JAB + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_OVER + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_FRAG + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_UNDERRUN + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_COL + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_1_COL + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_M_COL + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_EX_COL + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_LATE + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_DEF + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_CRS + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TX_PAUS + (0x1000 * amac_id)), 4);
		readValue = (uint32_t) cpu_rd_single((GMAC0_TXUNICASTPKT + (0x1000 * amac_id)), 4);
		rdata32=(rdata32 & 0xFFFFFFEF);
		cpu_wr_single((GMAC0_DEVCONTROL + (0x1000 * amac_id)), rdata32, 4);
		rdata32=cpu_rd_single((GMAC0_DEVCONTROL + (0x1000 * amac_id)),4);
		post_log("COUNTER RESET GMAC0_DEVCONTROL = 0x%x\n",rdata32);
}

void waitforTrans(const uint8_t amac_id,uint32_t tx_desc_len,uint32_t rx_desc_len){
	uint32_t rdata32=0;
	post_log("\t [INFO] Waiting for stabilizing AMAC %d status registers\n",amac_id);
	while(1) {
		rdata32 = (uint32_t) cpu_rd_single((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)),4);
	#ifndef MMU
		 delay_noise(1,0);
   #endif
		post_log("\t [INFO] Total Packet Transmitted. = %d\n",rdata32);
		  //if (rdata32 == (uint32_t) cpu_rd_single((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)),4)) {
		  if (rdata32 == tx_desc_len-1) { //commented by sathishs to debug
	  //the underrun scenario!!!
		   break;
		  }
	}
	rdata32=0;
	while(1) {
		rdata32 = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)),4);
		 post_log("\t [INFO] Receiving Packet No. = %d\n",rdata32);

	#ifndef MMU
		  delay_noise(1,0);
	 #endif
rdata32 = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)),4);

		if (rdata32 == rx_desc_len-2) {
	//if (rdata32 == (uint32_t) cpu_rd_single((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)),4)) {
		   break;
		   post_log("\t [INFO] Receiving Packet No. = %d\n",rdata32);
		 }


	  post_log("\t [INFO] Receiving Packet No. = %d\n",rdata32);
		  // if (rdata32=50){
		  // rdata32 = (uint32_t) cpu_rd_single((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)),4);
		   //post_log("\t [INFO] No.of Receiving Packet No. = %d\n",rdata32);
			//break;
	   // }
	}


//       do {
//            rdata32 = (uint32_t) cpu_rd_single((GMAC0_RX_GD_PKTS + (0x1000 * amac_id)),4);
//
//           post_log("\t [INFO] Receiving Packet No. = %d\n",rdata32);
//
//          } while (rdata32=50);
//
//
}

void waitforTrans_internalloopback(const uint8_t amac_id){
	uint32_t rdata32=0;
	post_log("\t [INFO] Waiting for stabilizing AMAC %d status registers\n",amac_id);
	while(1) {
		rdata32 = (uint32_t) CPU_READ_SINGLE((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)),BIT32);
		delay_noise(1,0);
		if (rdata32 == (uint32_t) CPU_READ_SINGLE((GMAC0_TX_ALL_PKTS + (0x1000 * amac_id)),BIT32)) {
		  break;
		}
	}
	rdata32=0;
	while(1) {
		rdata32 = (uint32_t) CPU_READ_SINGLE((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)),BIT32);
		delay_noise(1,0);
		if (rdata32 == (uint32_t) CPU_READ_SINGLE((GMAC0_RX_ALL_PKTS + (0x1000 * amac_id)),BIT32)) {
			break;
		}
	}

}

uint32_t delay_noise(uint32_t seconds, int noise) {
   //Not a very accurate timer
   int i,j,k=0;
   volatile uint32_t raddr;
   volatile uint32_t rdata32;

   raddr = 0x60000000;

   while(seconds--) {
		for (i=0;i<20;i++) {
		   for (j=0;j<30;j++) {
			  {
				 if (noise ==1) {
					rdata32 = RdReg(raddr);
				 } else {
					for (k=0;k<100;k++) {}
				 }
			  }

		   }
		}
		post_log("\r    %d",seconds);
   }

   return 0;
}


uint32_t chkpktdata(const uint8_t amac_id,uint32_t tx_desc_base,uint32_t rx_desc_base,uint32_t chk_noPkt){
   amac_pkt_descriptor_t* amac_tx_pkt_dscr_ptr;
   amac_pkt_descriptor_t* amac_rx_pkt_dscr_ptr;
   amac_rx_pkt_info_t* pkt_ptr;
   uint32_t rcvPtr,currDscr,pkt_diff,nextDscr;
   test_status_t rpkt_chk;



   rcvPtr=((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0x00001fff);
   currDscr=(chk_noPkt*0x10);
   rpkt_chk=TEST_PASS;
   while (rcvPtr < currDscr) {
		amac_tx_pkt_dscr_ptr = (amac_pkt_descriptor_t*) (((tx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0xffffe000) | rcvPtr);
		amac_rx_pkt_dscr_ptr = (amac_pkt_descriptor_t*) (((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0xffffe000) | rcvPtr);
		post_log("AMAC TX PKT DSCR PTR ADDR : %08X \n",(uint32_t *)amac_tx_pkt_dscr_ptr);
		post_log("AMAC RX PKT DSCR PTR ADDR : %08X \n",(uint32_t *)amac_rx_pkt_dscr_ptr);
		 post_log("RCV PTR                  : %08X \n",rcvPtr);


		pkt_ptr = (amac_rx_pkt_info_t*)(amac_rx_pkt_dscr_ptr -> dataBufPtrLow);

		post_log("amac_rx_pkt_dscr_ptr->dataBufPtrLow = 0x%x\n", (amac_rx_pkt_dscr_ptr -> dataBufPtrLow));
		post_log("((pkt_ptr -> frameLen) - 4) = %d\n",((pkt_ptr -> frameLen) - 4));

	  //  amac_display_rx_pkt((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)), (rcvPtr / AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES));

		pkt_diff = cmpmem((amac_tx_pkt_dscr_ptr -> dataBufPtrLow), ((uint32_t)(pkt_ptr) + 4), ((pkt_ptr -> frameLen) - 4));
		if (pkt_diff == 0) {
		   post_log("Packet[%03d] => PASS\n", ((rcvPtr / AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES)+1));
		} else {
		   post_log("Packet[%03d] => FAIL\n", ((rcvPtr / AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES)+1));
		   rpkt_chk=TEST_FAIL;
		   break;
		}

		if (!(amac_rx_pkt_dscr_ptr -> eot)) {
				nextDscr = rcvPtr + AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES;
		} else {
				nextDscr = ((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0x00001fff);
		}

		rcvPtr += AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES;
   }

   return rpkt_chk;
}

//////////Additional Packet compare

uint32_t chkpktdata_add(const uint8_t amac_id,uint32_t tx_desc_base,uint32_t rx_desc_base,uint32_t chk_noPkt){
   amac_pkt_descriptor_t* amac_tx_pkt_dscr_ptr;
   amac_pkt_descriptor_t* amac_rx_pkt_dscr_ptr;
   amac_rx_pkt_info_t* pkt_ptr;
   uint32_t rcvPtr,currDscr,pkt_diff,nextDscr,cntr,txptr;
   test_status_t rpkt_chk;


   cntr=0;
   rcvPtr=((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0x00001fff);
   currDscr=(chk_noPkt*0x10);
   rpkt_chk=TEST_PASS;
   while (rcvPtr < currDscr) {
		if(cntr<= chk_noPkt/2)
		{
		txptr=rcvPtr;
		amac_tx_pkt_dscr_ptr = (amac_pkt_descriptor_t*) (((tx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0xffffe000) | txptr);
		amac_rx_pkt_dscr_ptr = (amac_pkt_descriptor_t*) (((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0xffffe000) | rcvPtr);
		post_log("AMAC TX PKT DSCR PTR ADDR : %08X \n",(uint32_t *)amac_tx_pkt_dscr_ptr);
		post_log("AMAC RX PKT DSCR PTR ADDR : %08X \n",(uint32_t *)amac_rx_pkt_dscr_ptr);
		 post_log("RCV PTR                  : %08X \n",rcvPtr);
		 }

		if (cntr == chk_noPkt/2)
		   txptr = 0;

		if(cntr> chk_noPkt/2){
		amac_tx_pkt_dscr_ptr = (amac_pkt_descriptor_t*) (((tx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0xffffe000) | txptr);
		amac_rx_pkt_dscr_ptr = (amac_pkt_descriptor_t*) (((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0xffffe000) | rcvPtr);
		post_log("AMAC TX PKT DSCR PTR ADDR : %08X \n",(uint32_t *)amac_tx_pkt_dscr_ptr);
		post_log("AMAC RX PKT DSCR PTR ADDR : %08X \n",(uint32_t *)amac_rx_pkt_dscr_ptr);
		post_log("RCV PTR                  : %08X \n",rcvPtr);
		 txptr += AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES;


		}


		pkt_ptr = (amac_rx_pkt_info_t*)(amac_rx_pkt_dscr_ptr -> dataBufPtrLow);

		post_log("amac_rx_pkt_dscr_ptr->dataBufPtrLow = 0x%x\n", (amac_rx_pkt_dscr_ptr -> dataBufPtrLow));
		post_log("((pkt_ptr -> frameLen) - 4) = %d\n",((pkt_ptr -> frameLen) - 4));

	  //  amac_display_rx_pkt((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)), (rcvPtr / AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES));

		pkt_diff = cmpmem((amac_tx_pkt_dscr_ptr -> dataBufPtrLow), ((uint32_t)(pkt_ptr) + 4), ((pkt_ptr -> frameLen) - 4));

		post_log("(amac_tx_pkt_dscr_ptr -> dataBufPtrLow) = %8x\n",(uint32_t)(amac_tx_pkt_dscr_ptr -> dataBufPtrLow));

		if (pkt_diff == 0) {
		   post_log("Packet[%03d] => PASS\n", ((rcvPtr / AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES)+1));
		} else {
		   post_log("Packet[%03d] => FAIL\n", ((rcvPtr / AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES)+1));
		   rpkt_chk=TEST_FAIL;
		   //break;
		}

		if (!(amac_rx_pkt_dscr_ptr -> eot)) {
				nextDscr = rcvPtr + AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES;
		} else {
				nextDscr = ((rx_desc_base + (AMAC_DESC_TABLE_SIZE * amac_id)) & 0x00001fff);
		}



	   rcvPtr += AMAC_RX_PKT_DESCRIPTOR_LENGTH_IN_BYTES;

	   cntr++;



   }

   return rpkt_chk;
}



/////////////////////////////

////////////////////////////////////////// Functions for managed mode in CYGNUS /////////////////////////////////////////////////////////
void cfg_sw(void)
{
	uint32_t page;
	uint32_t offset;
	uint32_t wdatah, wdatal;
	uint32_t rdatah, rdatal;
	uint32_t port_id;
	uint32_t *rdata;

	// write port control register
	page=0x0;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x0;
		if(port_id==1) offset=0x1;
		if(port_id==2) offset=0x5;
		if(port_id==3) offset=0x8;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		//post_log("Port control rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		wdatal= 0;//(rdatal&0xffffffc0);
		wdatah=rdatah;
		//post_log("Port control wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
		//post_log("Port control rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);

	}
	// write switch mode register
	offset=0xb;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	////post_log("Switch mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	//post_log("switch mode rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=((rdatal&0xfffffffe) | 0x2);
	wdatah=rdatah;
	//post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	//rdata = srab_rd(page, offset);
	//post_log("switch mode rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// write switch control register
	offset=0x22;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	////post_log("Switch ctrl mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	//post_log("switch ctrl rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=rdatal | ((rdatal&0xffffffff) | 0x40);
	wdatah=rdatah;
	//post_log("switch ctrl wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	//rdata = srab_rd(page, offset);
	//post_log("switch ctrl rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// port override register
	page=0xe4;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x00;
		if(port_id==1) offset=0x02;
		if(port_id==2) offset=0x0a;
		if(port_id==3) offset=0x10;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		////post_log("Port override rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
		//post_log("port override rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		if(port_id==3)
			wdatal=(rdatal | 0x1 | 0xf0);
		else
			wdatal=(rdatal | 0x1 | 0x70);
		wdatah=rdatah;
		//post_log("port override wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
		//post_log("port override rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	}

	// BRCM Header disable
	page = 0x02;
	offset = 0x3;
	wdatal=0x0;
	wdatah=0x0;
	//post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);

/////////Configuration for jumbo frames - Added By Nishant///////
	page=0x40;
	offset = 0x1;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x123);
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

 offset = 0x5;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x2710);
		 wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);

post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

///Configure RGMII Delay - Added by Nishant////
	page=0xe4;
	offset = 0x2a;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x3);
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Delay config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

#ifdef MAG_MODE

//  //Global ManagemetControl Register
//  page=0x2;
//	offset = 0x0;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//			wdatal=(rdatal | 0xC2);  // bit 7,6 =1 Enable IMP0/1
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("Global Management Configuration register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
// page=0x0;
//	offset = 0xB;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//   wdatal=(rdatal | 0x1);  // bit 0 =1 Managed Mode forwarding enabled
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("Switch Mode register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
////Broadcom Header control Register
// page=0x2;
//	offset = 0x3;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//   wdatal=(rdatal | 0x1);  // bit 0 = 1 BRCM Header disabled
//
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("BRCM register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);


 page=0x31;
	offset = 0x2;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
   wdatal=(rdatal & 0x102);  // bit 8 and bit 1 = Port 8 & Port 1 egress enabled

		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("VLAN Port 1 register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

 page=0x31;
	offset = 0xa;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
   wdatal=(rdatal & 0x120);  // bit 8 and bit 5 = Port 8 & Port 2 egress enabled

		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("VLAN Port 2 register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);


#endif
}

/*void cfg_sw(void)
{
	uint32_t page;
	uint32_t offset;
	uint32_t wdatah, wdatal;
	uint32_t rdatah, rdatal;
	uint32_t port_id;
	uint32_t *rdata;

	// write port control register
	page=0x0;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x0;
		if(port_id==1) offset=0x1;
		if(port_id==2) offset=0x5;
		if(port_id==3) offset=0x8;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		//post_log("Port control rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		wdatal= 0;//(rdatal&0xffffffc0);
		wdatah=rdatah;
		//post_log("Port control wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
		//post_log("Port control rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);

	}
	// write switch mode register
	offset=0xb;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	////post_log("Switch mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	//post_log("switch mode rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=((rdatal&0xfffffffe) | 0x2);
	wdatah=rdatah;
	//post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	//rdata = srab_rd(page, offset);
	//post_log("switch mode rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// write switch control register
	offset=0x22;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	////post_log("Switch ctrl mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	//post_log("switch ctrl rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=rdatal | ((rdatal&0xffffffff) | 0x40);
	wdatah=rdatah;
	//post_log("switch ctrl wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	//rdata = srab_rd(page, offset);
	//post_log("switch ctrl rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// port override register
	page=0xe4;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x00;
		if(port_id==1) offset=0x02;
		if(port_id==2) offset=0x0a;
		if(port_id==3) offset=0x10;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		////post_log("Port override rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
		//post_log("port override rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		if(port_id==3)
			wdatal=(rdatal | 0x1 | 0xf0);
		else
			wdatal=(rdatal | 0x1 | 0x70);
		wdatah=rdatah;
		//post_log("port override wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
		//post_log("port override rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	}

	// BRCM Header disable
	page = 0x02;
	offset = 0x3;
	wdatal=0x0;
	wdatah=0x0;
	//post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);

/////////Configuration for jumbo frames - Added By Nishant///////
	page=0x40;
	offset = 0x1;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x123);
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

 offset = 0x5;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x2710);
		 wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);

post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

///Configure RGMII Delay - Added by Nishant////
	page=0xe4;
	offset = 0x2a;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x3);
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Delay config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

//#ifdef MAG_MODE

//  //Global ManagemetControl Register
//  page=0x2;
//	offset = 0x0;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//			wdatal=(rdatal | 0xC2);  // bit 7,6 =1 Enable IMP0/1
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("Global Management Configuration register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
// page=0x0;
//	offset = 0xB;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//   wdatal=(rdatal | 0x1);  // bit 0 =1 Managed Mode forwarding enabled
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("Switch Mode register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
////Broadcom Header control Register
// page=0x2;
//	offset = 0x3;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//   wdatal=(rdatal | 0x1);  // bit 0 = 1 BRCM Header disabled
//
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("BRCM register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);


 page=0x31;
	offset = 0x2;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
   wdatal=(rdatal & 0x102);  // bit 8 and bit 1 = Port 8 & Port 1 egress enabled

		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("VLAN Port 1 register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

 page=0x31;
	offset = 0xa;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
   wdatal=(rdatal & 0x120);  // bit 8 and bit 5 = Port 8 & Port 2 ingress enabled

		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("VLAN Port 2 register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);


//#endif
}*/

void cfg_sw_0_1(void)
{
	uint32_t page;
	uint32_t offset;
	uint32_t wdatah, wdatal;
	uint32_t rdatah, rdatal;
	uint32_t port_id;
	uint32_t *rdata;

	// write port control register
	page=0x0;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x0;
		if(port_id==1) offset=0x1;
		if(port_id==2) offset=0x5;
		if(port_id==3) offset=0x8;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		//post_log("Port control rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		wdatal= 0;//(rdatal&0xffffffc0);
		wdatah=rdatah;
		//post_log("Port control wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
		//post_log("Port control rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);

	}
	// write switch mode register
	offset=0xb;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	////post_log("Switch mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	//post_log("switch mode rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=((rdatal&0xfffffffe) | 0x2);
	wdatah=rdatah;
	//post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	//rdata = srab_rd(page, offset);
	//post_log("switch mode rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// write switch control register
	offset=0x22;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	////post_log("Switch ctrl mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	//post_log("switch ctrl rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=rdatal | ((rdatal&0xffffffff) | 0x40);
	wdatah=rdatah;
	//post_log("switch ctrl wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	//rdata = srab_rd(page, offset);
	//post_log("switch ctrl rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// port override register
	page=0xe4;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x00;
		if(port_id==1) offset=0x02;
		if(port_id==2) offset=0x0a;
		if(port_id==3) offset=0x10;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		////post_log("Port override rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
		//post_log("port override rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		if(port_id==3)
			wdatal=(rdatal | 0x1 | 0xf0);
		else
			wdatal=(rdatal | 0x1 | 0x70);
		wdatah=rdatah;
		//post_log("port override wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
		//post_log("port override rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	}

	// BRCM Header disable
	page = 0x02;
	offset = 0x3;
	wdatal=0x0;
	wdatah=0x0;
	//post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);

/*/////////Configuration for jumbo frames - Added By Nishant///////
	page=0x40;
	offset = 0x1;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x123);
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

 offset = 0x5;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x2710);
		 wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);

post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);*/

///Configure RGMII Delay - Added by Nishant////
	page=0xe4;
	offset = 0x2a;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x3);
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Delay config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

//#ifdef MAG_MODE

//  //Global ManagemetControl Register
//  page=0x2;
//	offset = 0x0;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//			wdatal=(rdatal | 0xC2);  // bit 7,6 =1 Enable IMP0/1
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("Global Management Configuration register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
// page=0x0;
//	offset = 0xB;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//   wdatal=(rdatal | 0x1);  // bit 0 =1 Managed Mode forwarding enabled
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("Switch Mode register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
////Broadcom Header control Register
// page=0x2;
//	offset = 0x3;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//   wdatal=(rdatal | 0x1);  // bit 0 = 1 BRCM Header disabled
//
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("BRCM register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

#if 0
	page=0x31;
	offset = 0x2;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	wdatal=0x100;
	wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);

	page=0x31;
	offset = 0x10;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	wdatal=0x1;
	wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);
#endif

	page=0x31;
	offset = 0x2;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	wdatal=0x100;
	wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);

	page=0x31;
	offset = 0x10;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	wdatal=0x1;
	wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);

	page=0x0;
		offset=0x6f;
		rdata = srab_rd(page, offset);
		post_log("Page = 0x%02X - Offset = 0x%02X - Rdata = 0x%08X\n", page, offset, rdata[0]);
		rdatal=rdata[0];
		rdatah=rdata[1];
		wdatal=(rdatal | 0x1);
		wdatah=rdatah;
		srab_wr(page, offset, wdatah, wdatal);
		rdata = srab_rd(page, offset);
		post_log("Page = 0x%02X - Offset = 0x%02X - Rdata = 0x%08X\n", page, offset, rdata[0]);

		post_log("Done with the Switch Configuration \n");


   /*page=0x31;
	offset = 0x10;
		rdata = srab_rd(page, offset);
		   rdatal=rdata[0];
		   rdatah=rdata[1];
	  wdatal=(rdatal & 0x100);  // bit 8 a= Port 8

		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
	  post_log("VLAN Port 8 register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);*/


//#endif
}


//------------S/w configuration for managed mode -Nishant-------------------
uint32_t cfg_sw_mag()
{
	uint32_t page;
	uint32_t offset;
	uint32_t wdatah, wdatal;
	uint32_t rdatah, rdatal;
	uint32_t port_id;
 uint32_t *rdata;

	// write port control register
	page=0x0;
	for(port_id=1; port_id<4; port_id++)
	{
		//if(port_id==0) continue;// offset=0x0;
		if(port_id==1) offset=0x1;
		if(port_id==2) offset=0x5;
		if(port_id==3) offset=0x8;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		post_log("Port control rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		wdatal= 0;//(rdatal&0xffffffc0);
		wdatah=rdatah;
		post_log("Port control wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
		post_log("Port control rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);

	}


// port override register
	page=0xe4;
	for(port_id=1; port_id<4; port_id++)
	{
	//	if(port_id==0) continue; //offset=0x00;
		if(port_id==1) offset=0x02;
		if(port_id==2) offset=0x0a;
		if(port_id==3) offset=0x10;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		////post_log("Port override rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
		post_log("port override rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		if(port_id==3)
			wdatal=(rdatal | 0x1 | 0xf0);
		else
			wdatal=(rdatal | 0x1 | 0x70);
		wdatah=rdatah;
		post_log("port override wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		srab_wr(page, offset, wdatah, wdatal);
		//rdata = srab_rd(page, offset);
	   post_log("port override rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	}


 page=0x0;
	offset = 0x8;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x1C);  // bit 4,3,2 =1 allow unicast, multicast,broadcast
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("IMP port control register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

//Switch Mode Register
page=0x0;
	offset = 0xB;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			//wdatal=(rdatal | 0x3);  // bit 1,0 =1 Managed Mode forwarding enabled
   wdatal=(rdatal | 0x1);  // bit 1,0 =1 Managed Mode forwarding enabled
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Switch Mode register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

/*
//Switch Control Register
page=0x0;
	offset = 0x22;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x40);  // bit 6 =1 IMP Port 8 forwarding enabled
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Switch Control register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

//Port Control Register
page=0x0;
	offset = 0x1;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0xA0);  // bit 5,6,7 =0 Port 1 forwarding enabled
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Port 1 Control register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

*/


//Global ManagemetControl Register
page=0x2;
	offset = 0x0;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0xC2);  // bit 7,6 =1 Enable IMP0/1
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Global Management register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

//Broadcom Header control Register
page=0x2;
	offset = 0x3;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			//wdatal=(rdatal & 0x08);  // bit 2:0 = 0 BRCM Header disabled
   wdatal=(rdatal | 0x1);  // bit 0 = 1 BRCM Header disabled

		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("BRCM register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

/*
//Global ManagemetControl Register
page=0x2;
	offset = 0x0;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x1);  // bit 7,6 =1 Enable IMP0/1
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Global Management register wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
*/

}

//------------------------------------------------------------------


//----------------------------------
//switch configuration task
//----------------------------------
uint32_t  srab_wr(uint32_t page,  uint32_t offset, uint32_t wdatah, uint32_t wdatal)
{
	wrData(wdatah, wdatal);
	setCmdStat(page, offset, 2); // do write
	wait_for_done();            // wait done
	return 0;
}


uint32_t*   srab_rd(uint32_t page, uint32_t offset)
{
	uint32_t rdata[2],rdatah,rdatal;
	setCmdStat(page, offset, 0); // do read
	wait_for_done();            // wait done
	rdatah=RD32(ChipcommonG_SRAB_RDH);
	rdatal=RD32(ChipcommonG_SRAB_RDL);
	//post_log("Inside SRAB_RW Routine RDATAL = %04X RDATAH = %04X \n",rdatal,rdatah);

	rdata[0] = rdatal;
	rdata[1] = rdatah;

	return rdata;
}

uint32_t setCmdStat(uint32_t page , uint32_t offset , uint32_t rdwr )
{
	uint32_t data=0x0;
	data= ((page << 24) | (offset << 16) | (rdwr) | 0x1 );
	//post_log("setCmdStst = %#x\n",data);
	WR32(ChipcommonG_SRAB_CMDSTAT, data);
	return 0;

}

uint32_t  wrData(uint32_t datah,  uint32_t datal)
{
	WR32(ChipcommonG_SRAB_WDH , datah);
	WR32(ChipcommonG_SRAB_WDL , datal);
	return 0;
}

/*int rdData()
{
	rdatah=RD32(ChipcommonG_SRAB_RDH);
	rdatal=RD32(ChipcommonG_SRAB_RDL);
}*/

uint32_t wait_for_done()
{
	uint32_t data=0x1;
	while(data == 0x1 ) {
		data=RD32(ChipcommonG_SRAB_CMDSTAT);
		data=(data & 0x00000001);
		//delay_noise(1,0);

	}
	return 0;
}

void cfg_sw_gphy0_to_gphy1(void)
{
	uint32_t page;
	uint32_t offset;
	uint32_t wdatah, wdatal;
	uint32_t rdatah, rdatal;
	uint32_t port_id;
	uint32_t *rdata;

	// write port control register
	page=0x0;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x0;
		if(port_id==1) offset=0x1;
		if(port_id==2) offset=0x5;
		if(port_id==3) offset=0x8;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		post_log("Port control rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		wdatal= 0;//(rdatal&0xffffffc0);
		wdatah=rdatah;
		post_log("Port control wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		rdata = srab_rd(page, offset);
		post_log("Port control rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);

	}
	// write switch mode register
	offset=0xb;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	post_log("Switch mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	post_log("switch mode rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=((rdatal&0xfffffffe) | 0x2);
	wdatah=rdatah;
	post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	rdata = srab_rd(page, offset);
	post_log("switch mode rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// write switch control register
	offset=0x22;
	rdata = srab_rd(page, offset);
	rdatal=rdata[0];
	rdatah=rdata[1];
	post_log("Switch ctrl mode rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	post_log("switch ctrl rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
	wdatal=rdatal | ((rdatal&0xffffffff) | 0x40);
	wdatah=rdatah;
	//post_log("switch ctrl wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);
	rdata = srab_rd(page, offset);
	post_log("switch ctrl rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	// port override register
	page=0xe4;
	for(port_id=0; port_id<4; port_id++)
	{
		if(port_id==0) offset=0x00;
		if(port_id==1) offset=0x02;
		if(port_id==2) offset=0x0a;
		if(port_id==3) offset=0x10;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
		post_log("Port override rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
		post_log("port override rdatal=%#x rdatah=%#x\n",rdatal,rdatah);
		if(port_id==3)
			wdatal=(rdatal | 0x1 | 0xf0);
		else
			wdatal=(rdatal | 0x1 | 0x70);
		wdatah=rdatah;
		post_log("port override wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
		srab_wr(page, offset, wdatah, wdatal);
		srab_wr(page, offset, wdatah, wdatal);
		rdata = srab_rd(page, offset);
		post_log("port override rdata[0]=%#x rdata[1]=%#x\n",rdata[0],rdata[1]);
	}

	// BRCM Header disable
	page = 0x02;
	offset = 0x3;
	wdatal=0x0;
	wdatah=0x0;
	post_log("switch mode wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
	srab_wr(page, offset, wdatah, wdatal);

///////////Configuration for jumbo frames - Added By Nishant///////
//	page=0x40;
//	offset = 0x1;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//			wdatal=(rdatal | 0x123);
//		 wdatah=rdatah;
//	 srab_wr(page, offset, wdatah, wdatal);
//   post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
// offset = 0x5;
//		rdata = srab_rd(page, offset);
//        rdatal=rdata[0];
//        rdatah=rdata[1];
//			wdatal=(rdatal | 0x2710);
//		 wdatah=rdatah;
//	srab_wr(page, offset, wdatah, wdatal);
//
//post_log("Jumbo frame config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);
//
/////Configure RGMII Delay - Added by Nishant////
	page=0xe4;
	offset = 0x2a;
		rdata = srab_rd(page, offset);
		rdatal=rdata[0];
		rdatah=rdata[1];
			wdatal=(rdatal | 0x3);
		 wdatah=rdatah;
	 srab_wr(page, offset, wdatah, wdatal);
   post_log("Delay config wdatal=%#x wdatah=%#x\n",wdatal,wdatah);

   //Configuration By Amit
/*page=0x31;
offset = 0x2;
rdata = srab_rd(page, offset);
rdatal=rdata[0];
rdatah=rdata[1];
wdatal=0x100;
wdatah=rdatah;
srab_wr(page, offset, wdatah, wdatal);

page=0x31;
offset = 0x10;
rdata = srab_rd(page, offset);
rdatal=rdata[0];
rdatah=rdata[1];
wdatal=0x1;
wdatah=rdatah;
srab_wr(page, offset, wdatah, wdatal);*/

   page=0x31;
   offset = 0x0;
   rdata = srab_rd(page, offset);
   rdatal=rdata[0];
   rdatah=rdata[1];
   post_log("Page 0x31 offset 0x0 rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
   wdatal=0x101;
   wdatah=rdatah;
   srab_wr(page, offset, wdatah, wdatal);
   rdata = srab_rd(page, offset);
	  rdatal=rdata[0];
	  rdatah=rdata[1];
   post_log("Page 0x31 offset 0x0 rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);

   page=0x31;
   offset = 0x2;
   rdata = srab_rd(page, offset);
   rdatal=rdata[0];
   rdatah=rdata[1];
   post_log("Page 0x31 offset 0x2 rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
   wdatal=0x102;
   wdatah=rdatah;
   srab_wr(page, offset, wdatah, wdatal);
   rdata = srab_rd(page, offset);
	  rdatal=rdata[0];
	  rdatah=rdata[1];
	  post_log("Page 0x31 offset 0x2 rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);

   page=0x31;
	  offset = 0x10;
	  rdata = srab_rd(page, offset);
	  rdatal=rdata[0];
	  rdatah=rdata[1];
	  post_log("Page 0x31 offset 0x10 rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);
	  wdatal=0x1;
	  wdatah=rdatah;
	  srab_wr(page, offset, wdatah, wdatal);
	  rdata = srab_rd(page, offset);
			rdatal=rdata[0];
			rdatah=rdata[1];
			post_log("Page 0x31 offset 0x10 rdata[0]=%#x rdata[1]=%#x\n",rdatal,rdatah);

//page=0x40;
//offset = 0x1;
//rdata = srab_rd(page, offset);
//rdatal=rdata[0];
//rdatah=rdata[1];
//wdatal=0x10001FF;
//wdatah=rdatah;
//srab_wr(page, offset, wdatah, wdatal);



   //amit
   //

	page=0x0;
	offset=0x6f;
	rdata = srab_rd(page, offset);
	post_log("Page = 0x%02X - Offset = 0x%02X - Rdata = 0x%08X\n", page, offset, rdata[0]);
	rdatal=rdata[0];
	rdatah=rdata[1];
	wdatal=(rdatal | 0x1);
	wdatah=rdatah;
	srab_wr(page, offset, wdatah, wdatal);
	rdata = srab_rd(page, offset);
	post_log("Page = 0x%02X - Offset = 0x%02X - Rdata = 0x%08X\n", page, offset, rdata[0]);
}


