#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "lwip_comm.h"
#include <string.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//lwipͨ������ ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 

#define NUM_CONFIG_CGI_URIS	6  //CGI��URI����
#define NUM_CONFIG_SSI_TAGS	15  //SSI��TAG����

/*SSI*/
//*pcInsert ���Ҫ��ʾ������ 
 uint16 LocalIPInfo_Handler(char *const pcInsert);      //����IP�����Ϣ
 uint16 NTPIPInfo_Handler(char *const pcInsert);		//NTP\PTP�����Ϣ
 uint16 SystemState_Handler(char *const pcInsert);	    //ϵͳ״̬��Ϣ
 uint16 PTPE1State_Handler(char *const pcInsert);		//PTP\E1״̬��Ϣ
 uint16 TimeSource_Handler(char *const pcInsert);		//ʱ��Դ
 uint16 PTPGroup1_Handler(char *const pcInsert);		//PTP��1
 uint16 PTPGroup2_Handler(char *const pcInsert);	    //PTP��2
 uint16 NTPGroup1_Handler(char *const pcInsert);		//NTP��1
 uint16 NTPGroup2_Handler(char *const pcInsert);		//NTP��2
 uint16 E1Group1_Handler(char *const pcInsert);         //E1��1
 uint16 E1Group2_Handler(char *const pcInsert);			//E1��2

/*CGI*/
//iIndex��δ�õ�  iNumParams:���ݸ���  pcParam:��������  pcValue:�����Ӧ������
 const char *LocalIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *PTPIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *TimeSourceSelect_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *PTPModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *E1ModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);  
 const char *RemoteIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
u8_t TimeSource[7] = {0, 1, 2, 3, 4, 5, 6};


static const char *ppcTAGs[] = 
{
	"LocalIPInfo",  	 //����IP��Ϣ
	"NTPIPInfo",      	 //NTPIP��Ϣ
	"SystemState",       //ϵͳ״̬			
	"PTPE1State",        //PTP\E1״̬	
	"OrderInfo",        //ʱ��Դ
	
	"PTPGroup1",		 //PTP�����1
	"PTPGroup2",		 //PTP�����2
	"NTPGroup1",	     //NTP�����1
	"NTPGroup2",         //NTP�����2 	
	"E1Group1",          //E1�����1
	"E1Group2"			 //E1�����2
};

static const tCGI ppcURLs[] = 
{
	{"/localip.cgi",   LocalIPSet_CGI_Handler},
	{"/ptpip.cgi",     PTPIPSet_CGI_Handler},
	{"/Order.cgi",     TimeSourceSelect_CGI_Handler},
	{"/PTPMode.cgi",   PTPModeSet_CGI_Handler},
	{"/E1Mode.cgi",    E1ModeSet_CGI_Handler},
	{"/remote.cgi",    RemoteIPSet_CGI_Handler}
};


typedef uint16 (*func_t)(char *);

func_t Handler_Arr[] = {LocalIPInfo_Handler,  NTPIPInfo_Handler,   SystemState_Handler,
						PTPE1State_Handler,   TimeSource_Handler,  PTPGroup1_Handler, 
						PTPGroup2_Handler,    NTPGroup1_Handler,   NTPGroup2_Handler,
						E1Group1_Handler,     E1Group2_Handler 
};

static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	return (Handler_Arr[iIndex])(pcInsert);
}

//SSI�����ʼ��
void httpd_ssi_init(void)
{  
	//����SSI���
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI�����ʼ��
void httpd_cgi_init(void)
{ 
  //����CGI���
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


