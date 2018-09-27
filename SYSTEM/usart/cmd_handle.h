#ifndef _CMD_HANDLE_
#define _CMD_HANDLE_
#include "usart.h"
#include "sys.h"
#include "timer.h"

#define CONNECT   0
#define UNCONNECT 1
#define SYSTEM    2
#define SEND      3
#define UDP_CONNET 4
#define IPSHOW 5
#define REMOTE_IPSHOW 6
#define ERROR_IP 7
#define LOCAL_CONNET 8
#define REMOTE_CONNET 9

#define COM_OTHERS     12
#define COM_ERROR      13
#define COM_SUCCESS    14

/*PTP*/
#define PTP_PTBK_ERROR 16
#define PTP_PTBK_SUCCESS 17
#define PTP_PTP_ERROR  18
#define PTP_PTP_SUCCESS 19
#define PTP_TIME_ERROR 20
#define PTP_TIME_SUCCESS 21


uint8 cmd_handle_usart1(char str[], uint16 len);
uint8 cmd_handle_usart2(char str[], uint16 len);
uint8 cmd_handle_usart4(char str[], uint16 len);
uint8 cmd_handle_usart5(void *str, uint16 len);
uint8 cmd_handle_usart3(void *str, uint16 len);

#endif

