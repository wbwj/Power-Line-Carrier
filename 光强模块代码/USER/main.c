#include "delay.h"
#include "usart.h"
#include "stdio.h" 
#include "SFE_TSL2561.h"

int main(void)  
{
  
    unsigned int data[4];
    unsigned char dat; 
	  delay_init();
    uart_init(9600);   
    SFE_TSL2561_init();
    
    SFE_TSL2561_read(TSL2561_REG_COMMAND+TSL2561_REG_TIMING,1,&dat);
    dat|=1<<4;
    SFE_TSL2561_write(TSL2561_REG_COMMAND+TSL2561_REG_TIMING,1,&dat);
    while(1)
    {
//        delay_ms(1000);
        SFE_TSL2561_read_data0_and_data1(data);
        SFE_TSL2561_read_gain_and_intge(data+2);
        printf("data0:%d data1:%d gain:%d integ:%d \r\n",data[0],data[1],data[2],data[3]);
        data[0]=CalculateLux(data[2],data[3],data[0],data[1],1);
        printf("¹âÕÕÇ¿¶È£º%d\r\n\r\n",data[0]);
    }
    
    

}  