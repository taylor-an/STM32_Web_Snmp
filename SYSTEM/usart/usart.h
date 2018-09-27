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

//<<<<<<<<<<<<<<<<��������<<<<<<<<<<<<

#ifdef __cplusplus
extern "C" {
#endif
	
/***********************************************
	Description: void usart_init(PSTUsart com);
		���ڳ�ʼ��
	para:
		����������Ϣ
	return:
	
***********************************************/
void com_init(PSTUsart com);
	
/***********************************************
	Description: void usart_send_str(PSTUsart com,void* str,Usart_uint16 len);
		�����ַ�����
	para:
		(PSTUsart, 		  void*		,				Usart_uint16)
		����ѡ�� 	�����ַ����׵�ַ	�����ַ�������
	return:
	
***********************************************/
void com_send_str(PSTUsart com,void* str,Usart_uint16 len);

/***********************************************
	Description: void set_usart_interrupt_state(PSTUsart com, EUsart_Interrupt_State usart_interrupt_state);
		�趨�����ж�״̬
	para:
		( PSTUsart,        EUSART_Interrupt_State)
		  ����ѡ�� 	        �ж�״̬(on or off)
	return:
	
***********************************************/
void set_com_interrupt_state(PSTUsart com, EUsart_Interrupt_State usart_interrupt_state);

/***********************************************
	Description: 	Usart_int8 receive_str_manage(PSTUsart com, void *store, Usart_uint8 count);
		�����������ݴ�����
	para:
		(		PSTUsart,	void *store, Usart_uint8 count);
			 ����ѡ��     ��ȡ��ַ        ����
	return:
		����ֵΪfunction �����ķ���ֵ
***********************************************/
Usart_int8 receive_str_manage(PSTUsart com, void *store, Usart_uint8 count);

/***********************************************
	Description: 	void clear_usart_buff(PSTUsart com);
		�������
	para:
		(		PSTUsart	);
			 ����ѡ�� 
	return:
	
***********************************************/
void clear_com_buff(PSTUsart com);


/***********************************************
	Description: 	void usart_send_byte(PSTUsart com,Usart_uint8 byte);
		����һ���ַ�
	para:
		(		PSTUsart,					Usart_uint8);
			 ����ѡ��           ����
	return:
	
***********************************************/
void com_send_byte(PSTUsart com,Usart_uint8 byte);

/***********************************************
	Description: 	void usart_send_pstr(PSTUsart com,void *p);
		����һ������
	para:
		(		PSTUsart,					void *);
			 ����ѡ��     Ҫ���͵������׵�ַ
	return:
	
***********************************************/
void com_send_pstr(PSTUsart com,void *p);

void mystrncpy(void * des, void * souce, uint32 len);

#ifdef __cplusplus
}
#endif

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//----------------���Ժ��� start----------------
#if defined(TEST_usart_driver) && defined (TEST_SWITCH)
	int _usart_test_(void);
#endif
//----------------���Ժ��� end------------------

//
//..ͷ�ļ���������
//

#endif/*_USART_H*/
