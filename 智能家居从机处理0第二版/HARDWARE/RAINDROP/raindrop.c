#include "raindrop.h"

void raindrop_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_8;				 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.4.8
  GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_8);	
	
}

