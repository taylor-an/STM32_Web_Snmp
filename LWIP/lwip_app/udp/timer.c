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
 * ��������: �õ�ϵͳ��ǰʱ��.
 *
 * ����˵��: [OUT] current, SNTP ʱ��.
 *
 * �� �� ֵ: �ɹ����� 0, ʧ�ܷ��� -1.
 *
 **************************************************************************/
 char str[30];
void get_sys_timer(time_fp *current)
{
		double temp_d = 0;

       
	  
	    	current->fp_ui = ETH_GetPTPRegister(ETH_PTPTSHR) + JAN_1900;//ʱ��ת�� 1970 - 1900 
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
 * ��������: ��ʼ����ʱ������ģʽ.
 *
 * ����˵��: ��
 *
 * �� �� ֵ: ��
 *
 **************************************************************************/
void timer_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1 ���ű�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 
  //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = 65535; 
	TIM_TimeBaseStructure.TIM_Prescaler = 0 ; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  TIM_ARRPreloadConfig(TIM1,ENABLE) ;//ʹ�ܻ���
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // ѡ������� IC4ӳ�䵽TI4��
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0;//IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  TIM_ICInit(TIM1, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��
	
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update); 
	
  TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);  
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
	TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);  
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  //�����ȼ�7��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	
}

TimeInternal offset ;
TimeInternal ntime ;
TimeInternal tmptime ;
	
void TIM1_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET)//�����ز���
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

