#include "start.h"
#include "dht11.h"
#include "delay.h"
#include "oled.h"

void Humiture_Initialize()    //检测是否有温湿度模块存在
{
	while(DHT11_Init_Indoor())	//DHT11初始化	
	{
		OLED_ShowString(30,12,"DHT11 Error",12);
		OLED_Refresh_Gram();
		delay_ms(200);
    OLED_ShowString(30,12,"DHT11 Correct",12);
 		delay_ms(200);
		OLED_Refresh_Gram();    //更新OLED显示
		OLED_Clear();           //清除OLED显示
		
		while(DHT11_Init_Outdoor())	//DHT11初始化	
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