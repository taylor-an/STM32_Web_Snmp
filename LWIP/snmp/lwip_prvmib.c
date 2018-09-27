/**
 * @file
 * lwip Private MIB 
 *
 * @todo create MIB file for this example
 * @note the lwip enterprise tree root (26381) is owned by the lwIP project.
 * It is NOT allowed to allocate new objects under this ID (26381) without our,
 * the lwip developers, permission!
 *
 * Please apply for your own ID with IANA: http://www.iana.org/numbers.html
 *  
 * lwip        OBJECT IDENTIFIER ::= { enterprises 26381 }
 * example     OBJECT IDENTIFIER ::= { lwip 1 }
 */
 
/*
 * Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Christiaan Simons <christiaan.simons@axon.tv>
 */

#include "private_mib.h"
#include "private_mib.h"
#include "lwip/snmp.h"
#include "lwip/snmp_msg.h"
#include "lwip/snmp_asn1.h"
#include "lwip/snmp_structs.h"
#include "type.h"


#if LWIP_SNMP
void lwip_privmib_init(void);
void objectidncpy(s32_t *dst, s32_t *src, u8_t n);
__weak u8_t lwip_privmib_MspInit(u8_t **par[], u8_t **par_len[]);


/*LocalInfo*/
/** XHTimeLocalIP  */
static u8_t *xhtimelocalIP_len_ptr;
static u8_t *xhtimelocalIP_ptr;
/** XHTimeLocalGetway */
static u8_t *xhtimelocalgateway_len_ptr;
static u8_t* xhtimelocalgateway_ptr;
/** XHTimeLocalNewmask */
static u8_t *xhtimelocalnetmask_len_ptr;
static u8_t* xhtimelocalnetmask_ptr;

/*NTP/PTP Info*/
/** XHTimeNTPIP */
static u8_t *xhtimeNTPIP_len_ptr;
static u8_t* xhtimeNTPIP_ptr;
/** XHTimeNTPGetway */
static u8_t *xhtimeNTPgateway_len_ptr;
static u8_t* xhtimeNTPgateway_ptr;
/** XHTimeLocalNewmask */
static u8_t *xhtimeNTPnetmask_len_ptr;
static u8_t* xhtimeNTPnetmask_ptr;

/*State Info      -----Const------- */
/** xHTimeUTCTime */
static u8_t *xhtimeUTCtime_len_ptr;
static u8_t *xhtimeUTCtime_ptr;
/** xHTimePos */
static u8_t *xhtimepos_len_ptr;
static u8_t* xhtimepos_ptr;
/** xHTimeTameState */
static u8_t *xhtimetamestate_len_ptr;
static u8_t *xhtimetamestate_ptr;
/** xHTimeTimeSource*/
static u8_t *xhtimeTimeSource_len_ptr;
static u8_t *xhtimeTimeSource_ptr;
	
/** xHTimePtpMode */
static u8_t *xhtimePTPmode_len_ptr;
static u8_t* xhtimePTPmode_ptr;
/** xHTimePtpState */
static u8_t *xhtimePTPstate_len_ptr;
static u8_t *xhtimePTPstate_ptr;
/** xHTimePtpTime*/
static u8_t *xhtimePTPTime_len_ptr;
static u8_t *xhtimePTPTime_ptr;
/** xHTimeRemoteIp*/
static u8_t *xhtimeRemoteIP_len_ptr;
static u8_t *xhtimeRemoteIP_ptr;


/** xHTimePtpE1Mode */
static u8_t *xhtimePTPE1mode_len_ptr;
static u8_t* xhtimePTPE1mode_ptr;
/** xHTimePtpE1State */
static u8_t *xhtimePTPE1state_len_ptr;
static u8_t *xhtimePTPE1state_ptr;
/** xHTimePtpE1Time*/
static u8_t *xhtimePTPE1Time_len_ptr;
static u8_t *xhtimePTPE1Time_ptr;
/**
 * 字符串拷贝
 */
static void ocstrncpy(u8_t *dst, u8_t *src, u16_t n)
{
  u16_t i = n;
  while (i > 0) {
    i--;
    *dst++ = *src++;
  }
}

/**
 * 获取对象定义 该对象的类型、读写定义、ASN类型(字符串、整型等)、长度
 */
static void xHTimeEntry_get_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;  //返回上一层 即父节点
  ident -= 1;      //OID也返回上一层 即父节点的OID
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    switch (id)
    {
      case 1:    /* xHTimeLocalIPInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimelocalIP_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 2:    /* xHTimeLocalGWInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimelocalgateway_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 3:    /* xHTimeLocalNMInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimelocalnetmask_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 4:    /* xHTimeNTPIPInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimeNTPIP_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 5:    /* xHTimeNTPGWInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimeNTPgateway_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 6:    /* xHTimeNTPNMInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimeNTPnetmask_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 7:    /* xHTimeUTCTimeInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimeUTCtime_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 8:    /* xHTimePosInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimepos_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 9:    /* xHTimeTameStateInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimePTPstate_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 10:    /* xHTimeTimeSourceInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimeTimeSource_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 11:    /* xHTimePtpModeInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimePTPmode_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 12:    /* xHTimePtpStateInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimePTPstate_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 13:    /* xHTimePtpTimeInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimePTPTime_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 14:    /* xHTimeRemoteIpInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimeRemoteIP_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 15:    /* xHTimePtpE1ModeInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimePTPE1mode_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 16:    /* xHTimePtpE1StateInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimePTPE1state_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      case 17:    /* xHTimePtpE1TimeInfo  */
        od->instance = MIB_OBJECT_TAB;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = *xhtimePTPE1Time_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
        break;
      default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("xHTimeEntry_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    od->instance = MIB_OBJECT_NONE;
  }
}

/**
 * 获取对象的数据
 * value 指向保存数据的缓存
 * len 为上一个函数获取到的长度
 */
static void xHTimeEntry_get_value(struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
  /* the index value can be found in: od->id_inst_ptr[1] */
  id = od->id_inst_ptr[0];
  switch (id)
  {
   case 1:    /* xHTimeLocalIPInfo  */
      ocstrncpy(value, xhtimelocalIP_ptr,len);
      break;
    case 2:    /* xHTimeLocalGWInfo  */
      ocstrncpy(value, xhtimelocalgateway_ptr,len);
      break;
    case 3:    /* xHTimeLocalNMInfo  */
      ocstrncpy(value, xhtimelocalnetmask_ptr,len);
      break;
    case 4:    /* xHTimeNTPIPInfo  */
      ocstrncpy(value, xhtimeNTPIP_ptr,len);
      break;
    case 5:    /* xHTimeNTPGWInfo  */
      ocstrncpy(value, xhtimeNTPgateway_ptr,len);
      break;
    case 6:    /* xHTimeNTPNMInfo  */
      ocstrncpy(value, xhtimeNTPnetmask_ptr,len);
      break;
    case 7:    /* xHTimeUTCTimeInfo  */
      ocstrncpy(value, xhtimeUTCtime_ptr,len);
      break;
    case 8:    /* xHTimePosInfo  */
      ocstrncpy(value, xhtimepos_ptr,len);
      break;
    case 9:    /* xHTimeTameStateInfo  */
      ocstrncpy(value, xhtimetamestate_ptr,len);
      break;
    case 10:    /* xHTimeTimeSourceInfo  */
      ocstrncpy(value, xhtimeTimeSource_ptr,len);
      break;
    case 11:    /* xHTimePtpModeInfo  */
      ocstrncpy(value, xhtimePTPmode_ptr,len);
      break;
    case 12:    /* xHTimePtpStateInfo  */
      ocstrncpy(value, xhtimePTPstate_ptr,len);
      break;
    case 13:    /* xHTimePtpTimeInfo  */
      ocstrncpy(value, xhtimePTPTime_ptr,len);
      break;
    case 14:    /* xHTimeRemoteIpInfo  */
      ocstrncpy(value, xhtimeRemoteIP_ptr,len);
      break;
    case 15:    /* xHTimePtpE1ModeInfo  */
      ocstrncpy(value, xhtimePTPE1mode_ptr,len);
      break;
    case 16:    /* xHTimePtpE1StateInfo  */
      ocstrncpy(value, xhtimePTPE1state_ptr,len);
      break;
    case 17:    /* xHTimePtpE1TimeInfo  */
      ocstrncpy(value, xhtimePTPE1Time_ptr,len);
      break;
  };
}
/**
 *验证参数是否正确  
 *判断要写入的数据长度是否大于该节点缓存的长度 防止溢出 以及避免对只读对象的写入
 *返回1表示可写  返回0表示不可写
 */
static u8_t xHTimeEntry_set_test(struct obj_def *od, u16_t len, void *value)
{
	 u8_t id, set_ok;
	 /* the index value can be found in: od->id_inst_ptr[1] */
	 set_ok = 0;
	 id = od->id_inst_ptr[0];
	//分支中没有只读对象
	  switch (id)
	  {
		    case 1:    /* xHTimeLocalIPInfo  */
				if(len <= 15) set_ok = 1;
			  break;
			case 2:    /* xHTimeLocalGWInfo  */
				if(len <= 15) set_ok = 1;
			  break;
			case 3:    /* xHTimeLocalNMInfo  */
				if(len <= 15) set_ok = 1;
			  break;
			case 4:    /* xHTimeNTPIPInfo  */
				if(len <= 15) set_ok = 1;
			  break;
			case 5:    /* xHTimeNTPGWInfo  */
				if(len <= 15) set_ok = 1;
			  break;
			case 6:    /* xHTimeNTPNMInfo  */
				if(len <= 15) set_ok = 1;
			  break;
			case 10:    /* xHTimeTimeSourceInfo  */
				if(len <= 6) set_ok = 1;
			  break;
			case 11:    /* xHTimePtpModeInfo  */
				if(len <= 6) set_ok = 1;
			  break;
			case 15:    /* xHTimePtpE1ModeInfo  */
				if(len <= 6) set_ok = 1;
			  break;
	  };
	  return set_ok;
}

/**
 * 设置对象的值
 * value指向要更新的数据
 * len 要更新数据的长度
 */
static void xHTimeEntry_set_value(struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
  /* the index value can be found in: od->id_inst_ptr[1] */
  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 1:    /* xHTimeLocalIPInfo  */
      //ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 2:    /* xHTimeLocalGWInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 3:    /* xHTimeLocalNMInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 4:    /* xHTimeNTPIPInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 5:    /* xHTimeNTPGWInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 6:    /* xHTimeNTPNMInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 10:    /* xHTimeTimeSourceInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 11:    /* xHTimePtpModeInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
    case 15:    /* xHTimePtpE1ModeInfo  */
     // ocstrncpy((u8_t*)your_string_pointer,value,len);
      break;
  };
}

/**
*/
const mib_scalar_node xHTimeEntry_root = {
  &xHTimeEntry_get_object_def,
  &xHTimeEntry_get_value,
  &xHTimeEntry_set_test,
  &xHTimeEntry_set_value,
  MIB_NODE_SC,
  0,
};

/* xHTimeEntry  .1.3.6.1.4.1.22566.1.1    */
const s32_t xHTimeEntry_ids[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
struct mib_node* const xHTimeEntry_nodes[20] = { 
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root,
  (struct mib_node* const)&xHTimeEntry_root
};

const struct mib_array_node xHTimeEntry = {
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  17,
  xHTimeEntry_ids,
  xHTimeEntry_nodes,
};

/* xHTimeTable  .1.3.6.1.4.1.22566.1    */
s32_t xHTimeTable_ids[1] = { 1 };
struct mib_node* xHTimeTable_nodes[1] = { 
  (struct mib_node* const)&xHTimeEntry
};

struct mib_array_node xHTimeTable = {
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1,
  xHTimeTable_ids,
  xHTimeTable_nodes
};

/* xHTime  .1.3.6.1.4.1.22566    */
const s32_t xHTime_ids[1] = {1};
struct mib_node* const xHTime_nodes[1] = { 
  (struct mib_node* const)&xHTimeTable
};

const struct mib_array_node xHTime = {
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1,
  xHTime_ids,
  xHTime_nodes
};

/* enterprises  .1.3.6.1.4.1    */
const s32_t enterprises_ids[1] = { 22566 };
struct mib_node* const enterprises_nodes[1] = { 
  (struct mib_node* const)&xHTime
};

const struct mib_array_node enterprises = {
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1,
  enterprises_ids,
  enterprises_nodes
};

/* private  .1.3.6.1.4    */
const s32_t private_ids[1] = { 1 };
struct mib_node* const private_nodes[1] = { 
  (struct mib_node* const)&enterprises
};

const struct mib_array_node mib_private = {
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1,
  private_ids,
  private_nodes
};

u8_t **xhTimeParaTable[] =   {&xhtimelocalIP_ptr,   &xhtimelocalgateway_ptr, &xhtimelocalnetmask_ptr, 
							  &xhtimeNTPIP_ptr,     &xhtimeNTPgateway_ptr,   &xhtimeNTPnetmask_ptr,
						      &xhtimeUTCtime_ptr,   &xhtimepos_ptr,          &xhtimetamestate_ptr,    &xhtimeTimeSource_ptr,  
							  &xhtimePTPmode_ptr,   &xhtimePTPstate_ptr,     &xhtimePTPTime_ptr,      &xhtimeRemoteIP_ptr,
						      &xhtimePTPE1mode_ptr, &xhtimePTPE1state_ptr,   &xhtimePTPE1Time_ptr
}; 
u8_t **hxTimeParaLenTable[] = {&xhtimelocalIP_len_ptr,   &xhtimelocalgateway_len_ptr , &xhtimelocalnetmask_len_ptr,
							   &xhtimeNTPIP_len_ptr,     &xhtimeNTPgateway_len_ptr,    &xhtimeNTPnetmask_len_ptr,
							   &xhtimeUTCtime_len_ptr,   &xhtimepos_len_ptr,           &xhtimetamestate_len_ptr,     &xhtimeTimeSource_len_ptr,
							   &xhtimePTPmode_len_ptr,   &xhtimePTPstate_len_ptr,      &xhtimePTPTime_len_ptr,       &xhtimeRemoteIP_len_ptr,
					           &xhtimePTPE1mode_len_ptr, &xhtimePTPE1state_len_ptr,	   &xhtimePTPE1Time_len_ptr		
};
void lwip_privmib_init(void)
{
	lwip_privmib_MspInit(xhTimeParaTable, hxTimeParaLenTable);
}

__weak u8_t lwip_privmib_MspInit(u8_t **par[], u8_t **par_len[])
{	
	*par[0] = lwipdev.ipbuf;
	*par[1] = lwipdev.gwbuf;
	*par[2] = lwipdev.nmbuf;
	*par[3] = lwipdev.mcbuf;
	
	*par[4] = ptp_state.ipbuf;
	*par[5] = ptp_state.gwbuf;
	*par[6] = ptp_state.nmbuf;
	*par[7] = (uint8 *)"-------";
	
//	*par[8] = (uint8 *)ptp_state.datebuf;
//	
//	*par[9] = (u8_t *)"45'46, 12'13";
//	*par[10] = (u8_t *) "success";
//	
//	*par[11] = (u8_t *)"mode_1";
//	*par[12] = (u8_t *)"RUN";
//	*par[13] = (u8_t *)"mode_3";
//	*par[14] = (u8_t *)"stop";
//	
//	*par[15] = (u8_t *)"beidou";
//	*par[16] = (u8_t *)"1PTPS";
//	*par[17] = (u8_t *)"E1";
//	*par[18] = (u8_t *)"NTP";
//	*par[19] = (u8_t *)"PTP";
//	
//	*par_len[0] = 15;
//	*par_len[1] = 15;
//	*par_len[2] = 15;
//	*par_len[3] = 24;
//	
//	*par_len[4] = 15;
//	*par_len[5] = 15;
//	*par_len[6] = 15;
//	*par_len[7] = 24;
//	
//	*par_len[8] = 24;
//	*par_len[9] = 12;
//	*par_len[10] = 7;
//	*par_len[11] = 6;
//	*par_len[12] = 3;
//	*par_len[13] = 6;
//	*par_len[14] = 4;
//	*par_len[15] = 6;
//	*par_len[16] = 5;
//	*par_len[17] = 2;
//	*par_len[18] = 3;
//	*par_len[19] = 3;
	
	return 0;
}

#endif /* LWIP_SNMP */
