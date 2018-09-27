#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "lan8720.h" 
#include "type.h"


#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数

void lwip_pkt_handle(void);
void lwip_periodic_handle(void);
	
void lwip_comm_default_ip_set(__lwip_dev *lwipx);
u8 lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
u8 lwip_comm_init(void);
void lwip_dhcp_process_handle(void);

#endif













