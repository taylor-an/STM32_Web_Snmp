#include "type.h"

unsigned char udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP�������ݻ�����


PTP_NTP_Info_t PtpMonitorl[MonitorlConstant];
PTP_NTP_Info_t SntpMonitorl[MonitorlConstant];
PTP_NTP_Info_t E1Monitor1[MonitorlConstant];



State_t        ptp_state;
State_t         e1_state;

__IO uint32_t sys_time = 0;
__IO uint32 old_sec = 0;
__IO state sntp_state;

STUsart usart2, usart4, usart1;

__lwip_dev lwipdev;						//lwip���ƽṹ�� 
struct netif lwip_netif;				//����һ��ȫ�ֵ�����ӿ�

SystemState_t sysstate;

