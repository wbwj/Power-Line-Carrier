/*********************************************************************
		 作者：神秘藏宝室
		 店铺：ILoveMCU.taobao.com
	本例程仅适用于在本店购买模块的用户使用，鄙视其它店铺的盗用行为
	版权所有，盗版必究！！！
	MC20 GSM/GPRS/GPS开发板：
	https://item.taobao.com/item.htm?id=562661881042
	ARDUINO UNO R3开发板：
	https://item.taobao.com/item.htm?id=27554596680
	ARDUINO MEGA2560 R3开发板：
	https://item.taobao.com/item.htm?id=38041409136
	USB-TTL模块：
	https://item.taobao.com/item.htm?id=39481188174
	锂电池套装适合移动场合给GSM模块供电：
	https://item.taobao.com/item.htm?id=530904849115
*********************************************************************/

#include <TimerOne.h>
#include <SoftwareSerial.h>


#define GprsSerail  Serial
SoftwareSerial DebugSerial(10, 11); // RX, TX			

// #define DebugSerial Serial
// #define GprsSerail Serial3	//使用mega2560板子可以用Serial3，不用SoftSerial调试，这样更稳定


#define Success 1U
#define Failure 0U

int L = 13; //LED指示灯引脚

unsigned long  Time_Cont = 0;       //定时器计数器

const unsigned int gprsRxBufferLength = 1000;
char gprsRxBuffer[gprsRxBufferLength];
unsigned int gprsBufferCount = 0;


char msg[] = "ILoveMCU.taobao.com";		//短信内容

char messageBuffer[100] = {};		//短信缓存数组	

void setup() {
	pinMode(L, OUTPUT);
	digitalWrite(L, LOW);

	DebugSerial.begin(9600);
	GprsSerail.begin(9600);

	Timer1.initialize(1000);
	Timer1.attachInterrupt(Timer1_handler);

	initGprs();		//初始化模块

	DebugSerial.println("\r\nsetup end!");
	clrGprsRxBuffer();
}

void loop() {
	if(getMessage() == Success)			//当接收到短信时候进入
	{
		DebugSerial.println("gprsRxBuffer:");
		DebugSerial.println(gprsRxBuffer);
		parseMessage();							//解析短信内容
	
		DebugSerial.println("getMessage:");
		DebugSerial.println(messageBuffer);


		if(strstr(messageBuffer, "OFF") != NULL)
		{
			DebugSerial.println("GET OFF Command!");
			digitalWrite(L, LOW);
		}


		if(strstr(messageBuffer, "ON") != NULL)
		{
			DebugSerial.println("GET ON Command!");
			digitalWrite(L, HIGH);
		}
		
	}
}

void parseMessage()
{
	char *messageHead=NULL;
	char *messageEnd1=NULL;
	char *messageEnd2=NULL;

	memset(messageBuffer, 0, sizeof(messageBuffer)-1); 	//清空短信内容缓冲区
	messageHead = strstr(gprsRxBuffer, "CMT");
	if (messageHead != NULL)
	{
		messageEnd1 = strstr(messageHead, "\n");
		if (messageEnd1 != NULL)
		{
			messageEnd1 ++;
			messageEnd2 = strstr(messageEnd1, "\n");
			if (messageEnd2 != NULL)
			{	
				memcpy(messageBuffer, messageEnd1, messageEnd2 - messageEnd1);
									

			}else DebugSerial.println("messageEnd2 NULL"); 
		}else DebugSerial.println("messageEnd1 NULL"); 

	}else DebugSerial.println("messageHead NULL"); 

	clrGprsRxBuffer();
}


int getMessage() {
	while (GprsSerail.available())
	{
		gprsRxBuffer[gprsBufferCount] = GprsSerail.read();
		if (gprsBufferCount == gprsRxBufferLength)clrGprsRxBuffer();

		DebugSerial.print(gprsRxBuffer[gprsBufferCount]);
		if( gprsRxBuffer[gprsBufferCount] == '+')
		{
			Time_Cont = 0;
			while (Time_Cont < 500)
			{
				while (GprsSerail.available())
				{
					gprsRxBuffer[gprsBufferCount] = (char)GprsSerail.read();
					DebugSerial.print(gprsRxBuffer[gprsBufferCount]);

					gprsBufferCount++;
					if (gprsBufferCount == gprsRxBufferLength)clrGprsRxBuffer();
				}	
			}
			gprsRxBuffer[gprsBufferCount]='\0';	
			return Success;			
		}
	}

	return Failure;
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

void sendMessage(char *number,char *msg)
{
	char send_buf[40] = {0};
	memset(send_buf, 0, 40);    //清空
	strcpy(send_buf, "AT+CMGS=\"");
	strcat(send_buf, number);
	strcat(send_buf, "\"\r\n");
	if (sendCommand(send_buf, ">", 3000, 10) == Success);
	else errorLog(7);

	if (sendCommand(msg, msg, 3000, 10) == Success);
	else errorLog(8);

	memset(send_buf, 0, 40);    //清空
	send_buf[0] = 0x1a;
	send_buf[1] = '\0';
	if (sendCommand(send_buf, "OK\r\n", 10000, 5) == Success);
	else errorLog(9);
}


void initGprs()
{
	if (sendCommand("AT\r\n", "OK\r\n", 500, 10) == Success);
	else errorLog(1);

	delay(10);

	if (sendCommandReceive2Keyword("AT+CPIN?\r\n", "READY", "OK\r\n", 3000, 10) == Success);
	else errorLog(2);
	delay(10);

	if (sendCommandReceive2Keyword("AT+CREG?\r\n", ",1", "OK\r\n", 3000, 10) == Success);	//本地SIM卡
	else if(sendCommandReceive2Keyword("AT+CREG?\r\n", ",5", "OK\r\n", 3000, 10) == Success	);//漫游SIM卡
	else	errorLog(3);
	delay(10);

	if (sendCommand("AT+CMGF=1\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(4);
	delay(10);

	// if (sendCommand("AT+CSMP=17,167,0,8\r\n", "OK\r\n", 3000, 10) == Success);
	// else errorLog(5);
	// delay(10);

	if (sendCommand("AT+CSCS=\"GSM\"\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(6);
	delay(10);

	if (sendCommand("AT+CNMI=2,2\r\n", "OK\r\n", 3000, 10) == Success);
	else errorLog(7);
	delay(10);
}

void(* resetFunc) (void) = 0; //制造重启命令

void errorLog(int num)
{
	DebugSerial.print("ERROR");
	DebugSerial.println(num);
	while (1)
	{
		if (sendCommand("AT\r\n", "OK\r\n", 100, 10) == Success)
		{
			DebugSerial.print("\r\nRESET!!!!!!\r\n");
			resetFunc();
		}
	}
}



unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	clrGprsRxBuffer();
	for (unsigned char n = 0; n < Retry; n++)
	{
		DebugSerial.print("\r\n---------send AT Command:---------\r\n");
		DebugSerial.write(Command);

		GprsSerail.write(Command);

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			gprsReadBuffer();
			if (strstr(gprsRxBuffer, Response) != NULL)
			{
				DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
				DebugSerial.print(gprsRxBuffer); //输出接收到的信息
				clrGprsRxBuffer();
				return Success;
			}
		}
		Time_Cont = 0;
	}
	DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
	DebugSerial.print(gprsRxBuffer);//输出接收到的信息
	clrGprsRxBuffer();
	return Failure;
}

unsigned int sendCommandReceive2Keyword(char *Command, char *Response, char *Response2, unsigned long Timeout, unsigned char Retry)
{
	clrGprsRxBuffer();
	for (unsigned char n = 0; n < Retry; n++)
	{
		DebugSerial.print("\r\n---------send AT Command:---------\r\n");
		DebugSerial.write(Command);

		GprsSerail.write(Command);

		Time_Cont = 0;
		while (Time_Cont < Timeout)
		{
			gprsReadBuffer();
			if (strstr(gprsRxBuffer, Response) != NULL && strstr(gprsRxBuffer, Response2) != NULL)
			{
				DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
				DebugSerial.print(gprsRxBuffer); //输出接收到的信息
				clrGprsRxBuffer();
				return Success;
			}
		}
		Time_Cont = 0;
	}
	DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
	DebugSerial.print(gprsRxBuffer);//输出接收到的信息
	clrGprsRxBuffer();
	return Failure;
}




void Timer1_handler(void)
{
	Time_Cont++;
}



void gprsReadBuffer() {
	while (GprsSerail.available())
	{
		gprsRxBuffer[gprsBufferCount++] = GprsSerail.read();
		if (gprsBufferCount == gprsRxBufferLength)clrGprsRxBuffer();
	}
}

void clrGprsRxBuffer(void)
{
	memset(gprsRxBuffer, 0, gprsRxBufferLength);      //清空
	gprsBufferCount = 0;
}