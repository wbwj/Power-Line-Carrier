#include "bit_define.h"
#include "power_line.h"
#include "iostm8s103.h"

unsigned char curr_state = 0;

const unsigned char next_state_table[] =
{
	0, 1, 1, 0, 2, 1, 3, 2
};

void delay(volatile unsigned long delay_time)   //delay(1000) is about 2.28ms at 16MHz
{
	while(delay_time--) ;
}

void power_line_init(void)
{
	unsigned long reg_value ;

  //reg_value = read_ctrl_reg();
  reg_value = 0x1362d7;
//   	reg_value |= 0x40 ;          //set watch dog
  reg_value &= 0xffffffbf ;        //clr watch dog
  reg_value &= 0xffffbfff;         //sel sync mode
 	reg_value |= 0x18 ;              //set baud rate to 4800
	reg_value &= 0xfffe7fff;         //set MCLK to 16MHz
	reg_value &= 0xfffffffe;         //set MCLK to 110KHz
	reg_value |= 0x00800000;         //enable input filter
  
	write_ctrl_reg(reg_value);
  delay(500);
	while(reg_value != read_ctrl_reg())
	{
		 write_ctrl_reg(reg_value);
		 delay(500);
	}
	
	PC_CR2 |= BIT6;//ÓÐÖÐ¶Ï
	EXTI_CR1 &= ~BIT5 ;
	EXTI_CR1 |=  BIT4 ;
	
	
}

unsigned long read_ctrl_reg(void)
{
	unsigned long return_value = 0;
	unsigned long curr_bit = 0 ;
    int i ;
    set_pin_rxtx(0x1);     //set pin rxtx
    wait_posedge(); //find posedge for Tcr
    _asm("NOP");
		_asm("NOP");
   // delay(100) ;
    set_pin_reg_data(0x1);  //set pin reg_data
    set_pin_rxtx(0x1);     //set pin rxtx
    
	for(i=0;i<=23;i++)
	{
		wait_posedge();

		curr_bit = get_pin_rxd() ;

		return_value |= ((curr_bit) << (23-i)) ;

	}
	//wait_negedge();

	set_pin_reg_data(0x0);   //clr pin reg_data

	delay(1000) ;
	return return_value ;
}

void write_ctrl_reg(unsigned long write_data)
{
    int i ;

    wait_posedge();   //find posedge for Tcr
    _asm("NOP");
		_asm("NOP");
    //delay(100) ;
    set_pin_reg_data(0x1);   //set pin reg_data
    set_pin_rxtx(0x0);   //clr pin rxtx
  //  wait_negedge();
	//delay(50) ;       //delay for Tcc

	for(i=0;i<=23;i++) //low code effciency may result in wrong writing
	{
		if(((write_data >> (23-i)) & 0x1) == 0)
		{
			set_pin_txd(0x0);
		}
		else
		{
			set_pin_txd(0x1);
		}

		wait_posedge();
		//wait_negedge();
		//delay(32) ;                  //give enough hold time
	}

	set_pin_rxtx(0x1);    //set pin rxtx
	set_pin_reg_data(0x0);   //clr pin reg_data
	set_pin_txd(0x0);
	delay(1000) ;
}

void data_trans(unsigned long tx_data_h,unsigned long tx_data_l)
{
  unsigned long tx_data_real ;
	int i ;
	tx_data_real = tx_data_h ;

	wait_posedge();    //find posedge for Tcr

  delay(100) ;
  set_pin_reg_data(0x0);   //clr pin reg_data
  set_pin_rxtx(0x0);  //clr pin rxtx

	delay(50) ;       //delay for Tcc

	for(i=0;i<=31;i++) //low code effciency may result in wrong writing
	{
		if(((tx_data_real >> (31-i)) & 0x1) == 0)
		{
			set_pin_txd(0x0);
		}
		else
		{
			set_pin_txd(0x1);
		}
		wait_posedge();
		wait_negedge();
		//delay(32) ;                  //give enough hold time
	}

  tx_data_real = tx_data_l ;
	for(i=0;i<=31;i++) //low code effciency may result in wrong writing
	{
		if(((tx_data_real >> (31-i)) & 0x1) == 0)
		{
			set_pin_txd(0x0);
		}
		else
		{
			set_pin_txd(0x1);
		}
		wait_posedge();
		wait_negedge();
		//delay(32) ;                  //give enough hold time
	}

	set_pin_rxtx(0x1);   //set pin rxtx
	set_pin_reg_data(0x0);   //clr pin reg_data
	set_pin_txd(0x0);
	delay(1000) ;
	//while(get_pin_cdpd() == 0x0);
}

unsigned char data_recv(unsigned long *return_value_h,unsigned long *return_value_l,int try_times)
{
//	unsigned short return_value = 0;
	unsigned long curr_bit = 0 ;
	unsigned char  frame_head_find = 0 ;

  int i ;
//    int try_times = 500 ;
  *return_value_h = 0x0;
	*return_value_l = 0x0;

  //wait_posedge();   //find posedge for Tcr

  //delay(100) ;
//  set_pin_reg_data(0x0);   //clr pin reg_data
//  set_pin_rxtx(0x1);   //set pin rxtx

	frame_head_find = frame_head_srch(try_times);

	if(frame_head_find == 0)
	{
		return 0 ;
	}
  
	*return_value_h = (((unsigned long)FRAME_HEAD) << 24) ;

	for(i=0;i<=23;i++)
	{
		wait_posedge(); //find posedge
		curr_bit = get_pin_rxd() ;
    *return_value_h |= ((curr_bit) << (23-i)) ;
	}
	
	for(i=0;i<=31;i++)
	{
		wait_posedge(); //find posedge
		curr_bit = get_pin_rxd() ;
    *return_value_l |= ((curr_bit) << (31-i)) ;
	}
//	wait_negedge();

//	set_pin_reg_data(0x0);  //clr pin reg_data

	//delay(50) ;
	
	return 1 ;
}

unsigned char frame_head_srch(int try_times)
{
	unsigned char next_state = 0;

	while(try_times > 0)
	{
		wait_posedge();
		if(get_pin_rxd() ==  ((FRAME_HEAD >> (7-curr_state)) & 0x1))
		{
			if(curr_state == 7)
			{
				curr_state = 0 ;
				return 1 ;
			}
			else ;
			next_state = curr_state + 1;
		}
		else
		{
			next_state = next_state_table[curr_state];
			try_times -- ;
		}

		curr_state = next_state ;
	}

	return 0 ;
}

unsigned char conti_0_check(int conti_0_num)
{
	while(conti_0_num--)
	{
		wait_posedge();
		if(get_pin_rxd() !=  0x0)
		{
			return 0 ;
		}
		else ;
	}

	return 1 ;
}

void wait_posedge(void)
{
	if((PC_IDR & BIT6) != 0)
	{
	  	while((PC_IDR & BIT6) != 0) ;
	}
	else ;
	while((PC_IDR & BIT6) == 0) ;
}

void wait_negedge(void)
{
	if((PC_IDR & BIT6) == 0)
	{
	  	while((PC_IDR & BIT6) == 0) ;
	}	
	else ;
	while((PC_IDR & BIT6) != 0) ;
}

unsigned char get_pin_rxd(void)
{
	if((PB_IDR & BIT4) == 0)  //get rxd
	{
		return 0x0;
	}
	else
	{
		return 0x1;
	}
}

void set_pin_txd(unsigned char set_to_val)
{
	if(set_to_val == 0x0)
	{
		PC_ODR &= ~BIT4;
	}
	else
	{
		PC_ODR |= BIT4;
	}
}

void set_pin_reg_data(unsigned char set_to_val)
{
	if(set_to_val == 0x0)
	{
		PB_ODR &= ~BIT5;    //clr pin reg_data
	}
	else
	{
		PB_ODR |= BIT5;   //set pin reg_data
	}
}

void set_pin_rxtx(unsigned char set_to_val)
{
	if(set_to_val == 0x0)
	{
		PC_ODR &= ~BIT3;   //clr pin rxtx
	}
	else
	{
		PC_ODR |= BIT3;   //set pin rxtx
	}
}

unsigned char get_pin_cdpd(void)
{
	if((PA_IDR & BIT3) == 0)  //get cdpd
	{
		return 0x0;
	}
	else
	{
		return 0x1;
	}
}

