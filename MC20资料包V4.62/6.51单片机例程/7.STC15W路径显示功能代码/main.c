/*********************************************************************
		 作者：神秘藏宝室
		 店铺：ILoveMCU.taobao.com

	本例程仅适用于在本店购买模块的用户使用，鄙视其它店铺的盗用行为
	版权所有，盗版必究！！！
	MC20 GSM/GPRS/GPS开发板：
	https://item.taobao.com/item.htm?id=562661881042
	SIM800 GSM/GPRS模块
	https://item.taobao.com/item.htm?id=521599424969
	STC15W/IAP15W最小系统：
	https://item.taobao.com/item.htm?id=539809674723
	USB-TTL模块：
	https://item.taobao.com/item.htm?id=39481188174
	移动场合供电用锂电池套装：
	https://item.taobao.com/item.htm?id=530904849115
*********************************************************************/

/******************************************************************************
		晶振 11.0592MHz		
		串口1用于发送给电脑调试信息	  波特率9600
		串口3用于接GPRS模块	  GSM的TXD接P0.0,GPRS的RXD接P0.1	 波特率9600
******************************************************************************/
#include "main.h"
#include "delay.h"
#include "uart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*************	功能说明	**************

本程序演示LED闪烁  


******************************************/

/*************	本地常量声明	**************/
#define Success 1U
#define Failure 0U

/*************	本地变量声明	**************/
unsigned long  Time_Cont = 0;       //定时器计数器
bit ledState = LED_OFF;

unsigned int gprsBufferCount = 0;
//code char OneNetServer[] = "api.heclouds.com";       //不需要修改
code char OneNetServer[] = "183.230.40.33";       //不需要修改


code char device_id[] = "3225187";    //修改为自己的设备ID
code char API_KEY[] = "R9xO5NZm6oVI4YBHvCPKEqtwYtMA";    //修改为自己的API_KEY
code char sensor_gps[] = "location";				//不需要修改



/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/



/******************** IO配置函数 **************************/
void	GPIO_config(void)
{
	P5M0 = 0; 		//设置准双向口
	P5M1 = 0;
}

/******************** 主函数 **************************/
void main(void)
{	
	GPIO_config();
	UartInit();

	SendString("ILoveMCU.taobao.com");
	delay_ms(200);
	clrStruct();

	initGprs();
	while(1)
	{
		if (sendCommand("AT+QGNSSRD=\"NMEA/RMC\"\r\n", "OK\r\n", 2000, 10) == Success);
		else errorLog(10);
		
		Save_Data.isGetData = true;
		memset(Save_Data.GPS_Buffer, 0, 120);      //清空
		memcpy(Save_Data.GPS_Buffer, Uart3_Rec_Buf, point3);

		parseGpsBuffer();
		printGpsBuffer();	
	}
}

void initGprs()
{

	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(1);
	delay_ms(10);

	if (sendCommand("AT+CPIN?\r\n", "READY", 1000, 10) == Success);
	else errorLog(2);
	delay_ms(10);

	if (sendCommand("AT+CREG?\r\n", ",1", 1000, 10) == Success);
	else
	{
		delay_ms(10);
		if (sendCommand("AT+CREG?\r\n", ",5", 1000, 10) == Success);
		else errorLog(3);
	}

	if (sendCommand("AT+QGNSSC?\r\n", "+QGNSSC: 1", 1000, 1) == Success);
	else if (sendCommand("AT+QGNSSC=1\r\n", "OK\r\n", 100, 10) == Success);
	else errorLog(4);

	if (sendCommand("AT+QICLOSE\r\n", "\r\n", 5000, 10) == Success);
	else errorLog(5);
	delay_ms(100);
	
	if (sendCommand("AT+QIDEACT\r\n", "\r\n", 5000, 10) == Success);
	else errorLog(6);
	delay_ms(100);


}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	Uart3CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		SendString("\r\n---------send AT Command:---------\r\n");
		SendString(Command);

		Uart3SendString(Command); 		//发送GPRS指令

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(Uart3_Rec_Buf, Response) != NULL)
			{
				SendString("\r\n==========receive AT Command:==========\r\n");
				SendString(Uart3_Rec_Buf); //输出接收到的信息

				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	SendString("\r\n==========receive AT Command:==========\r\n");
	SendString(Uart3_Rec_Buf);//输出接收到的信息
	Uart3CLR_Buf();
	return Failure;
}

int Digcount(long num)
{
	int i=0;	
	while(num>0)
	{ 
		i++;
		num=num/10;
	}
  return i;
}

char* longitudeToOnenetFormat(char *lon_str_temp) 		//经度
{
	unsigned long xdata lon_Onenet = 0;
	unsigned int xdata dd_int = 0;
	unsigned long xdata mm_int = 0;
	float xdata lon_Onenet_double = 0;
	int i = 0;

	unsigned long tempInt = 0;
	unsigned long tempPoint = 0;
	char xdata result[20];
	char xdata point_result[15];
	int pointLength = 0;

	//51单片机没有double，double和float，精度不够，这里只能分开整数和小数换算。
	sscanf(lon_str_temp, "%ld.%ld", &tempInt,&tempPoint);
	lon_Onenet = tempInt%100;
	pointLength = strlen(lon_str_temp) - 1 - Digcount(tempInt);		
	for( i = 0 ; i < pointLength ; i++)	//小数点几位，整数部分就放大10的几次方
	{
		lon_Onenet *= 10; 	
	}

	dd_int = tempInt / 100; //取出dd

	mm_int = lon_Onenet + tempPoint; //取出MM部分

	mm_int = mm_int*10/6;	 		//本来是除以60，这里*10/6为了多2位小数点有有效数字


   	sprintf(result,"%d.",dd_int);
	for( i = 0 ; i < pointLength + 1 - Digcount(mm_int) ; i++)
	{
		strcat(result, "0");	
	}
	sprintf(point_result,"%ld",mm_int);
	strcat(result, point_result);

//	SendString("\r\n==========ONENET FORMART==========\r\n");
//	SendString(result);
	return result;
}



char* latitudeToOnenetFormat(char *lat_str_temp) 		//纬度
{
	unsigned long xdata lat_Onenet = 0;
	int xdata dd_int = 0;
	unsigned long xdata mm_int = 0;

	int i = 0;

	unsigned long tempInt = 0;
	unsigned long tempPoint = 0;
	char xdata result[20];
	char xdata point_result[15];
	int pointLength = 0;
//	char xdata debugTest[30];
	
	//51单片机没有double，double和float，精度不够，这里只能分开整数和小数换算。
	sscanf(lat_str_temp, "%ld.%ld", &tempInt,&tempPoint);
	lat_Onenet = tempInt%100;
	
//	SendString("\r\n==========ONENET FORMART strlen(lat_str_temp)==========\r\n");
//	sprintf(debugTest,"%d",strlen(lat_str_temp));
//	SendString(debugTest);

	pointLength = strlen(lat_str_temp) - 1 - Digcount(tempInt);	

//	SendString("\r\n==========ONENET FORMART pointLength==========\r\n");
//	sprintf(debugTest,"%d",pointLength);
//	SendString(debugTest);
	for( i = 0 ; i < pointLength ; i++)	//小数点几位，整数部分就放大10的几次方
	{
		lat_Onenet *= 10; 	
	}

//	SendString("\r\n==========ONENET FORMART tempPoint==========\r\n");
//	sprintf(debugTest,"%ld",tempPoint);
//	SendString(debugTest);
//
//	SendString("\r\n==========ONENET FORMART tempInt==========\r\n");
//	sprintf(debugTest,"%ld",tempInt);
//	SendString(debugTest);
//
//	SendString("\r\n==========ONENET FORMART lat_Onenet==========\r\n");
//	sprintf(debugTest,"%ld",lat_Onenet);
//	SendString(debugTest);

	dd_int = tempInt / 100; //取出dd

	mm_int = lat_Onenet + tempPoint; //取出MM部分

	mm_int = mm_int*10/6;	 		//本来是除以60，这里*10/6为了多2位小数点有有效数字

//	SendString("\r\n==========ONENET FORMART mm_int==========\r\n");
//	sprintf(debugTest,"%ld",mm_int);
//	SendString(debugTest);

	
	sprintf(result,"%d.",dd_int);
	for( i = 0 ; i < pointLength + 1 - Digcount(mm_int) ; i++)
	{
		strcat(result, "0");	
	}
	sprintf(point_result,"%ld",mm_int);
	strcat(result, point_result);

//	SendString("\r\n==========ONENET FORMART==========\r\n");
//	SendString(result);
	return result;
}

void postGpsDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, char* lon_temp, char* lat_temp)
{
	char xdata send_buf[400] = {0};
	char xdata text[200] = {0};
	char xdata tmp[25] = {0};

	char xdata lon_str_end[15] = {0};
	char xdata lat_str_end[15] = {0};

	char xdata sendCom[2] = {0x1A};

//	dtostrf(longitudeToOnenetFormat(lon_temp), 3, 6, lon_str_end); //转换成字符串输出
//	dtostrf(latitudeToOnenetFormat(lat_temp), 2, 6, lat_str_end); //转换成字符串输出

//	lon_temp = "11224.4992";
//	lat_temp = "3438.1633";

	sprintf(lon_str_end,"%s", longitudeToOnenetFormat(lon_temp)); 
	sprintf(lat_str_end,"%s", latitudeToOnenetFormat(lat_temp)); 

	//连接服务器
	memset(send_buf, 0, 400);    //清空
	strcpy(send_buf, "AT+QIOPEN=\"TCP\",\"");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\",80\r\n");
	if (sendCommand(send_buf, "CONNECT", 10000, 3) == Success);
	else errorLog(7);

	//发送数据
	if (sendCommand("AT+QISEND\r\n", ">", 3000, 1) == Success);
	else errorLog(8);

	memset(send_buf, 0, 400);    //清空

	/*准备JSON串*/
	//ARDUINO平台不支持sprintf的double的打印，只能转换到字符串然后打印
	sprintf(text, "{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%s,\"lat\":%s}}]}]}"
	        , sensor_id_temp, lon_str_end, lat_str_end);

	/*准备HTTP报头*/
	send_buf[0] = 0;
	strcat(send_buf, "POST /devices/");
	strcat(send_buf, device_id_temp);
	strcat(send_buf, "/datapoints HTTP/1.1\r\n"); //注意后面必须加上\r\n
	strcat(send_buf, "api-key:");
	strcat(send_buf, API_VALUE_temp);
	strcat(send_buf, "\r\n");
	strcat(send_buf, "Host:");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\r\n");
	sprintf(tmp, "Content-Length:%d\r\n\r\n", strlen(text)); //计算JSON串长度
	strcat(send_buf, tmp);
	strcat(send_buf, text);

	if (sendCommand(send_buf, send_buf, 3000, 1) == Success);
	else errorLog(9);

	
	if (sendCommand(sendCom, "\"succ\"}", 3000, 1) == Success);
	else errorLog(10);

	delay_ms(100);
	if (sendCommand("AT+QICLOSE\r\n", "OK\r\n", 3000, 1) == Success);
	else errorLog(11);
	delay_ms(100);

	if (sendCommand("AT+QIDEACT\r\n", "OK\r\n", 3000, 1) == Success);
	else errorLog(11);
	delay_ms(100);

}

void soft_reset(void)	 //制造重启命令
{
   ((void (code *) (void)) 0x0000) ();
}

void errorLog(int num)
{
	SendString("ERROR");
	SendData(num+0x30);
	SendString("\r\n");
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK", 100, 10) == Success)
		{
			SendString("\r\nRESET!!!!!!\r\n");

			soft_reset();
		}
		delay_ms(200);
	}
}




void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		SendString("**************\r\n");
		SendString(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		SendString("Save_Data.UTCTime = ");
		SendString(Save_Data.UTCTime);
		SendString("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			SendString("Save_Data.latitude = ");
			SendString(Save_Data.latitude);
			SendString("\r\n");


			SendString("Save_Data.N_S = ");
			SendString(Save_Data.N_S);
			SendString("\r\n");

			SendString("Save_Data.longitude = ");
			SendString(Save_Data.longitude);
			SendString("\r\n");

			SendString("Save_Data.E_W = ");
			SendString(Save_Data.E_W);
			SendString("\r\n");

			LED = LED_ON;
			delay_ms(100);
			LED = LED_OFF;

			postGpsDataToOneNet(API_KEY, device_id, sensor_gps, Save_Data.longitude, Save_Data.latitude);		//发送数据到Onenet

		}
		else
		{
			SendString("GPS DATA is not usefull!\r\n");
		}
		
	}
}


void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}
