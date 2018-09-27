#ifndef _systime_h_
#define _systime_h_

#include "common.h"
#include "stm32_eth.h"
#include "ptp_hardware.h"

typedef struct
{
    s32 seconds;
    s32 nanoseconds;
} TimeInternal;



void getTime(TimeInternal *time);
void setTime(const TimeInternal *time);
void updateTime(const TimeInternal *time);
void normalizeTime(TimeInternal *r);
void addTime(TimeInternal *r, const TimeInternal *x, const TimeInternal *y);
void subTime(TimeInternal *r, const TimeInternal *x, const TimeInternal *y);

void div2Time(TimeInternal *r);

s32 floorLog2(s32 n);

u32 getRand(u32 randMax);
void adjFreq(s32 adj);

#define ADJ_FREQ_MAX  512000


  /* Examples of subsecond increment and addend values using SysClk = 168 MHz
   
   Addend * Increment = 2^63 / SysClk

    ptp_tick = Increment * 10^9 / 2^31
  
   +-----------+-----------+------------+
   | ptp tick  | Increment | Addend     |
   +-----------+-----------+------------+
   |   6 ns    |    13     | 0xFBB83DEF |
   +-----------+-----------+------------+
  */
  
//0xFBB83DEF
// 13

#define ADJ_FREQ1_BASE_ADDEND      0xB191D856  
#define ADJ_FREQ1_BASE_INCREMENT   43 


u32 ETH_PTPSubSecond2NanoSecond(u32 SubSecondValue);

u32 ETH_PTPNanoSecond2SubSecond(u32 SubSecondValue);


#endif


