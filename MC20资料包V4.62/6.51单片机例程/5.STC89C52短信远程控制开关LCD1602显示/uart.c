/*********************************************************************
		 作者：神秘藏宝室
		 店铺：ILoveMCU.taobao.com

	本例程仅适用于在本店购买模块的用户使用，鄙视其它店铺的盗用行为
	版权所有，盗版必究！！！
	SIM800 GSM/GPRS模块
	https://item.taobao.com/item.htm?id=521599424969
	STC89C52最小系统：
	https://item.taobao.com/item.htm?id=524713612727
	STC12C5A60S2最小系统：
	https://item.taobao.com/item.htm?id=524701473371
	USB-TTL模块：
	https://item.taobao.com/item.htm?id=39481188174
	移动电源锂电池套装
	https://item.taobao.com/item.htm?id=530904849115
	移动场合供电用锂电池套装：
	https://item.taobao.com/item.htm?id=530904849115
*********************************************************************/
#include "uart.h"


#define Uart1_Buf_Max 100               //串口数据缓存长度
u8 xdata  Rec_Buf[Uart1_Buf_Max];  //串口数据缓存
u8 point1 = 0;             //绶存指针
bit isGetMessage=0;
bit isStartGetMessage = 0;

void Uart_Init()					  			   
{	
	SCON = 0X50;  //UART方式1；8位UART
	REN  = 1;     //不允许串行口接收数据
	PCON = 0x00;  //SMOD=0;波特率不加倍
	TMOD = 0x20;  //T1方式2，用于产生波特率
	TH1  = 0xFD;  //装初值
	TL1  = 0xFD;
	TR1  = 1;     //启动定时器1
	EA   = 1;     //打开全局中断控制
	ES   = 1;     //串行口中断	
}


/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //写数据到UART数据寄存器
		while(TI == 0);
		TI = 0;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

bit Hand(unsigned char *a)                   // 串口命令识别函数
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(Rec_Buf, 0, Uart1_Buf_Max);      //清空

    point1 = 0;                    
}

void RECEIVE_DATA(void) interrupt 4 using 1         //串口中断	  		
{    
	ES = 0;
	if (RI)
    {
      	RI = 0;                                //清除RI位
		Rec_Buf[point1] = SBUF; 
//		if (Rec_Buf[0] == 0xd9)
//			{
//				IAP_CONTR = 0x60;
//			}
		if( Rec_Buf[point1] == '+')
		{
			isStartGetMessage = 1;
		}
			
		point1++;               
		if(point1>=Uart1_Buf_Max)          
		{
			point1 = 0;
		}
		    

    }
	ES =  1;

}











