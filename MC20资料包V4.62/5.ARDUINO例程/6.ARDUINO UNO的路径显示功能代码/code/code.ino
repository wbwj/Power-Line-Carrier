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


//本程序基本已经用到了ARDUINO UNO的RAM空间的极限，基本上90%以上，如果有添加其他功能，
//请最好更换mega2560开发板去使用，以防止RAM空间溢出造成代码运行异常
// #include <TimerOne.h>
#include <SoftwareSerial.h>


#define GprsSerail  Serial
SoftwareSerial DebugSerial(10, 11); // RX, TX			

// #define DebugSerial Serial
// #define GprsSerail Serial3	//使用mega2560板子可以用Serial3，不用SoftSerial调试，硬件串口比虚拟的更稳定


#define Success 1U
#define Failure 0U

int L = 13; //LED指示灯引脚

unsigned long  Time_Cont = 0;       //定时器计数器

const unsigned int gprsRxBufferLength = 300;
char gprsRxBuffer[gprsRxBufferLength];
unsigned int gprsBufferCount = 0;

const char OneNetServer[] = "183.230.40.33";       //不需要修改
//const char OneNetServer[] = "api.heclouds.com"; 

const char device_id[] = "3225187";    //修改为自己的设备ID
const char API_KEY[] = "R9xO5NZm6oVI4YBHvCPKEqtwYtMA";    //修改为自己的API_KEY
const char sensor_gps[] = "location";


unsigned int count = 0;

struct
{
	char GPS_Buffer[120];
	bool isGetData;		//是否获取到GPS数据
	bool isParseData;	//是否解析完成
	// char UTCTime[11];		//UTC时间
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

	// Timer1.initialize(1000);
	// Timer1.attachInterrupt(Timer1_handler);

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
		gpsRead();  //获取GPS数据
		parseGpsBuffer();//解析GPS数据
		printGpsBuffer();//输出解析后的数据  ,包括发送到OneNet服务器
		delay(3000);
	}



}



void gpsRead()
{
	if (sendCommand("AT+QGNSSRD=\"NMEA/RMC\"\r\n", "OK\r\n", 2000, 10) == Success);
	else errorLog(6);

	Save_Data.isGetData = true;
	memset(Save_Data.GPS_Buffer, 0, 120);      //清空
	memcpy(Save_Data.GPS_Buffer, gprsRxBuffer, gprsBufferCount);

	DebugSerial.print("\r\n==========Save_Data.GPS_Buffer==========\r\n");
	DebugSerial.print(Save_Data.GPS_Buffer); //输出接收到的信息	
}


void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		// DebugSerial.print("Save_Data.UTCTime = ");
		// DebugSerial.println(Save_Data.UTCTime);

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

			postGpsDataToOneNet(Save_Data.longitude, Save_Data.latitude);
			digitalWrite(L,!digitalRead(L));	//发送一次数据，LED翻转一下
		}
		else
		{
			DebugSerial.println("GPS DATA is not usefull!");
		}
		
	}
}



void postGpsDataToOneNet(char* lon_temp, char* lat_temp)
{

	char send_buf[300] = {0};
	char text[100] = {0};
	char tmp[25] = {0};
	char lon_str_end[15] = {0};
	char lat_str_end[15] = {0};
	dtostrf(longitudeToOnenetFormat(lon_temp), 3, 6, lon_str_end); //转换成字符串输出
	dtostrf(latitudeToOnenetFormat(lat_temp), 2, 6, lat_str_end); //转换成字符串输出

	//连接服务器
	memset(send_buf, 0, 300);    //清空
	strcpy(send_buf, "AT+QIOPEN=\"TCP\",\"");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\",\"80\"\r\n");
	if (sendCommand(send_buf, "CONNECT", 10000, 5) == Success);
	else errorLog(7);

	//发送数据
	if (sendCommand("AT+QISEND\r\n", ">", 3000, 1) == Success);
	else errorLog(8);

	memset(send_buf, 0, 300);    //清空

	/*准备JSON串*/	
	//ARDUINO平台不支持sprintf的double的打印，只能转换到字符串然后打印
	sprintf(text, "{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%s,\"lat\":%s}}]}]}"
	        , sensor_gps, lon_str_end, lat_str_end);

	/*准备HTTP报头*/
	send_buf[0] = 0;
	strcat(send_buf, "POST /devices/");
	strcat(send_buf, device_id);
	strcat(send_buf, "/datapoints HTTP/1.1\r\n"); //注意后面必须加上\r\n
	strcat(send_buf, "api-key:");
	strcat(send_buf, API_KEY);
	strcat(send_buf, "\r\n");
	strcat(send_buf, "Host:");
	strcat(send_buf, OneNetServer);
	strcat(send_buf, "\r\n");
	sprintf(tmp, "Content-Length:%d\r\n\r\n", strlen(text)); //计算JSON串长度
	strcat(send_buf, tmp);
	strcat(send_buf, text);

	if (sendCommand(send_buf, send_buf, 3000, 1) == Success);
	else errorLog(9);

	char sendCom[2] = {0x1A};
	if (sendCommand(sendCom, "\"succ\"}", 3000, 1) == Success);
	else errorLog(10);

	if (sendCommand("AT+QICLOSE\r\n", "OK\r\n", 3000, 1) == Success);
	else errorLog(11);

	if (sendCommand("AT+QIDEACT\r\n", "OK\r\n", 3000, 1) == Success);
	else errorLog(11);
}

double longitudeToOnenetFormat(char *lon_str_temp)
{
	double lon_temp = 0;
	long lon_Onenet = 0;
	int dd_int = 0;
	long mm_int = 0;
	double lon_Onenet_double = 0;

	lon_temp = atof(lon_str_temp);
	lon_Onenet = lon_temp * 100000; //转换为整数

	dd_int = lon_Onenet / 10000000; //取出dd

	mm_int = lon_Onenet % 10000000; //取出MM部分


	lon_Onenet_double = dd_int + (double)mm_int / 60 / 100000; //换算为Onenet格式


	return lon_Onenet_double;
}

double latitudeToOnenetFormat(char *lat_str_temp)
{
	double lat_temp = 0;
	long lat_Onenet = 0;
	int dd_int = 0;
	long mm_int = 0;

	double lat_Onenet_double = 0;

	lat_temp = atof(lat_str_temp);
	lat_Onenet = lat_temp * 100000; //转换为整数

	dd_int = lat_Onenet / 10000000; //取出dd

	mm_int = lat_Onenet % 10000000; //取出MM部分

	lat_Onenet_double = dd_int + (double)mm_int / 60 / 100000; //换算为Onenet格式


	return lat_Onenet_double;
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
						// case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	
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

	if (sendCommandReceive2Keyword("AT+CPIN?\r\n", "READY", "OK\r\n", 3000, 10) == Success);
	else errorLog(2);
	delay(10);

	if (sendCommandReceive2Keyword("AT+CREG?\r\n", ",1", "OK\r\n", 3000, 10) == Success);	//本地SIM卡
	else if(sendCommandReceive2Keyword("AT+CREG?\r\n", ",5", "OK\r\n", 3000, 10) == Success	);//漫游SIM卡
	else	errorLog(3);
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

		unsigned long time;
		time = micros();

		while (micros() < Timeout*1000 + time)
		{
			gprsReadBuffer();
			if (strstr(gprsRxBuffer, Response) != NULL)
			{
				DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
				DebugSerial.print(gprsRxBuffer); //输出接收到的信息
				return Success;
			}
		}
		DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
		DebugSerial.print(gprsRxBuffer); //输出接收到的信息
		clrGprsRxBuffer();
	}
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

		unsigned long time;
		time = micros();

		while (micros() < Timeout*1000 + time)
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
	}
	DebugSerial.print("\r\n==========receive AT Command:==========\r\n");
	DebugSerial.print(gprsRxBuffer);//输出接收到的信息
	clrGprsRxBuffer();
	return Failure;
}




// void Timer1_handler(void)
// {
// 	Time_Cont++;
// }



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