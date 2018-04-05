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

#include "main.h"
#include "uart.h"

//常量
#define Success 1U
#define Failure 0U

//定义变量
unsigned long  Time_Cont = 0;       //定时器计数器

unsigned int count = 0;

char phoneNumber[] = "173xxxxxxx1";		//替换成需要被拨打电话的号码
char msg[] = "ILoveMCU.taobao.com";		//短信内容

code char TCPServer[] = "103.44.145.245";		//TCP服务器地址
code char Port[] = "10380";						//端口	

//****************************************************
//主函数
//****************************************************
void main()
{

    Uart_Init();						  

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



	while(1)
	{	
		xdata char send_buf[100] = {0};
		memset(send_buf, 0, 100);    //清空
		strcpy(send_buf, "AT+QIOPEN=\"TCP\",\"");
		strcat(send_buf, TCPServer);
		strcat(send_buf, "\",\"");
		strcat(send_buf, Port);
		strcat(send_buf, "\"\r\n");
		if (sendCommand(send_buf, "CONNECT OK", 10000, 5) == Success);
		else errorLog();
		delay_ms(100);
	
		while(1)
		{
			//发送数据
			if (sendCommand("AT+QISEND\r\n", ">", 3000, 5) == Success);
			else errorLog();
			delay_ms(10);
	
			memset(send_buf, 0, 100);    //清空
			sprintf(send_buf,"ILoveMCU.taobao.com %d\r\n",count);
			count++;
			
	
			if (sendCommand(send_buf, send_buf, 3000, 1) == Success);
			else errorLog();
			delay_ms(10);
	
			SendData(0x1a);//发送数据
			delay_ms(500);
	
		}
	
	
		if (sendCommand("AT+QICLOSE\r\n", "OK\r\n", 5000, 10) == Success);
		else errorLog();
		delay_ms(100);
	
		if (sendCommand("AT+QIDEACT\r\n", "OK\r\n", 5000, 10) == Success);
		else errorLog();
	
	
		delay_ms(1000);	
	}
}

void sendMessage(char *number,char *msg)
{
	xdata char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //清空
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog();

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else errorLog();

	memset(send_buf, 0, 40);    //清空
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog();
}

void phone(char *number)
{
	char send_buf[20] = {0};
	memset(send_buf, 0, 20);    //清空
	strcpy(send_buf, "ATD");
	strcat(send_buf, number);
	strcat(send_buf, ";\r\n");

	if (sendCommand(send_buf, "OK\r\n", 10000, 10) == Success);
	else errorLog();
}

void errorLog()
{
	while (1)
	{
	  	if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
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