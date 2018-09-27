#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "lwip_comm.h"
#include <string.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//lwip通用驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 

#define NUM_CONFIG_CGI_URIS	6  //CGI的URI数量
#define NUM_CONFIG_SSI_TAGS	15  //SSI的TAG数量

/*SSI*/
//*pcInsert 存放要显示的内容 
 uint16 LocalIPInfo_Handler(char *const pcInsert);      //本机IP相关信息
 uint16 NTPIPInfo_Handler(char *const pcInsert);		//NTP\PTP相关信息
 uint16 SystemState_Handler(char *const pcInsert);	    //系统状态信息
 uint16 PTPE1State_Handler(char *const pcInsert);		//PTP\E1状态信息
 uint16 TimeSource_Handler(char *const pcInsert);		//时钟源
 uint16 PTPGroup1_Handler(char *const pcInsert);		//PTP组1
 uint16 PTPGroup2_Handler(char *const pcInsert);	    //PTP组2
 uint16 NTPGroup1_Handler(char *const pcInsert);		//NTP组1
 uint16 NTPGroup2_Handler(char *const pcInsert);		//NTP组2
 uint16 E1Group1_Handler(char *const pcInsert);         //E1组1
 uint16 E1Group2_Handler(char *const pcInsert);			//E1组2

/*CGI*/
//iIndex：未用到  iNumParams:数据个数  pcParam:表单项名称  pcValue:表单项对应的数据
 const char *LocalIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *PTPIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *TimeSourceSelect_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *PTPModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *E1ModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);  
 const char *RemoteIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
u8_t TimeSource[7] = {0, 1, 2, 3, 4, 5, 6};


static const char *ppcTAGs[] = 
{
	"LocalIPInfo",  	 //本地IP信息
	"NTPIPInfo",      	 //NTPIP信息
	"SystemState",       //系统状态			
	"PTPE1State",        //PTP\E1状态	
	"OrderInfo",        //时钟源
	
	"PTPGroup1",		 //PTP监控组1
	"PTPGroup2",		 //PTP监控组2
	"NTPGroup1",	     //NTP监控组1
	"NTPGroup2",         //NTP监控组2 	
	"E1Group1",          //E1监控组1
	"E1Group2"			 //E1监控组2
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

//SSI句柄初始化
void httpd_ssi_init(void)
{  
	//配置SSI句柄
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI句柄初始化
void httpd_cgi_init(void)
{ 
  //配置CGI句柄
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


