/**********************************************************
		作者：神秘藏宝室
		店铺：ILoveMCU.taobao.com

	本例程仅适用于在本店购买模块的用户使用，鄙视其它店铺的盗用行为
	版权所有，盗版必究！！！
	MC20 GSM/GPRS/GPS开发板：
	https://item.taobao.com/item.htm?id=562661881042
	测试使用STM32最小系统：
	https://item.taobao.com/item.htm?id=523336310868
	USB-TTL模块：
	https://item.taobao.com/item.htm?id=39481188174
	移动场合供电用锂电池套装：
	https://item.taobao.com/item.htm?id=530904849115
	
	接线说明：
	STM32					MC20
	3.3V	------>	V_IO
	GND		<----->	GND
	PA3		<------	GSM_TX
	PA2		------>	GSM_RX
	

	//串口观看GPS的解析结果
	STM32					USB-TTL模块
	GND		------>	GND
	TX1/PA9		------>	RXD
***********************************************************/



#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usart2.h"


//常量
#define Success 1U
#define Failure 0U

//定义变量
unsigned long  Time_Cont = 0;       //定时器计数器
unsigned int count = 0;

char phoneNumber[] = "173xxxxxxx1";		//替换成需要被拨打电话的号码
char msg[] = "ILoveMCU.taobao.com";		//短信内容	

char TCPServer[] = "103.44.145.208";		//TCP服务器地址
char Port[] = "10380";						//端口	


void errorLog(int num);
void phone(char *number);
unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
void Sys_Soft_Reset(void);
void sendMessage(char *number,char *msg);
void parseGpsBuffer(void);
void printGpsBuffer(void);

int main(void)
{	
	delay_init();
	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	USART2_Init(9600);	//串口2波特率9600
	u2_printf("AT\r\n");
	Init_LEDpin();
	
	
	
	printf("Welcome to use!\r\n");
	printf("ILoveMcu.taobao.com!\r\n");
	
	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(1);
	delay_ms(10);
	
////为了不插卡可以使用GPS，屏蔽以下两条指令，如果有SIM卡时候可以释放
// 	if (sendCommand("AT+CPIN?\r\n", "READY", 3000, 10) == Success);
// 	else errorLog(3);
// 	delay_ms(10);

// 	if (sendCommand("AT+CREG?\r\n", ",1", 3000, 10) == Success);	//本地SIM卡
// 	else if(sendCommand("AT+CREG?\r\n", ",5", 3000, 10) == Success	);//漫游SIM卡
// 	else	errorLog(3);
// 	delay_ms(10);

	if (sendCommand("AT+QGNSSC?\r\n", "+QGNSSC: 1", 5000, 1) == Success);
	else if (sendCommand("AT+QGNSSC=1\r\n", "OK\r\n", 5000, 10) == Success);
	else errorLog(5);

	clrStruct();
	while(1)
	{
		if (sendCommand("AT+QGNSSRD=\"NMEA/RMC\"\r\n", "OK\r\n", 2000, 10) == Success);
		else errorLog(6);

		Save_Data.isGetData = true;
		memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
		memcpy(Save_Data.GPS_Buffer, USART2_RX_BUF, point2);
		
		printf("\r\n==========Save_Data.GPS_Buffer==========\r\n");
		printf(Save_Data.GPS_Buffer); //输出接收到的信息
		printf("\r\n");

		
		parseGpsBuffer();
		printGpsBuffer();
			
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
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
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
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

void sendMessage(char *number,char *msg)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //清空
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog(6);

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else errorLog(7);

	memset(send_buf, 0, 20);    //清空
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog(8);
}

void errorLog(int num)
{
	printf("ERROR%d\r\n",num);
	while (1)
	{
		if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
		{
			Sys_Soft_Reset();
		}
		delay_ms(200);
	}
}

void Sys_Soft_Reset(void)
{  
    SCB->AIRCR =0X05FA0000|(u32)0x04;      
}

void phone(char *number)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //清空
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
	else errorLog(4);
}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	USART2_CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		u2_printf(Command); 		//发送GPRS指令
		
		printf("\r\n***************send****************\r\n");
		printf(Command);
		
		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(USART2_RX_BUF, Response) != NULL)
			{				
				printf("\r\n***************receive****************\r\n");
				printf(USART2_RX_BUF);
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	printf("\r\n***************receive****************\r\n");
	printf(USART2_RX_BUF);
	USART2_CLR_Buf();
	return Failure;
}





