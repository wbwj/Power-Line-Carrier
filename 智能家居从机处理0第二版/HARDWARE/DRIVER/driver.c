#include "driver.h"
#include "delay.h"
#include "usart.h"

/*************窗户以及窗帘左边*****************************/

u8 rcr_remainder_Y;        //Y轴重复计数余数部分
u8 is_rcr_finish_Y=1;      //Y轴重复计数器是否设置完成
int finish_Y=0;
long rcr_integer_Y;	       //Y轴重复计数整数部分
long target_pos_Y=0;       //Y轴有符号方向
long current_pos_Y=0;      //Y轴有符号方向
DIR_Type motor_dir_Y=CW;   //Y轴顺时针


/************** 驱动器控制信号线初始化 ****************/
void Driver_Init_Y(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	//使能PG端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PG6，7 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					//根据设定参数初始化GPIOG
	GPIO_SetBits(GPIOG,GPIO_Pin_6);						 	//PG6输出高 顺时针方向  DRIVER_DIR_Y
	GPIO_SetBits(GPIOG,GPIO_Pin_7);						//PG7输出低 使能输出  DRIVER_OE_Y
}

/***********************************************
//TIM8_CH2(PC7) 单脉冲输出+重复计数功能初始化
//TIM8 时钟频率 72MHz
//arr：自动重装值
//psc：时钟预分频数
************************************************/
void TIM8_OPM_RCR_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //TIM8时钟使能
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIOC外设时钟使能	                                                                     	

  //设置该引脚为复用输出功能,输出TIM8 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM8_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM8,TIM_UpdateSource_Regular); /********* 设置只有计数溢出作为更新中断 ********/
	TIM_SelectOnePulseMode(TIM8,TIM_OPMode_Single);/******* 单脉冲模式 **********/
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出2使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /****** 比较输出2N失能 *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2预装载使能	 
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_ITConfig(TIM8, TIM_IT_Update ,ENABLE);  //TIM8   使能或者失能指定的TIM中断
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM8中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8									  
}
/******* TIM8更新中断服务程序 *********/
void TIM8_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_FLAG_Update)!=RESET)//更新中断
	{
		TIM_ClearITPendingBit(TIM8,TIM_FLAG_Update);//清除更新中断标志位		
		if(is_rcr_finish_Y==0)//重复计数器未设置完成
		{
			if(rcr_integer_Y!=0) //整数部分脉冲还未发送完成
			{
				TIM8->RCR=RCR_VAL_Y;//设置重复计数值
				rcr_integer_Y--;//减少RCR_VAL_Y+1个脉冲				
			}else if(rcr_remainder_Y!=0)//余数部分脉冲 不位0
			{
				TIM8->RCR=rcr_remainder_Y-1;//设置余数部分
				rcr_remainder_Y=0;//清零
				is_rcr_finish_Y=1;//重复计数器设置完成				
			}else goto out;   //rcr_remainder=0，直接退出			 
			TIM_GenerateEvent(TIM8,TIM_EventSource_Update);//产生一个更新事件 重新初始化计数器
			TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能	
			TIM_Cmd(TIM8, ENABLE);  //使能TIM8			
			if(motor_dir_Y==CW) //如果方向为顺时针   
				current_pos_Y+=(TIM8->RCR+1);//加上重复计数值
			else          //否则方向为逆时针
				current_pos_Y-=(TIM8->RCR+1);//减去重复计数值			
		}else
		{
out:		is_rcr_finish_Y=1;//重复计数器设置完成
			finish_Y=1;
			TIM_CtrlPWMOutputs(TIM8,DISABLE);	//MOE 主输出关闭
			TIM_Cmd(TIM8, DISABLE);  //关闭TIM8	
			
//			printf("Y轴当前位置=%fcm\r\n",current_pos_Y*One_Pulse_Distance);//打印输出
		}	
	}
}
/***************** 启动TIM8 *****************/
void TIM8_Startup(u32 frequency)   //启动定时器8
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM8,temp_arr);//设定自动重装值	
	TIM_SetCompare4(TIM8,temp_arr>>1); //匹配值2等于重装值一半，是以占空比为50%	
	TIM_SetCounter(TIM8,0);//计数器清零
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
}
/********************************************
//相对定位函数 
//num 0～2147483647
//frequency: 20Hz~100KHz
//dir: CW(顺时针方向)  CCW(逆时针方向)
*********************************************/
void Locate_Rle_Y(long num,u32 frequency,DIR_Type dir) //Y轴相对定位函数
{
	if(num<=0) //数值小等于0 则直接返回
	{
		return;
	}
	if(TIM8->CR1&0x01)//上一次脉冲还未发送完成  直接返回
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		return;
	}
	motor_dir_Y=dir;//得到方向	
	DRIVER_DIR_Y=motor_dir_Y;//设置方向
	
	if(motor_dir_Y==CW)//顺时针
		target_pos_Y=current_pos_Y+num;//目标位置
	else if(motor_dir_Y==CCW)//逆时针
		target_pos_Y=current_pos_Y-num;//目标位置
	
	rcr_integer_Y=num/(RCR_VAL_Y+1);//重复计数整数部分
	rcr_remainder_Y=num%(RCR_VAL_Y+1);//重复计数余数部分
	is_rcr_finish_Y=0;//重复计数器未设置完成
	TIM8_Startup(frequency);//开启TIM8
}

/********************************************
//相对定位函数 
//num 0～2147483647
//frequency: 20Hz~100KHz
//dir: CW(顺时针方向)  CCW(逆时针方向)
*********************************************/
void Locate_Rle_Y_dis(long dis,u32 frequency,DIR_Type dir) //Y轴相对定位函数
{
	long num;
	num = 2964*dis;
	if(num<=0) //数值小等于0 则直接返回
	{
		return;
	}
	if(TIM8->CR1&0x01)//上一次脉冲还未发送完成  直接返回
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		return;
	}
	motor_dir_Y=dir;//得到方向	
	DRIVER_DIR_Y=motor_dir_Y;//设置方向
	
	if(motor_dir_Y==CW)//顺时针
		target_pos_Y=current_pos_Y+num;//目标位置
	else if(motor_dir_Y==CCW)//逆时针
		target_pos_Y=current_pos_Y-num;//目标位置
	
	rcr_integer_Y=num/(RCR_VAL_Y+1);//重复计数整数部分
	rcr_remainder_Y=num%(RCR_VAL_Y+1);//重复计数余数部分
	is_rcr_finish_Y=0;//重复计数器未设置完成
	TIM8_Startup(frequency);//开启TIM8
}
/********************************************
//绝对定位函数 
//num   -2147483648～2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs_Y(long num,u32 frequency)//Y轴绝对定位函数
{
	if(TIM8->CR1&0x01)//上一次脉冲还未发送完成 直接返回
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		return;
	}
	target_pos_Y=num;//设置目标位置
	if(target_pos_Y!=current_pos_Y)//目标和当前位置不同
	{
		if(target_pos_Y>current_pos_Y)
			motor_dir_Y=CW;//顺时针
		else
			motor_dir_Y=CCW;//逆时针
		DRIVER_DIR_Y=motor_dir_Y;//设置方向
		
		rcr_integer_Y=abs(target_pos_Y-current_pos_Y)/(RCR_VAL_Y+1);//重复计数整数部分
		rcr_remainder_Y=abs(target_pos_Y-current_pos_Y)%(RCR_VAL_Y+1);//重复计数余数部分
		is_rcr_finish_Y=0;//重复计数器未设置完成
		TIM8_Startup(frequency);//开启TIM8
	}
}


/*************窗户以及窗帘右边*****************************/

u8 rcr_remainder_X;   //X轴重复计数余数部分
u8 is_rcr_finish_X=1; //X轴重复计数器是否设置完成
int finish_X=0;
long rcr_integer_X;	//X轴重复计数整数部分
long target_pos_X=0;  //X轴有符号方向
long current_pos_X=0; //X轴符号方向
DIR_Type motor_dir_X=CW;//X轴顺时针

/************** 驱动器控制信号线初始化 ****************/
void Driver_Init_X(void)   
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	//使能PG端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	//PG8,9 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					//根据设定参数初始化GPIOG
	GPIO_SetBits(GPIOG,GPIO_Pin_8);						 	//PG1输出高 顺时针方向  DRIVER_DIR_X
	GPIO_SetBits(GPIOG,GPIO_Pin_9);						//PG2输出高 使能输出  DRIVER_OE_X
}

/***********************************************
//TIM_CH4(PA11) 单脉冲输出+重复计数功能初始化
//TIM1 时钟频率 72MHz
//arr：自动重装值
//psc：时钟预分频数
************************************************/
void TIM1_OPM_RCR_Init(u16 arr,u16 psc)
{		 					
 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //TIM1时钟使能
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIOA外设时钟使能	                                                                     	

  //设置该引脚为复用输出功能,输出TIM1 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM1_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Regular); /********* 设置只有计数溢出作为更新中断 ********/
	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);/******* 单脉冲模式 **********/
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出2使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /****** 比较输出2N失能 *******/
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4预装载使能	 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_ITConfig(TIM1, TIM_IT_Update ,ENABLE);  //TIM1   使能或者失能指定的TIM中断
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1									  
}
/******* TIM1更新中断服务程序 *********/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_FLAG_Update)!=RESET)//更新中断
	{
		TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);//清除更新中断标志位		
		if(is_rcr_finish_X==0)//重复计数器未设置完成
		{
			if(rcr_integer_X!=0) //整数部分脉冲还未发送完成
			{
				TIM1->RCR=RCR_VAL_X;//设置重复计数值
				rcr_integer_X--;//减少RCR_VAL_X+1个脉冲				
			}else if(rcr_remainder_X!=0)//余数部分脉冲 不位0
			{
				TIM1->RCR=rcr_remainder_X-1;//设置余数部分
				rcr_remainder_X=0;//清零
				is_rcr_finish_X=1;//重复计数器设置完成				
			}else goto out;   //rcr_remainder=0，直接退出			 
			TIM_GenerateEvent(TIM1,TIM_EventSource_Update);//产生一个更新事件 重新初始化计数器
			TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	
			TIM_Cmd(TIM1, ENABLE);  //使能TIM1			
			if(motor_dir_X==CW) //如果方向为顺时针   
				current_pos_X+=(TIM1->RCR+1);//加上重复计数值
			else          //否则方向为逆时针
				current_pos_X-=(TIM1->RCR+1);//减去重复计数值			
		}else
		{
out:		is_rcr_finish_X=1;//重复计数器设置完成
			finish_X=1;
			TIM_CtrlPWMOutputs(TIM1,DISABLE);	//MOE 主输出关闭
			TIM_Cmd(TIM1, DISABLE);  //关闭TIM8				
//			printf("X轴当前位置=%fcm\r\n",current_pos_X*One_Pulse_Distance);//打印输出
		}	
	}
}
/***************** 启动TIM8 *****************/
void TIM1_Startup(u32 frequency)   //启动定时器1
{
	u16 temp_arr=1000000/frequency-1; 
	TIM_SetAutoreload(TIM1,temp_arr);//设定自动重装值	
	TIM_SetCompare4(TIM1,temp_arr>>1); //匹配值2等于重装值一半，是以占空比为50%	
	TIM_SetCounter(TIM1,0);//计数器清零
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}
/********************************************
//相对定位函数 
//num 0～2147483647
//frequency: 20Hz~100KHz
//dir: CW(顺时针方向)  CCW(逆时针方向)
*********************************************/
void Locate_Rle_X(long num,u32 frequency,DIR_Type dir) //X轴相对定位函数
{
	if(num<=0) //数值小等于0 则直接返回
	{
		return;
	}
	if(TIM1->CR1&0x01)//上一次脉冲还未发送完成  直接返回
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		return;
	}
	motor_dir_X=dir;//得到方向	
	DRIVER_DIR_X=motor_dir_X;//设置方向
	
	if(motor_dir_X==CW)//顺时针
		target_pos_X=current_pos_X+num;//目标位置
	else if(motor_dir_X==CCW)//逆时针
		target_pos_X=current_pos_X-num;//目标位置
	
	rcr_integer_X=num/(RCR_VAL_X+1);//重复计数整数部分
	rcr_remainder_X=num%(RCR_VAL_X+1);//重复计数余数部分
	is_rcr_finish_X=0;//重复计数器未设置完成
	TIM1_Startup(frequency);//开启TIM1
}
/********************************************
//相对定位函数  距离 
//num 0～2147483647
//frequency: 20Hz~100KHz
//dir: CW(顺时针方向)  CCW(逆时针方向)
*********************************************/
void Locate_Rle_X_dis(long dis,u32 frequency,DIR_Type dir) //X轴相对定位函数
{
	long num;
	num = 2964*dis;
	if(num<=0) //数值小等于0 则直接返回
	{
		return;
	}
	if(TIM1->CR1&0x01)//上一次脉冲还未发送完成  直接返回
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		return;
	}
	motor_dir_X=dir;//得到方向	
	DRIVER_DIR_X=motor_dir_X;//设置方向
	
	if(motor_dir_X==CW)//顺时针
		target_pos_X=current_pos_X+num;//目标位置
	else if(motor_dir_X==CCW)//逆时针
		target_pos_X=current_pos_X-num;//目标位置
	
	rcr_integer_X=num/(RCR_VAL_X+1);//重复计数整数部分
	rcr_remainder_X=num%(RCR_VAL_X+1);//重复计数余数部分
	is_rcr_finish_X=0;//重复计数器未设置完成
	TIM1_Startup(frequency);//开启TIM1
}
/********************************************
//绝对定位函数 
//num   -2147483648～2147483647
//frequency: 20Hz~100KHz
*********************************************/
void Locate_Abs_X(long num,u32 frequency)//X轴绝对定位函数
{
	if(TIM1->CR1&0x01)//上一次脉冲还未发送完成 直接返回
	{
		return;
	}
	if((frequency<20)||(frequency>100000))//脉冲频率不在范围内 直接返回
	{
		return;
	}
	target_pos_X=num;//设置目标位置
	if(target_pos_X!=current_pos_X)//目标和当前位置不同
	{
		if(target_pos_X>current_pos_X)
			motor_dir_X=CW;//顺时针
		else
			motor_dir_X=CCW;//逆时针
		DRIVER_DIR_X=motor_dir_X;//设置方向
		
		rcr_integer_X=abs(target_pos_X-current_pos_X)/(RCR_VAL_X+1);//重复计数整数部分
		rcr_remainder_X=abs(target_pos_X-current_pos_X)%(RCR_VAL_X+1);//重复计数余数部分
		is_rcr_finish_X=0;//重复计数器未设置完成
		TIM1_Startup(frequency);//开启TIM1
	}
}






