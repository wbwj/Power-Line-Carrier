#include "dropdowndemo.h"
#include "DIALOG.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "buf_switch.h"
#include "stmflash.h"
#include "os.h"
#include "delay.h"

u32 Implementation_Password=0;//用户输入的密码
u8 Passward_Buf[6]={1,2,3,4,5,3};  //需要写入的密码数组(六位)
#define Passward_Size sizeof(Passward_Buf)   //密码的长度，往flash中写入使用
#define Flash_Save_Passward_Addr 0x08070000  //从0x08000000----0x08070000中存放密码

u32 Test[6]={0};  //用于比较密码使用
u32 Test_Passward=123456;//初始密码
u8 Bit=0;//判断是第几位
u8 six;//密码第六位
u8 five;//密码第五位
u8 four;//密码第四位
u8 three;//密码第三位
u8 two;//密码第二位
u8 one;//密码第一位
u8 Passward_i=0; //读取输入密码按位赋值
u8 mode=0;    //模式选择
u16 response=400; //点灯响应时间(ms)
u8 i=0;  //灯的指针

OS_ERR err;//系统指针

extern char buf_led[]; //通过电力线载波发送的数据缓冲区

u8 data_Passward[Passward_Size]; //读取密码时使用

WM_HWIN CreateBoundary_1(void);     //创建第一个界面(初始界面)
WM_HWIN CreateBoundary_2(void);     //创建第二个界面（LED界面）
WM_HWIN CreateBoundary_3(void);     //创建第三个界面（flash选择LED界面）
WM_HWIN CreateBoundary_4(void);     //创建第四个界面（flash读取LED界面）
WM_HWIN CreateBoundary_5(void);     //创建第五个界面（密码界面）
WM_HWIN CreateBoundary_6(void);     //创建第六个界面（面膜界面）
WM_HWIN CreateBoundary_7(void);     //创建第七个界面（）
WM_HWIN CreateBoundary_8(void);     //创建第八个界面
WM_HWIN CreateBoundary_9(void);     //创建第九个界面


/*****************初始界面对话框（总体界面）***************************/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)   //界面一名称
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)   //进入所有灯亮界面
#define ID_BUTTON_1     (GUI_ID_USER + 0x02)   //进入所有灯灭界面
#define ID_BUTTON_2     (GUI_ID_USER + 0x03)   //进入所有灯分开控制界面
#define ID_BUTTON_3     (GUI_ID_USER + 0x04)   //进入模拟灯光界面（存入flash）
#define ID_BUTTON_4     (GUI_ID_USER + 0x05)   //进入模拟灯光界面（读取flash）
#define ID_BUTTON_5     (GUI_ID_USER + 0x06)   //修改密码
#define ID_BUTTON_6     (GUI_ID_USER + 0x07)   //输入密码

/*************************所有灯亮/灯灭界面********************************/
#define ID_FRAMEWIN_1   (GUI_ID_USER + 0x08)   //界面二名称
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
#define ID_BUTTON_32    (GUI_ID_USER + 0x22)   //返回主界面
#define ID_BUTTON_33    (GUI_ID_USER + 0x23)   //进入儿子设置界面
#define ID_BUTTON_34    (GUI_ID_USER + 0x24)   //进入父亲设置界面
#define ID_BUTTON_35    (GUI_ID_USER + 0x25)   //进入母亲设置界面
#define ID_BUTTON_36    (GUI_ID_USER + 0x26)   //进入下一界面

/*************************flash  LED界面********************************/
#define ID_FRAMEWIN_2     (GUI_ID_USER + 0x27) //界面三名称
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

/*****************密码解锁界对话框*********************************/
#define ID_FRAMEWIN_3   (GUI_ID_USER + 0x51)     //界面三名称
#define ID_BUTTON_37    (GUI_ID_USER + 0x52)     //数字0
#define ID_BUTTON_38    (GUI_ID_USER + 0x53)     //数字1
#define ID_BUTTON_39    (GUI_ID_USER + 0x54)     //数字2
#define ID_BUTTON_40    (GUI_ID_USER + 0x55)     //数字3
#define ID_BUTTON_41    (GUI_ID_USER + 0x56)     //数字4
#define ID_BUTTON_42    (GUI_ID_USER + 0x57)     //数字5
#define ID_BUTTON_43    (GUI_ID_USER + 0x58)     //数字6
#define ID_BUTTON_44    (GUI_ID_USER + 0x59)     //数字7
#define ID_BUTTON_45    (GUI_ID_USER + 0x5A)     //数字8
#define ID_BUTTON_46    (GUI_ID_USER + 0x5B)     //数字9
#define ID_BUTTON_32    (GUI_ID_USER + 0x22)   //返回主界面
#define ID_BUTTON_36    (GUI_ID_USER + 0x26)   //进入下一界面



/****************************初始化界面（总体界面）***********************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_1[] = {
  { FRAMEWIN_CreateIndirect, "Boundary_1", ID_FRAMEWIN_0, 0, 0, 480, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED_Bright", ID_BUTTON_0, 0, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED_Destroy", ID_BUTTON_1, 90, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "User-Part", ID_BUTTON_2, 180, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Set_Scene", ID_BUTTON_3, 270, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Read_Scene", ID_BUTTON_4, 360, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "passward_x", ID_BUTTON_5, 0, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "passward_s", ID_BUTTON_6, 90, 50, 80, 40, 0, 0x0, 0 },
};

/****************************所有灯亮/灯灭/独立控制界面*********************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_2[] = {
  { FRAMEWIN_CreateIndirect, "Boundary_2", ID_FRAMEWIN_1, 0, 0, 480, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED0",   ID_BUTTON_7, 0, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED1",   ID_BUTTON_8, 90, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED2",   ID_BUTTON_9, 180, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED3",   ID_BUTTON_10, 270, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED4",   ID_BUTTON_11, 360, 0, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED5",   ID_BUTTON_12, 0, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED6",   ID_BUTTON_13, 90, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED7",   ID_BUTTON_14, 180, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED8",   ID_BUTTON_15, 270, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED9",   ID_BUTTON_16, 360, 50, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED10",  ID_BUTTON_17, 0, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED11",  ID_BUTTON_18, 90, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED12",  ID_BUTTON_19, 180, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED13",  ID_BUTTON_20, 270, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED14",  ID_BUTTON_21, 360, 100, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED15",  ID_BUTTON_22, 0, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED16",  ID_BUTTON_23, 90, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED17",  ID_BUTTON_24, 180, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED18",  ID_BUTTON_25, 270, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED19",  ID_BUTTON_26, 360, 150, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED20",  ID_BUTTON_27, 0, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED21",  ID_BUTTON_28, 90, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED22",  ID_BUTTON_29, 180, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED23",  ID_BUTTON_30, 270, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED24",  ID_BUTTON_31, 360, 200, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "return", ID_BUTTON_32, 0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "son",    ID_BUTTON_33, 93, 252, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "father", ID_BUTTON_34, 180, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "mather", ID_BUTTON_35, 270, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "next",   ID_BUTTON_36, 360, 250, 80, 40, 0, 0x0, 0 },
};

/****************************所有灯亮/灯灭/独立控制界面*********************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_3[] = {
  { FRAMEWIN_CreateIndirect, "Boundary_3", ID_FRAMEWIN_0, 0, 0, 480, 320, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED0", ID_CHECKBOX_0, 0, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED1", ID_CHECKBOX_1, 90, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED2", ID_CHECKBOX_2, 180, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED3", ID_CHECKBOX_3, 270, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED4", ID_CHECKBOX_4, 360, 0, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED5", ID_CHECKBOX_5, 0, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED6", ID_CHECKBOX_6, 90, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED7", ID_CHECKBOX_7, 180, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED8", ID_CHECKBOX_8, 270, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED9", ID_CHECKBOX_9, 360, 50, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED10", ID_CHECKBOX_10, 0, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED11", ID_CHECKBOX_11, 90, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED12", ID_CHECKBOX_12, 180, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED13", ID_CHECKBOX_13, 270, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED14", ID_CHECKBOX_14, 360, 100, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED15", ID_CHECKBOX_15, 0, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED16", ID_CHECKBOX_16, 90, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED17", ID_CHECKBOX_17, 180, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED18", ID_CHECKBOX_18, 270, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED19", ID_CHECKBOX_19, 360, 150, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED20", ID_CHECKBOX_20, 0, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED21", ID_CHECKBOX_21, 90, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED22", ID_CHECKBOX_22, 180, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED23", ID_CHECKBOX_23, 270, 200, 80, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED24", ID_CHECKBOX_24, 360, 200, 80, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "return", ID_BUTTON_32, 0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "son",    ID_BUTTON_33, 93, 252, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "father", ID_BUTTON_34, 180, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "mather", ID_BUTTON_35, 270, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "next",   ID_BUTTON_36, 360, 250, 80, 40, 0, 0x0, 0 },
};



/**************************密码解锁界面*************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_4[] = {
	{ FRAMEWIN_CreateIndirect, "The password interface", ID_FRAMEWIN_1, 0, 0, 480, 320, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "0", ID_BUTTON_37, 160, 240, 120, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "1", ID_BUTTON_38, 0, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_39, 160, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_40, 320, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_41, 0, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_42, 160, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_43, 320, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7", ID_BUTTON_44, 0, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8", ID_BUTTON_45, 160, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9", ID_BUTTON_46, 320, 170, 150, 60, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "return", ID_BUTTON_32, 0, 250, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "next",   ID_BUTTON_36, 360, 250, 80, 40, 0, 0x0, 0 },
};



///**************************密码修改界面*************************************************/
//static const GUI_WIDGET_CREATE_INFO _aDialogCreate_5[] = {
//	{ FRAMEWIN_CreateIndirect, "The password interface", ID_FRAMEWIN_1, 0, 0, 480, 320, 0, 0x0, 0 },
//	{ BUTTON_CreateIndirect, "0", ID_BUTTON_13, 160, 240, 120, 50, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "1", ID_BUTTON_14, 0, 50, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "2", ID_BUTTON_15, 160, 50, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "3", ID_BUTTON_16, 320, 50, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "4", ID_BUTTON_17, 0, 110, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "5", ID_BUTTON_18, 160, 110, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "6", ID_BUTTON_19, 320, 110, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "7", ID_BUTTON_20, 0, 170, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "8", ID_BUTTON_21, 160, 170, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "9", ID_BUTTON_22, 320, 170, 150, 60, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Return", ID_BUTTON_23, 0, 240, 120, 50, 0, 0x0, 0 },//返回
//  { BUTTON_CreateIndirect, "Affirm", ID_BUTTON_24, 320, 240, 120, 50, 0, 0x0, 0 },//确定
//};



/*********************************界面初始化配置*******************************************/
static void _cbDialog_1(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'LED_Bright'
    //
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

    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // 所有灯亮
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					i=0;
        break;
      case WM_NOTIFICATION_RELEASED:   //按下时松开触摸屏有效
				for(i=0;i<11;i++)
			{
				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时200ms
				printf("%c\r\n",buf_led[i]);
//				if(i>=10)
//				{
//					i=0;
//				}
			}
//				printf("%c\r\n",buf_led[0]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[1]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[2]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[3]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//		  	printf("%c\r\n",buf_led[4]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[5]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[6]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[7]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[8]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[9]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//		  	printf("%c\r\n",buf_led[10]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
 				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_2();

        break;

      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'LED_Destroy'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					i=0;
        break;
      case WM_NOTIFICATION_RELEASED:
				for(i=0;i<11;i++)
			{
				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时200ms
				printf("%c\r\n",buf_led[i]);
//				if(i>=10)
//				{
//					i=0;
//				}
			}
//				printf("%c\r\n",buf_led[0]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[1]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[2]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[3]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//		  	printf("%c\r\n",buf_led[4]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[5]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[6]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[7]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[8]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			  printf("%c\r\n",buf_led[9]);
//			  OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//		  	printf("%c\r\n",buf_led[10]);
//				OSTimeDlyHMSM(0,0,0,response,OS_OPT_TIME_PERIODIC,&err);//延时500ms
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_2();
        break;

      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'User-Part'
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

    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/************************************所有灯亮/灯灭/独立控制界面**************************************/
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
					if(mode==1)
					{
					printf("%c\r\n",buf_led[0]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_8: // Notifications sent by 'LED1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[1]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_9: // Notifications sent by 'LED2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[2]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;
 
      }
      break;
    case ID_BUTTON_10: // Notifications sent by 'LED3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[3]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:
 
        break;

      }
      break;
    case ID_BUTTON_11: // Notifications sent by 'LED4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[4]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_12: // Notifications sent by 'LED5'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[5]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_13: // Notifications sent by 'LED6'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[6]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_14: // Notifications sent by 'LED7'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[7]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_15: // Notifications sent by 'LED8'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[8]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:
  
        break;

      }
      break;
    case ID_BUTTON_16: // Notifications sent by 'LED9'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[9]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_17: // Notifications sent by 'LED10'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
					if(mode==1)
					{
					printf("%c\r\n",buf_led[10]);
					}
        break;
      case WM_NOTIFICATION_RELEASED:

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
    case ID_BUTTON_32: // Notifications sent by 'return'       //返回
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
				
        break;

      }
      break;
    case ID_BUTTON_34: // Notifications sent by 'father'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_35: // Notifications sent by 'mather'
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

    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/***************************flash  LED界面*********************************************************/
static void _cbDialog_3(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;


  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "LED0");
    CHECKBOX_SetTextColor(hItem, 0x000000FF);//红色字体

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
    case ID_BUTTON_32: // Notifications sent by 'return'       //返回
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

        break;
			case WM_NOTIFICATION_RELEASED:
				
        break;
      }
      break;
    case ID_BUTTON_34: // Notifications sent by 'father'
      switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				
        break;
      case WM_NOTIFICATION_RELEASED:

        break;

      }
      break;
    case ID_BUTTON_35: // Notifications sent by 'mather'
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
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/************************************密码界面配置**************************************/
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
		//重新创建标题栏字体，字体大小
		hItem_2 = pMsg->hWin;
	  FRAMEWIN_SetText(hItem_2, "Password Unlock Interface");
	  FRAMEWIN_SetFont(hItem_2, GUI_FONT_16_ASCII);
		FRAMEWIN_SetTextAlign(hItem_2, GUI_TA_HCENTER | GUI_TA_VCENTER);	
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
			case ID_BUTTON_13: // Notifications sent by '0'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
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
    case ID_BUTTON_14: // Notifications sent by '1'
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
    case ID_BUTTON_15: // Notifications sent by '2'
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
    case ID_BUTTON_16: // Notifications sent by '3'
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
    case ID_BUTTON_17: // Notifications sent by '4'
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
    case ID_BUTTON_18: // Notifications sent by '5'
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
    case ID_BUTTON_19: // Notifications sent by '6'
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
    case ID_BUTTON_20: // Notifications sent by '7'
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
    case ID_BUTTON_21: // Notifications sent by '8'
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
    case ID_BUTTON_22: // Notifications sent by '9'
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
    case ID_BUTTON_32: // 返回
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateBoundary_1();
        break;
      }
      break;
    case ID_BUTTON_36: // 确认
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				  STMFLASH_Read(Flash_Save_Passward_Addr,(u16*)data_Passward,Passward_Size);
			for(Passward_i=0;Passward_i<6;Passward_i++)
			{
				 Test[Passward_i]=data_Passward[Passward_i];
			}
//			    Test[0]=data_Passward[0];
//			    Test[1]=data_Passward[1];
//			    Test[2]=data_Passward[2];
//			    Test[3]=data_Passward[3];
//			    Test[4]=data_Passward[4];
//			    Test[5]=data_Passward[5];
			    Test_Passward=Test[0]*100000+Test[1]*10000+Test[2]*1000+Test[3]*100+Test[4]*10+Test[5];
			    printf("%d\r\n",Test[0]);
			    printf("%d\r\n",Test[1]);
			    printf("%d\r\n",Test[2]);
			    printf("%d\r\n",Test[3]);
			    printf("%d\r\n",Test[4]);
			    printf("%d\r\n",Test[5]);
					Implementation_Password=six*100000+five*10000+four*1000+three*100+two*10+one;
			  if(Test_Passward==Implementation_Password)
				{
					printf("%c\r\n",buf_led[11]);
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



/*********************创建初始化界面***********************/
WM_HWIN CreateBoundary_1(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);
  return hWin;
}

/*********************创建所有灯亮/灯灭/独立口控制界面***********************/
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

/*********************创建密码输入界面*************************/
WM_HWIN CreateBoundary_4(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_4, GUI_COUNTOF(_aDialogCreate_4), _cbDialog_4, WM_HBKWIN, 0, 0); //创建窗口
  return hWin;
}


///*********************创建密码修改界面*************************/
//WM_HWIN CreateBoundary_5(void) {
//  WM_HWIN hWin;

//  hWin = GUI_CreateDialogBox(_aDialogCreate_5, GUI_COUNTOF(_aDialogCreate_5), _cbDialog_5, WM_HBKWIN, 0, 0); //创建窗口
//  return hWin;
//}



/********************创建初始化对话框**************************/
void dropdown_demo(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);//创建窗口
	while(1)
	{
		GUI_Delay(100);
	}
}


