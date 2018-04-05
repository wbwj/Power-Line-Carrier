#ifndef __MAIN_H
#define __MAIN_H

#include	"config.h"

sbit LED = P5^5;

#define LED_ON 0
#define LED_OFF 1


//函数或者变量声明
extern void Delay_ms(unsigned int n);

extern void printGpsBuffer();
extern void parseGpsBuffer();
extern void errorLog(int num);
extern void clrStruct();

extern void initGprs();
extern unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
extern void postGpsDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, char* lon_temp, char* lat_temp);
extern char* longitudeToOnenetFormat(char *lon_str_temp);
extern char* latitudeToOnenetFormat(char *lat_str_temp);

#endif