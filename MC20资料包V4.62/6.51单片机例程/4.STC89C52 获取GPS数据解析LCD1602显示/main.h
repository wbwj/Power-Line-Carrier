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
#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

#define false 0
#define true 1

//定义数组长度
#define GPS_Buffer_Length 120
//#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
//	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} xdata _SaveData;


//函数或者变量声明
extern void delay_ms(unsigned int n);

extern void printGpsBuffer();
extern void parseGpsBuffer();
extern void errorLog();
extern unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
extern void clrStruct();
extern void soft_reset(void);

#endif