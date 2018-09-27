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
   

extern struct _m_mallco_dev mallco_dev;
extern u32 memp_get_memorysize(void);	//在memp.c里面定义
extern u8_t *memp_memory;				//在memp.c里面定义.
extern u8_t *ram_heap;					//在mem.c里面定义.

u32 TCPTimer=0;			//TCP查询计时器
u32 ARPTimer=0;			//ARP查询计时器
u32 USARTTimer = 0;
u32 LEDTimer = 0;
u32 NTPInfoTImer = 0;
u32 StateDetectTimer = 0;
u32 lwip_localtime;		//lwip本地时间计数器,单位:ms



#define LED_TMR_INTERVAL   500
#define USART_TMR_INTERVAL  25
#define NTP_INFO_TMR_INTERVAL 4000
#define STATE_DETECT_TMR_INTERVAL 500

#if LWIP_DHCP
u32 DHCPfineTimer=0;	//DHCP精细处理计时器
u32 DHCPcoarseTimer=0;	//DHCP粗糙处理计时器
#endif

//lwip中mem和memp的内存申请
//返回值:0,成功;
//    其他,失败
u8 lwip_comm_mem_malloc(void)
{
	u32 mempsize;
	u32 ramheapsize; 
	mempsize=memp_get_memorysize();			//得到memp_memory数组大小
	memp_memory=mymalloc(SRAMIN,mempsize);	//为memp_memory申请内存
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//得到ram heap大小
	ram_heap=mymalloc(SRAMIN,ramheapsize);	//为ram_heap申请内存 
	if(!memp_memory||!ram_heap)//有申请失败的
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip中mem和memp内存释放
void lwip_comm_mem_free(void)
{ 	
	myfree(SRAMIN,memp_memory);
	myfree(SRAMIN,ram_heap);
}
//lwip 默认IP设置
//lwipx:lwip控制结构体指针

u32 myip = 0xE11FA8C0;
u32 mynm = 0x00FFFFFF;
u32 mygw = 0x011FA8C0;

void lwip_comm_default_ip_set(__lwip_dev *lwipx)
{
	u32 sn0;
	sn0=*(vu32*)(0x1FFFF7E8);//获取STM32的唯一ID的前24位作为MAC地址后三字节
	
	
//	STMFLASH_Write(FLASH_SAVE_IPaddr,(u16*)&myip,2);
//	STMFLASH_Write(FLASH_SAVE_Gatway,(u16*)&mygw,2);
//	STMFLASH_Write(FLASH_SAVE_Mask,(u16*)&mynm,2);
//	
	
	STMFLASH_Read(FLASH_SAVE_IPaddr, (u16*)(lwipdev.ip), 2);//ip数据保存
	STMFLASH_Read(FLASH_SAVE_Gatway, (u16*)(lwipdev.gateway), 2);//gw数据保存
	STMFLASH_Read(FLASH_SAVE_Mask  , (u16*)(lwipdev.netmask), 2);//netmask数据保存	
	
	sprintf((char *)lwipdev.ipbuf, "%03d.%03d.%03d.%03d", lwipdev.ip[0],      lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3]);
	sprintf((char *)lwipdev.nmbuf, "%03d.%03d.%03d.%03d", lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
	sprintf((char *)lwipdev.gwbuf, "%03d.%03d.%03d.%03d", lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
	
	
	//默认远端IP为:192.168.1.100
	lwipx->remoteip[0]=192;	
	lwipx->remoteip[1]=168;
	lwipx->remoteip[2]=31;
	lwipx->remoteip[3]=108;
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	lwipx->mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	lwipx->mac[1]=0;
	lwipx->mac[2]=0;
	lwipx->mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	lwipx->mac[4]=(sn0>>8)&0XFFF;;
	lwipx->mac[5]=sn0&0XFF;
	sprintf((char *)lwipdev.mcbuf, "%01d.%01d.%01d.%03d.%03d.%03d", lwipdev.mac[0], lwipdev.mac[1], lwipdev.mac[2], lwipdev.mac[3], lwipdev.mac[4], lwipdev.mac[5]);


	lwipx->dhcpstatus=0;//没有DHCP	
} 

//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,LAN8720初始化失败
//      3,网卡添加失败.
u8 lwip_comm_init(void)
{
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	struct ip_addr ipaddr;  			//ip地址
	struct ip_addr netmask; 			//子网掩码
	struct ip_addr gw;      			//默认网关 
	ETH_Mem_Malloc();
	lwip_comm_mem_malloc();
	LAN8720_Init();		
	ptp_init(ETH_PTP_FineUpdate);
	lwip_init();						//初始化LWIP内核
    lwip_comm_default_ip_set(&lwipdev);	//设置默认IP等信息

#if LWIP_DHCP		//使用动态IP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				//使用静态IP
	IP4_ADDR(&ipaddr,lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);
	IP4_ADDR(&netmask,lwipdev.netmask[0],lwipdev.netmask[1] ,lwipdev.netmask[2],lwipdev.netmask[3]);
	IP4_ADDR(&gw,lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);
#endif
	Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);//向网卡列表中添加一个网口
	
/**/
	
	if(Netif_Init_Flag==NULL)return 3;//网卡添加失败 
	else//网口添加成功后,设置netif为默认值,并且打开netif网口
	{
		netif_set_default(&lwip_netif); //设置netif为默认网口
		netif_set_up(&lwip_netif);		//打开netif网口
	}
	
#if LWIP_DHCP			//如果使用DHCP的话
	lwipdev.dhcpstatus=0;	//DHCP标记为0
	//dhcp_start(&lwip_netif);	//开启DHCP服务
	//lwipdev.dhcpstatus = 1;
#endif
	
#if LWIP_SNMP
	snmp_init();
#endif
	return 0;//操作OK.
}   

//当接收到数据后调用 
void lwip_pkt_handle(void)
{
  //从网络缓冲区中读取接收到的数据包并将其发送给LWIP处理 
 ethernetif_input(&lwip_netif);
}

//LWIP轮询任务
void lwip_periodic_handle()
{
#if LWIP_TCP
	//每250ms调用一次tcp_tmr()函数
  if (lwip_localtime - TCPTimer >= TCP_TMR_INTERVAL)
  {
    TCPTimer =  lwip_localtime;
    tcp_tmr();	 
  }
#endif
  //ARP每5s周期性调用一次
  if ((lwip_localtime - ARPTimer) >= ARP_TMR_INTERVAL)
  {
    ARPTimer =  lwip_localtime;
    etharp_tmr();
  }

#if LWIP_DHCP //如果使用DHCP的话
  //每500ms调用一次dhcp_fine_tmr()
  if (lwip_localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  lwip_localtime;
    dhcp_fine_tmr();
    if ((lwipdev.dhcpstatus != 2)&&(lwipdev.dhcpstatus != 0XFF))
    { 
      lwip_dhcp_process_handle();  //DHCP处理
    }
  }

  //每60s执行一次DHCP粗糙处理
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
	  ptp_state.state = (--ptp_state.state)%0xFF;     //限制自减的下溢出
	  e1_state.state  = (--e1_state.state)%0xFF;      //限制自减的下溢出
  }
}


//如果使能了DHCP
#if LWIP_DHCP

//DHCP处理任务
void lwip_dhcp_process_handle(void)
{
	u32 ip=0,netmask=0,gw=0;
	switch(lwipdev.dhcpstatus)
	{
		case 0: 	//开启DHCP
			dhcp_start(&lwip_netif);
			lwipdev.dhcpstatus = 1;		//等待通过DHCP获取到的地址
			break;
		case 1:		//等待获取到IP地址
		{
			ip=lwip_netif.ip_addr.addr;		//读取新IP地址
			netmask=lwip_netif.netmask.addr;//读取子网掩码
			gw=lwip_netif.gw.addr;			//读取默认网关 
			
			if(ip!=0)			//正确获取到IP地址的时候
			{
				lwipdev.dhcpstatus=2;	//DHCP成功
				//解析出通过DHCP获取到的IP地址
				lwipdev.ip[3]=(uint8_t)(ip>>24); 
				lwipdev.ip[2]=(uint8_t)(ip>>16);
				lwipdev.ip[1]=(uint8_t)(ip>>8);
				lwipdev.ip[0]=(uint8_t)(ip);
				//解析通过DHCP获取到的子网掩码地址
				lwipdev.netmask[3]=(uint8_t)(netmask>>24);
				lwipdev.netmask[2]=(uint8_t)(netmask>>16);
				lwipdev.netmask[1]=(uint8_t)(netmask>>8);
				lwipdev.netmask[0]=(uint8_t)(netmask);
				//解析出通过DHCP获取到的默认网关
				lwipdev.gateway[3]=(uint8_t)(gw>>24);
				lwipdev.gateway[2]=(uint8_t)(gw>>16);
				lwipdev.gateway[1]=(uint8_t)(gw>>8);
				lwipdev.gateway[0]=(uint8_t)(gw);
			}else if(lwip_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //通过DHCP服务获取IP地址失败,且超过最大尝试次数
			{
				lwipdev.dhcpstatus=0XFF;//DHCP超时失败.
				//使用静态IP地址
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



























