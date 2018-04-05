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

char phoneNumber[] = "173xxxxxxx1";		//替换成需要被拨打电话的号码
char msg[] = "ILoveMCU.taobao.com";		//短信内容


// const char TCPServer[] = "103.44.145.245";		//TCP服务器地址
// const char Port[] = "10380";						//端口

unsigned int count = 0;

struct
{
	char GPS_Buffer[150];
	bool isGetData;		//是否获取到GPS数据
	bool isParseData;	//是否解析完成
	char UTCTime[11];		//UTC时间
	char latitude[11];		//纬度
	char N_S[2];		//N/S
	char longitude[12];		//经度
	char E_W[2];		//E/W
	bool isUsefull;		//定位信息是否有效
} Save_Data;

void setup() {
	pinMode(L, OUTPUT);
	digitalWrite(L, LOW);

	DebugSerial.begin(9600);
	GprsSerail.begin(9600);

	Timer1.initialize(1000);
	Timer1.attachInterrupt(Timer1_handler);

	initGprs();		//初始化模块

	if (sendCommandReceive2Keyword("AT+QGNSSC?\r\n", "+QGNSSC: 1","OK\r\n", 1000, 1) == Success);
	else if (sendCommand("AT+QGNSSC=1\r\n", "OK\r\n", 100, 10) == Success);
	else errorLog(5);

	DebugSerial.println("\r\nsetup end!");

	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;

	clrGprsRxBuffer();
}

void loop() {
	

	while(1)
	{
		if (sendCommand("AT+QGNSSRD=\"NMEA/RMC\"\r\n", "OK\r\n", 2000, 10) == Success);
		else errorLog(6);

		Save_Data.isGetData = true;
		memset(Save_Data.GPS_Buffer, 0, 150);      //清空
		memcpy(Save_Data.GPS_Buffer, gprsRxBuffer, gprsBufferCount);

		DebugSerial.print("\r\n==========Save_Data.GPS_Buffer==========\r\n");
		DebugSerial.print(Save_Data.GPS_Buffer); //输出接收到的信息

		parseGpsBuffer();
		printGpsBuffer();

	}



}


void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		DebugSerial.print("Save_Data.UTCTime = ");
		DebugSerial.println(Save_Data.UTCTime);

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			DebugSerial.print("Save_Data.latitude = ");
			DebugSerial.println(Save_Data.latitude);
			DebugSerial.print("Save_Data.N_S = ");
			DebugSerial.println(Save_Data.N_S);
			DebugSerial.print("Save_Data.longitude = ");
			DebugSerial.println(Save_Data.longitude);
			DebugSerial.print("Save_Data.E_W = ");
			DebugSerial.println(Save_Data.E_W);
		}
		else
		{
			DebugSerial.println("GPS DATA is not usefull!");
		}
		
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		DebugSerial.println("**************");
		DebugSerial.println(Save_Data.GPS_Buffer);

		
		for (int i = 0 ; i <= 6 ; i++)
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
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取纬度信息
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

void initGprs()
{
	if (sendCommand("AT\r\n", "OK\r\n", 1000, 10) == Success);
	else errorLog(1);

	delay(10);

	//为了不插卡也能测试GPS，这里屏蔽对CPIN和CRGE指令的检测
	// if (sendCommandReceive2Keyword("AT+CPIN?\r\n", "READY", "OK\r\n", 3000, 10) == Success);
	// else errorLog(2);
	// delay(10);

	// if (sendCommandReceive2Keyword("AT+CREG?\r\n", ",1", "OK\r\n", 3000, 10) == Success);	//本地SIM卡
	// else if(sendCommandReceive2Keyword("AT+CREG?\r\n", ",5", "OK\r\n", 3000, 10) == Success	);//漫游SIM卡
	// else	errorLog(3);
	// delay(10);

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
				return Success;
			}
		}
		Time_Cont = 0;
	}
	DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
	DebugSerial.print(gprsRxBuffer);//
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