
#ifndef SFE_TSL2561_h
#define SFE_TSL2561_h

#include "myiic.h"
#include "stdio.h"
//写数据
void SFE_TSL2561_read (unsigned char cmd,\
                       unsigned char count,   unsigned char data[]);
//读数据
void SFE_TSL2561_write(unsigned char cmd,\
                       unsigned char count,   unsigned char data[]);
//初始化设备
void SFE_TSL2561_init(void);
//读数据寄存器，data[0]、data[1]分别保存了data0和data1的数据
void SFE_TSL2561_read_data0_and_data1(unsigned int data[]);
//      
void SFE_TSL2561_read_gain_and_intge(unsigned int data[]);
extern unsigned int CalculateLux(unsigned int iGain, unsigned int tInt, unsigned int
ch0, unsigned int ch1, int iType);
//
#define TSL2561_ADDR_0                   0x29 // address with '0' shorted on board
#define TSL2561_ADDR_                    0x39 // default address
#define TSL2561_ADDR_1                   0x49 // address with '1' shorted on board
//设备地址     
#define TSL2561_ADDR                     TSL2561_ADDR_
//寄存器 
#define TSL2561_REG_COMMAND              0x80
#define TSL2561_REG_CONTROL              0x00
#define TSL2561_REG_TIMING               0x01
#define TSL2561_REG_THRESHLOWLOW         0x02
#define TSL2561_REG_THRESHLOWHIGH        0x03
#define TSL2561_REG_THRESHHIGHLOW        0x04
#define TSL2561_REG_THRESHHIGHHIGH       0x05
#define TSL2561_REG_INTERRUPT            0x06
#define TSL2561_REG_ID                   0x0A
#define TSL2561_REG_DATA0LOW             0x0C
#define TSL2561_REG_DATA0HIGH            0x0D
#define TSL2561_REG_DATA1LOW             0x0E
#define TSL2561_REG_DATA1HIGH            0x0F
///////////////////////////////////////////////////////////////
//80h TSL2561_REG_COMMAND                                    //
// 0h CONTROL Control of basic functions                     //
// 1h TIMING Integration time/gain control                   //
// 2h THRESHLOWLOW Low byte of low interrupt threshold       //
// 3h THRESHLOWHIGH High byte of low interrupt threshold     //
// 4h THRESHHIGHLOW Low byte of high interrupt threshold     //
// 5h THRESHHIGHHIGH High byte of high interrupt threshold   //
// 6h INTERRUPT Interrupt control                            //
// 7h ?? Reserved                                            //
// 8h CRC Factory test — not a user register                //
// 9h ?? Reserved                                            //
// Ah ID Part number/ Rev ID                                 //
// Bh ?? Reserved                                            //
// Ch DATA0LOW Low byte of ADC channel 0                     //
// Dh DATA0HIGH High byte of ADC channel 0                   //
// Eh DATA1LOW Low byte of ADC channel 1                     //
// Fh DATA1HIGH High byte of ADC channel 1                   //
///////////////////////////////////////////////////////////////
#endif
