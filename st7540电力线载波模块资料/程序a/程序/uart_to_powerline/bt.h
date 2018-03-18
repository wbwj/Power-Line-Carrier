/*
乐电新南
*/
#ifndef BT_H
#define BT_H

#include "main.h"

extern float votage_lim;
extern float ad_value;
extern u16 LED_t;
extern float AD_K;

//用于存储
#define EE_AD_K		(0x4000)
#define EE_votage_lim	(0x4004)

#define LED_P	(1<<7)
#define BEEP_P	(1<<3)

#define LED_ON		PC_ODR |= LED_P
#define LED_OFF		PC_ODR &= ~LED_P
#define BEEP_ON		PA_ODR |= BEEP_P
#define BEEP_OFF	PA_ODR &= ~BEEP_P

void bt_pro(void);
void bt_ini(void);


#endif
