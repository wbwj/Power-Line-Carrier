/*
乐电新南
	
*/

#include "bt.h"
#include "STimer.h"

#define AD_CHANNEL	2

#define LED_MIN		5

u16 LED_t=100;
u16 LED_tick=0;
u8 isBeep=0;

float ad_value=0;//ad原始值

float AD_K=70.523416f;//比例
float votage_lim=12;		//报警门限


static STimer timer;

void bt_ini(void)
{
//AD设置
	PD_DDR &= ~BIT3;//输入
	PD_CR1 &= ~BIT3;//float
	PD_CR2 &= ~BIT3;//无中断
	
	ADC_CR1 = 0x72; //预分频fADC = fMASTER/18，连续转换模式
	ADC_CR2 = 0x00; //数据左对齐
	
	ADC_CSR = AD_CHANNEL; //选择AINx作为输入通道
	ADC_CR1 |= 0x01; //启动ADC
	//如果是第一次启动ADC，则需要等待时间>7us，确保ADC电源稳定
	ADC_CR1 |= 0x01; //ADON=1,启动ADC
//读取设置
	if(*(float*)EE_AD_K)
	{
		AD_K=*(float*)EE_AD_K;
	}
	if(*(float*)EE_votage_lim)
	{
		votage_lim=*(float*)EE_votage_lim;
	}
//定时器设置
	timer.time=100;//100个tick
	timer.cb=bt_pro;
	timer.p=0;
	STimer_start(&timer);
}

void bt_pro(void)//61Hz
{
	static int time=0;
	u16 ad;

	time++;

	while(!(ADC_CSR & 0x80));
	ad=((((unsigned int)ADC_DRH)<<2)+ADC_DRL);
	
	ADC_CSR =AD_CHANNEL; //选择AIN3作为输入通道
	ADC_CR1 |= 0x01; //启动ADC
	//如果是第一次启动ADC，则需要等待时间>7us，确保ADC电源稳定
	ADC_CR1 |= 0x01; //ADON=1,启动ADC
	
	if(ad<1024)
	{
		ad_value= ad_value*0.95f+ad*0.05f;
	}
//PC_ODR ^= LED_P;

	LED_tick++;
	if(LED_tick>LED_t)
	{
		LED_tick=0;
		//turn the led
		LED_ON;
		BEEP_OFF;
	}
	else if(LED_tick>=LED_MIN)
	{
		if(isBeep)
		{
			BEEP_ON;
		}
		else
		{
			LED_OFF;
		}
	}

	if(time%30==0)
	{
		if(AD_K==0)
		{
			printf("asdf\r\n");
		}
		ad=(u16)(votage_lim*AD_K);
		if(ad_value<ad)
		{
			LED_t=LED_MIN;
			//beep
			isBeep=1;
		}
		else
		{
			isBeep=0;
			LED_t=LED_MIN+(ad_value-ad)*2;
		}
	}
	//设置定时器
	timer.time=1;//5
}
