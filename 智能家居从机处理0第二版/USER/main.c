#include "delay.h"  //ÑÓÊ±
#include "sys.h"    //Æô¶¯ÎÄ¼ş£¨ºÜÖØÒª£©
#include "usart.h"  //´®¿Ú1
#include "usart3.h" //´®¿Ú3
#include "oled.h"   //OLEDÏÔÊ¾
#include "timer.h"  //¶¨Ê±Æ÷¼ÆÊıÆ÷ÖĞ¶Ï
#include "led.h"    //LEDÏÔÊ¾
#include "rx.h"     //´ÓÖ÷»ú½ÓÊÕµ½µÄÊı¾İ½øĞĞ´¦Àí·ÖÎö
#include "adc.h"    //ADC¸÷Â·Í¨µÀ²É¼¯
#include "manage.h" //ADC²É¼¯ĞÅÏ¢´¦Àí
#include "tsensor.h"//ÄÚ²¿Ö÷¿ØĞ¾Æ¬ÎÂ¶ÈÌáÊ¾
#include "dht11.h"  //ÊÒÄÚÎÂÊª¶ÈÌáÊ¾
#include "start.h"  //ĞèÒª¼ì²âµÄ¸÷¸öÄ£¿é¼ì²â³õÊ¼»¯
#include "driver.h" //²½½øµç»ú¿ØÖÆ
#include "stepmotor.h"  //´°»§ÒÔ¼°´°Á±¿ØÖÆ
 
 

u16 t;        //´®¿Ú1·¢ËÍÊı¾İÓë½ÓÊÕµ½µÄ³¤¶ÈÒ»ÖÂ
u16 len;      //´®¿Ú1½ÓÊÕµ½Êı¾İµÄ³¤¶È
//u8 Humiture_t=0;
u8 Humiture_Temperature_Indoor;  //ÊÒÄÚÎÂ¶ÈÖµ
u8 Humiture_Humidity_Indoor;     //ÊÒÄÚÊª¶ÈÖµ
u8 Humiture_Temperature_Outdoor;  //ÊÒÍâÎÂ¶ÈÖµ
u8 Humiture_Humidity_Outdoor;     //ÊÒÍâÊª¶ÈÖµ

u16 adcx_Voltage;         //Òı½Å²âÁ¿Ğ¡µçÑ¹£¨3.3VÒÔÄÚ£©
float temp_Voltage;       
short temp_Control_Chip;  //Ö÷¿ØĞ¾Æ¬ÎÂ¶ÈÖµ£¨±£ÁôÁËÁ½Î»Ğ¡Êı£©



int main(void)
{		
	delay_init();	    	 //ÑÓÊ±º¯Êı³õÊ¼»¯	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //ÉèÖÃNVICÖĞ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
	uart_init(4800);	 //´®¿Ú1³õÊ¼»¯Îª9600
	usart3_init(115200); //´®¿Ú3³õÊ¼»¯Îª115200
 	LED_Init();			     //LED¶Ë¿Ú³õÊ¼»¯
	OLED_Init();         //OLED³õÊ¼»¯
	Adc_Init();          //ADCÍ¨µÀ³õÊ¼»¯
	TIM7_Int_Init(899,799);  //´®¿Ú3½ÓÊÕ·¢ËÍÊ±¼ä¶¨Ê±³õÊ¼»¯
	Driver_Init_X();		 //XÖáÇı¶¯Æ÷³õÊ¼»¯
	Driver_Init_Y();		 //YÖáÇı¶¯Æ÷³õÊ¼»¯
	TIM1_OPM_RCR_Init(999,72-1); //1MHz¼ÆÊıÆµÂÊ  µ¥Âö³å+ÖØ¸´¼ÆÊıÄ£Ê½£¨XÖá£©
	TIM8_OPM_RCR_Init(999,72-1); //1MHz¼ÆÊıÆµÂÊ  µ¥Â+-9ö³å+ÖØ¸´¼ÆÊıÄ£Ê½£¨YÖá£©
	T_Adc_Init();           //Ö÷¿ØĞ¾Æ¬²É¼¯ÎÂ¶È³õÊ¼»¯
	Humiture_Initialize();  //¸÷¸öÄ£¿é¼ì²â³õÊ¼»¯
	OLED_ShowString(30,0, "a: 00.00" ,12);	 //Éè¶¨Ö÷¿ØĞ¾Æ¬ÎÂ¶È³õÊ¼Öµ
	OLED_ShowString(30,12,"b:  C",12);       //ÊÒÄÚÎÂ¶È
	OLED_ShowString(30,24,"c:  %",12);       //ÊÒÄÚÊª¶È
	OLED_ShowString(30,36,"d:  C",12);       //ÊÒÍâÎÂ¶È
	OLED_ShowString(30,48,"e:  %",12);       //ÊÒÍâÊª¶È
	OLED_Refresh_Gram();                    //OLED¸üĞÂÏÔÊ¾
	
 while(1)
	{
			RX_1();                          //½ÓÊÕÖ÷»ú¹ıÀ´µÄÊı¾İ
		  Adc_Voltage_Transition();        //Òı½Å²É¼¯µ½µÄµçÑ¹
		  Adc_Control_Chip_Temperature();	 //Ö÷¿ØĞ¾Æ¬²É¼¯µ½µÄÎÂ¶È
			Adc_Humiture_Measure_Indoor();          //ÊÒÄÚ²É¼¯µ½µÄÎÂÊª¶È
		  Adc_Humiture_Measure_Outdoor();          //ÊÒÄÚ²É¼¯µ½µÄÎÂÊª¶È
      Window_Control();                //´°»§¿ØÖÆ
			Curtain_Control();               //´°Á±¿ØÖÆ
		
 } 
}
