#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f10x.h"
#include "sntp.h"
#include "type.h"


#define JAN_1900 2208988800 //0X83AA7E80 
#define NULL 0

void get_sys_timer(time_fp *current);
void timer_init(void);
void tim2_init(void);
unsigned int mktime (unsigned int year,   int mon,
unsigned int day, unsigned int hour,
unsigned int min, unsigned int sec);//时间戳转换UTC->UNIX Gauss算法 最大2038年
void TIM4_Int_Init(u16 arr,u16 psc);
#endif
