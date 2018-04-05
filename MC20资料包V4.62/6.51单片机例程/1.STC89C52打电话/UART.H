/*********************************************************************
		 作者：神秘藏宝室
		 店铺：ILoveMCU.taobao.com

	本例程仅适用于在本店购买模块的用户使用，鄙视其它店铺的盗用行为
	版权所有，盗版必究！！！
	MC20 GSM/GPRS/GPS开发板：
	https://item.taobao.com/item.htm?id=562661881042
	SIM800 GSM/GPRS模块
	https://item.taobao.com/item.htm?id=521599424969
	STC89C52/STC12C5A60S2最小系统：
	https://item.taobao.com/item.htm?id=26410708738
	USB-TTL模块：
	https://item.taobao.com/item.htm?id=39481188174
	移动场合供电用锂电池套装：
	https://item.taobao.com/item.htm?id=530904849115
*********************************************************************/
#ifndef __UART_H__
#define __UART_H__

#include <REG52.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

//函数或者变量声明
void Uart_Init();
void SendData(unsigned char ch);
void SendString(char *s);
bit Hand(unsigned char *a);
void CLR_Buf(void);


extern u8 xdata  Rec_Buf[];  //串口数据缓存
extern u8 point1;             //绶存指针


#endif