/*
//
	Project 	: driver
	File Name 	: usart.h
	Author 		: 
	Copyright		:	
	Created on	: 2016-3-2
	Destribe	:
		
	modify :

//
*/

#ifndef _USART_H_
#define _USART_H_

#include "type.h"

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//<<<<<<<<<<<<<<<<函数申明<<<<<<<<<<<<

#ifdef __cplusplus
extern "C" {
#endif
	
/***********************************************
	Description: void usart_init(PSTUsart com);
		串口初始化
	para:
		串口配置信息
	return:
	
***********************************************/
void com_init(PSTUsart com);
	
/***********************************************
	Description: void usart_send_str(PSTUsart com,void* str,Usart_uint16 len);
		发送字符函数
	para:
		(PSTUsart, 		  void*		,				Usart_uint16)
		串口选择 	发送字符串首地址	发送字符串长度
	return:
	
***********************************************/
void com_send_str(PSTUsart com,void* str,Usart_uint16 len);

/***********************************************
	Description: void set_usart_interrupt_state(PSTUsart com, EUsart_Interrupt_State usart_interrupt_state);
		设定串口中断状态
	para:
		( PSTUsart,        EUSART_Interrupt_State)
		  串口选择 	        中断状态(on or off)
	return:
	
***********************************************/
void set_com_interrupt_state(PSTUsart com, EUsart_Interrupt_State usart_interrupt_state);

/***********************************************
	Description: 	Usart_int8 receive_str_manage(PSTUsart com, void *store, Usart_uint8 count);
		串口数组数据处理函数
	para:
		(		PSTUsart,	void *store, Usart_uint8 count);
			 串口选择     存取地址        备用
	return:
		返回值为function 函数的返回值
***********************************************/
Usart_int8 receive_str_manage(PSTUsart com, void *store, Usart_uint8 count);

/***********************************************
	Description: 	void clear_usart_buff(PSTUsart com);
		清除缓存
	para:
		(		PSTUsart	);
			 串口选择 
	return:
	
***********************************************/
void clear_com_buff(PSTUsart com);


/***********************************************
	Description: 	void usart_send_byte(PSTUsart com,Usart_uint8 byte);
		发送一个字符
	para:
		(		PSTUsart,					Usart_uint8);
			 串口选择           数据
	return:
	
***********************************************/
void com_send_byte(PSTUsart com,Usart_uint8 byte);

/***********************************************
	Description: 	void usart_send_pstr(PSTUsart com,void *p);
		发送一串数据
	para:
		(		PSTUsart,					void *);
			 串口选择     要发送的数据首地址
	return:
	
***********************************************/
void com_send_pstr(PSTUsart com,void *p);

void mystrncpy(void * des, void * souce, uint32 len);

#ifdef __cplusplus
}
#endif

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//----------------测试函数 start----------------
#if defined(TEST_usart_driver) && defined (TEST_SWITCH)
	int _usart_test_(void);
#endif
//----------------测试函数 end------------------

//
//..头文件其他内容
//

#endif/*_USART_H*/
