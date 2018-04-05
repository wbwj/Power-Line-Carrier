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

/**********************************************************************
		 功能描述：
		 发送短信 OFF，关闭
		 发送短信 ON，打开
***********************************************************************/

#include "main.h"
#include "uart.h"
#include "LCD1602.h"

//常量
#define Success 1U
#define Failure 0U

//定义变量
unsigned long  Time_Cont = 0;       //定时器计数器


char xdata messageBuffer[100];
bit isTimeOut = 0;

//****************************************************
//主函数
//****************************************************
void main()
{

    Uart_Init();
	Init_LCD1602();
	LCD1602_write_com(0x80);
	LCD1602_write_word("ILoveMCU.       ");
	LCD1602_write_com(0x80+0x40);
	LCD1602_write_word("      taobao.com");

	if (sendCommand("AT\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CPIN?\r\n", "READY", 1000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CREG?\r\n", ",1", 1000, 10) == Success);
	else
	{
		delay_ms(10);
		if (sendCommand("AT+CREG?\r\n", ",5", 1000, 10) == Success);
		else errorLog();
	}

	if (sendCommand("AT+CMGF=1\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CSCS=\"GSM\"\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog();
	delay_ms(10);

	if (sendCommand("AT+CNMI=2,2\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog();
	delay_ms(10);



	while(1)
	{	
		if(isStartGetMessage)
		{
			isStartGetMessage = 0;
			Time_Cont = 0;
			
			while (Time_Cont < 1000)	//等待1s接收短信数据
			{
				delay_ms(100);
				Time_Cont += 100;
			}
			isGetMessage = 	Success;
		}
		if( isGetMessage == Success)
		{
			parseMessage();
				
	
			if(strstr(messageBuffer, "ON") != NULL)	//打开
			{
			  	relay1 = RELAY_ON;

				LCD1602_write_com(0x01); //清屏

				LCD1602_write_com(0x80);
				LCD1602_write_word(messageBuffer);
			}
	
	
			if(strstr(messageBuffer, "OFF") != NULL)//关闭
			{
				relay1 = RELAY_OFF;

				LCD1602_write_com(0x01); //清屏

				LCD1602_write_com(0x80);
				LCD1602_write_word(messageBuffer);
			}
			isGetMessage = Failure	;
		}
	}
}

void parseMessage()
{
	char *messageHead=NULL;
	char *messageEnd1=NULL;
	char *messageEnd2=NULL;

	memset(messageBuffer, 0, 100); 	//清空短信内容缓冲区
	messageHead = strstr(Rec_Buf, "CMT");
	if (messageHead != NULL)
	{
		messageEnd1 = strstr(messageHead, "\n");
		if (messageEnd1 != NULL)
		{
			messageEnd1 ++;
			messageEnd2 = strstr(messageEnd1, "\r");
			if (messageEnd2 != NULL)
			{	
				memcpy(messageBuffer, messageEnd1, messageEnd2 - messageEnd1);
	
			}
		}

	}

	CLR_Buf();
}






void errorLog()
{
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK", 100, 10) == Success)
		{
			soft_reset();
		}
		delay_ms(200);
	}
}

void soft_reset(void)	 //制造重启命令
{
   ((void (code *) (void)) 0x0000) ();
}

unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char n;
	CLR_Buf();
	for (n = 0; n < Retry; n++)
	{
		SendString(Command); 		//发送GPRS指令

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			delay_ms(100);
			Time_Cont += 100;
			if (strstr(Rec_Buf, Response) != NULL)
			{
				
				CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	
	CLR_Buf();
	return Failure;
}

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void delay_ms(unsigned int n)
{
	unsigned int  i,j;
		for(i=0;i<n;i++)
			for(j=0;j<123;j++);
}