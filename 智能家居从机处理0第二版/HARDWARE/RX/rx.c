#include "rx.h"
#include "usart.h"
#include "led.h"
#include "driver.h"
#include "timer.h"

extern u16 t;     //用于长度的调度
extern u16 len;	  //接收发送数据的长度

void RX_1()       //接收数据函数
{	
	if(USART_RX_STA&0x8000)    //开启接收使能
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)      //从0-len发送
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				
				if(USART_RX_BUF[t]==48)
				{
					LED0=0;            //LED模拟房间内灯的亮灭
					LED1=0;
					LED2=0;
					LED3=0;
					LED4=0;
					LED5=0;
					LED6=0;
					LED7=0;
					LED8=0;
					LED9=0;
					LED10=0;
					Locate_Rle_Y(25600,50000,CCW);  //正转25600个脉冲（根据细分去计算圈数）
				}
				else if(USART_RX_BUF[t]==49)
				{
					LED0=1;
					LED1=1;
					LED2=1;
					LED3=1;
					LED4=1;
					LED5=1;
					LED6=1;
					LED7=1;
					LED8=1;
					LED9=1;
					LED10=1;
					Locate_Rle_X(25600,50000,CCW);  //正转25600个脉冲（根据细分去计算圈数）
				}
			  if(USART_RX_BUF[t]==50)
				{
					LED0=!LED0;
				}
				else if(USART_RX_BUF[t]==51)
				{
					LED1=!LED1;
				}
				else if(USART_RX_BUF[t]==52)
				{
					LED2=!LED2;
				}
				else if(USART_RX_BUF[t]==53)
				{
					LED3=!LED3;
				}
				else if(USART_RX_BUF[t]==54)
				{
					LED4=!LED4;
				}
				else if(USART_RX_BUF[t]==55)
				{
					LED5=!LED5;
				}
				else if(USART_RX_BUF[t]==56)
				{
					LED6=!LED6;
				}
				else if(USART_RX_BUF[t]==57)
				{
					LED7=!LED7;
				}
				else if(USART_RX_BUF[t]==97)
				{
					LED8=!LED8;  				
				}
				else if(USART_RX_BUF[t]==99)
				{
					LED9=!LED9;
				}
				else if(USART_RX_BUF[t]==100)
				{
					LED10=!LED10;
				} 
				if(USART_RX_BUF[t]==98)
				{
					front_door = 0;   //模拟大门打开
				}
				if(front_door == 0)
				{
					TIM_Cmd(TIM6,ENABLE);//开启定时器6
				}
				
				
				
				printf("  %d\r\n",USART_RX_BUF[t]);//用于调试时查看数据是否完整接收
			}
			USART_RX_STA=0;                 //清除缓存数据
    }
}	 