#include "iic.h"


uint8_t  IIC_Rx_Buff[40];
uint8_t  IIC_Rx_Buff_Count=0;
uint8_t  IIC_Rx_Flag = 0;
uint8_t  IIC_Tx_Buff_Count=0;
uint8_t  IIC_Tx_Buff[40];

void iic_init(void)
{
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	//使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	I2C_InitStruct.I2C_Mode                = I2C_Mode_I2C;
	I2C_InitStruct.I2C_Ack                 = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed          = 400000;
	I2C_InitStruct.I2C_DutyCycle           = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1         = 0x7E;
	
	I2C_Init(I2C1, &I2C_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2C_Cmd(I2C1, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                 
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	I2C_ITConfig(I2C1, I2C_IT_BUF | I2C_IT_EVT |I2C_IT_ERR, ENABLE);   
}


void I2C1_EV_IRQHandler(void)
{
	uint32_t sr1itflags = I2C1->SR1;
	uint32_t sr2itflags = I2C1->SR2;
	
	//从机模式
	if((sr2itflags & 0x0001) != 0x0001)
	{
		//地址信号
		if((sr1itflags & 0x0002) == 0x0002)
		{
			IIC_Rx_Buff_Count=0;
			IIC_Tx_Buff_Count=0;
		}//停止信号
		else if((sr1itflags & 0x0010) == 0x0010)
		{
			I2C1->CR1 |= 0x0001;
			IIC_Rx_Flag = 1;
		}//发送数据
		else if((sr1itflags & 0x0080) == 0x0080)
		{
			I2C1->DR = IIC_Tx_Buff[IIC_Tx_Buff_Count++];
		}//接收数据
		else if((sr1itflags & 0x0040) == 0x0040)  
		{
			IIC_Rx_Buff[IIC_Rx_Buff_Count++] = I2C1->DR;
		}//应答失败
	    else if(( sr1itflags & 0x0400) == 0x0400)
		{
			I2C1->SR1 &= 0xFDFF;      
		}       
	}
}

void I2C1_ER_IRQHandler(void) {

	uint32_t sr1itflags = I2C1->SR1;
	uint32_t sr2itflags = I2C1->SR2;

    if(     I2C_GetITStatus(I2C1, I2C_IT_SMBALERT)) {
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT)) {
    } 
    else if(I2C_GetITStatus(I2C1, I2C_IT_PECERR)) {
    } 
    else if(I2C_GetITStatus(I2C1, I2C_IT_OVR)) {

    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_AF)) {

        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_ARLO)) {

    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_BERR)) {

    }
    I2C1->CR1 |= 0x0001;
}