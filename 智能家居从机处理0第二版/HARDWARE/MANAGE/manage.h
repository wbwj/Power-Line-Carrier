#ifndef __MANAGE_H
#define __MANAGE_H

void Adc_Voltage_Transition(void);      //电压检测
void Adc_Control_Chip_Temperature(void);//主控芯片温度
void Adc_Humiture_Measure_Indoor(void); //室内温湿度返回
void Adc_Humiture_Measure_Outdoor(void);//室外温湿度返回
void Adc_Co_Gas(void);                  //有害气体含量     
void Adc_Raindrop_Indoor(void);             //室内雨滴传感器（判断是否会出现水贱到从机主控芯片上面）
void Adc_Raindrop_outdoor(void);            //室外雨滴传感器（判断外面是否下雨）



#endif

