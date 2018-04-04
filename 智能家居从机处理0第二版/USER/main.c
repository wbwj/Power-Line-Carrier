#include "delay.h"  //延时
#include "sys.h"    //启动文件（很重要）
#include "usart.h"  //串口1
#include "usart3.h" //串口3
#include "oled.h"   //OLED显示
#include "timer.h"  //定时器计数器中断
#include "led.h"    //LED显示
#include "rx.h"     //从主机接收到的数据进行处理分析
#include "adc.h"    //ADC各路通道采集
#include "manage.h" //ADC采集信息处理
#include "tsensor.h"//内部主控芯片温度提示
#include "dht11.h"  //室内温湿度提示
#include "start.h"  //需要检测的各个模块检测初始化
#include "driver.h" //步进电机控制
#include "stepmotor.h"  //窗户以及窗帘控制
#include "raindrop.h"  //窗外和窗内是否有水

/*************************引脚定义******************************
PG8：X轴方向控制引脚
PG9：X轴使能脚
PA11：X轴PWM控制脚
PG6：Y轴方向控制脚
PG7：Y轴使能脚
PC9：Y轴PWM方向控制脚
PG11：室内温湿度传感器输入端
PG12：室外温湿度传感器输入端
PA1：采集外部电压（3.3V 如果需要测量高电压则需要进过转压到0-3.3V）有害气体检测使用
PA2：采集外部电压（3.3V 如果需要测量高电压则需要进过转压到0-3.3V）
PB1：LED0
PB2：	LED1
PB5：LED2
PE5：LED3
PF9：LED4
PF10：LED5
PF11：LED6
PG1：LED7
PG2：LED8
PG3：LED9
PG4：LED10
PG5：使用继电器模拟开门关门
*****************************************************************/
 
 

u16 t;        //串口1发送数据与接收到的长度一致
u16 len;      //串口1接收到数据的长度
//u8 Humiture_t=0;
u8 Humiture_Temperature_Indoor;  //室内温度值
u8 Humiture_Humidity_Indoor;     //室内湿度值
u8 Humiture_Temperature_Outdoor;  //室外温度值
u8 Humiture_Humidity_Outdoor;     //室外湿度值

u16 adcx_Voltage;         //引脚测量小电压（3.3V以内）
float temp_Voltage;       
short temp_Control_Chip;  //主控芯片温度值（保留了两位小数）



int main(void)
{		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(4800);	   //串口1初始化为9600
	usart3_init(115200); //串口3初始化为115200
 	LED_Init();			     //LED端口初始化
	OLED_Init();         //OLED初始化
	Adc_Init();          //ADC通道初始化
	TIM7_Int_Init(899,799);  //串口3接收发送时间定时初始化
	Driver_Init_X();		     //X轴驱动器初始化
	Driver_Init_Y();		     //Y轴驱动器初始化
	TIM1_OPM_RCR_Init(999,72-1);   //1MHz计数频率  单脉冲+重复计数模式（X轴）
	TIM8_OPM_RCR_Init(999,72-1);   //1MHz计数频率  单脉冲+重复计数模式（Y轴）
	TIM6_Int_Init(9999,7199);   //10KHZ计数，延时1s
	T_Adc_Init();           //主控芯片采集温度初始化
	raindrop_Init();        //雨滴传感器初始化
	Humiture_Initialize();  //各个模块检测初始化
	OLED_ShowString(30,0, "a: 00.00" ,12);	 //设定主控芯片温度初始值
	OLED_ShowString(30,12,"b:  C",12);       //室内温度
	OLED_ShowString(30,24,"c:  %",12);       //室内湿度
	OLED_ShowString(30,36,"d:  C",12);       //室外温度
	OLED_ShowString(30,48,"e:  %",12);       //室外湿度
	OLED_Refresh_Gram();                     //OLED更新显示
	
 while(1)
	{
			RX_1();                          //接收主机过来的数据
		  Adc_Voltage_Transition();        //引脚采集到的电压
		  Adc_Control_Chip_Temperature();	 //主控芯片采集到的温度
			Adc_Humiture_Measure_Indoor();   //室内采集到的温湿度
		  Adc_Humiture_Measure_Outdoor();  //室内采集到的温湿度
      Window_Control();                //窗户控制
			Curtain_Control();               //窗帘控制
		  Adc_Raindrop_Indoor();           //室内雨滴传感器（判断是否会出现水贱到从机主控芯片上面）
		  Adc_Raindrop_outdoor();          //室外雨滴传感器（判断外面是否下雨）		
 } 
}
