#ifndef _ptp_h_
#define _ptp_h_

#include "stm32f10x.h"
#include "systime.h"
#include "ptp_hardware.h"


void ptp_init(uint32_t UpdateMethod);

void updateClock(TimeInternal * offset);//参数  时间戳 以及偏差时间

#define MAX_ADJ_OFFSET_NS				10000000 /* max offset to try to adjust it < 100ms*/

#define JAN_2018 1514736000u

#ifdef PTPD_ERR
#define ERROR(...)  { TimeInternal tmpTime; getTime(&tmpTime); printf("(E %d.%09d) ", tmpTime.seconds, tmpTime.nanoseconds); printf(__VA_ARGS__); }
/* #define ERROR(...)  { printf("(E) "); printf(__VA_ARGS__); } */
#else
#define ERROR(...)
#endif
/** \}*/

/** \name Debug messages*/
/**\{*/
#ifdef PTPD_DBGVV
#define PTPD_DBGV
#define PTPD_DBG
#define PTPD_ERR
#define DBGVV(...) printf("(V) " __VA_ARGS__)
#else
#define DBGVV(...)
#endif


#ifdef PTPD_DBGV
#define PTPD_DBG
#define PTPD_ERR
#define DBGV(...)  { TimeInternal tmpTime; getTime(&tmpTime); printf("(d %d.%09d) ", tmpTime.seconds, tmpTime.nanoseconds); printf(__VA_ARGS__); }
#else
#define DBGV(...)
#endif

#ifdef PTPD_DBG
#define PTPD_ERR
#define DBG(...)  { TimeInternal tmpTime; getTime(&tmpTime); printf("(D %d.%09d) ", tmpTime.seconds, tmpTime.nanoseconds); printf(__VA_ARGS__); }
#else
#define DBG(...)
#endif

#endif

