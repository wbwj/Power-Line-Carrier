#include "led.h"

	   

    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,PE端口时钟
 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1;				 //LED0-->PB.5 端口配置
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_1);						 //PB.5 输出高
// GPIO_SetBits(GPIOB,GPIO_Pin_2);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;	    		 //LED1-->PF.9 端口配置, 推挽输出
 GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11); 						 //PF.9 输出高 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOG, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOG,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); 						 //PE.5 输出高 
}
 
