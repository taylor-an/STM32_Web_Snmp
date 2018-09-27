#include "lwip_comm.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernetif.h" 
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h" 
#include "snmp_msg.h"
#include "malloc.h"
#include "delay.h"
#include "usart.h"
#include "ptp.h"
#include <stdio.h>
#include "led.h"
#include "stmflash.h"
#include "iwdg.h"

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
   

extern struct _m_mallco_dev mallco_dev;
extern u32 memp_get_memorysize(void);	//��memp.c���涨��
extern u8_t *memp_memory;				//��memp.c���涨��.
extern u8_t *ram_heap;					//��mem.c���涨��.

u32 TCPTimer=0;			//TCP��ѯ��ʱ��
u32 ARPTimer=0;			//ARP��ѯ��ʱ��
u32 USARTTimer = 0;
u32 LEDTimer = 0;
u32 NTPInfoTImer = 0;
u32 StateDetectTimer = 0;
u32 lwip_localtime;		//lwip����ʱ�������,��λ:ms



#define LED_TMR_INTERVAL   500
#define USART_TMR_INTERVAL  25
#define NTP_INFO_TMR_INTERVAL 4000
#define STATE_DETECT_TMR_INTERVAL 500

#if LWIP_DHCP
u32 DHCPfineTimer=0;	//DHCP��ϸ�����ʱ��
u32 DHCPcoarseTimer=0;	//DHCP�ֲڴ����ʱ��
#endif

//lwip��mem��memp���ڴ�����
//����ֵ:0,�ɹ�;
//    ����,ʧ��
u8 lwip_comm_mem_malloc(void)
{
	u32 mempsize;
	u32 ramheapsize; 
	mempsize=memp_get_memorysize();			//�õ�memp_memory�����С
	memp_memory=mymalloc(SRAMIN,mempsize);	//Ϊmemp_memory�����ڴ�
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//�õ�ram heap��С
	ram_heap=mymalloc(SRAMIN,ramheapsize);	//Ϊram_heap�����ڴ� 
	if(!memp_memory||!ram_heap)//������ʧ�ܵ�
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip��mem��memp�ڴ��ͷ�
void lwip_comm_mem_free(void)
{ 	
	myfree(SRAMIN,memp_memory);
	myfree(SRAMIN,ram_heap);
}
//lwip Ĭ��IP����
//lwipx:lwip���ƽṹ��ָ��

u32 myip = 0xE11FA8C0;
u32 mynm = 0x00FFFFFF;
u32 mygw = 0x011FA8C0;

void lwip_comm_default_ip_set(__lwip_dev *lwipx)
{
	u32 sn0;
	sn0=*(vu32*)(0x1FFFF7E8);//��ȡSTM32��ΨһID��ǰ24λ��ΪMAC��ַ�����ֽ�
	
	
//	STMFLASH_Write(FLASH_SAVE_IPaddr,(u16*)&myip,2);
//	STMFLASH_Write(FLASH_SAVE_Gatway,(u16*)&mygw,2);
//	STMFLASH_Write(FLASH_SAVE_Mask,(u16*)&mynm,2);
//	
	
	STMFLASH_Read(FLASH_SAVE_IPaddr, (u16*)(lwipdev.ip), 2);//ip���ݱ���
	STMFLASH_Read(FLASH_SAVE_Gatway, (u16*)(lwipdev.gateway), 2);//gw���ݱ���
	STMFLASH_Read(FLASH_SAVE_Mask  , (u16*)(lwipdev.netmask), 2);//netmask���ݱ���	
	
	sprintf((char *)lwipdev.ipbuf, "%03d.%03d.%03d.%03d", lwipdev.ip[0],      lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3]);
	sprintf((char *)lwipdev.nmbuf, "%03d.%03d.%03d.%03d", lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
	sprintf((char *)lwipdev.gwbuf, "%03d.%03d.%03d.%03d", lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
	
	
	//Ĭ��Զ��IPΪ:192.168.1.100
	lwipx->remoteip[0]=192;	
	lwipx->remoteip[1]=168;
	lwipx->remoteip[2]=31;
	lwipx->remoteip[3]=108;
	//MAC��ַ����(�����ֽڹ̶�Ϊ:2.0.0,�����ֽ���STM32ΨһID)
	lwipx->mac[0]=2;//�����ֽ�(IEEE��֮Ϊ��֯ΨһID,OUI)��ַ�̶�Ϊ:2.0.0
	lwipx->mac[1]=0;
	lwipx->mac[2]=0;
	lwipx->mac[3]=(sn0>>16)&0XFF;//�����ֽ���STM32��ΨһID
	lwipx->mac[4]=(sn0>>8)&0XFFF;;
	lwipx->mac[5]=sn0&0XFF;
	sprintf((char *)lwipdev.mcbuf, "%01d.%01d.%01d.%03d.%03d.%03d", lwipdev.mac[0], lwipdev.mac[1], lwipdev.mac[2], lwipdev.mac[3], lwipdev.mac[4], lwipdev.mac[5]);


	lwipx->dhcpstatus=0;//û��DHCP	
} 

//LWIP��ʼ��(LWIP������ʱ��ʹ��)
//����ֵ:0,�ɹ�
//      1,�ڴ����
//      2,LAN8720��ʼ��ʧ��
//      3,�������ʧ��.
u8 lwip_comm_init(void)
{
	struct netif *Netif_Init_Flag;		//����netif_add()����ʱ�ķ���ֵ,�����ж������ʼ���Ƿ�ɹ�
	struct ip_addr ipaddr;  			//ip��ַ
	struct ip_addr netmask; 			//��������
	struct ip_addr gw;      			//Ĭ������ 
	ETH_Mem_Malloc();
	lwip_comm_mem_malloc();
	LAN8720_Init();		
	ptp_init(ETH_PTP_FineUpdate);
	lwip_init();						//��ʼ��LWIP�ں�
    lwip_comm_default_ip_set(&lwipdev);	//����Ĭ��IP����Ϣ

#if LWIP_DHCP		//ʹ�ö�̬IP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				//ʹ�þ�̬IP
	IP4_ADDR(&ipaddr,lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);
	IP4_ADDR(&netmask,lwipdev.netmask[0],lwipdev.netmask[1] ,lwipdev.netmask[2],lwipdev.netmask[3]);
	IP4_ADDR(&gw,lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);
#endif
	Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);//�������б������һ������
	
/**/
	
	if(Netif_Init_Flag==NULL)return 3;//�������ʧ�� 
	else//������ӳɹ���,����netifΪĬ��ֵ,���Ҵ�netif����
	{
		netif_set_default(&lwip_netif); //����netifΪĬ������
		netif_set_up(&lwip_netif);		//��netif����
	}
	
#if LWIP_DHCP			//���ʹ��DHCP�Ļ�
	lwipdev.dhcpstatus=0;	//DHCP���Ϊ0
	//dhcp_start(&lwip_netif);	//����DHCP����
	//lwipdev.dhcpstatus = 1;
#endif
	
#if LWIP_SNMP
	snmp_init();
#endif
	return 0;//����OK.
}   

//�����յ����ݺ���� 
void lwip_pkt_handle(void)
{
  //�����绺�����ж�ȡ���յ������ݰ������䷢�͸�LWIP���� 
 ethernetif_input(&lwip_netif);
}

//LWIP��ѯ����
void lwip_periodic_handle()
{
#if LWIP_TCP
	//ÿ250ms����һ��tcp_tmr()����
  if (lwip_localtime - TCPTimer >= TCP_TMR_INTERVAL)
  {
    TCPTimer =  lwip_localtime;
    tcp_tmr();	 
  }
#endif
  //ARPÿ5s�����Ե���һ��
  if ((lwip_localtime - ARPTimer) >= ARP_TMR_INTERVAL)
  {
    ARPTimer =  lwip_localtime;
    etharp_tmr();
  }

#if LWIP_DHCP //���ʹ��DHCP�Ļ�
  //ÿ500ms����һ��dhcp_fine_tmr()
  if (lwip_localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  lwip_localtime;
    dhcp_fine_tmr();
    if ((lwipdev.dhcpstatus != 2)&&(lwipdev.dhcpstatus != 0XFF))
    { 
      lwip_dhcp_process_handle();  //DHCP����
    }
  }

  //ÿ60sִ��һ��DHCP�ֲڴ���
  if (lwip_localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
  {
    DHCPcoarseTimer =  lwip_localtime;
    dhcp_coarse_tmr();
  }  
#endif
  
  if(lwip_localtime - LEDTimer >= led_mode_con.LED_Step)
  {
	  LEDTimer = lwip_localtime;
	  led_mode_con.LED_Step = led_mode_con.LED_Mode_Arr[led_mode_con.LED_Mode][led_mode_con.LED_Cnt++];
	  led_mode_con.LED_Cnt %= led_mode_con.LED_Max_State;
	  LED0 = !LED0;
	  IWDG_Feed();
  }
  
  if(lwip_localtime - USARTTimer >= USART_TMR_INTERVAL)
  {
	    USARTTimer = lwip_localtime;
		receive_str_manage(&usart2,NULL,0);
		receive_str_manage(&usart4,NULL,0);
		receive_str_manage(&usart1, NULL, 0);
  }
  
  if(lwip_localtime - NTPInfoTImer >= NTP_INFO_TMR_INTERVAL)
  {
	  NTPInfoTImer = lwip_localtime;
	  u8 i = 0;
	  char buf[200];
	  buf[0] = '$';
	  buf[1] = 'N';
	  buf[2] = 'T';
	  buf[3] = 'P';
	  for(i = 0; i < 16; i++)
	  {	  
		  *(((int *)(buf+4)) + i*3)     = SntpMonitorl[i].ip;
		  *(((int *)(buf+4)) + i*3 + 1) = SntpMonitorl[i].sec;
		  *(((int *)(buf+4)) + i*3 + 2) = SntpMonitorl[i].nsec;
	  }
	  buf[196] = 0x55;
	  com_send_str(&usart1, buf, 197);
  }
  
  if(lwip_localtime - StateDetectTimer >= STATE_DETECT_TMR_INTERVAL)
  {
	  StateDetectTimer = lwip_localtime;  
	  ptp_state.state = (--ptp_state.state)%0xFF;     //�����Լ��������
	  e1_state.state  = (--e1_state.state)%0xFF;      //�����Լ��������
  }
}


//���ʹ����DHCP
#if LWIP_DHCP

//DHCP��������
void lwip_dhcp_process_handle(void)
{
	u32 ip=0,netmask=0,gw=0;
	switch(lwipdev.dhcpstatus)
	{
		case 0: 	//����DHCP
			dhcp_start(&lwip_netif);
			lwipdev.dhcpstatus = 1;		//�ȴ�ͨ��DHCP��ȡ���ĵ�ַ
			break;
		case 1:		//�ȴ���ȡ��IP��ַ
		{
			ip=lwip_netif.ip_addr.addr;		//��ȡ��IP��ַ
			netmask=lwip_netif.netmask.addr;//��ȡ��������
			gw=lwip_netif.gw.addr;			//��ȡĬ������ 
			
			if(ip!=0)			//��ȷ��ȡ��IP��ַ��ʱ��
			{
				lwipdev.dhcpstatus=2;	//DHCP�ɹ�
				//������ͨ��DHCP��ȡ����IP��ַ
				lwipdev.ip[3]=(uint8_t)(ip>>24); 
				lwipdev.ip[2]=(uint8_t)(ip>>16);
				lwipdev.ip[1]=(uint8_t)(ip>>8);
				lwipdev.ip[0]=(uint8_t)(ip);
				//����ͨ��DHCP��ȡ�������������ַ
				lwipdev.netmask[3]=(uint8_t)(netmask>>24);
				lwipdev.netmask[2]=(uint8_t)(netmask>>16);
				lwipdev.netmask[1]=(uint8_t)(netmask>>8);
				lwipdev.netmask[0]=(uint8_t)(netmask);
				//������ͨ��DHCP��ȡ����Ĭ������
				lwipdev.gateway[3]=(uint8_t)(gw>>24);
				lwipdev.gateway[2]=(uint8_t)(gw>>16);
				lwipdev.gateway[1]=(uint8_t)(gw>>8);
				lwipdev.gateway[0]=(uint8_t)(gw);
			}else if(lwip_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //ͨ��DHCP�����ȡIP��ַʧ��,�ҳ�������Դ���
			{
				lwipdev.dhcpstatus=0XFF;//DHCP��ʱʧ��.
				//ʹ�þ�̬IP��ַ
				IP4_ADDR(&(lwip_netif.ip_addr),lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);
				IP4_ADDR(&(lwip_netif.netmask),lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);
				IP4_ADDR(&(lwip_netif.gw),lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);
			}
		}
		break;
		default : break;
	}
}
#endif 



























