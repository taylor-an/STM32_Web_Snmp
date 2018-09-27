#include "ptp_hardware.h"


u32 ETH_PTPNanoSecond2SubSecond(u32 SubSecondValue)
{
  uint64_t val = SubSecondValue * 0x80000000ll;
  val /= 1000000000;
  return val;
}

u32 ETH_PTPSubSecond2NanoSecond(u32 SubSecondValue)
{
  uint64_t val = SubSecondValue * 1000000000ll;
  val >>=31;
  return val;
}

void ETH_PTPTime_UpdateOffset(struct ptptime_t * timeoffset)
{
    uint32_t Sign;
    uint32_t SecondValue;
    uint32_t NanoSecondValue;
    uint32_t SubSecondValue;
    uint32_t addend;

    if(timeoffset->tv_sec < 0 || (timeoffset->tv_sec == 0 && timeoffset->tv_nsec < 0)) {
        Sign = ETH_PTP_NegativeTime;
        SecondValue = -timeoffset->tv_sec;
        NanoSecondValue = -timeoffset->tv_nsec;
    } else {
        Sign = ETH_PTP_PositiveTime;
        SecondValue = timeoffset->tv_sec;
        NanoSecondValue = timeoffset->tv_nsec;
    }

    /* convert nanosecond to subseconds */
    SubSecondValue = ETH_PTPNanoSecond2SubSecond(NanoSecondValue);
 
    /* read old addend register value*/
    addend = ETH_GetPTPRegister(ETH_PTPTSAR);

    while(ETH_GetPTPFlagStatus(ETH_PTP_FLAG_TSSTU) == SET);
    while(ETH_GetPTPFlagStatus(ETH_PTP_FLAG_TSSTI) == SET);

    /* Write the offset (positive or negative) in the Time stamp update high and low registers. */
    ETH_SetPTPTimeStampUpdate(Sign, SecondValue, SubSecondValue);
    /* Set bit 3 (TSSTU) in the Time stamp control register. */
    ETH_EnablePTPTimeStampUpdate();
    /* The value in the Time stamp update registers is added to or subtracted from the system */
    /* time when the TSSTU bit is cleared. */
    while(ETH_GetPTPFlagStatus(ETH_PTP_FLAG_TSSTU) == SET);      

    /* write back old addend register value */
    ETH_SetPTPTimeStampAddend(addend);
    ETH_EnablePTPTimeStampAddend();
}

void ETH_PTPTime_SetTime(struct ptptime_t * timestamp)
{
    uint32_t Sign;
    uint32_t SecondValue;
    uint32_t NanoSecondValue;
    uint32_t SubSecondValue;

    /* determine sign and correct Second and Nanosecond values */
    if(timestamp->tv_sec < 0 || (timestamp->tv_sec == 0 && timestamp->tv_nsec < 0)) {
        Sign = ETH_PTP_NegativeTime;
        SecondValue = -timestamp->tv_sec;
        NanoSecondValue = -timestamp->tv_nsec;
    } else {
        Sign = ETH_PTP_PositiveTime;
        SecondValue = timestamp->tv_sec;
        NanoSecondValue = timestamp->tv_nsec;
    }

    SubSecondValue = ETH_PTPNanoSecond2SubSecond(NanoSecondValue);
 
    ETH_SetPTPTimeStampUpdate(Sign, SecondValue, SubSecondValue);

    ETH_InitializePTPTimeStamp();

    while(ETH_GetPTPFlagStatus(ETH_PTP_FLAG_TSSTI) == SET);
}


