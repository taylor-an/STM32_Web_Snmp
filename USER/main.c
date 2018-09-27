#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "timer.h"
#include "led.h"
#include "malloc.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "httpd.h"
#include "string.h"
#include "cmd_handle.h"
#include "ptp.h"
#include "udp_echoserver.h"
#include "type.h"
#include "iwdg.h"


void mySnmp_init(void);
void usart_init(void);
int usart2_call_back(void * str, Usart_uint16 len, void * store, Usart_uint8 count);
int usart4_call_back(void * str, Usart_uint16 len, void * store, Usart_uint8 count);

int main(void)
{
	mymem_init(SRAMIN);		//初始化内部内存池	
	lwip_comm_init();	    //lwip初始化
	delay_init();       	//延时初始化	
	LED_Init();		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组配置
	TIM4_Int_Init(1000-1,720-1); 	//100khz的频率,计数1000为10ms
	mySnmp_init();
	usart_init();
	httpd_init();  			//Web Server模式
	udp_echoserver_init();
	timer_init();
	IWDG_Init(IWDG_Prescaler_64, 625);
	
	com_send_str(&usart4, "AT+IPSHOW=", sizeof("AT+IPSHOW"));
	delay_ms(50);
	receive_str_manage(&usart4,NULL,0);
	delay_ms(50);
	receive_str_manage(&usart4,NULL,0);
	com_send_str(&usart4, "AT+REMOTEIP=", sizeof("AT+REMOTEIP="));
	while(1)
	{
		lwip_periodic_handle();
		if((ETH_ReadPHYRegister(LAN8720_PHY_ADDRESS, PHY_BSR) & PHY_Linked_Status) == 0)
		{
			while(1);
		}
	}
}
int usart1_call_back(void * str, Usart_uint16 len, void * store, Usart_uint8 count)
{
	com_send_str(&usart4, str, len);
	com_send_str(&usart2, str, len);
	cmd_handle_usart1(str, len);
	return 0;
}

int usart2_call_back(void * str, Usart_uint16 len, void * store, Usart_uint8 count)
{	
	com_send_str(&usart4, str, len);
	com_send_str(&usart1, str, len);
	cmd_handle_usart2(str, len);
	return 1;
}

int usart4_call_back(void * str, Usart_uint16 len, void * store, Usart_uint8 count)
{
	int ret = 0;
	com_send_str(&usart1, str, len);
	com_send_str(&usart2, str, len);
	ret = cmd_handle_usart4(str,len);
	return ret;
}

void usart_init(void)
{
	
	usart1.bound  = 115200;
	usart1.partity = EUSART_Parity_No;
	usart1.name   = EUsart_1;
	usart1.receive_count = 0;
	usart1.state  = 0;
	memset(usart1.buff, 0, USART_RECEIVE_MAX);
	usart1.func = usart1_call_back;
	com_init(&usart1);
	
	usart2.bound   = 115200;
	usart2.partity = EUSART_Parity_No;
	usart2.name    = EUsart_2;
	usart2.receive_count = 0;
	usart2.state   = 0;
	memset(usart2.buff, 0, USART_RECEIVE_MAX);
	usart2.func = usart2_call_back;
	com_init(&usart2);
	
	usart4.bound  = 115200;
	usart4.partity = EUSART_Parity_No;
	usart4.name   = EUsart_4;
	usart4.receive_count = 0;
	usart4.state  = 0;
	memset(usart4.buff, 0, USART_RECEIVE_MAX);
	usart4.func = usart4_call_back;
	com_init(&usart4);
}


struct ip_addr trap_addr;
u8_t syscontact_str[255];
u8_t syscontact_len = 0;
u8_t syslocation_str[255];
u8_t syslocation_len = 0;
u8_t snmpauthentraps_set = 1;

#include "snmp_msg.h"

void mySnmp_init(void)
{
	IP4_ADDR(&trap_addr, 192, 168, 31, 108);
	lwipdev.remoteip[0] = 192;
	lwipdev.remoteip[1] = 168;
	lwipdev.remoteip[2] = 31;
	lwipdev.remoteip[3] = 108;
	snmp_trap_dst_ip_set(0,&trap_addr);
    snmp_trap_dst_enable(0,1);
	
	sprintf((char *)syscontact_str, "abcdefg");
	syscontact_len = strlen((char *)syscontact_str);
	sprintf((char *)syslocation_str, "192.168.31.45");
	syslocation_len = strlen((char *)syslocation_str);
	
    snmp_set_syscontact(syscontact_str,&syscontact_len);
    snmp_set_syslocation(syslocation_str,&syslocation_len);
    snmp_set_snmpenableauthentraps(&snmpauthentraps_set);
}

