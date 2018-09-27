#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//LED端口定义
#define LED0 PBout(9)


typedef struct 
{
	uint16_t LED_Mode_Arr[4][4];   //LED状态数组
	uint16_t LED_Mode;           //LED模式
	uint16_t LED_Cnt;           //状态计数
	uint16_t LED_Step;        //状态改变间隔 单位ms
	uint16_t LED_Max_State;   //状态长度
}LED_t;

extern LED_t led_mode_con;
void LED_Init(void); //初始化
#endif
