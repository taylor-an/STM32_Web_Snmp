/* sys.c */

#include "systime.h"




void ETH_PTPTime_GetTime(struct ptptime_t * timestamp) {
	timestamp->tv_sec = ETH_GetPTPRegister(ETH_PTPTSHR);
	timestamp->tv_nsec = ETH_PTPSubSecond2NanoSecond(ETH_GetPTPRegister(ETH_PTPTSLR));
}

void ETH_GetTime( TimeInternal * timestamp) {
	timestamp->seconds = ETH_GetPTPRegister(ETH_PTPTSHR);
	timestamp->nanoseconds = ETH_GetPTPRegister(ETH_PTPTSLR);
}

void ETH_PTPTime_AdjFreq(int32_t Adj)
{
    uint32_t addend;

    if( Adj > 5120000) Adj = 5120000;
    if( Adj < -5120000) Adj = -5120000;

    addend = ((((275LL * Adj)>>8) * (ADJ_FREQ1_BASE_ADDEND>>24))>>6) + ADJ_FREQ1_BASE_ADDEND;
    
    ETH_SetPTPTimeStampAddend((uint32_t)addend);
    ETH_EnablePTPTimeStampAddend();
}

void getTime(TimeInternal *time)
{
    struct ptptime_t timestamp;
    ETH_PTPTime_GetTime(&timestamp);
    time->seconds = timestamp.tv_sec;
    time->nanoseconds = timestamp.tv_nsec;
}

void setTime(const TimeInternal *time)
{
    struct ptptime_t ts;
	
    ts.tv_sec = time->seconds;
    ts.tv_nsec = time->nanoseconds;

    ETH_PTPTime_SetTime(&ts);

}

void normalizeTime(TimeInternal *r)
{
    r->seconds += r->nanoseconds / 1000000000;
    r->nanoseconds -= r->nanoseconds / 1000000000 * 1000000000;

    if (r->seconds > 0 && r->nanoseconds < 0)
    {
        r->seconds -= 1;
        r->nanoseconds += 1000000000;
    }
    else if (r->seconds < 0 && r->nanoseconds > 0)
    {
        r->seconds += 1;
        r->nanoseconds -= 1000000000;
    }
}

void addTime(TimeInternal *r, const TimeInternal *x, const TimeInternal *y)
{
    r->seconds = x->seconds + y->seconds;
    r->nanoseconds = x->nanoseconds + y->nanoseconds;

    normalizeTime(r);
}

void subTime(TimeInternal *r, const TimeInternal *x, const TimeInternal *y)
{
    r->seconds = x->seconds - y->seconds;
    r->nanoseconds = x->nanoseconds - y->nanoseconds;

    normalizeTime(r);
}

void div2Time(TimeInternal *r)
{
    r->nanoseconds += r->seconds % 2 * 1000000000;
    r->seconds /= 2;
    r->nanoseconds /= 2;
	
    normalizeTime(r);
}

s32 floorLog2(s32 n)
{
  int pos = 0;

  if (n == 0)
    return -1;
 
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  return pos;
}

void updateTime(const TimeInternal *time)
{

    struct ptptime_t timeoffset;


    timeoffset.tv_sec = -time->seconds;
    timeoffset.tv_nsec = -time->nanoseconds;

	/* Coarse update method */
    ETH_PTPTime_UpdateOffset(&timeoffset);
}


void adjFreq(s32 adj)
{

    if (adj > ADJ_FREQ_MAX)
        adj = ADJ_FREQ_MAX;
    else if (adj < -ADJ_FREQ_MAX)
        adj = -ADJ_FREQ_MAX;

    /* Fine update method */
	ETH_PTPTime_AdjFreq(adj);

}
