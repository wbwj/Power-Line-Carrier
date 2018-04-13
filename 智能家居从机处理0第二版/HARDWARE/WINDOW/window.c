#include "window.h"


void windows_control_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;				 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.5.6
  GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6);	     //PA5.6拉高
}

