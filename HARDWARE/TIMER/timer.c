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
	current->fp_ui = ETH_GetPTPRegister(ETH_PTPTSHR) + JAN_1900;//ʱ��ת�� 1970 - 1900 
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
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	
	//PC6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM3 ���ű�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	
 

    //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = 65535; 
	TIM_TimeBaseStructure.TIM_Prescaler = 0 ; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_ARRPreloadConfig(TIM3,ENABLE) ;//ʹ�ܻ���
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // ѡ������� IC4ӳ�䵽TI4��
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
    TIM_ICInitStructure.TIM_ICFilter = 0;//IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
    TIM_ICInit(TIM3, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update); 
	
	 
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM1
	
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�7��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	
}


void TIM4_Int_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

TimeInternal offset ;
TimeInternal ntime ;
TimeInternal tmptime ;

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1)!=RESET)//�����ز���
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
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		lwip_localtime +=10; //��10
		snmp_inc_sysuptime();
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}
