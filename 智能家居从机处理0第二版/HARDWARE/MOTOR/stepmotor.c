#include "stepmotor.h"
#include "delay.h"
#include "manage.h"
#include "driver.h"
#include "led.h"
#include "window.h"
#include "SFE_TSL2561.h" 
#include "raindrop.h"  
#include "tsensor.h"
#include "adc.h"
#include "dht11.h"

extern u8 Humiture_Temperature_Indoor;    //室内温度
extern u8 Humiture_Humidity_Indoor;       //室内湿度
extern u8 Humiture_Temperature_Outdoor;   //室外温度
extern u8 Humiture_Humidity_Outdoor;      //室外湿度

void Window_Control()  //模拟窗户控制
{
	if(Humiture_Temperature_Outdoor-5>Humiture_Temperature_Indoor)
	{
		Locate_Abs_X(0,50000);
		Locate_Abs_Y(0,50000);
		Locate_Rle_Y(25600,50000,CCW);
		Locate_Rle_X(25600,50000,CW);
	}
//	if(windows_Right==0)
//	{
//		LED7=!LED7;
//	}
}


void Curtain_Control() //模拟窗帘控制
{
	
}



