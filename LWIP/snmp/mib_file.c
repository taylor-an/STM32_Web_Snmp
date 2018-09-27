//#include "private_mib.h"
//#include "lwip/snmp.h"
//#include "lwip/snmp_msg.h"
//#include "lwip/snmp_asn1.h"
//#include "lwip/snmp_structs.h"

///**
// * Initialises this private MIB before use.
// *
// */
//void lwip_privmib_init(void);

//void ocstrncpy(u8_t *dst, u8_t *src, u8_t n);
//void objectidncpy(s32_t *dst, s32_t *src, u8_t n);

///*LocalInfo*/
///** XHTimeLocalIP  */
//static const u8_t xhtimelocalIP_len_default = 15;
//static const u8_t xhtimelocalIP_default[] = "192.168.031.045";
//static u8_t* xhtimelocalIP_len_ptr = (u8_t*)&xhtimelocalIP_len_default;
//static u8_t* xhtimelocalIP_ptr = (u8_t*)&xhtimelocalIP_default[0];
///** XHTimeLocalGetway */
//static const u8_t xhtimelocalgetwa_len_default = 15;
//static const u8_t xhtimelocalgetwa_default[] = "192.168.031.001";
//static u8_t* xhtimelocalgetwa_len_ptr = (u8_t*)&xhtimelocalgetwa_len_default;
//static u8_t* xhtimelocalgetwa_ptr = (u8_t*)&xhtimelocalgetwa_default[0];
///** XHTimeLocalNewmask */
//static const u8_t xhtimelocalnewmask_len_default = 15;
//static const u8_t xhtimelocalnewmask_default[] = "255.255.255.000";
//static u8_t* xhtimelocalnewmask_len_ptr = (u8_t*)&xhtimelocalnewmask_len_default;
//static u8_t* xhtimelocalnewmask_ptr = (u8_t*)&xhtimelocalnewmask_default[0];
///** XHTimeLocalMAC */
//static const u8_t xhtimelocalMAC_len_default = 15;
//static const u8_t xhtimelocalMAC_default[] = "2.0.0.123.456.1";
//static u8_t* xhtimelocalMAC_len_ptr = (u8_t*)&xhtimelocalMAC_len_default;
//static u8_t* xhtimelocalMAC_ptr = (u8_t*)&xhtimelocalMAC_default[0];


///*NTP/PTP Info*/
///** XHTimeNTPIP */
//static const u8_t xhtimeNTPIP_len_default = 15;
//static const u8_t xhtimeNTPIP_default[] = "192.168.031.011";
//static u8_t* xhtimeNTPIP_len_ptr = (u8_t*)&xhtimeNTPIP_len_default;
//static u8_t* xhtimeNTPIP_ptr = (u8_t*)&xhtimeNTPIP_default[0];
///** XHTimeNTPGetway */
//static const u8_t xhtimeNTPgetwa_len_default = 15;
//static const u8_t xhtimeNTPgetwa_default[] = "192.168.031.001";
//static u8_t* xhtimeNTPgetwa_len_ptr = (u8_t*)&xhtimeNTPgetwa_len_default;
//static u8_t* xhtimeNTPgetwa_ptr = (u8_t*)&xhtimeNTPgetwa_default[0];
///** XHTimeLocalNewmask */
//static const u8_t xhtimeNTPnewmask_len_default = 15;
//static const u8_t xhtimeNTPnewmask_default[] = "255.255.255.000";
//static u8_t* xhtimeNTPnewmask_len_ptr = (u8_t*)&xhtimeNTPnewmask_len_default;
//static u8_t* xhtimeNTPnewmask_ptr = (u8_t*)&xhtimeNTPnewmask_default[0];
///** XHTimeLocalMAC */
//static const u8_t xhtimeNTPMAC_len_default = 15;
//static const u8_t xhtimeNTPMAC_default[] = "2.0.0.123.456.1";
//static u8_t* xhtimeNTPMAC_len_ptr = (u8_t*)&xhtimeNTPMAC_len_default;
//static u8_t* xhtimeNTPMAC_ptr = (u8_t*)&xhtimeNTPMAC_default[0];

///*State Info*/
///** xHTimeUTCTime */
//static const u8_t xhtimeUTCtime_len_default = 8;
//static const u8_t xhtimeUTCtime_default[] = "14:07:58";
//static u8_t* xhtimeUTCtime_len_ptr = (u8_t*)&xhtimeUTCtime_len_default;
//static u8_t* xhtimeUTCtime_ptr = (u8_t*)&xhtimeUTCtime_default[0];
///** xHTimePos */
//static const u8_t xhtimepos_len_default = 12;
//static const u8_t xhtimepos_default[] = "45'46, 12'13";
//static u8_t* xhtimepos_len_ptr = (u8_t*)&xhtimepos_len_default;
//static u8_t* xhtimepos_ptr = (u8_t*)&xhtimepos_default[0];
///** xHTimeTameState */
//static const u8_t xhtimetamestate_len_default = 7;
//static const u8_t xhtimetamestate_default[] = "success";
//static u8_t* xhtimetamestate_len_ptr = (u8_t*)&xhtimetamestate_len_default;
//static u8_t* xhtimetamestate_ptr = (u8_t*)&xhtimetamestate_default[0];
///** xHTimePtpMode */
//static const u8_t xhtimePTPmode_len_default = 6;
//static const u8_t xhtimePTPmode_default[] = "mode_1";
//static u8_t* xhtimePTPmode_len_ptr = (u8_t*)&xhtimePTPmode_len_default;
//static u8_t* xhtimePTPmode_ptr = (u8_t*)&xhtimePTPmode_default[0];

///** xHTimePtpState */
//static const u8_t xhtimePTPstate_len_default = 3;
//static const u8_t xhtimePTPstate_default[] = "RUN";
//static u8_t* xhtimePTPstate_len_ptr = (u8_t*)&xhtimePTPstate_len_default;
//static u8_t* xhtimePTPstate_ptr = (u8_t*)&xhtimePTPstate_default[0];
///** xHTimePtpE1Mode */
//static const u8_t xhtimePTPE1mode_len_default = 6;
//static const u8_t xhtimePTPE1mode_default[] = "mode_3";
//static u8_t* xhtimePTPE1mode_len_ptr = (u8_t*)&xhtimePTPE1mode_len_default;
//static u8_t* xhtimePTPE1mode_ptr = (u8_t*)&xhtimePTPE1mode_default[0];
///** xHTimePtpE1State */
//static const u8_t xhtimePTPE1state_len_default = 4;
//static const u8_t xhtimePTPE1state_default[] = "stop";
//static u8_t* xhtimePTPE1state_len_ptr = (u8_t*)&xhtimePTPE1state_len_default;
//static u8_t* xhtimePTPE1state_ptr = (u8_t*)&xhtimePTPE1state_default[0];

///*xHTimeTimeSource*/
///** xHTimeTimeSource2*/
//static const u8_t xhtimetimesource1_len_default = 6;
//static const u8_t xhtimetimesource1_default[] = "beidou";
//static u8_t* xhtimetimesource1_len_ptr = (u8_t*)&xhtimetimesource1_len_default;
//static u8_t* xhtimetimesource1_ptr = (u8_t*)&xhtimetimesource1_default[0];
///** xHTimeTimeSource2 */
//static const u8_t xhtimetimesource2_len_default = 5;
//static const u8_t xhtimetimesource2_default[] = "1PTPS";
//static u8_t* xhtimetimesource2_len_ptr = (u8_t*)&xhtimetimesource2_len_default;
//static u8_t* xhtimetimesource2_ptr = (u8_t*)&xhtimetimesource2_default[0];
///** xHTimeTimeSource3 */
//static const u8_t xhtimetimesource3_len_default = 2;
//static const u8_t xhtimetimesource3_default[] = "E1";
//static u8_t* xhtimetimesource3_len_ptr = (u8_t*)&xhtimetimesource3_len_default;
//static u8_t* xhtimetimesource3_ptr = (u8_t*)&xhtimetimesource3_default[0];
///** xHTimeTimeSource4 */
//static const u8_t xhtimetimesource4_len_default = 3;
//static const u8_t xhtimetimesource4_default[] = "NTP";
//static u8_t* xhtimetimesource4_len_ptr = (u8_t*)&xhtimetimesource4_len_default;
//static u8_t* xhtimetimesource4_ptr = (u8_t*)&xhtimetimesource4_default[0];
///** xHTimeTimeSource5 */
//static const u8_t xhtimetimesource5_len_default = 3;
//static const u8_t xhtimetimesource5_default[] = "PTP";
//static u8_t* xhtimetimesource5_len_ptr = (u8_t*)&xhtimetimesource5_len_default;
//static u8_t* xhtimetimesource5_ptr = (u8_t*)&xhtimetimesource5_default[0];



//static void xHTimeEntry_get_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
//{
//  u8_t id;

//  /* return to object name, adding index depth (1) */
//  ident_len += 1;
//  ident -= 1;
//  if (ident_len == 2)
//  {
//    od->id_inst_len = ident_len;
//    od->id_inst_ptr = ident;

//    id = ident[0];
//    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def private xHTimeEntry.%"U16_F".0\n",(u16_t)id));
//    switch (id)
//    {
//      case 1:    /* xHTimeLocalIPInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimelocalIP_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 2:    /* xHTimeLocalGWInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimelocalgetwa_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 3:    /* xHTimeLocalNMInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimelocalnewmask_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 4:    /* xHTimeLocalMACInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimelocalMAC_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 5:    /* xHTimeNTPIPInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimeNTPIP_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 6:    /* xHTimeNTPGWInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimeNTPgetwa_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 7:    /* xHTimeNTPNMInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimeNTPnewmask_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 8:    /* xHTimeNTPMACInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimeNTPMAC_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 9:    /* xHTimeUTCTimeInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimeUTCtime_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 10:    /* xHTimePosInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimepos_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 11:    /* xHTimeTameStateInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimetamestate_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 12:    /* xHTimePtpModeInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimePTPmode_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 13:    /* xHTimePtpStateInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimePTPstate_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 14:    /* xHTimePtpE1ModeInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimePTPE1mode_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 15:    /* xHTimePtpE1StateInfo  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimePTPE1state_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 16:    /* xHTimeTimeSource1Info  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimetimesource1_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 17:    /* xHTimeTimeSource2Info  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimetimesource2_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 18:    /* xHTimeTimeSource3Info  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimetimesource3_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 19:    /* xHTimeTimeSource4Info  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimetimesource4_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      case 20:    /* xHTimeTimeSource5Info  */
//        od->instance = MIB_OBJECT_TAB;
//        od->access = MIB_OBJECT_READ_WRITE;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = *xhtimetimesource5_len_ptr;// todo:  set the appropriate length eg. sizeof(char_buffer);
//        break;
//      default:
//        LWIP_DEBUGF(SNMP_MIB_DEBUG,("xHTimeEntry_get_object_def: no such object\n"));
//        od->instance = MIB_OBJECT_NONE;
//        break;
//    };
//  }
//  else
//  {
//    LWIP_DEBUGF(SNMP_MIB_DEBUG,("private xHTimeEntry_get_object_def: no scalar\n"));
//    od->instance = MIB_OBJECT_NONE;
//  }
//}

//static void xHTimeEntry_get_value(struct obj_def *od, u16_t len, void *value)
//{
//  u8_t id;

//  /* the index value can be found in: od->id_inst_ptr[1] */
//  id = od->id_inst_ptr[0];
//  switch (id)
//  {
//    case 1:    /* xHTimeLocalIPInfo  */
//      ocstrncpy(value,(u8_t*)xhtimelocalIP_ptr,len);
//      break;
//    case 2:    /* xHTimeLocalGWInfo  */
//      ocstrncpy(value,(u8_t*)xhtimelocalgetwa_ptr,len);
//      break;
//    case 3:    /* xHTimeLocalNMInfo  */
//      ocstrncpy(value,(u8_t*)xhtimelocalnewmask_ptr,len);
//      break;
//    case 4:    /* xHTimeLocalMACInfo  */
//      ocstrncpy(value,(u8_t*)xhtimelocalMAC_ptr,len);
//      break;
//    case 5:    /* xHTimeNTPIPInfo  */
//      ocstrncpy(value,(u8_t*)xhtimeNTPIP_ptr,len);
//      break;
//    case 6:    /* xHTimeNTPGWInfo  */
//      ocstrncpy(value,(u8_t*)xhtimeNTPgetwa_ptr,len);
//      break;
//    case 7:    /* xHTimeNTPNMInfo  */
//      ocstrncpy(value,(u8_t*)xhtimeNTPnewmask_ptr,len);
//      break;
//    case 8:    /* xHTimeNTPMACInfo  */
//      ocstrncpy(value,(u8_t*)xhtimeNTPMAC_ptr,len);
//      break;
//    case 9:    /* xHTimeUTCTimeInfo  */
//      ocstrncpy(value,(u8_t*)xhtimeUTCtime_ptr,len);
//      break;
//    case 10:    /* xHTimePosInfo  */
//      ocstrncpy(value,(u8_t*)xhtimepos_ptr,len);
//      break;
//    case 11:    /* xHTimeTameStateInfo  */
//      ocstrncpy(value,(u8_t*)xhtimetamestate_ptr,len);
//      break;
//    case 12:    /* xHTimePtpModeInfo  */
//      ocstrncpy(value,(u8_t*)xhtimePTPmode_ptr,len);
//      break;
//    case 13:    /* xHTimePtpStateInfo  */
//      ocstrncpy(value,(u8_t*)xhtimePTPstate_ptr,len);
//      break;
//    case 14:    /* xHTimePtpE1ModeInfo  */
//      ocstrncpy(value,(u8_t*)xhtimePTPE1mode_ptr,len);
//      break;
//    case 15:    /* xHTimePtpE1StateInfo  */
//      ocstrncpy(value,(u8_t*)xhtimePTPE1state_ptr,len);
//      break;
//    case 16:    /* xHTimeTimeSource1Info  */
//      ocstrncpy(value,(u8_t*)xhtimetimesource1_ptr,len);
//      break;
//    case 17:    /* xHTimeTimeSource2Info  */
//      ocstrncpy(value,(u8_t*)xhtimetimesource2_ptr,len);
//      break;
//    case 18:    /* xHTimeTimeSource3Info  */
//      ocstrncpy(value,(u8_t*)xhtimetimesource3_ptr,len);
//      break;
//    case 19:    /* xHTimeTimeSource4Info  */
//      ocstrncpy(value,(u8_t*)xhtimetimesource4_ptr,len);
//      break;
//    case 20:    /* xHTimeTimeSource5Info  */
//      ocstrncpy(value,(u8_t*)xhtimetimesource5_ptr,len);
//      break;
//  };
//}
////验证参数是否正确  
////直接跳过 不进行验证
//static u8_t xHTimeEntry_set_test(struct obj_def *od, u16_t len, void *value)
//{
//  u8_t id, set_ok;

//  /* the index value can be found in: od->id_inst_ptr[1] */
//  set_ok = 0;
//  id = od->id_inst_ptr[0];
//  switch (id)
//  {
//    case 1:    /* xHTimeLocalIPInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 2:    /* xHTimeLocalGWInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 3:    /* xHTimeLocalNMInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 4:    /* xHTimeLocalMACInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 5:    /* xHTimeNTPIPInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 6:    /* xHTimeNTPGWInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 7:    /* xHTimeNTPNMInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 8:    /* xHTimeNTPMACInfo  */
//  /* validate the value argument and set ok  */
//      break;
//    case 16:    /* xHTimeTimeSource1Info  */
//  /* validate the value argument and set ok  */
//      break;
//    case 17:    /* xHTimeTimeSource2Info  */
//  /* validate the value argument and set ok  */
//      break;
//    case 18:    /* xHTimeTimeSource3Info  */
//  /* validate the value argument and set ok  */
//      break;
//    case 19:    /* xHTimeTimeSource4Info  */
//  /* validate the value argument and set ok  */
//      break;
//    case 20:    /* xHTimeTimeSource5Info  */
//  /* validate the value argument and set ok  */
//      break;
//  };
//  return set_ok;
//}

//static void xHTimeEntry_set_value(struct obj_def *od, u16_t len, void *value)
//{
//  u8_t id;

//  /* the index value can be found in: od->id_inst_ptr[1] */
//  id = od->id_inst_ptr[0];
//  switch (id)
//  {
//    case 1:    /* xHTimeLocalIPInfo  */
//      ocstrncpy((u8_t*)xhtimelocalIP_ptr,value,len);
//      break;
//    case 2:    /* xHTimeLocalGWInfo  */
//      ocstrncpy((u8_t*)xhtimelocalgetwa_ptr,value,len);
//      break;
//    case 3:    /* xHTimeLocalNMInfo  */
//      ocstrncpy((u8_t*)xhtimelocalnewmask_ptr,value,len);
//      break;
//    case 4:    /* xHTimeLocalMACInfo  */
//      ocstrncpy((u8_t*)xhtimelocalMAC_ptr,value,len);
//      break;
//    case 5:    /* xHTimeNTPIPInfo  */
//      ocstrncpy((u8_t*)xhtimeNTPIP_ptr,value,len);
//      break;
//    case 6:    /* xHTimeNTPGWInfo  */
//      ocstrncpy((u8_t*)xhtimeNTPgetwa_ptr,value,len);
//      break;
//    case 7:    /* xHTimeNTPNMInfo  */
//      ocstrncpy((u8_t*)xhtimeNTPnewmask_ptr,value,len);
//      break;
//    case 8:    /* xHTimeNTPMACInfo  */
//      ocstrncpy((u8_t*)xhtimeNTPMAC_ptr,value,len);
//      break;
//    case 16:    /* xHTimeTimeSource1Info  */
//      ocstrncpy((u8_t*)xhtimetimesource1_ptr,value,len);
//      break;
//    case 17:    /* xHTimeTimeSource2Info  */
//      ocstrncpy((u8_t*)xhtimetimesource2_ptr,value,len);
//      break;
//    case 18:    /* xHTimeTimeSource3Info  */
//      ocstrncpy((u8_t*)xhtimetimesource3_ptr,value,len);
//      break;
//    case 19:    /* xHTimeTimeSource4Info  */
//      ocstrncpy((u8_t*)xhtimetimesource4_ptr,value,len);
//      break;
//    case 20:    /* xHTimeTimeSource5Info  */
//      ocstrncpy((u8_t*)xhtimetimesource5_ptr,value,len);
//      break;
//  };
//}

//struct mib_list_rootnode xHTimeEntry_root = {
//  &xHTimeEntry_get_object_def,
//  &xHTimeEntry_get_value,
//  &xHTimeEntry_set_test,
//  &xHTimeEntry_set_value,
//  MIB_NODE_LR,
//  0,
//  NULL,
//  NULL,  0,
//};

///* xHTimeEntry  .1.3.6.1.4.1.22566.1.1    */
//const s32_t xHTimeEntry_ids[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
//struct mib_node* const xHTimeEntry_nodes[20] = { 
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root,
//  (struct mib_node* const)&xHTimeEntry_root
//};

//const struct mib_array_node xHTimeEntry = {
//  &noleafs_get_object_def,
//  &noleafs_get_value,
//  &noleafs_set_test,
//  &noleafs_set_value,
//  MIB_NODE_AR,
//  20,
//  xHTimeEntry_ids,
//  xHTimeEntry_nodes
//};

///* xHTimeTable  .1.3.6.1.4.1.22566.1    */
//s32_t xHTimeTable_ids[1] = { 1 };
//struct mib_node* xHTimeTable_nodes[1] = { 
//  (struct mib_node* const)&xHTimeEntry
//};

//struct mib_ram_array_node xHTimeTable = {
//  &noleafs_get_object_def,
//  &noleafs_get_value,
//  &noleafs_set_test,
//  &noleafs_set_value,
//  MIB_NODE_RA,
//  0,
//  xHTimeTable_ids,
//  xHTimeTable_nodes
//};

///* xHTime  .1.3.6.1.4.1.22566    */
//const s32_t xHTime_ids[1] = {1};
//struct mib_node* const xHTime_nodes[1] = { 
//  (struct mib_node* const)&xHTimeTable
//};

//const struct mib_array_node xHTime = {
//  &noleafs_get_object_def,
//  &noleafs_get_value,
//  &noleafs_set_test,
//  &noleafs_set_value,
//  MIB_NODE_AR,
//  1,
//  xHTime_ids,
//  xHTime_nodes
//};

///* enterprises  .1.3.6.1.4.1    */
//const s32_t enterprises_ids[1] = { 22566 };
//struct mib_node* const enterprises_nodes[1] = { 
//  (struct mib_node* const)&xHTime
//};

//const struct mib_array_node enterprises = {
//  &noleafs_get_object_def,
//  &noleafs_get_value,
//  &noleafs_set_test,
//  &noleafs_set_value,
//  MIB_NODE_AR,
//  1,
//  enterprises_ids,
//  enterprises_nodes
//};

///* private  .1.3.6.1.4    */
//const s32_t private_ids[1] = { 1 };
//struct mib_node* const private_nodes[1] = { 
//  (struct mib_node* const)&enterprises
//};

//const struct mib_array_node mib_private = {
//  &noleafs_get_object_def,
//  &noleafs_get_value,
//  &noleafs_set_test,
//  &noleafs_set_value,
//  MIB_NODE_AR,
//  1,
//  private_ids,
//  private_nodes
//};

//void
//lwip_privmib_init(void)
//{
//}

