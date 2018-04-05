#ifndef __UART_H
#define __UART_H

#include	"config.h"

#define		S1_USE_P30P31()		P_SW1 &= ~0xc0					//UART1 使用P30 P31口	默认
#define		S1_USE_P36P37()		P_SW1 = (P_SW1 & ~0xc0) | 0x40	//UART1 使用P36 P37口
#define		S1_USE_P16P17()		P_SW1 = (P_SW1 & ~0xc0) | 0x80	//UART1 使用P16 P17口

#define		TI2					(S2CON & 2) != 0
#define		RI2					(S2CON & 1) != 0
#define		SET_TI2()			S2CON |=  2
#define		CLR_TI2()			S2CON &= ~2
#define		CLR_RI2()			S2CON &= ~1

#define		UART2_INT_ENABLE()		IE2 |=  1	//允许串口2中断
#define		UART2_INT_DISABLE()		IE2 &= ~1	//允许串口2中断

#define		TI3					(S3CON & 2) != 0
#define		RI3					(S3CON & 1) != 0
#define		SET_TI3()			S3CON |=  2
#define		CLR_TI3()			S3CON &= ~2
#define		CLR_RI3()			S3CON &= ~1

#define		UART3_INT_ENABLE()		IE2 |=  (1<<3)	//允许串口3中断
#define		UART3_INT_DISABLE()		IE2 &= ~(1<<3)	//允许串口3中断

#define false 0
#define true 1

//定义数组长度
#define GPS_Buffer_Length 120
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} xdata _SaveData;

#define gpsRxBufferLength  76
extern char idata gpsRxBuffer[gpsRxBufferLength];
extern unsigned char RX_Count;
extern _SaveData Save_Data;

extern u8 xdata  Uart3_Rec_Buf[];
extern u8 point3;


void UartInit();
void SendData(unsigned char ch);
void SendString(char *s);
bit Hand(unsigned char *a) ;
void CLR_Buf(void) ;

void Uart2SendData(unsigned char ch);
void Uart2SendString(char *s);
//bit Uart2Hand(unsigned char *a) ;
//void Uart2CLR_Buf(void) ;

void Uart3SendData(unsigned char ch);
void Uart3SendString(char *s);
bit Uart3Hand(unsigned char *a) ;
void Uart3CLR_Buf(void) ;



#endif