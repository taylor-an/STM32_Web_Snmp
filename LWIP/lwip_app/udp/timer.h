#ifndef _timer_h_
#define _timer_h_

#include "stm32f10x.h"
#include "sntp.h"
#include "type.h"
//typedef unsigned long long tstamp;

//typedef struct _DateTime	/*此结构体定义了NTP时间同步的相关变量*/
//{
//  uint16_t year;					/*年为2字节 分为高低两位 */
//  uint8_t month;						/*	月 */
//  uint8_t day;							/*	天 */
//  uint8_t hour;							/*	时 */
//  uint8_t minute;						/*	分 */
//  uint8_t second;						/*	秒 */	
//} DateTime;

//typedef struct ntp_state	/*此结构体定义了NTP时间同步的相关状态*/
//{
//  uint8_t leap_state;					//闰秒状态
//  uint8_t sync_state;					//同步状态
//	uint8_t com_leap_state;     //串口输入状态判断
//} state;


#define JAN_1900 2208988800 //0X83AA7E80 
#define NULL 0

void get_sys_timer(time_fp *current);
void timer_init(void);
void tim2_init(void);
  unsigned int mktime (unsigned int year,   int mon,
	unsigned int day, unsigned int hour,
	unsigned int min, unsigned int sec);//时间戳转换UTC->UNIX Gauss算法 最大2038年

#endif
