#ifndef __DRIVER_H
#define __DRIVER_H
#include "sys.h"
#include "stdlib.h"	



#define DRIVER_DIR_X   PGout(8) // X轴旋转方向 
#define DRIVER_OE_X    PGout(9) // X轴使能脚 高电平有效
#define DRIVER_DIR_Y   PGout(6)// Y轴旋转方向 
#define DIRVER_OE_Y    PGout(7)// Y轴使能脚 低电平有效
#define RCR_VAL_X    255  //每计数（RCR_VAL_X+1）次，中断一次，这个值（0~255）设置大一些可以降低中断频率
#define RCR_VAL_Y    255   //每计数（RCR_VAL_Y+1）次，中断一次，这个值（0~255）设置大一些可以降低中断频率

#define One_Circle_Pulse 25600 //转动一圈所需要的脉冲数（128细分）
#define One_Circle_Distance 0.8000000       //转动一圈的距离（按实际情况而定）
#define One_Pulse_Distance One_Circle_Distance/One_Circle_Pulse //一个脉冲的距离



typedef enum
{
	CW = 1,//高电平顺时针
	CCW = 0,//低电平逆时针
}DIR_Type;//运行方向 



extern long target_pos_X;//有符号方向； X轴目标地点
extern long current_pos_X;//有符号方向； X轴到达正确地点

extern long target_pos_Y;//有符号方向； Y轴目标地点
extern long current_pos_Y;//有符号方向； Y轴到达正确地点



void Driver_Init_X(void);//X轴驱动器初始化
void Locate_Rle_X(long num,u32 frequency,DIR_Type dir); //X轴相对定位函数
void Locate_Rle_X_dis(long dis,u32 frequency,DIR_Type dir); //X轴相对定位函数 距离
void Locate_Abs_X(long num,u32 frequency);//X轴绝对定位函数

void Driver_Init_Y(void);//Y轴驱动器初始化
void Locate_Rle_Y(long num,u32 frequency,DIR_Type dir); //Y轴相对定位函数
void Locate_Rle_Y_dis(long dis,u32 frequency,DIR_Type dir); //Y轴相对定位函数 距离
void Locate_Abs_Y(long num,u32 frequency);//Y轴绝对定位函数

void TIM1_OPM_RCR_Init(u16 arr,u16 psc);//TIM1_CH4 单脉冲输出+重复计数功能初始化
void TIM1_Startup(u32 frequency);   //启动定时器1

void TIM8_OPM_RCR_Init(u16 arr,u16 psc);//TIM8_CH2 单脉冲输出+重复计数功能初始化
void TIM8_Startup(u32 frequency);   //启动定时器8

#endif


