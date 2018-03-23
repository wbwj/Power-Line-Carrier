#include "rx.h"
#include "usart.h"
#include "led.h"
#include "driver.h"

extern u16 t;  
extern u16 len;	

void RX_1()
{	
	if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				
				if(USART_RX_BUF[t]==48)
				{
					LED0=!LED0;            //LED模拟房间内灯的亮灭
					Locate_Rle_Y(25600,50000,CW);
				}
				else if(USART_RX_BUF[t]==49)
				{
					LED1=!LED1;
				}
			  else if(USART_RX_BUF[t]==50)
				{
					LED2=!LED2;
				}
				else if(USART_RX_BUF[t]==51)
				{
					LED3=!LED3;
				}
				else if(USART_RX_BUF[t]==52)
				{
					LED4=!LED4;
				}
				else if(USART_RX_BUF[t]==53)
				{
					LED5=!LED5;
				}
				else if(USART_RX_BUF[t]==54)
				{
					LED6=!LED6;
				}
				else if(USART_RX_BUF[t]==55)
				{
					LED7=!LED7;
				}
				else if(USART_RX_BUF[t]==56)
				{
					LED8=!LED8;
				}
				else if(USART_RX_BUF[t]==57)
				{
					LED9=!LED9;
				}
				else if(USART_RX_BUF[t]==97)
				{
					LED10=!LED10;               
				}
				else if(USART_RX_BUF[t]==98)
				{
					front_door = !front_door;   //模拟大门打开
				}
				printf("  %d\r\n",USART_RX_BUF[t]);
			}
			USART_RX_STA=0;
    }
}	 