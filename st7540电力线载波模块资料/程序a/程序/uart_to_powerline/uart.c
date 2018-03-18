/*
文件名：uart.c
作者：北京交通大学 思源0702 杨孜
时间：2012-8-7
功能：

*/
#include "uart.h"
#include "queue.h"

Queue uart_rx_queue;
Queue uart_tx_queue;

static u8 uart_rx_queue_buff[64];
static u8 uart_tx_queue_buff[64];

u8 uart_busy=0;//0为空闲
u16 baud=4800;
void uart_ini(u16 b)
{
	baud=b;
	//初始化引脚：
	PD_DDR &= ~(1<<6);//输入（输出）
	PD_CR1 |= (1<<6);//上拉（浮空）
	PD_CR2 &= ~(1<<6);//无中断（中断）
	
	PD_DDR |= (1<<5);//输出（输入）
	PD_CR1 |= (1<<5);//推挽（开漏）
	PD_CR2 &= ~(1<<5);//低速（高速）
	//初始化uart
	b=16e6/b;  //16M外部时钟
	UART1_BRR2=((b&0xf000)>>8)+(b&0x000f);
	UART1_BRR1=((b&0x0ff0)>>4);
	//UART2_CR1=0;
	//UART2_CR2=0x6c;//01101100//准许发送中断
	UART1_CR2=0x2c;//00101100//禁止发送中断
	//UART2_CR3=0;
	//UART2_CR4=
	uart_busy=0;
	UART1_SR=0xc0;

	Queue_ini(uart_rx_queue_buff,sizeof(uart_rx_queue_buff),&uart_rx_queue);
	Queue_ini(uart_tx_queue_buff,sizeof(uart_tx_queue_buff),&uart_tx_queue);
}

char data_to_uart(void)
{
	s8 c;
	if(UART1_SR & 0x40)//接收中断
	{
			if(Queue_get(&c,&uart_tx_queue) == 0)
			{
			    UART1_SR &= ~0x40;
				  UART1_DR=(char)c;
				  return 1;
			}
			else
			{
				  return 0;
			}
	}
	else
	{
		  return 0;
	}
	//return c;
}

@far @interrupt void uart1_int(void)
{
	if(UART1_SR & 0x20)//接收中断
	{
		Queue_set(UART1_DR,&uart_rx_queue);
//		START_TASK(TASK_CMD);
		//更改晶振
	}
	UART1_SR&=~0x20;
}
