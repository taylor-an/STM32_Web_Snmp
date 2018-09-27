#include "timer.h"
#include "led.h"
#include "lwip_comm.h"
#include "snmp.h"
#include "ptp.h"
#include "usart.h"


uint32_t  time_cnt = 0;
uint8_t time_flag = 0u;

void get_sys_timer(time_fp *current)
{   
	current->fp_ui = ETH_GetPTPRegister(ETH_PTPTSHR) + JAN_1900;//时间转换 1970 - 1900 
	current->fp_uf =  ETH_GetPTPRegister(ETH_PTPTSLR) << 1 ;
			 
}
unsigned int mktime (unsigned int year, int mon, unsigned int day, unsigned int hour, unsigned int min, unsigned int sec)
{
	if ( 0 >= (mon -= 2))
	{ /**//* 1..12 -> 11,12,1..10 */
			mon += 12;	 /**//* Puts Feb last since it has leap day */
			year -= 1;
	}
	
	return ((((unsigned int)(year/4 - year/100 + year/400 + 367*mon/12 + day) + year*365 - 719499
	)*24 + hour /**//* now have hours */
	)*60 + min /**//* now have minutes */
	)*60 + sec; /**//* finally seconds */
}


extern u32 lwip_localtime;
void timer_init()
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure; 
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);	//使能定时器1时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	
	//PC6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM3 上桥臂
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化	
 

    //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = 65535; 
	TIM_TimeBaseStructure.TIM_Prescaler = 0 ; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ARRPreloadConfig(TIM3,ENABLE) ;//使能缓冲
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // 选择输入端 IC4映射到TI4上
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
    TIM_ICInitStructure.TIM_ICFilter = 0;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
    TIM_ICInit(TIM3, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update); 
	
	 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM1
	
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级7级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
}


void TIM4_Int_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

TimeInternal offset ;
TimeInternal ntime ;
TimeInternal tmptime ;

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1)!=RESET)//上升沿捕获
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
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		lwip_localtime +=10; //加10
		snmp_inc_sysuptime();
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
