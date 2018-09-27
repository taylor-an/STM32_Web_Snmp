#ifndef _timer_h_
#define _timer_h_

#include "stm32f10x.h"
#include "sntp.h"
#include "type.h"
//typedef unsigned long long tstamp;

//typedef struct _DateTime	/*�˽ṹ�嶨����NTPʱ��ͬ������ر���*/
//{
//  uint16_t year;					/*��Ϊ2�ֽ� ��Ϊ�ߵ���λ */
//  uint8_t month;						/*	�� */
//  uint8_t day;							/*	�� */
//  uint8_t hour;							/*	ʱ */
//  uint8_t minute;						/*	�� */
//  uint8_t second;						/*	�� */	
//} DateTime;

//typedef struct ntp_state	/*�˽ṹ�嶨����NTPʱ��ͬ�������״̬*/
//{
//  uint8_t leap_state;					//����״̬
//  uint8_t sync_state;					//ͬ��״̬
//	uint8_t com_leap_state;     //��������״̬�ж�
//} state;


#define JAN_1900 2208988800 //0X83AA7E80 
#define NULL 0

void get_sys_timer(time_fp *current);
void timer_init(void);
void tim2_init(void);
  unsigned int mktime (unsigned int year,   int mon,
	unsigned int day, unsigned int hour,
	unsigned int min, unsigned int sec);//ʱ���ת��UTC->UNIX Gauss�㷨 ���2038��

#endif
