#include "string.h"
#include "usart.h"
#include "timer.h"
#include "cmd_handle.h"
#include "stdio.h"
#include "stdlib.h"
#include "lwip_comm.h"
#include "ptp.h"
#include "type.h"
#include "stmflash.h"


extern uint8 TimeSource[];


uint16 ip_port;

uint8 TOD_Comma_Pos(uint8 *buf,uint8 cx);
uint32 NMEA_Pow(uint8 m,uint8 n);
int TOD_Str2num(uint8 *buf,uint8*dx);
uint8 Check_Sum(uint8 len, uint8 data[]);

uint8 PTP_State_Num = 0;



/**************************************************************************
 *
 * 函数功能: AT串口命令查询
 *
 * 参数说明: 字符串，长度
 *
 * 返 回 值: 命令代码
 *
 **************************************************************************/

uint8 cmd_handle_usart1(char str[], uint16 len)
{
	uint8_t ret = 0;
	uint16 i;
	uint8 *p1 = NULL;
    TimeInternal offset1;
    TimeInternal stime;
    TimeInternal time ;
	
	//判断数据
	char buf[100] = {0};
	buf[99] = ',';
	if(strstr((char *)str, "AT+IPSHOW=") != NULL)//本地IP查看
	{
		sprintf(buf, "AT+SNBK=%d,%d,%d,%d/%d,%d,%d,%d/%d,%d,%d,%d/\r\n", lwipdev.ip[0], 	 lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3], 
																		 lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3],
																		 lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
		com_send_str(&usart1, buf, strlen(buf));
		com_send_str(&usart2, buf, strlen(buf));
	}
	else if(strstr(str, "AT+SNMP=") != NULL)//本地IP设置
	{
		uint8 count = 0;
		uint8 err   = 0;
		for(i = 0; i < len; i++)
		{
			if(str[i] == ',')
			{
				err++;
			}
		}
		//数据处理 IP以及端口处理
		if(err > 8)
		{			
			/*IP段0*/
			lwipdev.ip[0] = atoi(str+count+8) & 0xff;
			
			/*IP段1*/
			while(str[count + 8] != ',')
			{
				count++;
			}				
			count++;	
			lwipdev.ip[1] = atoi(str+count+8) & 0xff;
			
			/*IP段2*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			lwipdev.ip[2] = atoi(str+count+8) & 0xff;	

			/*IP段3*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;	
			lwipdev.ip[3] = atoi(str+count+8) & 0xff;
					
			/*掩码段0*/
			while(str[count + 8] != '/')
			{
				count ++;
			}
			count++;
			lwipdev.netmask[0] = atoi(str+count+8) & 0xff;
			
			/*掩码段1*/
			while(str[count + 8] != ',')//netmask
			{
				count ++;
			}
			count++;
			lwipdev.netmask[1] = atoi(str+count+8) & 0xff;
			
			/*掩码段2*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			lwipdev.netmask[2] = atoi(str+count+8) & 0xff;
						
			/*掩码段3*/		
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			lwipdev.netmask[3] = atoi(str+count+8) & 0xff;
			
			/*网关段0*/	
			while(str[count + 8] != '/')
			{
				count ++;
			}
			count++;
			lwipdev.gateway[0] = atoi(str+count+8) & 0xff;
			
			/*网关段1*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			lwipdev.gateway[1] = atoi(str+count+8) & 0xff;
			
			/*网关段2*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			lwipdev.gateway[2] = atoi(str+count+8) & 0xff;

			/*网关段3*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count ++;
			lwipdev.gateway[3] = atoi(str+count+8) & 0xff;
			
			
			IP4_ADDR(&(lwip_netif.ip_addr),lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);
			IP4_ADDR(&(lwip_netif.netmask),lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);
			IP4_ADDR(&(lwip_netif.gw),lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);
			
			sprintf((char *)lwipdev.ipbuf, "%03d.%03d.%03d.%03d", lwipdev.ip[0],      lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3]);
			sprintf((char *)lwipdev.nmbuf, "%03d.%03d.%03d.%03d", lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
			sprintf((char *)lwipdev.gwbuf, "%03d.%03d.%03d.%03d", lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
			
			STMFLASH_Write(FLASH_SAVE_IPaddr,(u16*)lwipdev.ip,2);
			STMFLASH_Write(FLASH_SAVE_Gatway,(u16*)lwipdev.gateway,2);
			STMFLASH_Write(FLASH_SAVE_Mask,(u16*)lwipdev.netmask,2);
			sprintf(buf, "AT+SNBK=%d,%d,%d,%d/%d,%d,%d,%d/%d,%d,%d,%d/\r\n", lwipdev.ip[0], 	 lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3], 
																			 lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3],
																			 lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
			com_send_str(&usart1, buf, strlen(buf));
			com_send_str(&usart2, buf, strlen(buf));
			
		}
	}
	else if(strstr(str, "AT+UDPT=") != NULL)//远端IP设置
	{
		com_send_str(&usart4, str, len);
	}
	else if(strstr(str, "AT+REMOTEIP=") != NULL)//远端IP查看
	{
		sprintf(buf, "AT+UDPT=%d,%d,%d,%d/\r\n", ptp_state.remoteip[0], ptp_state.remoteip[1], ptp_state.remoteip[2], ptp_state.remoteip[3]);
		com_send_str(&usart1, buf, sizeof(buf));
		com_send_str(&usart2, buf, sizeof(buf));
	}
	else if(strstr(str, "PTP Master") != NULL)//主模式设置
	{
		ptp_state.mode = 1;
		com_send_str(&usart4, "PTP Master", sizeof("PTP Master"));
	}
	else if(strstr(str, "PTP Slave") != NULL)//从模式设置
	{
		ptp_state.mode = 0;
		com_send_str(&usart4, "PTP Slave", sizeof("PTP Slave"));
	}
	else if(strstr(str, "$TIMESOURCE"))//优先级设置
	{
		for(i = 0; i < 7; i++)
		{
			TimeSource[i] = str[i+11];
		}
	}
	else if((p1 = (uint8*)strstr(str, "XHTOD")) != NULL) //TOD 信息
	{
		uint8 posx = 0;
		uint8 dx = 0;
		posx=TOD_Comma_Pos(p1,1);								//得到UTC时间
		if(posx!=0XFF)
		{
			sysstate.date.year = TOD_Str2num(p1+posx,&dx);	 			
		} 
		
		posx=TOD_Comma_Pos(p1,2);								//得到UTC时间
		if(posx!=0XFF)
		{
			sysstate.date.month = TOD_Str2num(p1+posx,&dx);	 	
		} 
		
		posx=TOD_Comma_Pos(p1,3);								//得到UTC时间
		if(posx!=0XFF)
		{
			sysstate.date.day = TOD_Str2num(p1+posx,&dx);	 	
		} 
		
		posx=TOD_Comma_Pos(p1,4);								//得到UTC时间
		if(posx!=0XFF)
		{
			sysstate.date.hour = TOD_Str2num(p1+posx,&dx);	 	
		} 
		
		posx=TOD_Comma_Pos(p1,5);							
		if(posx!=0XFF)
		{
			sysstate.date.minute = TOD_Str2num(p1+posx,&dx);	 	
		}
		
		posx=TOD_Comma_Pos(p1,6);							
		if(posx!=0XFF)
		{
			sysstate.date.second = TOD_Str2num(p1+posx,&dx);	 	
		}
		
		posx = TOD_Comma_Pos(p1, 7);
		if(posx != 0xFF)
		{
			if(*(p1+posx) == 'A')
			{
				posx = TOD_Comma_Pos(p1, 8);
				if(posx != 0xFF)
				{
					sysstate.x = TOD_Str2num(p1 + posx, &sysstate.x_dx);
				}
				
				posx = TOD_Comma_Pos(p1, 9);
				if(posx != 0xFF)
				{
					sysstate.ns = *(p1+posx);
				}
				
				posx = TOD_Comma_Pos(p1, 10);
				if(posx != 0xFF)
				{
					sysstate.y = TOD_Str2num(p1+posx, &sysstate.y_dx);
				}
				
				posx = TOD_Comma_Pos(p1, 11);
				if(posx != 0xFF)
				{
					sysstate.ew = *(p1+posx);
				}
			}
			else 
			{
				sysstate.x = 0;
				sysstate.y = 0;
			}
		}
		
		posx = TOD_Comma_Pos(p1, 12);
		if(posx != 0xFF)
		{
			sysstate.tameState = *(p1+posx);
		}
		
		if(sysstate.date.year <= 16 )
		{
			sntp_state.sync_state = LEAP_NOT_SYNC;
		}
		else
		{
			sntp_state.sync_state = LEAP_NO_WARNING;
		}		
		sys_time = mktime(sysstate.date.year,sysstate.date.month,sysstate.date.day,sysstate.date.hour,sysstate.date.minute,sysstate.date.second);
		old_sec = 0;//时间清0		
		time.seconds = sys_time;
		time.nanoseconds = 0;
		getTime(&stime);
		subTime(&offset1,&stime,&time);
		if( offset1.seconds != 0 || abs(offset1.nanoseconds) > 500e6)
		{
			time.nanoseconds = stime.nanoseconds;
			setTime(&time);	
		}		
	}	
	else
	{
		ret = COM_OTHERS;
	}
	return ret;
}

uint8 cmd_handle_usart2(char str[], uint16 len)
{
	char buf[100];
	if(strstr((char *)str, "AT+SNMPIP=") != NULL)//本地IP查看
	{
		sprintf(buf, "AT+SNBK=%d,%d,%d,%d/%d,%d,%d,%d/%d,%d,%d,%d/\r\n", lwipdev.ip[0], 	 lwipdev.ip[1],      lwipdev.ip[2],      lwipdev.ip[3], 
																		 lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3],
																		 lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
		com_send_str(&usart2, buf, sizeof(buf));
	}
	return 0;
}

uint8 cmd_handle_usart4(char str[], uint16 len)
{	
	uint8_t ret = 0;
	uint16 i;
	
	if(strstr(str, "AT+PTBK=") != NULL) //PTP IP回应
	{
		uint8 flag = 0;
		uint8 err  = 0;
		uint8 count = 0;
		for(i = 0; i < len; i++)
		{
			if((str[i] == '\"') || (str[i] == ','))
			{
				err++;
			}
			
			if(err != 8)
			{
				flag = 1;
			}
			else
			{
				flag = 0;
			}
		}
		//数据处理 IP以及端口处理
		if(flag == 1)
		{			
			/*IP段0*/
			ptp_state.ip[0] = ((atoi(str+count+8)&0xff));
			
			/*IP段1*/
			while(str[count + 8] != ',')
			{
				count++;
			}				
			count++;	
			ptp_state.ip[1] = ((atoi(str+count+8)&0xff));
			
			/*IP段2*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			ptp_state.ip[2] = ((atoi(str+count+8)&0xff));		

			/*IP段3*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;	
			ptp_state.ip[3] = ((atoi(str+count+8)&0xff));
			
			
			/*掩码段0*/
			while(str[count + 8] != '/')
			{
				count ++;
			}
			count++;
			ptp_state.netmask[0] = atoi(str+count+8) & 0xff;
			
			/*掩码段1*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			ptp_state.netmask[1] = atoi(str+count+8) & 0xff;
			
			/*掩码段2*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			ptp_state.netmask[2] = atoi(str+count+8) & 0xff;
						
			/*掩码段3*/		
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			ptp_state.netmask[3] = atoi(str+count+8) & 0xff;
			
			/*网关段0*/	
			while(str[count + 8] != '/')
			{
				count ++;
			}
			count++;
			ptp_state.gateway[0] = atoi(str+count+8) & 0xff;
			
			/*网关段1*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			ptp_state.gateway[1] = atoi(str+count+8) & 0xff;
			
			/*网关段2*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			 ptp_state.gateway[2] = atoi(str+count+8);

			/*网关段3*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count ++;
			ptp_state.gateway[3] = atoi(str+count+8);
			
			sprintf((char *)ptp_state.ipbuf, "%03d.%03d.%03d.%03d", ptp_state.ip[0],      ptp_state.ip[1],      ptp_state.ip[2],      ptp_state.ip[3]);
			sprintf((char *)ptp_state.nmbuf, "%03d.%03d.%03d.%03d", ptp_state.netmask[0], ptp_state.netmask[1], ptp_state.netmask[2], ptp_state.netmask[3]);
			sprintf((char *)ptp_state.gwbuf, "%03d.%03d.%03d.%03d", ptp_state.gateway[0], ptp_state.gateway[1], ptp_state.gateway[2], ptp_state.gateway[3]);			
			
			ret = COM_SUCCESS;
			PTP_State_Num = PTP_PTBK_SUCCESS;
		}
		else
		{
			ret = COM_ERROR;
			PTP_State_Num = PTP_PTBK_ERROR;
		}
	}
	else if(strstr(str, "$PTP") != NULL) //PTP监控数据
	{ 		
		for(i = 0; i < 16; i++)
		{
			PtpMonitorl[i].ip   = *(((int *)(str+4)) + i*4);
			PtpMonitorl[i].sec  = *(((int *)(str+4)) + i*4 + 1);
			PtpMonitorl[i].nsec = *(((int *)(str+4)) + i*4 + 2);
			PtpMonitorl[i].updatetime = *(((int *)(str+4)) + i*4 + 3);
		}
		ret = COM_SUCCESS;
		PTP_State_Num = PTP_PTP_SUCCESS;
		com_send_str(&usart1, str, len);
		
	}
	else if(strstr(str, "$PTOD") != NULL) //PTP TOD信息
	{
		uint8 posx = 0;
		uint8 dx = 0;
		
		posx=TOD_Comma_Pos((uint8 *)str,1);								//得到UTC时间
		if(posx!=0XFF)
		{		
			ptp_state.date.year = TOD_Str2num((uint8 *)str+posx,&dx);; 
		} 
		
		posx=TOD_Comma_Pos((uint8 *)str,2);								//得到UTC时间
		if(posx!=0XFF)
		{			 	
			ptp_state.date.month = TOD_Str2num((uint8 *)str+posx,&dx);
		} 
		
		posx=TOD_Comma_Pos((uint8 *)str,3);								//得到UTC时间
		if(posx!=0XFF)
		{
			ptp_state.date.day = TOD_Str2num((uint8 *)str+posx,&dx);	 	
		} 
		
		posx=TOD_Comma_Pos((uint8 *)str,4);								//得到UTC时间
		if(posx!=0XFF)
		{
			ptp_state.date.hour = TOD_Str2num((uint8 *)str+posx,&dx);	 	 
		} 
		
		posx=TOD_Comma_Pos((uint8 *)str,5);							
		if(posx!=0XFF)
		{
			ptp_state.date.minute = TOD_Str2num((uint8 *)str+posx,&dx);	 	
		}
		
		posx=TOD_Comma_Pos((uint8 *)str,6);							
		if(posx!=0XFF)
		{
			ptp_state.date.second = TOD_Str2num((uint8 *)str+posx,&dx);	 	
		}
		
		posx=TOD_Comma_Pos((uint8 *)str, 7);
		if(posx!=0xFF)
		{
			ptp_state.mode = TOD_Str2num((uint8 *)str+posx, &dx);
		}
	
		sprintf(ptp_state.datebuf, "%04d-%02d-%02d-%02d-%02d-%02d",
													  ptp_state.date.year, ptp_state.date.month, ptp_state.date.day, 
													  ptp_state.date.hour, ptp_state.date.minute, ptp_state.date.second);
		ptp_state.state = 5;
		ret = COM_SUCCESS;
		PTP_State_Num = PTP_TIME_SUCCESS;
	}
	else if(strstr(str, "AT+RMBK=") != NULL)//远端IP回应
	{
		uint8 flag = 0;
		uint8 err  = 0;
		uint16 ipinfo = 0;
		uint8 count = 0;
		for(i = 0; i < len; i++)
		{
			if((str[i] == '\"') || (str[i] == ','))
			{
				err++;
			}
			
			if(err != 8)
			{
				flag = 1;
			}
			else
			{
				flag = 0;
			}
		}
		//数据处理 IP以及端口处理
		if(flag == 1)
		{			
			/*IP段0*/
			ipinfo = atoi(str+count+8);
			if(ipinfo>255)return ERROR_IP;
			else ptp_state.remoteip[0] = ipinfo;
			
			/*IP段1*/
			while(str[count + 8] != ',')
			{
				count++;
			}				
			count++;	
			ipinfo = atoi(str+count+8);
			if(ipinfo>255)return ERROR_IP;
			else ptp_state.remoteip[1] = ipinfo;
			
			/*IP段2*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;
			ipinfo = atoi(str+count+8);
			if(ipinfo>255)return ERROR_IP;
			else ptp_state.remoteip[2] = ipinfo;			

			/*IP段3*/
			while(str[count + 8] != ',')
			{
				count ++;
			}
			count++;	
			ipinfo = atoi(str+count+8);
			if(ipinfo>255)return ERROR_IP;
			else ptp_state.remoteip[3] = ipinfo;	
			
			ret = COM_SUCCESS;
		}
		else
		{
			ret = COM_ERROR;
		}
	}
	else //其他命令
	{
		ret = COM_OTHERS;
	}
	return ret;
}

uint8 cmd_handle_usart3(void *str, uint16 len)
{
	return 0;
}

uint8 Check_Sum(uint8 len, uint8 data[])
{
	uint8 res = 0;
	uint8 i   = 0;
	
	for(i = 0; i < len; i++)
	{
		res ^= data[i];
	}
	return res;
}

uint8 TOD_Comma_Pos(uint8 *buf,uint8 cx)
{	 		    
	uint8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}

uint32 NMEA_Pow(uint8 m,uint8 n)
{
	uint32 result=1;	 
	while(n--)result*=m;    
	return result;
}


int TOD_Str2num(uint8 *buf,uint8*dx)
{
	uint8 *p=buf;
	uint32 ires=0,fres=0;
	uint8 ilen=0,flen=0,i;
	uint8 mask=0;
	int res;
	while(1) //得到整数和小数的长度
	{
		if(*p=='-'){mask|=0X02;p++;}//是负数
		if(*p==','||(*p=='*'))break;//遇到结束了
		if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
		else if(*p>'9'||(*p<'0'))	//有非法字符
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//去掉负号
	for(i=0;i<ilen;i++)	//得到整数部分数据
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//最多取5位小数
	*dx=flen;	 		//小数点位数
	for(i=0;i<flen;i++)	//得到小数部分数据
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	 
