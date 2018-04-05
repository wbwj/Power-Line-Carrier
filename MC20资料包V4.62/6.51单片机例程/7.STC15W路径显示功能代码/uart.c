#include "uart.h"
#include<string.h>

#define Uart1_Buf_Max 20               //串口数据缓存长度
u8 idata  Rec_Buf[Uart1_Buf_Max];  //串口数据缓存
u8 point1 = 0;             //绶存指针

//#define Uart2_Buf_Max 20               //串口数据缓存长度
//u8 xdata  Uart2_Rec_Buf[Uart2_Buf_Max];  //串口数据缓存
u8 point2 = 0;             //绶存指针
bit	B_TX2_Busy = 0;	//发送忙标志

char idata gpsRxBuffer[gpsRxBufferLength];
unsigned char RX_Count = 0;
_SaveData Save_Data;

#define Uart3_Buf_Max 600               //串口数据缓存长度
u8 xdata  Uart3_Rec_Buf[Uart3_Buf_Max];  //串口数据缓存
u8 point3 = 0;             //绶存指针
bit	B_TX3_Busy = 0;	//发送忙标志




void UartInit(void)		//9600bps@11.0592MHz
{
//注意: STC15W4K32S4系列的芯片,上电后所有与PWM相关的IO口均为
//      高阻态,需将这些口设置为准双向口或强推挽模式方可正常使用
//相关IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5
//	P3M0 &= ~(1<<7);		//设置P3.7为准双向IO
//	P3M1 &= ~(1<<7);
//
//	S1_USE_P36P37();

	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xDC;		//设定定时初值
	TH1 = 0xDC;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1


//	S2CON = 0x50;		//8位数据,可变波特率
//	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
//	T2L = 0xE8;		//设定定时初值			  	//115200
//	T2H = 0xFF;		//设定定时初值
//	AUXR |= 0x10;		//启动定时器2

	S3CON = 0x10;		//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x02;		//定时器3时钟为Fosc,即1T
	T3L = 0xE0;		//设定定时初值
	T3H = 0xFE;		//设定定时初值
	T4T3M |= 0x08;		//启动定时器3

//	UART2_INT_ENABLE();
	UART3_INT_ENABLE();
	ES = 1;
	EA = 1;
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

void Usart() interrupt 4 using 1            // 串口中断函数
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
			
		point1++;               
		if(point1>=Uart1_Buf_Max)          
		{
			point1 = 0;
		}           

    }
    if (TI)
    {
        TI = 0;                 //清除TI位

    }
	ES =  1;
}

/*----------------------------
通过串口2发送串口数据
----------------------------*/
void Uart2SendData(unsigned char ch)
{
	while(B_TX2_Busy);	//不忙后发送数据
    S2BUF = ch;                 //写数据到UART数据寄存器
	B_TX2_Busy = 1;
}

/*----------------------------
通过串口2发送字符串
----------------------------*/
void Uart2SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        Uart2SendData(*s++);         //发送当前字符
    }
}

//bit Uart2Hand(unsigned char *a)                   // 串口命令识别函数
//{ 
//    if(strstr(Uart2_Rec_Buf,a)!=NULL)
//	    return 1;
//	else
//		return 0;
//}

//void Uart2CLR_Buf(void)                           // 串口缓存清理
//{
//	memset(Uart2_Rec_Buf, 0, Uart2_Buf_Max);      //清空
//
//    point2 = 0;                    
//}

void Usart2() interrupt 8 using 1 
{
	unsigned char temp = 0;
	char i = 0;

	UART2_INT_DISABLE();		
	if(RI2)
    {
		temp = S2BUF;
		CLR_RI2();                               //清除RI位

		if(temp == '$')
		{
			RX_Count = 0;	
		}
			
		if(RX_Count <= 5)
		{
		   gpsRxBuffer[RX_Count++] = temp;
		}
		else if(gpsRxBuffer[0] == '$' &gpsRxBuffer[4] == 'M' && gpsRxBuffer[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
		{
			gpsRxBuffer[RX_Count++] = temp;
			if(temp == '\n')									   
			{
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
				memcpy(Save_Data.GPS_Buffer, gpsRxBuffer, RX_Count); 	//保存数据
				Save_Data.isGetData = true;
				RX_Count = 0;
				memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空				
			}
			
			if(RX_Count >= 75)
			{
				RX_Count = 75;
				gpsRxBuffer[RX_Count] = '\0';//添加结束符
			}			
		} 
         

    }
    if (TI2)
    {
        CLR_TI2();                //清除TI位
		B_TX2_Busy = 0;	//清空忙标志

    }
	UART2_INT_ENABLE();
	
	

}

/*----------------------------
通过串口3发送串口数据
----------------------------*/
void Uart3SendData(unsigned char ch)
{	
    while(B_TX3_Busy);	//不忙后发送数据
    S3BUF = ch;                 //写数据到UART数据寄存器
	B_TX3_Busy = 1;
}

/*----------------------------
通过串口3发送字符串
----------------------------*/
void Uart3SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        Uart3SendData(*s++);         //发送当前字符
    }
}

bit Uart3Hand(unsigned char *a)                   // 串口命令识别函数
{ 
    if(strstr(Uart3_Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

void Uart3CLR_Buf(void)                           // 串口缓存清理
{
	memset(Uart3_Rec_Buf, 0, Uart3_Buf_Max);      //清空

    point3 = 0;                    
}

void Usart3() interrupt 17 
{
	UART3_INT_DISABLE();	
	if(RI3)
    {
		CLR_RI3();                               //清除RI位
		Uart3_Rec_Buf[point3] = S3BUF; 
//		if (Rec_Buf[0] == 0xd9)
//			{
//				IAP_CONTR = 0x60;
//			}
			
		point3++;               
		if(point3>=Uart3_Buf_Max)          
		{
			point3 = 0;
		}           

    }
    if (TI3)
    {
        CLR_TI3();                //清除TI位
		B_TX3_Busy = 0;
    }
	UART3_INT_ENABLE();	
}

