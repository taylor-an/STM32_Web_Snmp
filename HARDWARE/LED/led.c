#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

LED_t led_mode_con = 
{
	{
				500, 500, 500, 500,
				100, 100, 500, 500,
				1000, 1000, 1000, 1000,
				100,100,100,100
	},
		0,
		0,
	100,
	sizeof(led_mode_con.LED_Mode_Arr[0])/sizeof(led_mode_con.LED_Mode_Arr[0][0])
};


//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //����GPIO
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9); //GPIOF9,10�ߵ�ƽ
	

}

