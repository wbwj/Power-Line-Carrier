#include "start.h"
#include "dht11.h"
#include "delay.h"
#include "oled.h"

void Humiture_Initialize()    //温湿度传感器检测
{
		while(DHT11_Init_Indoor())	//检测室内温湿度模块是否存在
	{
		OLED_ShowString(30,12,"DHT11 Error",12);
		OLED_Refresh_Gram();
		delay_ms(200);
    OLED_ShowString(30,12,"DHT11 Correct",12);
 		delay_ms(200);
		OLED_Refresh_Gram();    //更新OLED显示
		OLED_Clear();           //清除OLED显示
		
		while(DHT11_Init_Outdoor())	//检测室内温湿度传感器是否存在	
	{
		OLED_ShowString(30,24,"DHT11 Error",12);
		OLED_Refresh_Gram();
		delay_ms(200);
    OLED_ShowString(30,24,"DHT11 Correct",12);
 		delay_ms(200);
		OLED_Refresh_Gram();    //更新OLED显示
		OLED_Clear();           //清除OLED显示
	}
	}
}