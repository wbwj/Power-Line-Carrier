#include "manage.h"
#include "adc.h"
#include "oled.h"
#include "delay.h"
#include "tsensor.h"
#include "dht11.h"
#include "led.h"
#include "raindrop.h"


extern u16 adcx_Voltage;           //ADC1通道1电压转换
extern float temp_Voltage;         
extern short temp_Control_Chip;
//extern u8 Humiture_t;                
extern u8 Humiture_Temperature_Indoor;   //室内温度
extern u8 Humiture_Humidity_Indoor;      //室内湿度
extern u8 Humiture_Temperature_Outdoor;   //室外温度
extern u8 Humiture_Humidity_Outdoor;      //室外湿度

void Adc_Voltage_Transition()             //用来测量小于3.3V电压使用    
{
		adcx_Voltage=Get_Adc_Average(ADC_Channel_1,10);
//		OLED_ShowNum(0,0,adcx_Voltage,4,12);
		temp_Voltage=(float)adcx_Voltage*(3.3/4096);  
		adcx_Voltage=temp_Voltage;
//		OLED_ShowNum(0,12,adcx_Voltage,1,12);
		temp_Voltage-=adcx_Voltage;
		temp_Voltage*=20;
		OLED_ShowNum(0,24,temp_Voltage,3,12);
	  OLED_Refresh_Gram();
	if(adcx_Voltage>1000)
		{
//			LED0=!LED0;
			delay_ms(1000);
		}
}

void Adc_Control_Chip_Temperature()    //用于测量主控芯片温度
{
	temp_Control_Chip=Get_Temprate();	  //得到主控芯片温度
		if(temp_Control_Chip<0)            
		{
			temp_Control_Chip=-temp_Control_Chip; 
			OLED_ShowString(42,0,"-",12);	   //如果检测到的数值小于零，则显示负号
		}else OLED_ShowString(42,0," ",12);	    //如果检测到的数值大于零，则不显示（即为正号）
		OLED_ShowNum(48,0,temp_Control_Chip/100,2,12);		 //显示整数部分
		OLED_ShowNum(66,0,temp_Control_Chip%100,2,12);	  //显示小数部分
		OLED_Refresh_Gram();     //OLED更新显示
}

void Adc_Humiture_Measure_Indoor()     //测量室内温湿度
{
	DHT11_Read_Data_Indoor(&Humiture_Temperature_Indoor,&Humiture_Humidity_Indoor);	
	OLED_ShowNum(42,12,Humiture_Temperature_Indoor,2,12);
	OLED_ShowNum(42,24,Humiture_Humidity_Indoor,2,12);
	OLED_Refresh_Gram();
}

void Adc_Humiture_Measure_Outdoor()     //测量室外温湿度
{
	DHT11_Read_Data_Outdoor(&Humiture_Temperature_Outdoor,&Humiture_Humidity_Outdoor);	
	OLED_ShowNum(42,36,Humiture_Temperature_Outdoor,2,12);
	OLED_ShowNum(42,48,Humiture_Humidity_Outdoor,2,12);
	OLED_Refresh_Gram();
}


void Adc_Co_Gas(void)                  //有害气体含量 （直接使用了ADC电压直接转换）  
{
	
}  


void Adc_Raindrop_Indoor()             //室内雨滴传感器（判断是否会出现水贱到从机主控芯片上面）
{
	if(raindrop_Indoor==0)
	{
		LED4=0;
	}
	else if(raindrop_Indoor==1)
	{
		LED4=1;
	}
}

void Adc_Raindrop_outdoor()            //室外雨滴传感器（判断外面是否下雨）
{
	
}
