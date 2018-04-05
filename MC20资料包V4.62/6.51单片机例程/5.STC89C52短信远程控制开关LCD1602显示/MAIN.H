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
#ifndef __MAIN_H__
#define __MAIN_H__

#include <REG52.h>


sbit relay1 = P2^0;
#define RELAY_OFF 1
#define RELAY_ON 0



//函数或者变量声明
extern void delay_ms(unsigned int n);
extern unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
extern void errorLog();
extern void soft_reset(void);
extern void parseMessage();



#endif