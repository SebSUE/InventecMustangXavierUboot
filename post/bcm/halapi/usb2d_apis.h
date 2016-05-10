#ifndef USBD_APIS_H
#define USBD_APIS_H

#define CYG   1

#define DEV_EP_IN_OFFSET 0
#define DEV_EP_OUT_OFFSET 16
#define DEV_IMPLEMENTED_OUT_EPS 4
#define DEV_IMPLEMENTED_IN_EPS 4

#define USB2D_ENDPNT_OUT_BUFSIZE_0 0x1804c20c
#define USB2D_ENDPNT_OUT_BUFSIZE_1 0x1804c22c
#define USB2D_ENDPNT_OUT_BUFSIZE_2 0x1804c24c
#define USB2D_ENDPNT_OUT_BUFSIZE_3 0x1804c26c
#define USB2D_ENDPNT_OUT_BUFSIZE_4 0x1804c28c

#define USB2D_ENDPNT_IN_MAXPACKSIZE_0 0x1804c00c
#define USB2D_ENDPNT_IN_MAXPACKSIZE_1 0x1804c02c
#define USB2D_ENDPNT_IN_MAXPACKSIZE_2 0x1804c04c
#define USB2D_ENDPNT_IN_MAXPACKSIZE_3 0x1804c06c
#define USB2D_ENDPNT_IN_MAXPACKSIZE_4 0x1804c08c

#define USB2D_ENDPNT_OUT_CTRL_0 0x1804c200
#define USB2D_ENDPNT_OUT_CTRL_1 0x1804c220
#define USB2D_ENDPNT_OUT_CTRL_2 0x1804c240
#define USB2D_ENDPNT_OUT_CTRL_3 0x1804c260
#define USB2D_ENDPNT_OUT_CTRL_4 0x1804c280
#define USB2D_ENDPNT_OUT_CTRL_5 0x1804c2a0
#define USB2D_ENDPNT_OUT_CTRL_6 0x1804c2c0
#define USB2D_ENDPNT_OUT_CTRL_7 0x1804c2e0
#define USB2D_ENDPNT_OUT_CTRL_8 0x1804c300
#define USB2D_ENDPNT_OUT_CTRL_9 0x1804c320

#define USB2D_ENDPNT_IN_CTRL_0 0x1804c000
#define USB2D_ENDPNT_IN_CTRL_1 0x1804c020
#define USB2D_ENDPNT_IN_CTRL_1__ET_R 4
#define USB2D_ENDPNT_IN_CTRL_2 0x1804c040
#define USB2D_ENDPNT_IN_CTRL_3 0x1804c060
#define USB2D_ENDPNT_IN_CTRL_4 0x1804c080
#define USB2D_ENDPNT_IN_CTRL_5 0x1804c0a0
#define USB2D_ENDPNT_IN_CTRL_6 0x1804c0c0
#define USB2D_ENDPNT_IN_CTRL_7 0x1804c0e0
#define USB2D_ENDPNT_IN_CTRL_8 0x1804c100
#define USB2D_ENDPNT_IN_CTRL_9 0x1804c120

#define USB2D_ENDPNT_IN_BUFFER_0 0x1804c008
#define USB2D_ENDPNT_IN_BUFFER_1 0x1804c028
#define USB2D_ENDPNT_IN_BUFFER_2 0x1804c048
#define USB2D_ENDPNT_IN_BUFFER_3 0x1804c068
#define USB2D_ENDPNT_IN_BUFFER_4 0x1804c088

#define USB2D_UDC20_ENDPOINT0 0x1804c504
#define USB2D_UDC20_ENDPOINT0__MPS_R 19
#define USB2D_UDC20_ENDPOINT0__ED 4
#define USB2D_UDC20_ENDPOINT0__CNW_R 7
#define USB2D_UDC20_ENDPOINT0__ET_R 5
#define USB2D_UDC20_ENDPOINT0__LEN_R 0

#define USB2D_UDC20_ENDPOINT1 0x1804c508
#define USB2D_UDC20_ENDPOINT2 0x1804c50c
#define USB2D_UDC20_ENDPOINT3 0x1804c510
#define USB2D_UDC20_ENDPOINT4 0x1804c514
#define USB2D_UDC20_ENDPOINT5 0x1804c518
#define USB2D_UDC20_ENDPOINT6 0x1804c51c

#define USB2D_ENDPNT_IN_DATADESCR_0 0x1804c014
#define USB2D_ENDPNT_IN_DATADESCR_1 0x1804c034
#define USB2D_ENDPNT_IN_DATADESCR_2 0x1804c054
#define USB2D_ENDPNT_IN_DATADESCR_3 0x1804c074
#define USB2D_ENDPNT_IN_DATADESCR_4 0x1804c094
#define USB2D_ENDPNT_IN_DATADESCR_5 0x1804c0b4
#define USB2D_ENDPNT_IN_DATADESCR_6 0x1804c0d4
#define USB2D_ENDPNT_IN_DATADESCR_7 0x1804c0f4
#define USB2D_ENDPNT_IN_DATADESCR_8 0x1804c114
#define USB2D_ENDPNT_IN_DATADESCR_9 0x1804c134

#define USB2D_ENDPNT_OUT_DATADESCR_0 0x1804c214
#define USB2D_ENDPNT_OUT_DATADESCR_1 0x1804c234
#define USB2D_ENDPNT_OUT_DATADESCR_2 0x1804c254
#define USB2D_ENDPNT_OUT_DATADESCR_3 0x1804c274
#define USB2D_ENDPNT_OUT_DATADESCR_4 0x1804c294
#define USB2D_ENDPNT_OUT_DATADESCR_5 0x1804c2b4
#define USB2D_ENDPNT_OUT_DATADESCR_6 0x1804c2d4
#define USB2D_ENDPNT_OUT_DATADESCR_7 0x1804c2f4
#define USB2D_ENDPNT_OUT_DATADESCR_8 0x1804c314
#define USB2D_ENDPNT_OUT_DATADESCR_9 0x1804c334
#define USB2D_ENDPNT_OUT_SETUPBUF_0 0x1804c210

#define USB2D_DEVCONFIG 0x1804c400
#define USB2D_DEVCONFIG__SET_DESC 18
#define USB2D_DEVCONFIG__DIR 6
#define USB2D_DEVCONFIG__PI 5
#define USB2D_DEVCONFIG__SP 3

#define USB2D_DEVCTRL 0x1804c404
#define USB2D_DEVCTRL__MODE 9
#define USB2D_DEVCTRL__BRLEN_R 16
#define USB2D_DEVCTRL__THLEN_R 24
#define USB2D_DEVCTRL__BF 6
#define USB2D_DEVCTRL__BREN 8
#define USB2D_DEVCTRL__TDE 3
#define USB2D_DEVCTRL__RDE 2

#define USB2D_DEVINTRMASK 0x1804c410
#define USB2D_ENDPNTINTRMASK 0x1804c418

#define CDRU_USBPHY_CLK_RST_SEL 0x0301d1b4
#define CDRU_SPARE_REG_0 0x0301d238

#define CRMU_USB_PHY_AON_CTRL 0x0301c028
#define CRMU_USB_PHY_AON_CTRL__CRMU_USBPHY_P0_AFE_CORERDY_VDDC 1
#define CDRU_USBPHY_P0_STATUS 0x0301d1d0
#define CDRU_USBPHY_P0_STATUS__USBPHY_ILDO_ON_FLAG 1

#define USB2_IDM_IDM_IO_CONTROL_DIRECT 0x18115408
#define USB2_IDM_IDM_IO_CONTROL_DIRECT__clk_enable 0
#define USB2_IDM_IDM_RESET_CONTROL 0x18115800
#define USB2_IDM_IDM_RESET_CONTROL__RESET 0

#define USBH_Phy_Ctrl_P0 0x18049200

#define USBH_HCCAPBASE 0x18048000
#define USBH_HCCAPBASE__HCIVERSION_R 16

#define USBH_USBSTS 0x18048014
#define USBH_USBSTS__HCHalted 12
#define USBH_USBSTS__Port_Change_Detect 2

#define USBH_USBCMD 0x18048010
#define USBH_CONFIGFLAG 0x18048050

#define USBH_PORTSC_0 0x18048054
#define USBH_PORTSC_0__Port_Power_PP 12
#define USBH_PORTSC_0__Port_Reset 8
#define USBH_PORTSC_0__Port_Enabled_Disabled 2
#define USBH_PORTSC_0__Current_Connect_Status 0

#define CRMU_USB_PHY_AON_CTRL__CRMU_USBPHY_P1_AFE_CORERDY_VDDC 9

#define CDRU_USBPHY_P1_STATUS 0x0301d1e8
#define CDRU_USBPHY_P1_STATUS__USBPHY_ILDO_ON_FLAG 1

#define USBH_Phy_Ctrl_P1 0x18049204

#define USBH_PORTSC_1 0x18048058
#define USBH_PORTSC_1__Port_Power_PP 12
#define USBH_PORTSC_1__Port_Reset 8
#define USBH_PORTSC_1__Port_Enabled_Disabled 2
#define USBH_PORTSC_1__Current_Connect_Status 0

#define CRMU_USB_PHY_AON_CTRL__CRMU_USBPHY_P2_AFE_CORERDY_VDDC 17

#define CDRU_USBPHY_P2_STATUS 0x0301d200
#define CDRU_USBPHY_P2_STATUS__USBPHY_ILDO_ON_FLAG 1
#define CDRU_USBPHY_P2_STATUS__USBPHY_PLL_LOCK 0

#define USB2D_IDM_IDM_IO_CONTROL_DIRECT 0x18111408
#define USB2D_IDM_IDM_IO_CONTROL_DIRECT__clk_enable 0
#define USB2D_IDM_IDM_RESET_CONTROL 0x18111800
#define USB2D_IDM_IDM_RESET_CONTROL__RESET 0

#define CDRU_USBPHY2_HOST_DEV_SEL 0x0301d1b8
#define CDRU_USBPHY2_HOST_DEV_SEL__PHY2_DEV_HOST_CTRL_SEL 0

#define USB2D_DEVINTR 0x1804c40c
#define USB2D_DEVINTR__UR 3
#define USB2D_DEVINTR__SOF 5

#define USBH_ASYNCLISTADDR 0x18048028
#define USBH_USBSTS__Asynchronous_Schedule_Status 15

#define USB2D_ENDPNT_IN_CTRL_0__CNAK 8
#define USB2D_ENDPNT_IN_CTRL_0__P 3
#define USBH_USBSTS__USB_Interrupt_USBINT 0

#define USBH_Phy_Ctrl_P2 0x18049208
#define USBH_PORTSC_2 0x1804805c
#define USBH_PORTSC_2__Port_Power_PP 12
#define USBH_PORTSC_2__Port_Reset 8
#define USBH_PORTSC_2__Port_Enabled_Disabled 2
#define USBH_PORTSC_2__Current_Connect_Status 0

typedef enum
{
	TEST_PASS     = 0,
	TEST_FAIL     = 1,
	TEST_ERROR    = 2,
	TEST_NOT_INIT = 3
} test_status_t;

typedef enum usbdIntrType {
  DEVINTR,
  EPINTR
} usbdIntrType_t;

typedef enum usbdTestType {
  BULK,
  ISO,
  INTR
} usbdTestType_t;


test_status_t init_usbd(uint32_t argc, uint32_t *argv[]);
uint32_t get_intr_detail_and_clear(uint32_t epIntr);
uint32_t fillEpDataDescReg(uint32_t addr, uint32_t regIndex);
uint32_t setEpControlReg(uint32_t regIndex, uint32_t regData);
uint32_t getEpControlReg(uint32_t regIndex);
uint32_t setEpDetails(uint32_t epNum, uint32_t epdir, uint32_t epType, uint32_t epMpkt);
uint32_t getEpControlRegAddr(uint32_t epNum, uint32_t epDir);
uint32_t getEpBufOrMpktSizeRegAddr(uint32_t epNum, uint32_t epDir);


extern uint32_t *descAddr, *dataAddr;

typedef enum testType
{
  BASIC_BULK_OUT,
  BASIC_INTR_OUT,
  BASIC_ISO_OUT,
  BASIC_BULK_IN,
  BASIC_INTR_IN,
  BASIC_ISO_IN,
  SUSP_RESUME,
  HEAVY_BULK_OUT,       //random size ranging to 60 k
  HEAVY_BULK_IN,        //random size ranging to 60 k
  HEAVY_BULK_OUT_IN     //random size ranging to 60 k 
} testType_t;

typedef struct testInfo {
  testType_t TT;
  uint32_t trSize;
  uint32_t bufferFill;
  uint32_t bulkNumItr;
  uint32_t bulkCurItr;
  uint32_t done;
  uint32_t usbdDescMem; //expect 512K
  uint32_t usbdDescDataMem; //expect 512K
  uint32_t usbdXferMem; //expect 64M
  usbdTestType_t testType;
} testInfo_s;

extern testInfo_s devTestInfo;

typedef struct devTest {
  uint32_t config;
  uint32_t junk; 
} devTest_s;

typedef enum usb_dev_state {DEV_UNCONNECTED,DEV_DEFAULT ,DEV_SUSPENDED, DEV_ENABLED, DEV_ADDRESSED, DEV_CONFIGURED } usb_dev_state_t;

typedef enum bulkTest { BULK_INACTIVE, BULK_ACTIVE } bulkTest_t;
typedef enum intrTest { INTR_INACTIVE, INTR_ACTIVE } intrTest_t;
typedef enum  isoTest {  ISO_INACTIVE,  ISO_ACTIVE }  isoTest_t;

typedef enum transferDir { OUT, IN, BI } transferDir_t;

typedef struct bulkTestInfo {
   bulkTest_t bulkTest;
   transferDir_t dir;
   uint32_t size; //in bytes
   uint32_t done;
   uint32_t EpNum;
   uint32_t EpDt;
} bulkTestInfo_t;

typedef struct intrTestInfo {
   intrTest_t intrTest;
   transferDir_t dir;
   uint32_t size; //in bytes
   uint32_t done;
   uint32_t EpNum;
   uint32_t EpDt;
   uint32_t qH; //qH Address
   uint32_t FrmNum; //scheduled frame index
} intrTestInfo_t;

typedef struct  isoTestInfo {
   isoTest_t  isoTest;
   transferDir_t dir;
   uint32_t size; //in bytes
   uint32_t done;
   uint32_t EpNum;
   uint32_t EpDt;
   uint32_t iTD; //iTD Address
   uint32_t FrmNum; //scheduled frame index
}  isoTestInfo_t;

typedef struct device_info  {
   usb_dev_state_t devState;
   uint32_t devConfig;
   uint32_t devSpeed;

   uint32_t onlyEnumeration;
   uint32_t suspendResume;

   bulkTestInfo_t bulkTestInfo;
   intrTestInfo_t intrTestInfo;
    isoTestInfo_t  isoTestInfo;

} usb_device_info_t;   

extern usb_device_info_t usbDevInfo;

/////////////////////////////////////////////////////
//   DEVICE RELATED DATA or DESCRIPTOR STRUCTURES  //
/////////////////////////////////////////////////////
//DEVICE EP CONFIGURATION STRUCTURE
typedef struct epConfig {
  uint32_t epType; //0-Control, 1-iso, 2-bulk, 3-intr
  uint32_t epMaxPktSize;
} dEpConfig_s;


/////////////////////////////////////////////////////
//   DEVICE SETUP DATA and STATUS DESCRIPTOR       //
/////////////////////////////////////////////////////

typedef struct dSetupStatusQ {
	uint32_t BS       :  2;
	uint32_t RxSts    :  2;
	uint32_t CfgSts   : 12;
	uint32_t Reserved : 16;
} dSetupStatusQ_s; 

typedef union dSetupStatusQ_u {
  dSetupStatusQ_s dSS;
  uint32_t dSS_val;
}dSetupStatusQ_t; 


typedef struct usbSetupDataMem {
  dSetupStatusQ_t DSS;
	uint32_t res_0;
	uint32_t SetupDw0;
	uint32_t SetupDw1;
} usbSetupDataMem_t; 

#define __packed __attribute__((packed))
/////////////////////////////////////////////////////
// DEVICE IN or OUT EP DATA and STATUS DESCRIPTOR  //
/////////////////////////////////////////////////////

typedef  struct dPayload {   //Access this for ISO transfers
  uint16_t Size     : 13;
  uint16_t PID      :  2;
} __packed dPayload_s;

typedef  union dPayload_u {
  dPayload_s isoSize;
  uint16_t   Size;
}__packed dPayload_t;

typedef  struct dDataStatusQ {
	dPayload_t PayloadSize    ;  // Access 16 bit for non ISO
	uint32_t FrmNum   : 11;
	uint32_t L        :  1;
	uint32_t TsfrSts  :  2;
	uint32_t BS       :  2;
}__packed dDataStatusQ_s; 

typedef  union dDataStatusQ_u {
  dDataStatusQ_s dDS;
  uint32_t dDS_val;
}__packed dDataStatusQ_t; 


typedef struct usbDevDataMem {
  dDataStatusQ_t DDS;
	uint32_t res_0;
	uint32_t BufferP;
	uint32_t NextDs;
} usbDevDataMem_t; 

/////////////////////////////////////////////////////
//       USB DEVICE SETUP DATA STRUCTURE           //
/////////////////////////////////////////////////////

typedef struct usbSetupCmd {
  uint32_t bmRequestType : 8 ;
  uint32_t bRequest      : 8 ;
  uint32_t wValue        : 16 ;
  uint32_t wIndex        : 16 ;
  uint32_t wLength       : 16 ;

} usbSetupCmd_t;

extern usbSetupCmd_t *setupBuffer;
#endif //USBD_APIS_H
