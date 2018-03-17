#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

 
//IO方向设置
#define DHT11_IO_IN_Indoor()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
#define DHT11_IO_OUT_Indoor() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}

#define DHT11_IO_IN_Outdoor()  {GPIOG->CRH&=0XFFF0FFFF;GPIOG->CRH|=8<<16;}  
#define DHT11_IO_OUT_Outdoor() {GPIOG->CRH&=0XFFF0FFFF;GPIOG->CRH|=3<<16;}

////IO操作函数											   
#define	DHT11_DQ_OUT_Indoor PGout(11) //数据端口	PA0 
#define	DHT11_DQ_IN_Indoor  PGin(11)  //数据端口	PA0 

#define	DHT11_DQ_OUT_Outdoor PGout(12) //数据端口	PA0 
#define	DHT11_DQ_IN_Outdoor  PGin(12)  //数据端口	PA0 


u8 DHT11_Init_Indoor(void);//初始化DHT11_1
u8 DHT11_Read_Data_Indoor(u8 *temp,u8 *humi);//读取温湿度_1
u8 DHT11_Read_Byte_Indoor(void);//读出一个字节_1
u8 DHT11_Read_Bit_Indoor(void);//读出一个位_1
u8 DHT11_Check_Indoor(void);//检测是否存在DHT11_1
void DHT11_Rst_Indoor(void);//复位DHT11_1   

u8 DHT11_Init_Outdoor(void);//初始化DHT11_2
u8 DHT11_Read_Data_Outdoor(u8 *temp,u8 *humi);//读取温湿度_2
u8 DHT11_Read_Byte_Outdoor(void);//读出一个字节_2
u8 DHT11_Read_Bit_Outdoor(void);//读出一个位_2
u8 DHT11_Check_Outdoor(void);//检测是否存在DHT11_2
void DHT11_Rst_Outdoor(void);//复位DHT11_2
#endif















