#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//LED�˿ڶ���
#define LED0 PBout(9)


typedef struct 
{
	uint16_t LED_Mode_Arr[4][4];   //LED״̬����
	uint16_t LED_Mode;           //LEDģʽ
	uint16_t LED_Cnt;           //״̬����
	uint16_t LED_Step;        //״̬�ı��� ��λms
	uint16_t LED_Max_State;   //״̬����
}LED_t;

extern LED_t led_mode_con;
void LED_Init(void); //��ʼ��
#endif
