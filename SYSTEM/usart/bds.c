#include "bds.h" 
#include "usart.h" 								   
#include "stdio.h"	 
#include "stdarg.h"	 
#include "string.h"	 
#include "math.h"
 
char *my_strrstr(char *dest,char *src)  
{  
    char *first=NULL;  
    char *second=NULL;  
    char *str1=dest;  
    char *str2=src;  
//    assert(dest);  
//    assert(src);  
    while((*str1 != '\0'))  
    {  
        first=str1;    //记录下一次比较的起始位置  
        while((*str1 != '\0') && (*str2 != '\0') && (*str1 == *str2))  
        {  
            str1++;  
            str2++;  
        }  
        if(*str2 == '\0')  
            second=first;  //从第一次查找到子串的位置后继续查找  
        str1=first+1;  
        str2=src;  
    }  
    if(*str1 == '\0')  
        return second;  
    return 0;  
} 

char *my_strstr(const char *s1, const char *s2,const int s1len,const int s2len)
{
    int l1, l2;

    l2 = s2len;
    if (!l2)
        return (char *)s1;
    l1 = s1len;
    while (l1 >= l2)
    {
        l1 --;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1 ++;
    }

    return NULL;
}

//从buf里面得到第cx个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
//       0XFF,代表不存在第cx个逗号							  
uint8_t NMEA_Comma_Pos(uint8_t *buf,uint8_t cx)
{	 		    
	uint8_t *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}
//m^n函数
//返回值:m^n次方.
uint32_t NMEA_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}
//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
int NMEA_Str2num(uint8_t *buf,uint8_t*dx)
{
	uint8_t *p=buf;
	uint32_t ires=0,fres=0;
	uint8_t ilen=0,flen=0,i;
	uint8_t mask=0;
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


void NMEA_GPGSV_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p,*p1,dx;
	uint8_t len,i,j,slx=0;
	uint8_t posx;   	 
	p=buf;
	p1=(uint8_t*)strstr((const char *)p,"GPGSV");
	len=p1[7]-'0';								//得到GPGSV的条数
	posx=NMEA_Comma_Pos(p1,3); 					//得到可见卫星总数
	
	if(posx!=0XFF)gpsx->gpsvnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(uint8_t*)strstr((const char *)p,"GPGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);	//得到卫星编号
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//得到卫星仰角 
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//得到卫星方位角
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);	//得到卫星信噪比
			else break;
			slx++;	   
		}   
 		p=p1+1;//切换到下一个GPGSV信息
	}   
}

void NMEA_BDGSV_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p,*p1,dx;
	uint8_t len,i,j,slx=0;
	uint8_t posx;   	 
	p=buf;
	p1=(uint8_t*)strstr((const char *)p,"BDGSV");
	len=p1[7]-'0';								//得到GPGSV的条数
	posx=NMEA_Comma_Pos(p1,3); 					//得到可见卫星总数
	if(posx!=0XFF)gpsx->bdsvnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(uint8_t*)strstr((const char *)p,"BDGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->BDslmsg[slx].num=NMEA_Str2num(p1+posx,&dx);	//得到卫星编号
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->BDslmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//得到卫星仰角 
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->BDslmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//得到卫星方位角
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->BDslmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);	//得到卫星信噪比
			else break;
			slx++;	   
		}   
 		p=p1+1;
	}   
}

//分析GPGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_BDGGA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx;    
	p1=(uint8_t*)strstr((const char *)buf,"BDGGA");
	posx=NMEA_Comma_Pos(p1,6);								//得到GPS状态
	if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);								//得到用于定位的卫星数
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);								//得到海拔高度
	if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}


//分析GPGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx;    
	p1=(uint8_t*)strstr((const char *)buf,"GPGGA");
	posx=NMEA_Comma_Pos(p1,6);								//得到GPS状态
	if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);								//得到用于定位的卫星数
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);								//得到海拔高度
	if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}
//分析GPGSA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx; 
	uint8_t i;   
	p1=(uint8_t*)strstr((const char *)buf,"GNGSA");
	posx=NMEA_Comma_Pos(p1,2);								//得到定位类型
	if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);	
	for(i=0;i<12;i++)										//得到定位卫星编号
	{
		posx=NMEA_Comma_Pos(p1,3+i);					 
		if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
		else break; 
	}				  
	posx=NMEA_Comma_Pos(p1,15);								//得到PDOP位置精度因子
	if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,16);								//得到HDOP位置精度因子
	if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,17);								//得到VDOP位置精度因子
	if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);  
}
//分析GPRMC信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,uint8_t *buf)//双星解调
{
	static uint8_t *p1,dx;			 
	static uint8_t posx;     
	uint32_t temp;	   
	p1=(uint8_t*)strstr((const char *)buf,"GNRMC");//"$GPRMC",经常有&和GPRMC分开的情况,故只判断GPRMC.
		if( p1 == NULL)
	{
	  return;
	}
  posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.minute=(temp/100)%100;
		gpsx->utc.second=temp%100;	 	 
	}
	posx=NMEA_Comma_Pos(p1,2);							
	if(posx!=0XFF)gpsx->posstate=*(p1+posx);		
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->latitude=temp;	//得到°
	}
	posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp;	
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->utc.day=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
		posx=NMEA_Comma_Pos(p1,12);								
	if(posx!=0XFF)gpsx->posmode=*(p1+posx);		
}

void NMEA_BDRMC_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	static uint8_t *p1,dx;			 
	static uint8_t posx;     
	uint32_t temp;	   
	p1=(uint8_t*)strstr((const char *)buf,"BDRMC");//"$GPRMC",经常有&和GPRMC分开的情况,故只判断GPRMC.
	if( p1 == NULL)
	{
	  return;
	}
  posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.minute=(temp/100)%100;
		gpsx->utc.second=temp%100;	 	 
	}
	posx=NMEA_Comma_Pos(p1,2);							
	if(posx!=0XFF)gpsx->posstate=*(p1+posx);		
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->latitude=temp;	//得到°
	}
	posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp;	
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->utc.day=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
		posx=NMEA_Comma_Pos(p1,12);								
	if(posx!=0XFF)gpsx->posmode=*(p1+posx);		
}

void NMEA_GPRMC_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	static uint8_t *p1,dx;			 
	static uint8_t posx;     
	uint32_t temp;	   
	p1=(uint8_t*)strstr((const char *)buf,"GPRMC");//"$GPRMC",经常有&和GPRMC分开的情况,故只判断GPRMC.
		if( p1 == NULL)
	{
	  return;
	}
  posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.minute=(temp/100)%100;
		gpsx->utc.second=temp%100;	 	 
	}
	posx=NMEA_Comma_Pos(p1,2);							
	if(posx!=0XFF)gpsx->posstate=*(p1+posx);		
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->latitude=temp;	//得到°
	}
	posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp;	
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->utc.day=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
		posx=NMEA_Comma_Pos(p1,12);								
	if(posx!=0XFF)gpsx->posmode=*(p1+posx);		
}
//分析GPVTG信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx;    
	p1=(uint8_t*)strstr((const char *)buf,"GNVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								//得到地面速率
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//确保扩大1000倍
	}
}  

void NMEA_BDZDA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx; 
	uint32_t temp;		
	p1=(uint8_t*)strstr((const char *)buf,"BDZDA");							 
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
			gpsx->utc.hour=temp/10000;
			gpsx->utc.minute=(temp/100)%100;
			gpsx->utc.second=temp%100;
		}			
		posx=NMEA_Comma_Pos(p1,2);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->utc.day=temp;
		} 
		posx=NMEA_Comma_Pos(p1,3);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->utc.month=temp;
		} 
		posx=NMEA_Comma_Pos(p1,4);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->utc.year=temp;
		}
		posx=NMEA_Comma_Pos(p1,5);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->time.PPsState = temp;
		}
		posx=NMEA_Comma_Pos(p1,6);								
		if(posx!=0XFF)
		{
			gpsx->time.TimeSource = *(p1+posx);
		}
}

void NMEA_GNZDA_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	uint8_t *p1,dx;			 
	uint8_t posx; 
	uint32_t temp;		
	p1=(uint8_t*)strstr((const char *)buf,"GNZDA");							 
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
			gpsx->utc.hour=temp/10000;
			gpsx->utc.minute=(temp/100)%100;
			gpsx->utc.second=temp%100;
		}			
		posx=NMEA_Comma_Pos(p1,2);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->utc.day=temp;
		} 
		posx=NMEA_Comma_Pos(p1,3);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->utc.month=temp;
		} 
		posx=NMEA_Comma_Pos(p1,4);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->utc.year=temp;
		}
		posx=NMEA_Comma_Pos(p1,5);								
		if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);		 				
			gpsx->time.PPsState = temp;
		}
		posx=NMEA_Comma_Pos(p1,6);								
		if(posx!=0XFF)
		{
			gpsx->time.TimeSource = *(p1+posx);
		}
}

void XHTime_PTP_Analysis(tod_msg *msg, uint8_t *buf,uint16_t len)
{
  uint8_t *p1,dx;			 
	uint8_t posx; 
	uint32_t temp;		
	p1=(uint8_t*)my_strstr((char*)buf,"$PTOD",len,4);							 
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.year=temp; 
	} 
	
	posx=NMEA_Comma_Pos(p1,2);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.month=temp; 
	} 
	posx=NMEA_Comma_Pos(p1,3);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.day=temp; 
	} 
	posx=NMEA_Comma_Pos(p1,4);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.hour=temp;	 
	}
	posx=NMEA_Comma_Pos(p1,5);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.minute = temp;	 
	}
	posx=NMEA_Comma_Pos(p1,6);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.second = temp;	 
		msg->utc.UtcSec = TimetoSec( msg->utc.year, msg->utc.month, msg->utc.day, msg->utc.hour,msg->utc.minute,msg->utc.second);
	}
 	posx=NMEA_Comma_Pos(p1,7);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->port_state = temp;	 
	}
}


void XHTime_NTP_Analysis(tod_msg *msg, uint8_t *buf,uint16_t len)
{
	uint8_t *p1,dx;			 
	uint8_t posx; 
	uint32_t temp;		
	p1=(uint8_t*)my_strstr((  char *)buf,"NTOD",len,4);							 
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.year=temp; 
	} 
	
	posx=NMEA_Comma_Pos(p1,2);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.month=temp; 
	} 
	posx=NMEA_Comma_Pos(p1,3);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.day=temp; 
	} 
	posx=NMEA_Comma_Pos(p1,4);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.hour=temp;	 
	}
	posx=NMEA_Comma_Pos(p1,5);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.minute = temp;	 
	}
	posx=NMEA_Comma_Pos(p1,6);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.second = temp;	 
		msg->utc.UtcSec = TimetoSec( msg->utc.year, msg->utc.month, msg->utc.day, msg->utc.hour,msg->utc.minute,msg->utc.second);
	}
 	posx=NMEA_Comma_Pos(p1,7);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->port_state = temp;	 
	}
}


void XHTime_E1_Analysis(tod_msg *msg, uint8_t *buf,uint16_t len)
{
	uint8_t *p1,dx;			 
	uint8_t posx; 
	uint32_t temp;		
	p1=(uint8_t*)my_strstr((char*)buf,"ETOD",len,4);							 
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.year=temp; 
	} 
	
	posx=NMEA_Comma_Pos(p1,2);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.month=temp; 
	} 
	posx=NMEA_Comma_Pos(p1,3);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		msg->utc.day=temp; 
	} 
	posx=NMEA_Comma_Pos(p1,4);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.hour=temp;	 
	}
	posx=NMEA_Comma_Pos(p1,5);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.minute = temp;	 
	}
	posx=NMEA_Comma_Pos(p1,6);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->utc.second = temp;
		msg->utc.UtcSec = TimetoSec( msg->utc.year, msg->utc.month, msg->utc.day, msg->utc.hour,msg->utc.minute,msg->utc.second);

	}
 	posx=NMEA_Comma_Pos(p1,7);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);	 	
		msg->port_state = temp;	 
	}
}

//提取NMEA-0183信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GNSS_Analysis(nmea_msg *gpsx,uint8_t *buf)
{
	NMEA_BDRMC_Analysis(gpsx,buf);
	NMEA_BDZDA_Analysis(gpsx,buf);
  NMEA_BDGGA_Analysis(gpsx,buf);
}

 



void Ublox_CheckSum(uint8_t *buf,uint16_t len,uint8_t* cka,uint8_t*ckb)
{
	uint16_t i;
	*cka=0;*ckb=0;
	for(i=0;i<len;i++)
	{
		*cka=*cka+buf[i];
		*ckb=*ckb+*cka;
	}
}






