#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/10
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
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


//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //高速GPIO
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9); //GPIOF9,10高电平
	

}

