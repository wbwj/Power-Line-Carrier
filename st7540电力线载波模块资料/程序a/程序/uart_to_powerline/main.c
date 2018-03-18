/*
乐电新南
	
*/
#include <stdlib.h>
#include "main.h"
#include "queue.h"
#include "uart.h"
#include "bit_define.h"
#include "power_line.h"


u16 sys_time=0;
u8 SYS_task=0;
void CLK_ini(void)
{
	*(u8*)0x4807 |= (1<<3);
//	CLK_ECKCR=1;//打开外部振荡器
	CLK_SWR=0xb4;//切换为外部震荡;E1:HSI;D2:LSI
	CLK_SWCR|=2;//开始切换
	CLK_CKDIVR=0x0;//不分频
	
//	CLK_PCKENR1=0x18;//低功耗，关闭不用的设备
//	CLK_PCKENR2=0x08;
}
void TIM4_ini(void)
{
	TIM4_IER = 0;//禁止中断
	TIM4_EGR = 1;//允许产生更新事件
	TIM4_PSCR= 7;//分频
	TIM4_ARR = 255;
	TIM4_CNTR= 255;
	TIM4_CR1 =0x01; //允许计数工作
	TIM4_IER = 1;//禁止中断
	//TIM4_SR  = 
	//TIM4_CNTR= 
	TIM4_EGR = 1;
}

void GPIO_init(void)
{
//cd/pd	
	PA_DDR &= ~BIT3;//输入
	PA_CR1 &= ~BIT3;//float
	PA_CR2 &= ~BIT3;//无中断
//rxd	
	PB_DDR &= ~BIT4;//输入
	PB_CR1 &= ~BIT4;//float
	PB_CR2 &= ~BIT4;//无中断
//reg/data	
	PB_DDR |= BIT5;//输出
	PB_CR1 |= BIT5;//推挽
	PB_CR2 &= ~BIT5;//低速
	PB_ODR &= ~BIT5;//输出
	
//rx/tx
	PC_DDR |= BIT3;//输出
	PC_CR1 |= BIT3;//推挽
	PC_CR2 &= ~BIT3;//低速
  PC_ODR |= BIT3;//输出
//txd
	PC_DDR |= BIT4;//输出
	PC_CR1 |= BIT4;//推挽
	PC_CR2 &= ~BIT4;//低速
	PC_ODR &= ~BIT4;//输出
//bu/theam
	PC_DDR &= ~BIT5;//输入
	PC_CR1 &= ~BIT5;//float
	PC_CR2 &= ~BIT5;//无中断
//clr/t
	PC_DDR &= ~BIT6;//输入
	PC_CR1 &= ~BIT6;//float
	PC_CR2 &= ~BIT6;//无中断
//rsto
	PC_DDR &= ~BIT7;//输入
	PC_CR1 &= ~BIT7;//float
	PC_CR2 &= ~BIT7;//无中断
//adcin
	PD_DDR &= ~BIT3;//输入
	PD_CR1 &= ~BIT3;//float
	PD_CR2 &= ~BIT3;//无中断
  //PD_ADC_TDR |= BIT3;//ADC
	
//	PD_DDR |= BIT3;//输出
//	PD_CR1 |= BIT3;//推挽
//	PD_CR2 |= BIT3;//低速
//  PD_ODR &= ~BIT3;
	
	
	
//uart/spi	
	PD_DDR |= BIT2;//输出
	PD_CR1 |= BIT2;//推挽
	PD_CR2 &= ~BIT2;//低速
	PD_ODR &= ~BIT2;
//wd		
	PD_DDR |= BIT4;//输出
	PD_CR1 |= BIT4;//推挽
	PD_CR2 &= ~BIT4;//低速
  PD_ODR &= ~BIT4;

	//while((PC_IDR & BIT6) == 0x0) ; //wait rtso == 1
	delay(1000) ;
	PD_ODR |= BIT4;    //watch dog
	delay(100) ;
	PD_ODR &= ~BIT4;

}


main()
{
//	EnableInt;
	//printf("aa");
	s8 uart_rx_queue_data = 0 ;
	u8 proc_cnt = 0 ;
	u32 power_line_tx_data_h = 0 ;
	u32 power_line_tx_data_l = 0 ;	
	u32 power_line_rx_data_h = 0 ;
	u32 power_line_rx_data_l = 0 ;
	u8 uart_rx_queue_cnt = 0 ;
	u8 rand_delay_cnt = 0 ;
	delay(10000);
	GPIO_init();
	power_line_init();	
	CLK_ini();
//	TIM4_ini();
	uart_ini(4800);
//	cmd_ini();
//	bt_ini();
	srand(0);
	
	EnableInt;

	sys_time=0;
	while (1)
	{

    uart_rx_queue_cnt = Queue_cnt(&uart_rx_queue) ;
    if((uart_rx_queue_cnt >= 8) && (rand_delay_cnt == 0))
		{
			    Queue_get(&uart_rx_queue_data,&uart_rx_queue);
			    if(((u8)uart_rx_queue_data) == 0x9b)
			    {
					  power_line_tx_data_h = (((u32)uart_rx_queue_data) & 0xff) << 24 ;
					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
					  power_line_tx_data_h = power_line_tx_data_h + ((((u32)uart_rx_queue_data) & 0xff) << 16) ;
					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
					  power_line_tx_data_h = power_line_tx_data_h + ((((u32)uart_rx_queue_data) & 0xff) << 8) ;
					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
					  power_line_tx_data_h = power_line_tx_data_h + (((u32)uart_rx_queue_data) & 0xff) ;
						
						Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
					  power_line_tx_data_l = ((((u32)uart_rx_queue_data) & 0xff) << 24) ;
						Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
					  power_line_tx_data_l = power_line_tx_data_l + ((((u32)uart_rx_queue_data) & 0xff) << 16) ;
					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
					  power_line_tx_data_l = power_line_tx_data_l + ((((u32)uart_rx_queue_data) & 0xff) << 8) ;
					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
					  power_line_tx_data_l = power_line_tx_data_l + (((u32)uart_rx_queue_data) & 0xff) ;
						
					  data_trans(power_line_tx_data_h,power_line_tx_data_l);
						curr_state = 0;						
				  }
				  else ;					
		}
		else 
		{			
			if(data_recv(&power_line_rx_data_h,&power_line_rx_data_l,1) != 0)
			{
				Queue_set(((s8)(power_line_rx_data_h >> 24)),&uart_tx_queue);
				Queue_set(((s8)(power_line_rx_data_h >> 16)),&uart_tx_queue);
				Queue_set(((s8)(power_line_rx_data_h >>  8)),&uart_tx_queue);
				Queue_set(((s8)(power_line_rx_data_h >>  0)),&uart_tx_queue);
				
				Queue_set(((s8)(power_line_rx_data_l >> 24)),&uart_tx_queue);
				Queue_set(((s8)(power_line_rx_data_l >> 16)),&uart_tx_queue);
				Queue_set(((s8)(power_line_rx_data_l >>  8)),&uart_tx_queue);
				Queue_set(((s8)(power_line_rx_data_l >>  0)),&uart_tx_queue);
				//delay(1000);
				
			}
			else ;
			
			//if(rand_delay_cnt > 0)
			//{
			//	rand_delay_cnt--;
			//}
			//else ;
		}	
		data_to_uart();

	}
}
void wait(vu32 i)
{
	while(i--);
}

@far @interrupt void Tim4_int(void)//61Hz
{
	TIM4_SR =0;
	//软件定时器
}
@far @interrupt void trap_int(void)//软中断
{
	
}