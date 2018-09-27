#include "sys.h"
#include "lwip_comm.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "timer.h"
#include "sntp.h"
#include "usart.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "type.h"
#include "stmflash.h"

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
 const char *LocalIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);           //����IP����
 const char *PTPIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);             //PTP IP ����
 const char *TimeSourceSelect_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);     //ʱ�����ȼ�����
 const char *PTPModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
 const char *E1ModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);           //E1 ����ģʽ����
 const char *RemoteIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);

/*SNMP*/
//par[i]ָ��SNMP�����ݻ�����  par_len[i]ָ�����ݻ������ĳ���
 u8 lwip_privmib_MspInit(u8 **par[], u8 **par_len[]);

 uint16 LocalIPInfo_Handler(char *const pcInsert)
{
	//IP ����  �������� 
	if(((uint16)lwipdev.ip[0] + lwipdev.ip[1] + lwipdev.ip[2] + lwipdev.ip[3]) != 0)  //��IP��ȫΪ0ʱ ������Ч
	{
		sprintf(pcInsert, "$%d.%d.%d.%d$%d.%d.%d.%d$%d.%d.%d.%d",lwipdev.ip[0],      lwipdev.ip[1],		lwipdev.ip[2],	    lwipdev.ip[3],
															lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3],
															lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
	}
	else
	{
		sprintf(pcInsert, "$暂无数据$暂无数据$暂无数据");  //UTF-8�����3�� "��������"
	}
	return strlen(pcInsert);
}

 uint16 NTPIPInfo_Handler(char *const pcInsert)
{
	//IP ����  ��������
	if(((uint16)ptp_state.ip[0] + ptp_state.ip[1] + ptp_state.ip[2] + ptp_state.ip[3]) != 0) //��IP��ȫΪ0ʱ ������Ч
	{
		sprintf(pcInsert, "$%d.%d.%d.%d$%d.%d.%d.%d$%d.%d.%d.%d",ptp_state.ip[0],      ptp_state.ip[1],		ptp_state.ip[2],	  ptp_state.ip[3],
																ptp_state.gateway[0], ptp_state.gateway[1], ptp_state.gateway[2], ptp_state.gateway[3],
																ptp_state.netmask[0], ptp_state.netmask[1], ptp_state.netmask[2], ptp_state.netmask[3]);
	}
	else
	{
		sprintf(pcInsert, "$暂无数据$暂无数据$暂无数据"); //UTF-8�����3�� "��������"
	}
	return strlen(pcInsert);
}



uint32 my_Pow(uint8 m,uint8 n)
{
	uint32 result=1;	 
	while(n--)result*=m;    
	return result;
}
uint16 SystemState_Handler(char *const pcInsert)
{
	
	if((sysstate.x != 0) && (sysstate.y != 0))
	{	//�� - �� - �� - ʱ - �� - �� - ��γ�� - ѱ��״̬ -- ʱ��Դѡ��
		u32  x1, y1;
		u32 x2, y2;
		
		
		
		x1 = sysstate.x / my_Pow(10, sysstate.x_dx); //�õ���������  
		x2 = sysstate.x %my_Pow(10, sysstate.x_dx);   //�õ�С������
	
		y1 = sysstate.y / my_Pow(10, sysstate.y_dx); //�õ���������  
		y2 = sysstate.y % my_Pow(10, sysstate.y_dx);   //�õ�С������
		
		sprintf(pcInsert, "$%d/%d/%d--%d:%d:%d$%d°%d.%d'%c,  %d°%d.%d'%c$%d$%d", sysstate.date.year, sysstate.date.month, sysstate.date.day, 
															 sysstate.date.hour, sysstate.date.minute, sysstate.date.second, 
															 x1/100, x1%100, x2, sysstate.ns, y1/100, y1%100, y2, sysstate.ew, 
															 sysstate.tameState, TimeSource[0]);
	}
	else
	{                                         //"��������"//
		sprintf(pcInsert, "$%d/%d/%d--%d:%d:%d$暂无数据$%d$%d", sysstate.date.year, sysstate.date.month, sysstate.date.day, 
															 sysstate.date.hour, sysstate.date.minute, sysstate.date.second, 
															 sysstate.tameState, TimeSource[0]);
	}
	return strlen(pcInsert);
}

uint16 PTPE1State_Handler(char *const pcInsert)
{
	//ģʽ  ����״̬  ��-��-��-ʱ-��-��
	u8 len = 0;
	sprintf(pcInsert, "$%d$%d$%d/%d/%d--%d:%d:%d", ptp_state.mode, (ptp_state.state!=0)+10, 
												   ptp_state.date.year, ptp_state.date.month, ptp_state.date.day, 
												   ptp_state.date.hour, ptp_state.date.minute, ptp_state.date.second);
	len = strlen(pcInsert);
	if(((uint16)ptp_state.remoteip[0] + ptp_state.remoteip[1] + ptp_state.remoteip[2] + ptp_state.remoteip[3]) != 0) //IP��ȫΪ0ʱ ������Ч
	{
		//Զ��IP
		sprintf(pcInsert+len, "$%d.%d.%d.%d", ptp_state.remoteip[0], ptp_state.remoteip[1], ptp_state.remoteip[2], ptp_state.remoteip[3]);
	}
	else 
	{
		sprintf(pcInsert+len, "$暂无数据"); //UTF-8�����3�� "��������"
	}
	
	len = strlen(pcInsert);
	//E1ģʽ ����״̬ ������ ʱ����
	sprintf(pcInsert+len, "$%d$%d$%d/%d/%d--%d:%d:%d", e1_state.mode,      (e1_state.state!=0)+20, 
													   e1_state.date.year, e1_state.date.month, e1_state.date.day, 
													   e1_state.date.hour, e1_state.date.minute, e1_state.date.second);
	return strlen(pcInsert);
}

uint16 TimeSource_Handler(char *const pcInsert)
{
	//������ȼ� -- ������ȼ�
	sprintf(pcInsert, "$%d$%d$%d$%d$%d$%d$%d", TimeSource[0], TimeSource[1], TimeSource[2], TimeSource[3], TimeSource[4], TimeSource[5], TimeSource[6]);
	return strlen(pcInsert);
}

uint16 PTPGroup1_Handler(char *const pcInsert)
{	
	u8 *pTemp =(u8 *) &(PtpMonitorl[0].ip);
	u16 len = 0;

	for(u8 i = 0; i < 8; i++)
	{
		pTemp = (u8 *)&(PtpMonitorl[i].ip);
		if(((uint16)(*(pTemp)) + *(pTemp+1) + *(pTemp+2) + *(pTemp+3)) != 0) //IP��ȫΪ0ʱ ������Ч
		{
			sprintf(pcInsert+len, "$%d.%d.%d.%d$%d$%d$%d", *(pTemp), *(pTemp+1), *(pTemp+2), *(pTemp+3), PtpMonitorl[i].sec, PtpMonitorl[i].nsec,																		PtpMonitorl[i].updatetime);
			len = strlen(pcInsert);
		}
	}
	
	return len;
}

uint16 PTPGroup2_Handler(char *const pcInsert)
{
	u8 *pTemp =(u8 *) &(PtpMonitorl[8].ip);
	u16 len = 0;
	
	for(u8 i = 8; i < 16; i++)
	{
		pTemp = (u8 *)&(PtpMonitorl[i].ip);
		if(((uint16)(*(pTemp)) + *(pTemp+1) + *(pTemp+2) + *(pTemp+3)) != 0)
		{
			sprintf(pcInsert+len, "$%d.%d.%d.%d$%d$%d$%d", *(pTemp), *(pTemp+1), *(pTemp+2), *(pTemp+3), PtpMonitorl[i].sec, PtpMonitorl[i].nsec, PtpMonitorl[i].updatetime);
			len = strlen(pcInsert);
		}
	}
	return len;
}

uint16 NTPGroup1_Handler(char *const pcInsert)
{
	u8 *pTemp = (u8 *)&(SntpMonitorl[0].ip);
	u16 len = 0;
	
	for(u8 i = 0; i < 8; i++)
	{
		pTemp = (u8 *)&(SntpMonitorl[i].ip);
		if(((uint16)(*(pTemp)) + *(pTemp+1) + *(pTemp+2) + *(pTemp+3)) != 0)
		{
			sprintf(pcInsert+len, "$%d.%d.%d.%d$%d$%d$%d", *(pTemp), *(pTemp+1), *(pTemp+2), *(pTemp+3), SntpMonitorl[i].sec, SntpMonitorl[i].nsec, SntpMonitorl[i].updatetime);
			len = strlen(pcInsert);		
		}
	}
	return len;
}

uint16 NTPGroup2_Handler(char *const pcInsert)
{
	u8 *pTemp = (u8 *)&(SntpMonitorl[8].ip);
	u16 len = 0;

	for(u8 i = 8; i < 16; i++)
	{
		pTemp = (u8 *)&(SntpMonitorl[i].ip);
		if(((uint16)(*(pTemp)) + *(pTemp+1) + *(pTemp+2) + *(pTemp+3)) != 0)
		{
			sprintf(pcInsert+len, "$%d.%d.%d.%d$%d$%d$%d", *(pTemp), *(pTemp+1), *(pTemp+2), *(pTemp+3), SntpMonitorl[i].sec, SntpMonitorl[i].nsec, SntpMonitorl[i].updatetime);
			len = strlen(pcInsert);	
		}
	}
	return len;
}

uint16 E1Group1_Handler(char *const pcInsert)
{
	u8 *pTemp = (u8 *)&(E1Monitor1[0].ip);
	u16 len = 0;

	for(u8 i = 0; i < 8; i++)
	{
		pTemp = (u8 *)&(E1Monitor1[i].ip);
		if(((uint16)(*(pTemp)) + *(pTemp+1) + *(pTemp+2) + *(pTemp+3)) != 0)
		{
			sprintf(pcInsert+len, "$%d.%d.%d.%d$%d$%d$%d", *(pTemp), *(pTemp+1), *(pTemp+2), *(pTemp+3), E1Monitor1[i].sec, E1Monitor1[i].nsec, E1Monitor1[i].updatetime);
			len = strlen(pcInsert);	
		}
	}
	return len;
}

uint16 E1Group2_Handler(char *const pcInsert)
{
	u8 *pTemp = (u8 *)&(E1Monitor1[8].ip);
	u16 len = 0;

	for(u8 i = 8; i < 16; i++)
	{
		pTemp = (u8 *)&(E1Monitor1[i].ip);
		if(((uint16)(*(pTemp)) + *(pTemp+1) + *(pTemp+2) + *(pTemp+3)) != 0)
		{
			sprintf(pcInsert+len, "$%d.%d.%d.%d$%d$%d$%d", *(pTemp), *(pTemp+1), *(pTemp+2), *(pTemp+3), E1Monitor1[i].sec, E1Monitor1[i].nsec, E1Monitor1[i].updatetime);
			len = strlen(pcInsert);	
		}
	}
	return len;
}



const char *LocalIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[])
{
	char buf[100];
	lwipdev.ip[0] = (pcValue[0][0] - '0') * 100 + (pcValue[0][1] - '0') * 10 + (pcValue[0][2] - '0');
	lwipdev.ip[1] = (pcValue[0][3] - '0') * 100 + (pcValue[0][4] - '0') * 10 + (pcValue[0][5] - '0');
	lwipdev.ip[2] = (pcValue[0][6] - '0') * 100 + (pcValue[0][7] - '0') * 10 + (pcValue[0][8] - '0');
	lwipdev.ip[3] = (pcValue[0][9] - '0') * 100 + (pcValue[0][10] - '0') * 10 + (pcValue[0][11] - '0');
	
	lwipdev.gateway[0] = (pcValue[1][0] - '0') * 100 + (pcValue[1][1] - '0') * 10 + (pcValue[1][2] - '0');
	lwipdev.gateway[1] = (pcValue[1][3] - '0') * 100 + (pcValue[1][4] - '0') * 10 + (pcValue[1][5] - '0');
	lwipdev.gateway[2] = (pcValue[1][6] - '0') * 100 + (pcValue[1][7] - '0') * 10 + (pcValue[1][8] - '0');
	lwipdev.gateway[3] = (pcValue[1][9] - '0') * 100 + (pcValue[1][10] - '0') * 10 + (pcValue[1][11] - '0');
	
	lwipdev.netmask[0] = (pcValue[2][0] - '0') * 100 + (pcValue[2][1] - '0') * 10 + (pcValue[2][2] - '0');
	lwipdev.netmask[1] = (pcValue[2][3] - '0') * 100 + (pcValue[2][4] - '0') * 10 + (pcValue[2][5] - '0');
	lwipdev.netmask[2] = (pcValue[2][6] - '0') * 100 + (pcValue[2][7] - '0') * 10 + (pcValue[2][8] - '0');
	lwipdev.netmask[3] = (pcValue[2][9] - '0') * 100 + (pcValue[2][10] - '0') * 10 + (pcValue[2][11] - '0');
	
	
	STMFLASH_Write(FLASH_SAVE_IPaddr,(u16*)lwipdev.ip,2);
	STMFLASH_Write(FLASH_SAVE_Gatway,(u16*)lwipdev.gateway,2);
	STMFLASH_Write(FLASH_SAVE_Mask,(u16*)lwipdev.netmask,2);
	
	IP4_ADDR(&(lwip_netif.ip_addr),lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);
	IP4_ADDR(&(lwip_netif.netmask),lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);
	IP4_ADDR(&(lwip_netif.gw),lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);
	
	sprintf((char *)lwipdev.ipbuf, "%03d.%03d.%03d.%03d", lwipdev.ip[0],      lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3]);
	sprintf((char *)lwipdev.nmbuf, "%03d.%03d.%03d.%03d", lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
	sprintf((char *)lwipdev.gwbuf, "%03d.%03d.%03d.%03d", lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
	
	sprintf(buf, "AT+SNBK=%d,%d,%d,%d/%d,%d,%d,%d/%d,%d,%d,%d/\r\n", lwipdev.ip[0], 	 lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3], 
																     lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3],
																  	 lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
	com_send_str(&usart1, buf, sizeof(buf));
	return "/set.shtml";
}
const char *PTPIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[])
{
	char buf[100];
	ptp_state.ip[0] = (pcValue[0][0] - '0') * 100 + (pcValue[0][1] - '0') * 10 + (pcValue[0][2] - '0');
	ptp_state.ip[1] = (pcValue[0][3] - '0') * 100 + (pcValue[0][4] - '0') * 10 + (pcValue[0][5] - '0');
	ptp_state.ip[2] = (pcValue[0][6] - '0') * 100 + (pcValue[0][7] - '0') * 10 + (pcValue[0][8] - '0');
	ptp_state.ip[3] = (pcValue[0][9] - '0') * 100 + (pcValue[0][10] - '0') * 10 + (pcValue[0][11] - '0');
	
	ptp_state.gateway[0] = (pcValue[1][0] - '0') * 100 + (pcValue[1][1] - '0') * 10 + (pcValue[1][2] - '0');
	ptp_state.gateway[1] = (pcValue[1][3] - '0') * 100 + (pcValue[1][4] - '0') * 10 + (pcValue[1][5] - '0');
	ptp_state.gateway[2] = (pcValue[1][6] - '0') * 100 + (pcValue[1][7] - '0') * 10 + (pcValue[1][8] - '0');
	ptp_state.gateway[3] = (pcValue[1][9] - '0') * 100 + (pcValue[1][10] - '0') * 10 + (pcValue[1][11] - '0');
	
	ptp_state.netmask[0] = (pcValue[2][0] - '0') * 100 + (pcValue[2][1] - '0') * 10 + (pcValue[2][2] - '0');
	ptp_state.netmask[1] = (pcValue[2][3] - '0') * 100 + (pcValue[2][4] - '0') * 10 + (pcValue[2][5] - '0');
	ptp_state.netmask[2] = (pcValue[2][6] - '0') * 100 + (pcValue[2][7] - '0') * 10 + (pcValue[2][8] - '0');
	ptp_state.netmask[3] = (pcValue[2][9] - '0') * 100 + (pcValue[2][10] - '0') * 10 + (pcValue[2][11] - '0');
	
	sprintf(buf, "AT+PTBK=%d,%d,%d,%d/%d,%d,%d,%d/%d,%d,%d,%d/\r\n", ptp_state.ip[0], 	 ptp_state.ip[1],      ptp_state.ip[2],      ptp_state.ip[3], 
																     ptp_state.netmask[0], ptp_state.netmask[1], ptp_state.netmask[2], ptp_state.netmask[3],
																  	 ptp_state.gateway[0], ptp_state.gateway[1], ptp_state.gateway[2], ptp_state.gateway[3]);
	com_send_str(&usart1, buf, strlen(buf));
	
	sprintf(buf, "AT+SPTP=%d,%d,%d,%d/%d,%d,%d,%d/%d,%d,%d,%d/\r\n", ptp_state.ip[0], 	   ptp_state.ip[1],      ptp_state.ip[2],      ptp_state.ip[3], 
																     ptp_state.netmask[0], ptp_state.netmask[1], ptp_state.netmask[2], ptp_state.netmask[3],
																  	 ptp_state.gateway[0], ptp_state.gateway[1], ptp_state.gateway[2], ptp_state.gateway[3]);
	com_send_str(&usart4, buf, strlen(buf));
	return "/set.shtml";
}
const char *TimeSourceSelect_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[])
{
	int temp = 0;
	unsigned char i = 0;
	char buf[20];
	temp = atoi(pcValue[0]);
	for(i = 0; i < 7; i++)
	{
		TimeSource[6-i] = temp%10;
		temp/=10;
	}
	sprintf(buf, "$TIMESOURCE");
	
	buf[11] = TimeSource[0];
	buf[12] = TimeSource[1];
	buf[13]	= TimeSource[2]; 
    buf[14]	= TimeSource[3];
	buf[15] = TimeSource[4];
    buf[16] = TimeSource[5];
    buf[17] = TimeSource[6]; 
	buf[18] = 0x55;
	com_send_str(&usart1, buf, 19);
	return "/set.shtml";
}

const char *PTPModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[])
{
	ptp_state.mode = atoi(pcValue[0]);
	if(ptp_state.mode)
	{
		com_send_str(&usart4, "PTP Master", sizeof("PTP Master"));
	}
	else
	{
		com_send_str(&usart4, "PTP Slave", sizeof("PTP Slave"));
	}
	return "/set.shtml";
}
const char *E1ModeSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[])
{
	e1_state.mode = atoi(pcValue[0]);
	if(e1_state.mode)
	{
		
	}
	else
	{
		
	}
	return "/set.shtml";
}

const char *RemoteIPSet_CGI_Handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[])
{
	char buf[50];
	ptp_state.remoteip[0] = (pcValue[0][0] - '0') * 100 + (pcValue[0][1] - '0') * 10 + (pcValue[0][2] - '0');
	ptp_state.remoteip[1] = (pcValue[0][3] - '0') * 100 + (pcValue[0][4] - '0') * 10 + (pcValue[0][5] - '0');
	ptp_state.remoteip[2] = (pcValue[0][6] - '0') * 100 + (pcValue[0][7] - '0') * 10 + (pcValue[0][8] - '0');
	ptp_state.remoteip[3] = (pcValue[0][9] - '0') * 100 + (pcValue[0][10] - '0') * 10 + (pcValue[0][11] - '0');
	sprintf(buf, "AT+UDPT=%d,%d,%d,%d/8080/", ptp_state.remoteip[0], ptp_state.remoteip[1], ptp_state.remoteip[2], ptp_state.remoteip[3]);
	sprintf((char *)ptp_state.rpbuf, "%03d.%03d.%03d.%03d", ptp_state.remoteip[0], ptp_state.remoteip[1], ptp_state.remoteip[2], ptp_state.remoteip[3]);		
	
	com_send_str(&usart4, buf, strlen(buf));
	com_send_str(&usart2, buf, strlen(buf));
	return "/set.shtml";
}


