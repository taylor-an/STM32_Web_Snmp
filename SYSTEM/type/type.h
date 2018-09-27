#ifndef _TYPE_H
#define _TYPE_H
#include "stm32f10x.h"
#include "lwip/netif.h"


#define USART_RECEIVE_MAX  555
#define MonitorlConstant 16
#define UDP_DEMO_RX_BUFSIZE		2048

#define FLASH_SAVE_ADDR  0X08035000
#define FLASH_SAVE_IPaddr (FLASH_SAVE_ADDR + 0x10)
#define FLASH_SAVE_Gatway (FLASH_SAVE_IPaddr + 0x100)
#define FLASH_SAVE_Mask   (FLASH_SAVE_Gatway + 0x100)


typedef unsigned char         u8;
typedef unsigned short int    u16;
typedef unsigned int          u32;
typedef signed   char         s8;
typedef signed   short int    s16;
typedef signed   int          s32;


typedef signed char boolen;
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

typedef  unsigned char      Usart_boolean;     /* Boolean value type. */ 
typedef  unsigned long int  Usart_uint32;      /* Unsigned 32 bit value */ 
typedef  unsigned short     Usart_uint16;      /* Unsigned 16 bit value */ 
typedef  unsigned char      Usart_uint8;       /* Unsigned 8  bit value */ 
typedef  signed int    Usart_int32;       /* Signed 32 bit value */ 
typedef  signed short       Usart_int16;       /* Signed 16 bit value */ 
typedef  signed char        Usart_int8;        /* Signed 8  bit value */ 


typedef struct{
	u32      	  ip;
	int     	 sec;
	int    		nsec;
	u32   updatetime;
}PTP_NTP_Info_t;

typedef unsigned long long tstamp;

typedef struct _DateTime	/*此结构体定义了NTP时间同步的相关变量*/
{
  uint16_t year;					/*年为2字节 分为高低两位 */
  uint8_t month;						/*	月 */
  uint8_t day;							/*	天 */
  uint8_t hour;							/*	时 */
  uint8_t minute;						/*	分 */
  uint8_t second;						/*	秒 */
} DateTime;

typedef struct ntp_state	/*此结构体定义了NTP时间同步的相关状态*/
{
  uint8_t leap_state;					//闰秒状态
  uint8_t sync_state;					//同步状态
  uint8_t com_leap_state;     //串口输入状态判断
} state;

typedef struct{
	uint8_t          mode;
	uint8_t         state;
	
	uint8_t          ip[4];
	uint8_t      ipbuf[16];   //SNMP 使用
	
	uint8_t     netmask[4];
	uint8_t      nmbuf[16];   //SNMP 使用
	
	uint8_t     gateway[4];
	uint8_t      gwbuf[16];   //SNMP 使用
	
	uint8_t    remoteip[4];
	uint8_t      rpbuf[16];   //SNMP 使用
	
	u32_t             ip_t;
	u32_t             nm_t;
	u32_t			  gw_t;
	u32_t			  rp_t;
	u32_t             tm_t;
	
	DateTime          date;
	char        datebuf[24];   //SNMP 使用
}State_t;

typedef struct  
{
	u8 mac[6];      //MAC地址
	u8 mcbuf[24];    //SNMP 使用
	
	u8 remoteip[4];	//远端主机IP地址 
	
	u8 ip[4];       //本机IP地址
	
	u8 ipbuf[16];   //SNMP 使用
	u8 nmbuf[16];   //SNMP 使用
	u8 gwbuf[16];   //SNMP 使用
	
	u8 netmask[4]; 	//子网掩码
	u8 gateway[4]; 	//默认网关的IP地址
	
	vu8 dhcpstatus;	//dhcp状态 
					//0,未获取DHCP地址;
					//1,进入DHCP获取状态
					//2,成功获取DHCP地址
					//0XFF,获取失败.
	u16 port;
}__lwip_dev;

typedef struct
{
	DateTime      date;
	u8       tameState;
	int              x;
	int              y;
	char            ns;
	char            ew;
	uint8_t       x_dx;
	uint8_t       y_dx;
}SystemState_t;



//<<<<<<<<<<<<<<<<枚举定义<<<<<<<<<<<<
//串口编号
typedef enum
{
	EUsart_1,
	EUsart_2,
	EUsart_3,
	EUsart_4,
	EUsart_6,
	EUsart_5,
}EUsart;

//奇偶校验位
typedef enum
{
	EUSART_Parity_No,
	EUSART_Parity_Even,
	EUSART_Parity_Odd
}EUSART_Parity;

//中断开关状态
typedef enum
{
	EUSART_Interrupt_State_On,
	EUSART_Interrupt_State_Off,
}EUsart_Interrupt_State;

//功能函数
typedef int (*USART_Func)(void *, Usart_uint16, void *, Usart_uint8);

typedef struct _st_usart
{
	EUsart name;
	Usart_uint8 state;
	Usart_uint32 bound;
	EUSART_Parity partity;
	Usart_uint16 receive_count;
	Usart_uint8 buff[USART_RECEIVE_MAX];
	USART_Func func;
}STUsart, *PSTUsart;


extern  __IO uint32_t sys_time;
extern __IO uint32 old_sec;
extern __IO state sntp_state;

extern __lwip_dev lwipdev;	//lwip控制结构体


extern u8 TimeSource[];
extern u8 tameState;
extern PTP_NTP_Info_t PtpMonitorl[];
extern PTP_NTP_Info_t SntpMonitorl[];
extern PTP_NTP_Info_t E1Monitor1[];
extern State_t ptp_state;
extern State_t e1_state;
extern STUsart usart2, usart4, usart1;

extern struct netif lwip_netif;
extern __lwip_dev lwipdev;

extern STUsart usart1;
extern STUsart usart2;
extern STUsart usart3;
extern STUsart usart4;

extern PTP_NTP_Info_t SntpMonitorl[];

extern u8 udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];
extern SystemState_t sysstate;

#endif



