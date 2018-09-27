#include "timer.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "ptp.h"
uint32_t  time_cnt = 0;
__IO uint32_t sys_time = 0;
uint8_t time_flag = 0u;
__IO uint32 old_sec = 0;
__IO DateTime nowdate;
__IO state sntp_state;


/**************************************************************************
 *
 * 函数功能: 得到系统当前时间.
 *
 * 参数说明: [OUT] current, SNTP 时间.
 *
 * 返 回 值: 成功返回 0, 失败返回 -1.
 *
 **************************************************************************/
 char str[30];
void get_sys_timer(time_fp *current)
{
		double temp_d = 0;

       
	  
	    	current->fp_ui = ETH_GetPTPRegister(ETH_PTPTSHR) + JAN_1900;//时间转换 1970 - 1900 
				current->fp_uf =  ETH_GetPTPRegister(ETH_PTPTSLR) << 1 ;
			 
}

  unsigned int mktime (unsigned int year,   int mon,
	unsigned int day, unsigned int hour,
	unsigned int min, unsigned int sec)
{
		if ( 0 >= (mon -= 2))
	{ /**//* 1..12 -> 11,12,1..10 */
			mon += 12;	 /**//* Puts Feb last since it has leap day */
			year -= 1;
	}
	
	return ((((unsigned int) (year/4 - year/100 + year/400 + 367*mon/12 + day) + year*365 - 719499
	)*24 + hour /**//* now have hours */
	)*60 + min /**//* now have minutes */
	)*60 + sec; /**//* finally seconds */
}

 void tim2_init(void)
{				 
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	            
	  
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period= 65535;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
 
  TIM_ARRPreloadConfig(TIM2,ENABLE);
	
	TIM_SelectInputTrigger(TIM2,TIM_TS_ITR0);
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_External1);
	
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); 
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);  
	
	TIM_Cmd(TIM2, ENABLE); 
}


/**************************************************************************
 *
 * 函数功能: 初始化定时器捕获模式.
 *
 * 参数说明: 无
 *
 * 返 回 值: 无
 *
 **************************************************************************/
void timer_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//使能定时器1时钟
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1 上桥臂
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 
  //初始化TIM1
	TIM_TimeBaseStructure.TIM_Period = 65535; 
	TIM_TimeBaseStructure.TIM_Prescaler = 0 ; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  TIM_ARRPreloadConfig(TIM1,ENABLE) ;//使能缓冲
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // 选择输入端 IC4映射到TI4上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM1, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update); 
	
  TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);  
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
	TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  //从优先级7级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
}

TimeInternal offset ;
TimeInternal ntime ;
TimeInternal tmptime ;
	
void TIM1_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET)//上升沿捕获
	{
			sys_time = sys_time + 1;
			getTime(&ntime);
			tmptime.seconds = sys_time;
			tmptime.nanoseconds = 0;
			 offset.nanoseconds = ntime.nanoseconds;
		 if(offset.nanoseconds > 500e6  )
		{
			offset.nanoseconds = offset.nanoseconds - 1000e6 ;
		}
		updateClock(&offset);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	}
}

