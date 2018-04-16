#include "dropdowndemo.h"     //ÅäÖÃ½çÃæÍ·ÎÄ¼þ
#include "DIALOG.h"           //Éú³ÉGUI
#include "led.h"              //ÅäÖÃLED
#include "beep.h"             //ÅäÖÃ·äÃùÆ÷
#include "usart.h"            //´®¿ÚÍ¨Ñ¶º¯Êý 
#include "buf_switch.h"       //ÅäÖÃ´®¿Ú·¢ËÍÊý¾Ý
#include "stmflash.h"         //µ¥Æ¬»úflashÄÚ´æ
#include "os.h"               //ÏµÍ³Í·ÎÄ¼þ
#include "delay.h"            //ÑÓÊ±Í·ÎÄ¼þ£¨µ÷ÊÔÊ±Ê¹ÓÃ£©



/*************************ËùÓÐ´úÂë±äÁ¿*******************************/
u32 Implementation_Password=0;                //ÓÃ»§ÊäÈëµÄÃÜÂë
u8 Passward_Buf[6]={1,2,3,4,5,6};             //ÐèÒªÐ´ÈëµÄÃÜÂëÊý×é(ÁùÎ»)
#define Passward_Size sizeof(Passward_Buf)    //ÃÜÂëµÄ³¤¶È£¬ÍùflashÖÐÐ´ÈëÊ¹ÓÃ
#define Flash_Save_Passward_Addr 0x08032000   //´Ó0x08000000----0x08040000ÖÐ´æ·ÅÃÜÂë
#define Amend_Parameter_Size sizeof(amend_parameter)  //¼ÇÂ¼µÆµÄ¸öÊýÒÔ¼°³¤¶È
#define Flash_Save_Amend_Parameter_Addr0 0x08038000   //´æ·ÅLED0µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr1 0x0803F000   //´æ·ÅLED1µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr2 0x08046000   //´æ·ÅLED2µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr3 0x0804C000   //´æ·ÅLED3µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr4 0x08053000   //´æ·ÅLED4µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr5 0x08059000   //´æ·ÅLED5µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr6 0x0805F000   //´æ·ÅLED6µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr7 0x08066000   //´æ·ÅLED7µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr8 0x0806C000   //´æ·ÅLED8µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr9 0x08073000   //´æ·ÅLED9µÄ¿ØÖÆµÆ
#define Flash_Save_Amend_Parameter_Addr10 0x08079000  //´æ·ÅLED10µÄ¿ØÖÆµÆ
u32 Test[6]={0};    //ÓÃÓÚ±È½ÏÃÜÂëÊ¹ÓÃ
u32 Test_Passward;  //³õÊ¼ÃÜÂë
u8 Bit=0;           //ÅÐ¶ÏÊÇµÚ¼¸Î»
u8 six;             //ÃÜÂëµÚÁùÎ»
u8 five;            //ÃÜÂëµÚÎåÎ»
u8 four;            //ÃÜÂëµÚËÄÎ»
u8 three;           //ÃÜÂëµÚÈýÎ»
u8 two;             //ÃÜÂëµÚ¶þÎ»
u8 one;             //ÃÜÂëµÚÒ»Î»
u8 Passward_i=0;    //¶ÁÈ¡ÊäÈëÃÜÂë°´Î»¸³Öµ
u8 mode=0;          //Ä£Ê½Ñ¡Ôñ
u16 response=500;   //µãµÆÏìÓ¦Ê±¼ä(ms)
u8 i=0;             //µÆµÄÖ¸Õë
u16 amend_parameter[12];    //ÓÃÓÚ±äÁ¿´æÈ¡Ê¹ÓÃ
OS_ERR err;         //ÏµÍ³Ö¸Õë
u8 data_Passward[Passward_Size];   //¶ÁÈ¡ÃÜÂëÊ±Ê¹ÓÃ
u8 amend_parameter_i1=0;  //LED0ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i2=0;  //LED1ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i3=0;  //LED2ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i4=0;  //LED3ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i5=0;  //LED4ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i6=0;  //LED5ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i7=0;  //LED6ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i8=0;  //LED7ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i9=0;  //LED8ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i10=0; //LED9Ð¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i11=0; //LED10ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
u8 amend_parameter_i12=0; //LED11ÖÐ¼ÇÂ¼ËûµÄ×´Ì¬£¨1-LED0ÁÁ£¬0-LED0Ãð£©
extern char buf_led[];      //Í¨¹ýµçÁ¦ÏßÔØ²¨·¢ËÍµÄÊý¾Ý»º³åÇø



/*************************ËùÓÐ½çÃæµÄUI*******************************/
WM_HWIN CreateBoundary_1(void);     //´´½¨µÚÒ»¸ö½çÃæ(³õÊ¼½çÃæ)
WM_HWIN CreateBoundary_2(void);     //´´½¨µÚ¶þ¸ö½çÃæ£¨LED½çÃæ£©
WM_HWIN CreateBoundary_3(void);     //´´½¨µÚÈý¸ö½çÃæ£¨flashÑ¡ÔñLED½çÃæ£©
WM_HWIN CreateBoundary_4(void);     //´´½¨µÚËÄ¸ö½çÃæ£¨flash¶ÁÈ¡LED½çÃæ£©
WM_HWIN CreateBoundary_5(void);     //´´½¨µÚÎå¸ö½çÃæ£¨ÃÜÂë½çÃæ£©
WM_HWIN CreateBoundary_6(void);     //´´½¨µÚÁù¸ö½çÃæ£¨´°ºÍ´°»§¿ØÖÆ½çÃæ£©
WM_HWIN CreateBoundary_7(void);     //´´½¨µÚÆß¸ö½çÃæ£¨°´¼üºÍµÆËæÒâÆ¥Åä½çÃæ£©
WM_HWIN CreateBoundary_8(void);     //´´½¨µÚ°Ë¸ö½çÃæ£¨ÐÞ¸Ä°´¼ü¿ØÖÆÈÎÒâµÆ½çÃæ£©
WM_HWIN CreateBoundary_9(void);     //´´½¨µÚ¾Å¸ö½çÃæ£¨½çÃæ£©



/*****************³õÊ¼½çÃæ¶Ô»°¿ò£¨×ÜÌå½çÃæ£©***************************/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)   //½çÃæÒ»Ãû³Æ
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)   //½øÈëËùÓÐµÆÁÁ½çÃæ
#define ID_BUTTON_1     (GUI_ID_USER + 0x02)   //½øÈëËùÓÐµÆÃð½çÃæ
#define ID_BUTTON_2     (GUI_ID_USER + 0x03)   //½øÈëËùÓÐµÆ·Ö¿ª¿ØÖÆ½çÃæ
#define ID_BUTTON_3     (GUI_ID_USER + 0x04)   //½øÈëÄ£ÄâµÆ¹â½çÃæ£¨´æÈëflash£©
#define ID_BUTTON_4     (GUI_ID_USER + 0x05)   //½øÈëÄ£ÄâµÆ¹â½çÃæ£¨¶ÁÈ¡flash£©
#define ID_BUTTON_5     (GUI_ID_USER + 0x06)   //ÐÞ¸ÄÃÜÂë
#define ID_BUTTON_6     (GUI_ID_USER + 0x07)   //ÊäÈëÃÜÂë
#define ID_BUTTON_47    (GUI_ID_USER + 0x5C)   //´ò¿ª»òÕß¹Ø±Õ´°ÒÔ¼°´°Á±
#define ID_BUTTON_58    (GUI_ID_USER + 0x68)   //ÐÞ¸ÄµÆµÄ¿ØÖÆ
#define ID_BUTTON_60    (GUI_ID_USER + 0x6B)   //



/*************************ËùÓÐµÆÁÁ/µÆÃð½çÃæ********************************/
#define ID_FRAMEWIN_1   (GUI_ID_USER + 0x08)   //½çÃæ¶þÃû³Æ
#define ID_BUTTON_7     (GUI_ID_USER + 0x09)   //LED0
#define ID_BUTTON_8     (GUI_ID_USER + 0x0A)   //LED1
#define ID_BUTTON_9     (GUI_ID_USER + 0x0B)   //LED2
#define ID_BUTTON_10    (GUI_ID_USER + 0x0C)   //LED3
#define ID_BUTTON_11    (GUI_ID_USER + 0x0D)   //LED4
#define ID_BUTTON_12    (GUI_ID_USER + 0x0E)   //LED5
#define ID_BUTTON_13    (GUI_ID_USER + 0x0F)   //LED6
#define ID_BUTTON_14    (GUI_ID_USER + 0x10)   //LED7
#define ID_BUTTON_15    (GUI_ID_USER + 0x11)   //LED8
#define ID_BUTTON_16    (GUI_ID_USER + 0x12)   //LED9
#define ID_BUTTON_17    (GUI_ID_USER + 0x13)   //LED10
#define ID_BUTTON_18    (GUI_ID_USER + 0x14)   //LED11
#define ID_BUTTON_19    (GUI_ID_USER + 0x15)   //LED12
#define ID_BUTTON_20    (GUI_ID_USER + 0x16)   //LED13
#define ID_BUTTON_21    (GUI_ID_USER + 0x17)   //LED14
#define ID_BUTTON_22    (GUI_ID_USER + 0x18)   //LED15
#define ID_BUTTON_23    (GUI_ID_USER + 0x19)   //LED16
#define ID_BUTTON_24    (GUI_ID_USER + 0x1A)   //LED17
#define ID_BUTTON_25    (GUI_ID_USER + 0x1B)   //LED18
#define ID_BUTTON_26    (GUI_ID_USER + 0x1C)   //LED19
#define ID_BUTTON_27    (GUI_ID_USER + 0x1D)   //LED20
#define ID_BUTTON_28    (GUI_ID_USER + 0x1E)   //LED21
#define ID_BUTTON_29    (GUI_ID_USER + 0x1F)   //LED22
#define ID_BUTTON_30    (GUI_ID_USER + 0x20)   //LED23
#define ID_BUTTON_31    (GUI_ID_USER + 0x21)   //LED24
#define ID_BUTTON_32    (GUI_ID_USER + 0x22)   //·µ»ØÖ÷½çÃæ
#define ID_BUTTON_33    (GUI_ID_USER + 0x23)   //½øÈë¶ù×ÓÉèÖÃ½çÃæ
#define ID_BUTTON_34    (GUI_ID_USER + 0x24)   //½øÈë¸¸Ç×ÉèÖÃ½çÃæ
#define ID_BUTTON_35    (GUI_ID_USER + 0x25)   //½øÈëÄ¸Ç×ÉèÖÃ½çÃæ
#define ID_BUTTON_36    (GUI_ID_USER + 0x26)   //½øÈëÏÂÒ»½çÃæ



/*************************flash  LED½çÃæ********************************/
#define ID_FRAMEWIN_2     (GUI_ID_USER + 0x27) //½çÃæÈýÃû³Æ
#define ID_CHECKBOX_0     (GUI_ID_USER + 0x28) //LED0
#define ID_CHECKBOX_1     (GUI_ID_USER + 0x29) //LED1
#define ID_CHECKBOX_2     (GUI_ID_USER + 0x3A) //LED2
#define ID_CHECKBOX_3     (GUI_ID_USER + 0x3B) //LED3
#define ID_CHECKBOX_4     (GUI_ID_USER + 0x3C) //LED4
#define ID_CHECKBOX_5     (GUI_ID_USER + 0x3D) //LED5
#define ID_CHECKBOX_6     (GUI_ID_USER + 0x3E) //LED6
#define ID_CHECKBOX_7     (GUI_ID_USER + 0x3F) //LED7
#define ID_CHECKBOX_8     (GUI_ID_USER + 0x40) //LED8
#define ID_CHECKBOX_9     (GUI_ID_USER + 0x41) //LED9
#define ID_CHECKBOX_10    (GUI_ID_USER + 0x42) //LED10
#define ID_CHECKBOX_11    (GUI_ID_USER + 0x43) //LED11
#define ID_CHECKBOX_12    (GUI_ID_USER + 0x44) //LED12
#define ID_CHECKBOX_13    (GUI_ID_USER + 0x45) //LED13
#define ID_CHECKBOX_14    (GUI_ID_USER + 0x46) //LED14
#define ID_CHECKBOX_15    (GUI_ID_USER + 0x47) //LED15
#define ID_CHECKBOX_16    (GUI_ID_USER + 0x48) //LED16
#define ID_CHECKBOX_17    (GUI_ID_USER + 0x49) //LED17
#define ID_CHECKBOX_18    (GUI_ID_USER + 0x4A) //LED18
#define ID_CHECKBOX_19    (GUI_ID_USER + 0x4B) //LED19
#define ID_CHECKBOX_20    (GUI_ID_USER + 0x4C) //LED20
#define ID_CHECKBOX_21    (GUI_ID_USER + 0x4D) //LED21
#define ID_CHECKBOX_22    (GUI_ID_USER + 0x4E) //LED22
#define ID_CHECKBOX_23    (GUI_ID_USER + 0x4F) //LED23
#define ID_CHECKBOX_24    (GUI_ID_USER + 0x50) //LED24



/*****************ÃÜÂë½âËø½ç¶Ô»°¿ò*********************************/
#define ID_FRAMEWIN_3   (GUI_ID_USER + 0x51)     //½çÃæÈýÃû³Æ
#define ID_BUTTON_37    (GUI_ID_USER + 0x52)     //Êý×Ö0
#define ID_BUTTON_38    (GUI_ID_USER + 0x53)     //Êý×Ö1
#define ID_BUTTON_39    (GUI_ID_USER + 0x54)     //Êý×Ö2
#define ID_BUTTON_40    (GUI_ID_USER + 0x55)     //Êý×Ö3
#define ID_BUTTON_41    (GUI_ID_USER + 0x56)     //Êý×Ö4
#define ID_BUTTON_42    (GUI_ID_USER + 0x57)     //Êý×Ö5
#define ID_BUTTON_43    (GUI_ID_USER + 0x58)     //Êý×Ö6
#define ID_BUTTON_44    (GUI_ID_USER + 0x59)     //Êý×Ö7
#define ID_BUTTON_45    (GUI_ID_USER + 0x5A)     //Êý×Ö8
#define ID_BUTTON_46    (GUI_ID_USER + 0x5B)     //Êý×Ö9
#define ID_BUTTON_32    (GUI_ID_USER + 0x22)     //·µ»ØÖ÷½çÃæ
#define ID_BUTTON_36    (GUI_ID_USER + 0x26)     //½øÈëÏÂÒ»½çÃæ



/*****************´°»§¹Ø±Õ´ò¿ª½çÃæ*********************************/
#define ID_FRAMEWIN_4   (GUI_ID_USER + 0x5D)     //½çÃæËÄÃû³Æ
#define ID_BUTTON_48    (GUI_ID_USER + 0x5E)     //´ò¿ª´°»§10%
#define ID_BUTTON_49    (GUI_ID_USER + 0x5F)     //´ò¿ª´°»§30%
#define ID_BUTTON_50    (GUI_ID_USER + 0x60)     //´ò¿ª´°»§50%
#define ID_BUTTON_51    (GUI_ID_USER + 0x61)     //´ò¿ª´°»§70%
#define ID_BUTTON_52    (GUI_ID_USER + 0x62)     //´ò¿ª´°»§100%
#define ID_BUTTON_53    (GUI_ID_USER + 0x63)     //¹Ø±Õ´°»§10%
#define ID_BUTTON_54    (GUI_ID_USER + 0x64)     //¹Ø±Õ´°»§30%
#define ID_BUTTON_55    (GUI_ID_USER + 0x65)     //¹Ø±Õ´°»§50%
#define ID_BUTTON_56    (GUI_ID_USER + 0x66)     //¹Ø±Õ´°»§70%
#define ID_BUTTON_57    (GUI_ID_USER + 0x67)     //¹Ø±Õ´°»§100%



/**************************ÐÞ¸Ä¿ØÖÆ°´¼üÄÜ²Ù¿ØËæ»úµÄµÆ*************************************************/
#define ID_FRAMEWIN_5   (GUI_ID_USER + 0x69)     //½çÃæÎåÃû³Æ
#define ID_BUTTON_59    (GUI_ID_USER + 0x6A)     //´æ´¢Êý¾Ý



/***********************************BUTTONÈ«¾ÖÐÞ¸Ä*************************************************/
#define ID_FRAMEWIN_6   (GUI_ID_USER + 0x6C)     //½çÃæÎåÃû³Æ
#define ID_BUTTON_61    (GUI_ID_USER + 0x6D)     //´æ´¢Êý¾Ý
#define ID_BUTTON_62    (GUI_ID_USER + 0x6F)     //´æ´¢Êý¾Ý
#define ID_BUTTON_63    (GUI_ID_USER + 0x70)     //´æ´¢Êý¾Ý
#define ID_BUTTON_64    (GUI_ID_USER + 0x71)     //´æ´¢Êý¾Ý
#define ID_BUTTON_65    (GUI_ID_USER + 0x72)     //´æ´¢Êý¾Ý
#define ID_BUTTON_66    (GUI_ID_USER + 0x73)     //´æ´¢Êý¾Ý
#define ID_BUTTON_67    (GUI_ID_USER + 0x74)     //´æ´¢Êý¾Ý
#define ID_BUTTON_68    (GUI_ID_USER + 0x75)     //´æ´¢Êý¾Ý
#define ID_BUTTON_69    (GUI_ID_USER + 0x76)     //´æ´¢Êý¾Ý
#define ID_BUTTON_70    (GUI_ID_USER + 0x77)     //´æ´¢Êý¾Ý
#define ID_BUTTON_71    (GUI_ID_USER + 0x78)     //´æ´¢Êý¾Ý
#define ID_BUTTON_72    (GUI_ID_USER + 0x79)     //´æ´¢Êý¾Ý
#define ID_BUTTON_73    (GUI_ID_USER + 0x7A)     //´æ´¢Êý¾Ý
#define ID_BUTTON_74    (GUI_ID_USER + 0x7B)     //´æ´¢Êý¾Ý
#define ID_BUTTON_75    (GUI_ID_USER + 0x7C)     //´æ´¢Êý¾Ý



/****************************³õÊ¼»¯½çÃæ£¨×ÜÌå½çÃæ£©***********************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_1[] = {
  { FRAMEWIN_CreateIndirect, "Boundary_1",  ID_FRAMEWIN_0,    0, 0, 480, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED_Bright",  ID_BUTTON_0,      0, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED_Destroy", ID_BUTTON_1,      90, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "User-Part",   ID_BUTTON_2,      180, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "Set_Scene",   ID_BUTTON_3,      270, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "Read_Scene",  ID_BUTTON_4,      360, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "passward_x",  ID_BUTTON_5,      0, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "passward_s",  ID_BUTTON_6,      90, 50, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "control",     ID_BUTTON_47,     180, 50, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "amend_parameter", ID_BUTTON_58, 270, 50, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "menory",      ID_BUTTON_60,     360, 50, 80, 40, 0, 0x0, 0 },
};

/****************************ËùÓÐµÆÁÁ/µÆÃð/¶ÀÁ¢¿ØÖÆ½çÃæ*********************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_2[] = {
  { FRAMEWIN_CreateIndirect, "Boundary_2",  ID_FRAMEWIN_1, 0, 0, 480, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED0",        ID_BUTTON_7,   0, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED1",        ID_BUTTON_8,   90, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED2",        ID_BUTTON_9,   180, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED3",        ID_BUTTON_10,  270, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED4",        ID_BUTTON_11,  360, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED5",        ID_BUTTON_12,  0, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED6",        ID_BUTTON_13,  90, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED7",        ID_BUTTON_14,  180, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED8",        ID_BUTTON_15,  270, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED9",        ID_BUTTON_16,  360, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED10",       ID_BUTTON_17,  0, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED11",       ID_BUTTON_18,  90, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED12",       ID_BUTTON_19,  180, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED13",       ID_BUTTON_20,  270, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED14",       ID_BUTTON_21,  360, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED15",       ID_BUTTON_22,  0, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED16",       ID_BUTTON_23,  90, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED17",       ID_BUTTON_24,  180, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED18",       ID_BUTTON_25,  270, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED19",       ID_BUTTON_26,  360, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED20",       ID_BUTTON_27,  0, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED21",       ID_BUTTON_28,  90, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED22",       ID_BUTTON_29,  180, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED23",       ID_BUTTON_30,  270, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "LED24",       ID_BUTTON_31,  360, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "return",      ID_BUTTON_32,  0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "son",         ID_BUTTON_33,  90, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "father",      ID_BUTTON_34,  180, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "mather",      ID_BUTTON_35,  270, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "next",        ID_BUTTON_36,  360, 250, 80, 40, 0, 0x0, 0 },
};



/****************************ËùÓÐµÆÁÁ/µÆÃð/¶ÀÁ¢¿ØÖÆ½çÃæ*********************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_3[] = {
  { FRAMEWIN_CreateIndirect, "Boundary_3", ID_FRAMEWIN_2,  0, 0, 480, 320, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED0",       ID_CHECKBOX_0,  0, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED1",       ID_CHECKBOX_1,  90, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED2",       ID_CHECKBOX_2,  180, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED3",       ID_CHECKBOX_3,  270, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED4",       ID_CHECKBOX_4,  360, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED5",       ID_CHECKBOX_5,  0, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED6",       ID_CHECKBOX_6,  90, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED7",       ID_CHECKBOX_7,  180, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED8",       ID_CHECKBOX_8,  270, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED9",       ID_CHECKBOX_9,  360, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED10",      ID_CHECKBOX_10, 0, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED11",      ID_CHECKBOX_11, 90, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED12",      ID_CHECKBOX_12, 180, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED13",      ID_CHECKBOX_13, 270, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED14",      ID_CHECKBOX_14, 360, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED15",      ID_CHECKBOX_15, 0, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED16",      ID_CHECKBOX_16, 90, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED17",      ID_CHECKBOX_17, 180, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED18",      ID_CHECKBOX_18, 270, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED19",      ID_CHECKBOX_19, 360, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED20",      ID_CHECKBOX_20, 0, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED21",      ID_CHECKBOX_21, 90, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED22",      ID_CHECKBOX_22, 180, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED23",      ID_CHECKBOX_23, 270, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED24",      ID_CHECKBOX_24, 360, 200, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "return",     ID_BUTTON_32,   0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "son",        ID_BUTTON_33,   90, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "father",     ID_BUTTON_34,   180, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "mather",     ID_BUTTON_35,   270, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "next",       ID_BUTTON_36,   360, 250, 80, 40, 0, 0x0, 0 },
};



/**************************ÃÜÂë½âËø½çÃæ*************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_4[] = {
	{ FRAMEWIN_CreateIndirect,  "The password interface", ID_FRAMEWIN_3,  0, 0, 480, 320, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,    "0",                      ID_BUTTON_37,   160, 240, 120, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "1",                      ID_BUTTON_38,   0, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "2",                      ID_BUTTON_39,   160, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "3",                      ID_BUTTON_40,   320, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "4",                      ID_BUTTON_41,   0, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "5",                      ID_BUTTON_42,   160, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "6",                      ID_BUTTON_43,   320, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "7",                      ID_BUTTON_44,   0, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "8",                      ID_BUTTON_45,   160, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "9",                      ID_BUTTON_46,   320, 170, 150, 60, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,    "return",                 ID_BUTTON_32,   0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "next",                   ID_BUTTON_36,   360, 250, 80, 40, 0, 0x0, 0 },
};



/**************************´°ºÍ´°»§µÄ¿ØÖÆ½çÃæ*************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_6[] = {
  { FRAMEWIN_CreateIndirect, "Control",  ID_FRAMEWIN_4, 0, 0, 480, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "O 10%",    ID_BUTTON_48,  0, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "O 30%",    ID_BUTTON_49,  90, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "O 50%",    ID_BUTTON_50,  180, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "O 70%",    ID_BUTTON_51,  270, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "O 100%",   ID_BUTTON_52,  360, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "C 10%",    ID_BUTTON_53,  0, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "C 30%",    ID_BUTTON_54,  90, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "C 50%",    ID_BUTTON_55,  180, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "C 70%",    ID_BUTTON_56,  270, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "C 100%",   ID_BUTTON_57,  360, 50, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "return",   ID_BUTTON_32,  0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "son",      ID_BUTTON_33,  90, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "father",   ID_BUTTON_34,  180, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "mather",   ID_BUTTON_35,  270, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "next",     ID_BUTTON_36,  360, 250, 80, 40, 0, 0x0, 0 },
};



/**************************Ñ¡ÔñÄ³¸öBUTTONÐèÒª¿ØÖÆµÄËæ»úµÆ****************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_7[] = {
  { FRAMEWIN_CreateIndirect, "amend_parameter", ID_FRAMEWIN_5,  0, 0, 480, 320, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED0",            ID_CHECKBOX_0,  0, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED1",            ID_CHECKBOX_1,  90, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED2",            ID_CHECKBOX_2,  180, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED3",            ID_CHECKBOX_3,  270, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED4",            ID_CHECKBOX_4,  360, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED5",            ID_CHECKBOX_5,  0, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED6",            ID_CHECKBOX_6,  90, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED7",            ID_CHECKBOX_7,  180, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED8",            ID_CHECKBOX_8,  270, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED9",            ID_CHECKBOX_9,  360, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED10",           ID_CHECKBOX_10, 0, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED11",           ID_CHECKBOX_11, 90, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED12",           ID_CHECKBOX_12, 180, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED13",           ID_CHECKBOX_13, 270, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED14",           ID_CHECKBOX_14, 360, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED15",           ID_CHECKBOX_15, 0, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED16",           ID_CHECKBOX_16, 90, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED17",           ID_CHECKBOX_17, 180, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED18",           ID_CHECKBOX_18, 270, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED19",           ID_CHECKBOX_19, 360, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED20",           ID_CHECKBOX_20, 0, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED21",           ID_CHECKBOX_21, 90, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED22",           ID_CHECKBOX_22, 180, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED23",           ID_CHECKBOX_23, 270, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED24",           ID_CHECKBOX_24, 360, 200, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "return",          ID_BUTTON_32,   0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory",          ID_BUTTON_59,   180, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "next",            ID_BUTTON_36,   360, 250, 80, 40, 0, 0x0, 0 },
};



/**************************ÐÞ¸Ä¿ØÖÆ°´¼üÄÜ²Ù¿ØËæ»úµÄµÆ*************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_8[] = {
  { FRAMEWIN_CreateIndirect, "Boundary_1",  ID_FRAMEWIN_6,     0, 0, 480, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory0",     ID_BUTTON_61,      0, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory1",     ID_BUTTON_62,      90, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory2",     ID_BUTTON_63,      180, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory3",     ID_BUTTON_64,      270, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory4",     ID_BUTTON_65,      360, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory5",     ID_BUTTON_66,      0, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory6",     ID_BUTTON_67,      90, 50, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "memory7",     ID_BUTTON_68,      180, 50, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "memory8",     ID_BUTTON_69,      270, 50, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "memory9",     ID_BUTTON_70,      360, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory10",    ID_BUTTON_71,      0, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory11",    ID_BUTTON_72,      90, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory12",    ID_BUTTON_73,      180, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory13",    ID_BUTTON_74,      270, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "memory14",    ID_BUTTON_75,      360, 100, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect,   "return",          ID_BUTTON_32,   0, 250, 80, 40, 0, 0x0, 0 },
 // { BUTTON_CreateIndirect,   "memory",          ID_BUTTON_59,   180, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "next",            ID_BUTTON_36,   360, 250, 80, 40, 0, 0x0, 0 },
};



/************************************½çÃæ³õÊ¼»¯ÅäÖÃ***********************************************/
static void _cbDialog_1(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_47);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_58);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_60);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
    break;
		
		GUI_DispString("Hello world!");
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // ËùÓÐµÆÁÁ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:     //°´ÏÂÊ±ËÉ¿ª´¥ÃþÆÁÓÐÐ§
				printf("%c\r\n",buf_led[0]);
// 				GUI_EndDialog(pMsg->hWin, 0);
//			  CreateBoundary_2();

        break;

      }
      break;
    case ID_BUTTON_1: // ËùÓÐµÆÃð
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				printf("%c\r\n",buf_led[1]);
//				GUI_EndDialog(pMsg->hWin, 0);
//			  CreateBoundary_2();
        break;

      }
      break;
    case ID_BUTTON_2: // ËùÓÐµÆ¶ÀÁ¢¿ØÖÆ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
				mode=1;
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_2();
        break;

      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'Set_Scene'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break; 
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_3();
        break;

      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Read_Scene'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_3();
        break;

      }
      break;
    case ID_BUTTON_5: // Notifications sent by 'passward_x'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					STMFLASH_Write(Flash_Save_Passward_Addr,(u16*)Passward_Buf,Passward_Size);//½«ÐÞ¸ÄºÃµÄÃÜÂë´æÈëflash£¬Ö®ºóÊ¹ÓÃ
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_6: // Notifications sent by 'passward_s'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_4();
        break;

      }
      break;
    case ID_BUTTON_47: // Notifications sent by 'control'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_6();
        break;

      }
      break;
			case ID_BUTTON_58: // ËùÓÐµÆÁÁ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:     //°´ÏÂÊ±ËÉ¿ª´¥ÃþÆÁÓÐÐ§
				  amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_7();

        break;

      }
      break;
			case ID_BUTTON_60: // 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:     //°´ÏÂÊ±ËÉ¿ª´¥ÃþÆÁÓÐÐ§
				
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_8();        
        break;

      }
      break;
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/************************************ËùÓÐµÆÁÁ/µÆÃð/¶ÀÁ¢¿ØÖÆ½çÃæ**************************************/
static void _cbDialog_2(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;


  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_32);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_33);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_34);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_35);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_36);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_7: // Notifications sent by 'LED0'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
					STMFLASH_Read(Flash_Save_Amend_Parameter_Addr0,(u16*)amend_parameter,Amend_Parameter_Size);
            if(mode==1)
					{
					printf("%c\r\n",buf_led[2]);
					}
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
					
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
						OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					}
//					if(amend_parameter[11]=1)
//					{
//						printf("%c\r\n",buf_led[2]);
//					}
        break;

      }
      break;
    case ID_BUTTON_8: // Notifications sent by 'LED1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr1,(u16*)amend_parameter,Amend_Parameter_Size);
           if(mode==1)
					{
					printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_9: // Notifications sent by 'LED2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr2,(u16*)amend_parameter,Amend_Parameter_Size);
             if(mode==1)
					{
					printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;
 
      }
      break;
    case ID_BUTTON_10: // Notifications sent by 'LED3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr3,(u16*)amend_parameter,Amend_Parameter_Size);
            if(mode==1)
					{
					printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_11: // Notifications sent by 'LED4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr4,(u16*)amend_parameter,Amend_Parameter_Size);
           if(mode==1)
					{
					printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_12: // Notifications sent by 'LED5'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr5,(u16*)amend_parameter,Amend_Parameter_Size);
          if(mode==1)
					{
					printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_13: // Notifications sent by 'LED6'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr6,(u16*)amend_parameter,Amend_Parameter_Size);
           if(mode==1)
					{
					printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_14: // Notifications sent by 'LED7'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr7,(u16*)amend_parameter,Amend_Parameter_Size);
          	if(mode==1)
					{
					printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_15: // Notifications sent by 'LED8'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr8,(u16*)amend_parameter,Amend_Parameter_Size);
           if(mode==1)
					{
					printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_16: // Notifications sent by 'LED9'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr9,(u16*)amend_parameter,Amend_Parameter_Size);
           if(mode==1)
					{
					printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_17: // Notifications sent by 'LED10'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr10,(u16*)amend_parameter,Amend_Parameter_Size);
          if(mode==1)
					{
					printf("%c\r\n",buf_led[12]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[0]==1)
					{
						printf("%c\r\n",buf_led[2]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[1]==1)
					{
						printf("%c\r\n",buf_led[3]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[2]==1)
					{
						printf("%c\r\n",buf_led[4]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[3]==1)
					{
						printf("%c\r\n",buf_led[5]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[4]==1)
					{
						printf("%c\r\n",buf_led[6]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[5]==1)
					{
						printf("%c\r\n",buf_led[7]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[6]==1)
					{
						printf("%c\r\n",buf_led[8]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[7]==1)
					{
						printf("%c\r\n",buf_led[9]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[8]==1)
					{
						printf("%c\r\n",buf_led[10]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[9]==1)
					{
						printf("%c\r\n",buf_led[11]);
					}
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					if(amend_parameter[10]==1)
					{
						printf("%c\r\n",buf_led[12]);
					}
        break;

      }
      break;
    case ID_BUTTON_18: // Notifications sent by 'LED11'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;

        break;

      }
      break;
    case ID_BUTTON_19: // Notifications sent by 'LED12'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_20: // Notifications sent by 'LED13'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
 
      }
      break;
    case ID_BUTTON_21: // Notifications sent by 'LED14'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
				switch (pMsg->MsgId) {
      case WM_INIT_DIALOG:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
        BUTTON_SetFont(hItem, GUI_FONT_20B_1);

        break;

      }
		}
      break;
    case ID_BUTTON_22: // Notifications sent by 'LED15'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_23: // Notifications sent by 'LED16'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_24: // Notifications sent by 'LED17'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_25: // Notifications sent by 'LED18'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_26: // Notifications sent by 'LED19'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_27: // Notifications sent by 'LED20'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_28: // Notifications sent by 'LED21'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_29: // Notifications sent by 'LED22'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_30: // Notifications sent by 'LED23'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_31: // Notifications sent by 'LED24'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_32: // Notifications sent by 'return'       //·µ»Ø
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
        mode=0;
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_1();
       break;

        break;

      }
      break;
    case ID_BUTTON_33: // Notifications sent by 'son'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
			case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_3();
        break;

      }
      break;
    case ID_BUTTON_34: // Notifications sent by 'father'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
			    GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;

      }
      break;
    case ID_BUTTON_35: // Notifications sent by 'mather'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;
      case WM_NOTIFICATION_RELEASED:
 
        break;

      }
      break;
    case ID_BUTTON_36: // Notifications sent by 'next'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;
      case WM_NOTIFICATION_RELEASED:
          
        break;

      }
      break;

    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/***************************flash  LED½çÃæ*********************************************************/
static void _cbDialog_3(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;


  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "LED0");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);//ºìÉ«×ÖÌå

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
    CHECKBOX_SetText(hItem, "LED1");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
    CHECKBOX_SetText(hItem, "LED2");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
    CHECKBOX_SetText(hItem, "LED3");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4);
    CHECKBOX_SetText(hItem, "LED4");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5);
    CHECKBOX_SetText(hItem, "LED5");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6);
    CHECKBOX_SetText(hItem, "LED6");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7);
    CHECKBOX_SetText(hItem, "LED7");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8);
    CHECKBOX_SetText(hItem, "LED8");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_9);
    CHECKBOX_SetText(hItem, "LED9");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_10);
    CHECKBOX_SetText(hItem, "LED10");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_11);
    CHECKBOX_SetText(hItem, "LED11");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_12);
    CHECKBOX_SetText(hItem, "LED12");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_13);
    CHECKBOX_SetText(hItem, "LED13");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_14);
    CHECKBOX_SetText(hItem, "LED14");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_15);
    CHECKBOX_SetText(hItem, "LED15");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_16);
    CHECKBOX_SetText(hItem, "LED16");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_17);
    CHECKBOX_SetText(hItem, "LED17");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_18);
    CHECKBOX_SetText(hItem, "LED18");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_19);
    CHECKBOX_SetText(hItem, "LED19");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_20);
    CHECKBOX_SetText(hItem, "LED20");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_21);
    CHECKBOX_SetText(hItem, "LED21");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_22);
    CHECKBOX_SetText(hItem, "LED22");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_23);
    CHECKBOX_SetText(hItem, "LED23");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_24);
    CHECKBOX_SetText(hItem, "LED24");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_32);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_33);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_34);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_35);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_36);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_CHECKBOX_0: // Notifications sent by 'LED0'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					printf("%c\r\n",buf_led[2]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_1: // Notifications sent by 'LED1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[3]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_2: // Notifications sent by 'LED2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[4]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
         
        break;

      }
      break;
    case ID_CHECKBOX_3: // Notifications sent by 'LED3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[5]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_4: // Notifications sent by 'LED4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[6]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_5: // Notifications sent by 'LED5'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[7]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_6: // Notifications sent by 'LED6'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[8]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
 
        break;

      }
      break;
    case ID_CHECKBOX_7: // Notifications sent by 'LED7'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[9]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_8: // Notifications sent by 'LED8'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[10]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_9: // Notifications sent by 'LED9'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          printf("%c\r\n",buf_led[11]);
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_10: // Notifications sent by 'LED10'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
 
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
				
        break;

      }
      break;
    case ID_CHECKBOX_11: // Notifications sent by 'LED11'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_12: // Notifications sent by 'LED12'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
 
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_13: // Notifications sent by 'LED13'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_14: // Notifications sent by 'LED14'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_15: // Notifications sent by 'LED15'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_16: // Notifications sent by 'LED16'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_17: // Notifications sent by 'LED17'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_18: // Notifications sent by 'LED18'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_19: // Notifications sent by 'LED19'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_20: // Notifications sent by 'LED20'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_21: // Notifications sent by 'LED21'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_22: // Notifications sent by 'LED22'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_23: // Notifications sent by 'LED23'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_24: // Notifications sent by 'LED24'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_BUTTON_32: // Notifications sent by 'return'       //·µ»Ø
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
        mode=0;
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_1();
			
       break;
      }
      break;
    case ID_BUTTON_33: // Notifications sent by 'son'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;
			case WM_NOTIFICATION_RELEASED:
				
        break;
      }
      break;
    case ID_BUTTON_34: // Notifications sent by 'father'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
					GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_35: // Notifications sent by 'mather'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_36: // Notifications sent by 'next'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_4();
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
			break;
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/************************************ÃÜÂë½çÃæÅäÖÃ**************************************/
static void _cbDialog_4(WM_MESSAGE * pMsg) {
	WM_HWIN hItem_2;
	WM_HWIN hItem;
  int NCode;
  int Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_32);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_36);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    break;
  case WM_NOTIFY_PARENT:
		//ÖØÐÂ´´½¨±êÌâÀ¸×ÖÌå£¬×ÖÌå´óÐ¡
		hItem_2 = pMsg->hWin;
	  FRAMEWIN_SetText(hItem_2, "Password Unlock Interface");
	  FRAMEWIN_SetFont(hItem_2, GUI_FONT_16_ASCII);
		FRAMEWIN_SetTextAlign(hItem_2, GUI_TA_HCENTER | GUI_TA_VCENTER);	
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
			case ID_BUTTON_37: // Notifications sent by '0'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
//				for(Bit=1;Bit<7;Bit++)
//			{
//				Passward_Buf[i]=0;
//			}
				Bit++;
			if(Bit==1)
			{
				six=0;
				Passward_Buf[5]=0;
			}
			if(Bit==2)
			{
				five=0;
				Passward_Buf[4]=0;
			}
			if(Bit==3)
			{
				four=0;
				Passward_Buf[3]=0;
			}
			if(Bit==4)
			{
				three=0;
				Passward_Buf[2]=0;
			}
			if(Bit==5)
			{
				two=0;
				Passward_Buf[1]=0;
			}
			if(Bit==6)
			{
				one=0;
				Passward_Buf[0]=0;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_38: // Notifications sent by '1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=1;
				Passward_Buf[5]=1;
			}
			if(Bit==2)
			{
				five=1;
				Passward_Buf[4]=1;
			}
			if(Bit==3)
			{
				four=1;
				Passward_Buf[3]=1;
			}
			if(Bit==4)
			{
				three=1;
				Passward_Buf[2]=1;
			}
			if(Bit==5)
			{
				two=1;
				Passward_Buf[1]=1;
			}
			if(Bit==6)
			{
				one=1;
				Passward_Buf[0]=1;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_39: // Notifications sent by '2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=2;
				Passward_Buf[5]=2;
			}
			if(Bit==2)
			{
				five=2;
				Passward_Buf[4]=2;
			}
			if(Bit==3)
			{
				four=2;
				Passward_Buf[3]=2;
			}
			if(Bit==4)
			{
				three=2;
				Passward_Buf[2]=2;
			}
			if(Bit==5)
			{
				two=2;
				Passward_Buf[1]=2;
			}
			if(Bit==6)
			{
				one=2;
				Passward_Buf[0]=2;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_40: // Notifications sent by '3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=3;
				Passward_Buf[5]=3;
			}
			if(Bit==2)
			{
				five=3;
				Passward_Buf[4]=3;
			}
			if(Bit==3)
			{
				four=3;
				Passward_Buf[3]=3;
			}
			if(Bit==4)
			{
				three=3;
				Passward_Buf[2]=3;
			}
			if(Bit==5)
			{
				two=3;
				Passward_Buf[1]=3;
			}
			if(Bit==6)
			{
				one=3;
				Passward_Buf[0]=3;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_41: // Notifications sent by '4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=4;
				Passward_Buf[5]=4;
			}
			if(Bit==2)
			{
				five=4;
				Passward_Buf[4]=4;
			}
			if(Bit==3)
			{
				four=4;
				Passward_Buf[3]=4;
			}
			if(Bit==4)
			{
				three=4;
				Passward_Buf[2]=4;
			}
			if(Bit==5)
			{
				two=4;
				Passward_Buf[1]=4;
			}
			if(Bit==6)
			{
				one=4;
				Passward_Buf[0]=4;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_42: // Notifications sent by '5'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=5;
				Passward_Buf[5]=5;
			}
			if(Bit==2)
			{
				five=5;
				Passward_Buf[4]=5;
			}
			if(Bit==3)
			{
				four=5;
				Passward_Buf[3]=5;
			}
			if(Bit==4)
			{
				three=5;
				Passward_Buf[2]=5;
			}
			if(Bit==5)
			{
				two=5;
				Passward_Buf[1]=5;
			}
			if(Bit==6)
			{
				one=5;
				Passward_Buf[0]=5;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_43: // Notifications sent by '6'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=6;
				Passward_Buf[5]=6;
			}
			if(Bit==2)
			{
				five=6;
				Passward_Buf[4]=6;
			}
			if(Bit==3)
			{
				four=6;
				Passward_Buf[3]=6;
			}
			if(Bit==4)
			{
				three=6;
				Passward_Buf[2]=6;
			}
			if(Bit==5)
			{
				two=6;
				Passward_Buf[1]=6;
			}
			if(Bit==6)
			{
				one=6;
				Passward_Buf[0]=6;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_44: // Notifications sent by '7'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=7;
				Passward_Buf[5]=7;
			}
			if(Bit==2)
			{
				five=7;
				Passward_Buf[4]=7;
			}
			if(Bit==3)
			{
				four=7;
				Passward_Buf[3]=7;
			}
			if(Bit==4)
			{
				three=7;
				Passward_Buf[2]=7;
			}
			if(Bit==5)
			{
				two=7;
				Passward_Buf[1]=7;
			}
			if(Bit==6)
			{
				one=7;
				Passward_Buf[0]=7;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_45: // Notifications sent by '8'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=8;
				Passward_Buf[5]=8;
			}
			if(Bit==2)
			{
				five=8;
				Passward_Buf[4]=8;
			}
			if(Bit==3)
			{
				four=8;
				Passward_Buf[3]=8;
			}
			if(Bit==4)
			{
				three=8;
				Passward_Buf[2]=8;
			}
			if(Bit==5)
			{
				two=8;
				Passward_Buf[1]=8;
			}
			if(Bit==6)
			{
				one=8;
				Passward_Buf[0]=8;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_46: // Notifications sent by '9'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=9;
				Passward_Buf[5]=9;
			}
			if(Bit==2)
			{
				five=9;
				Passward_Buf[4]=9;
			}
			if(Bit==3)
			{
				four=9;
				Passward_Buf[3]=9;
			}
			if(Bit==4)
			{
				three=9;
				Passward_Buf[2]=9;
			}
			if(Bit==5)
			{
				two=9;
				Passward_Buf[1]=9;
			}
			if(Bit==6)
			{
				one=9;
				Passward_Buf[0]=9;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_32: // ·µ»Ø
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				
        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_1();
        break;
      }
      break;
    case ID_BUTTON_36: // È·ÈÏ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				  STMFLASH_Read(Flash_Save_Passward_Addr,(u16*)data_Passward,Passward_Size);
			for(Passward_i=0;Passward_i<6;Passward_i++)
			{
				 Test[Passward_i]=data_Passward[Passward_i];
			}
			    Test_Passward=Test[5]*100000+Test[4]*10000+Test[3]*1000+Test[2]*100+Test[1]*10+Test[0];		
//			    printf("%d\r\n",Test[0]);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",Test[1]);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",Test[2]);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",Test[3]);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",Test[4]);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",Test[5]);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
			
					Implementation_Password=six*100000+five*10000+four*1000+three*100+two*10+one;
//					printf("%d\r\n",six);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",five);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",four);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",three);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",two);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//			    printf("%d\r\n",one);
//			OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					
			  if(Test_Passward==Implementation_Password)
				{
					printf("%c\r\n",buf_led[13]);
//					printf("correct\r\n");
				}
//				else
//					printf("error\r\n");
//				printf("%d\r\n",Implementation_Password);
        break;
      case WM_NOTIFICATION_RELEASED:
				Implementation_Password=0;
			   Bit=0;
         six=0;
         five=0;
         four=0;
         three=0;
         two=0;
         one=0;	
        break;
      }
      break;
    }
    break;
  default:
	
    WM_DefaultProc(pMsg);
    break;
  }
}



/************************************ËùÓÐµÆÁÁ/µÆÃð/¶ÀÁ¢¿ØÖÆ½çÃæ**************************************/
static void _cbDialog_6(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;


  switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_32);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_33);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_34);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_35);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_36);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    break;
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_48: // 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
				
					printf("%c\r\n",buf_led[14]);
			
        break;

      }
      break;
    case ID_BUTTON_49: //
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
        
					printf("%c\r\n",buf_led[15]);

        break;

      }
      break;
    case ID_BUTTON_50: //
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:

					printf("%c\r\n",buf_led[16]);
				
        break;
 
      }
      break;
    case ID_BUTTON_51: // 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
         
					printf("%c\r\n",buf_led[17]);

        break;

      }
      break;
    case ID_BUTTON_52: // 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
       
					printf("%c\r\n",buf_led[18]);

        break;

      }
      break;
    case ID_BUTTON_53: // 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
         
					printf("%c\r\n",buf_led[19]);
			
        break;

      }
      break;
    case ID_BUTTON_54: //
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
        
					printf("%c\r\n",buf_led[20]);
				
        break;

      }
      break;
    case ID_BUTTON_55: //
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				
        break;
      case WM_NOTIFICATION_RELEASED:
       
					printf("%c\r\n",buf_led[21]);
				
        break;

      }
      break;
    case ID_BUTTON_56: //
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
       
					printf("%c\r\n",buf_led[22]);
			
        break;

      }
      break;
    case ID_BUTTON_57: // 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					
        break;
      case WM_NOTIFICATION_RELEASED:
        
					printf("%c\r\n",buf_led[23]);
			
        break;

      }
      break;
			
			    case ID_BUTTON_32: // Notifications sent by 'return'       //·µ»Ø
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
        mode=0;
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_1();
       break;

        break;

      }
      break;
    case ID_BUTTON_33: // Notifications sent by 'son'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
			case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_3();
        break;

      }
      break;
    case ID_BUTTON_34: // Notifications sent by 'father'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
			    GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;

      }
      break;
    case ID_BUTTON_35: // Notifications sent by 'mather'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;
      case WM_NOTIFICATION_RELEASED:
 
        break;

      }
      break;
    case ID_BUTTON_36: // Notifications sent by 'next'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          GUI_EndDialog(pMsg->hWin, 0);
			    CreateBoundary_3();
        break;
      case WM_NOTIFICATION_RELEASED:
          
        break;

      }
      break;
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/************************Ñ¡ÔñÄ³¸öBUTTONÐèÒª¿ØÖÆµÄËæ»úµÆ************************************/
static void _cbDialog_7(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;


  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "LED0");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);//ºìÉ«×ÖÌå

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
    CHECKBOX_SetText(hItem, "LED1");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
    CHECKBOX_SetText(hItem, "LED2");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
    CHECKBOX_SetText(hItem, "LED3");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4);
    CHECKBOX_SetText(hItem, "LED4");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5);
    CHECKBOX_SetText(hItem, "LED5");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6);
    CHECKBOX_SetText(hItem, "LED6");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7);
    CHECKBOX_SetText(hItem, "LED7");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8);
    CHECKBOX_SetText(hItem, "LED8");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_9);
    CHECKBOX_SetText(hItem, "LED9");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_10);
    CHECKBOX_SetText(hItem, "LED10");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_11);
    CHECKBOX_SetText(hItem, "LED11");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_12);
    CHECKBOX_SetText(hItem, "LED12");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_13);
    CHECKBOX_SetText(hItem, "LED13");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_14);
    CHECKBOX_SetText(hItem, "LED14");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_15);
    CHECKBOX_SetText(hItem, "LED15");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_16);
    CHECKBOX_SetText(hItem, "LED16");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_17);
    CHECKBOX_SetText(hItem, "LED17");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_18);
    CHECKBOX_SetText(hItem, "LED18");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_19);
    CHECKBOX_SetText(hItem, "LED19");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_20);
    CHECKBOX_SetText(hItem, "LED20");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_21);
    CHECKBOX_SetText(hItem, "LED21");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_22);
    CHECKBOX_SetText(hItem, "LED22");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_23);
    CHECKBOX_SetText(hItem, "LED23");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_24);
    CHECKBOX_SetText(hItem, "LED24");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);

	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_32);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_33);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_34);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_35);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_36);
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_CHECKBOX_0: // Notifications sent by 'LED0'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[0]=1;
			   amend_parameter_i1++;
			   if(amend_parameter_i1>=2)
				 {
					  amend_parameter[0]=0;
						amend_parameter_i1=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_1: // Notifications sent by 'LED1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				
        break;
      case WM_NOTIFICATION_RELEASED:
           amend_parameter[1]=1;
			   amend_parameter_i2++;
			   if(amend_parameter_i2>=2)
				 {
					  amend_parameter[1]=0;
						amend_parameter_i2=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_2: // Notifications sent by 'LED2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				 
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[2]=1;
				 amend_parameter_i3++;
			   if(amend_parameter_i3>=2)
				 {
					  amend_parameter[2]=0;
						amend_parameter_i3=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
         
        break;

      }
      break;
    case ID_CHECKBOX_3: // Notifications sent by 'LED3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[3]=1;
			   amend_parameter_i4++;
			   if(amend_parameter_i4>=2)
				 {
					  amend_parameter[3]=0;
						amend_parameter_i4=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_4: // Notifications sent by 'LED4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[4]=1;
			   amend_parameter_i5++;
			   if(amend_parameter_i5>=2)
				 {
					  amend_parameter[4]=0;
						amend_parameter_i5=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_5: // Notifications sent by 'LED5'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[5]=1;
			   amend_parameter_i6++;
			   if(amend_parameter_i6>=2)
				 {
					  amend_parameter[5]=0;
						amend_parameter_i6=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_6: // Notifications sent by 'LED6'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[6]=1;
			   amend_parameter_i7++;
			   if(amend_parameter_i7>=2)
				 {
					  amend_parameter[6]=0;
						amend_parameter_i7=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
 
        break;

      }
      break;
    case ID_CHECKBOX_7: // Notifications sent by 'LED7'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[7]=1;
         amend_parameter_i8++;
			   if(amend_parameter_i8>=2)
				 {
					  amend_parameter[7]=0;
						amend_parameter_i8=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_8: // Notifications sent by 'LED8'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[8]=1;
			   amend_parameter_i9++;
			   if(amend_parameter_i9>=2)
				 {
					  amend_parameter[8]=0;
						amend_parameter_i9=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_9: // Notifications sent by 'LED9'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[9]=1;
			   amend_parameter_i10++;
			   if(amend_parameter_i10>=2)
				 {
					  amend_parameter[9]=0;
						amend_parameter_i10=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_10: // Notifications sent by 'LED10'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[10]=1;
			   amend_parameter_i11++;
			   if(amend_parameter_i11>=2)
				 {
					  amend_parameter[10]=0;
						amend_parameter_i11=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
				
        break;

      }
      break;
    case ID_CHECKBOX_11: // Notifications sent by 'LED11'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          amend_parameter[11]=1;
			   amend_parameter_i12++;
			   if(amend_parameter_i12>=2)
				 {
					  amend_parameter[11]=0;
						amend_parameter_i12=0;
				 }
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_12: // Notifications sent by 'LED12'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
 
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_13: // Notifications sent by 'LED13'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_14: // Notifications sent by 'LED14'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_15: // Notifications sent by 'LED15'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_16: // Notifications sent by 'LED16'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_17: // Notifications sent by 'LED17'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_18: // Notifications sent by 'LED18'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_19: // Notifications sent by 'LED19'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_20: // Notifications sent by 'LED20'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_21: // Notifications sent by 'LED21'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_22: // Notifications sent by 'LED22'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_23: // Notifications sent by 'LED23'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_CHECKBOX_24: // Notifications sent by 'LED24'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;
    case ID_BUTTON_32: // Notifications sent by 'return'       //·µ»Ø
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
        mode=0;
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_1();
			
       break;
      }
      break;
    case ID_BUTTON_59: // Notifications sent by 'father'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr0,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr1,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr2,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr3,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr4,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr5,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr6,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr7,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr8,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr9,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					STMFLASH_Write(Flash_Save_Amend_Parameter_Addr10,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
//					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
//					amend_parameter[0]=0;
//					amend_parameter[1]=0;
//					amend_parameter[2]=0;
//					amend_parameter[3]=0;
//					amend_parameter[4]=0;
//					amend_parameter[5]=0;
//					amend_parameter[6]=0;
//					amend_parameter[7]=0;
//					amend_parameter[8]=0;
//					amend_parameter[9]=0;
//					amend_parameter[10]=0;
//					amend_parameter[11]=0;
			
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
			
    case ID_BUTTON_36: // Notifications sent by 'next'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				 

////				  GUI_EndDialog(pMsg->hWin, 0);
////			    CreateBoundary_4();
				  
        break;
      case WM_NOTIFICATION_RELEASED:
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr0,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr1,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr2,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr3,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr4,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr5,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr6,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr7,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr8,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr9,(u16*)amend_parameter,Amend_Parameter_Size);
//				STMFLASH_Read(Flash_Save_Amend_Parameter_Addr10,(u16*)amend_parameter,Amend_Parameter_Size);
//			  printf("%d\r\n",amend_parameter[0]);
//			  printf("%d\r\n",amend_parameter[1]);
//			  printf("%d\r\n",amend_parameter[2]);
//			  printf("%d\r\n",amend_parameter[3]);
//			  printf("%d\r\n",amend_parameter[4]);
//			  printf("%d\r\n",amend_parameter[5]);
//			  printf("%d\r\n",amend_parameter[6]); 
//        printf("%d\r\n",amend_parameter[7]);
//			  printf("%d\r\n",amend_parameter[8]);
//			  printf("%d\r\n",amend_parameter[9]);
//			  printf("%d\r\n",amend_parameter[10]);
//			  printf("%d\r\n",amend_parameter[11]); 				
        break;

      }
			break;
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/***************************ÐÞ¸Ä¿ØÖÆ°´¼üÄÜ²Ù¿ØËæ»úµÄµÆ****************************************/
static void _cbDialog_8(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_61);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_62);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_63);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_64);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_65);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_66);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_67);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_68);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_69);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_70);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_71);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_72);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_73);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_74);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_75);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
		
    break;
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_61: // ËùÓÐµÆÁÁ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				
        break;
      case WM_NOTIFICATION_RELEASED:     //°´ÏÂÊ±ËÉ¿ª´¥ÃþÆÁÓÐÐ§
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr0,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;
      }
      break;
    case ID_BUTTON_62: // ËùÓÐµÆÃð
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr1,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;
      }
      break;
    case ID_BUTTON_63: // ËùÓÐµÆ¶ÀÁ¢¿ØÖÆ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr2,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;

      }
      break;
    case ID_BUTTON_64: // Notifications sent by 'Set_Scene'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break; 
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr3,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;

      }
      break;
    case ID_BUTTON_65: // Notifications sent by 'Read_Scene'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr4,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;

      }
      break;
    case ID_BUTTON_66: // Notifications sent by 'passward_x'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
	
        break;
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr5,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;

      }
      break;
    case ID_BUTTON_67: // Notifications sent by 'passward_s'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
          
        break;
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr6,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;

      }
      break;
    case ID_BUTTON_68: // Notifications sent by 'control'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
         
        break;
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr7,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;

      }
      break;
			case ID_BUTTON_69: // ËùÓÐµÆÁÁ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:     //°´ÏÂÊ±ËÉ¿ª´¥ÃþÆÁÓÐÐ§
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr8,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;

      }
      break;
			case ID_BUTTON_70: // ËùÓÐµÆÁÁ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:     //°´ÏÂÊ±ËÉ¿ª´¥ÃþÆÁÓÐÐ§
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr9,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;
      }
      break;
    case ID_BUTTON_71: // ËùÓÐµÆÃð
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
          STMFLASH_Write(Flash_Save_Amend_Parameter_Addr10,(u16*)amend_parameter,Amend_Parameter_Size);//BUTTON0¿ØÖÆµÆµÄÕµÊý´¢´æ£¬Ö®ºóÊ¹ÓÃ
					OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±200ms
					amend_parameter[0]=0;
					amend_parameter[1]=0;
					amend_parameter[2]=0;
					amend_parameter[3]=0;
					amend_parameter[4]=0;
					amend_parameter[5]=0;
					amend_parameter[6]=0;
					amend_parameter[7]=0;
					amend_parameter[8]=0;
					amend_parameter[9]=0;
					amend_parameter[10]=0;
					amend_parameter[11]=0;
        break;
      }
      break;
    case ID_BUTTON_72: // ËùÓÐµÆ¶ÀÁ¢¿ØÖÆ
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_73: // Notifications sent by 'Set_Scene'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break; 
      case WM_NOTIFICATION_RELEASED:
          
        break;

      }
      break;
    case ID_BUTTON_74: // Notifications sent by 'Read_Scene'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_75: // Notifications sent by 'passward_x'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
			
		    case ID_BUTTON_32: // Notifications sent by 'return'       //·µ»Ø
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:
        mode=0;
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_1();
			
       break;
      }
      break;
    case ID_BUTTON_59: // Notifications sent by 'father'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:

			
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
			
    case ID_BUTTON_36: // Notifications sent by 'next'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				 


				  
        break;
      case WM_NOTIFICATION_RELEASED:
	
        break;

      }
			break;
      break;
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/**********************************´´½¨³õÊ¼»¯½çÃæ************************************/
WM_HWIN CreateBoundary_1(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);
  return hWin;
}

/**************************´´½¨ËùÓÐµÆÁÁ/µÆÃð/¶ÀÁ¢¿Ú¿ØÖÆ½çÃæ*************************/
WM_HWIN CreateBoundary_2(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_2, GUI_COUNTOF(_aDialogCreate_2), _cbDialog_2, WM_HBKWIN, 0, 0);
  return hWin;
}

WM_HWIN CreateBoundary_3(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_3, GUI_COUNTOF(_aDialogCreate_3), _cbDialog_3, WM_HBKWIN, 0, 0);
  return hWin;
}

/********************************´´½¨ÃÜÂëÊäÈë½çÃæ***********************************/
WM_HWIN CreateBoundary_4(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_4, GUI_COUNTOF(_aDialogCreate_4), _cbDialog_4, WM_HBKWIN, 0, 0); //´´½¨´°¿Ú
  return hWin;
}


///*********************´´½¨ÃÜÂëÐÞ¸Ä½çÃæ*************************/
//WM_HWIN CreateBoundary_5(void) {
//  WM_HWIN hWin;

//  hWin = GUI_CreateDialogBox(_aDialogCreate_5, GUI_COUNTOF(_aDialogCreate_5), _cbDialog_5, WM_HBKWIN, 0, 0); //´´½¨´°¿Ú
//  return hWin;
//}

/****************************´°ºÍ´°»§¿ØÖÆ******************************************/
WM_HWIN CreateBoundary_6(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_6, GUI_COUNTOF(_aDialogCreate_6), _cbDialog_6, WM_HBKWIN, 0, 0); //´´½¨´°¿Ú
  return hWin;
}

/********************ÐÞ¸Ä¿ØÖÆ°´¼üÄÜ²Ù¿ØËæ»úµÄµÆ***********************************/
WM_HWIN CreateBoundary_7(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_7, GUI_COUNTOF(_aDialogCreate_7), _cbDialog_7, WM_HBKWIN, 0, 0); //´´½¨´°¿Ú
  return hWin;
}

WM_HWIN CreateBoundary_8(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_8, GUI_COUNTOF(_aDialogCreate_8), _cbDialog_8, WM_HBKWIN, 0, 0); //´´½¨´°¿Ú
  return hWin;
}

/*************************´´½¨³õÊ¼»¯¶Ô»°¿ò***************************************/
void dropdown_demo(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);//´´½¨´°¿Ú
	while(1)
	{
		GUI_Delay(100);
	}
}


