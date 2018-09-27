#include "ptp.h"
#include "ptp_hardware.h"
#include "stdlib.h"
//#define abs(x) (x > 0 ? x : -x)


void ptp_init(uint32_t UpdateMethod)
{
  /* Mask the Time stamp trigger interrupt by setting bit 9 in the MACIMR register. */
  ETH_MACITConfig(ETH_MAC_IT_TST, DISABLE);
  /* Program Time stamp register bit 0 to enable time stamping. */
  ETH_PTPTimeStampCmd(ENABLE);
  /* Program the Subsecond increment register based on the PTP clock frequency. */
  ETH_SetPTPSubSecondIncrement(ADJ_FREQ1_BASE_INCREMENT); /* to achieve 20 ns accuracy, the value is ~ 43 */

  if (UpdateMethod == ETH_PTP_FineUpdate) {

    /* If you are using the Fine correction method, program the Time stamp addend register
     * and set Time stamp control register bit 5 (addend register update). */
    ETH_SetPTPTimeStampAddend(ADJ_FREQ1_BASE_ADDEND);
    ETH_EnablePTPTimeStampAddend();
    /* Poll the Time stamp control register until bit 5 is cleared. */
    while(ETH_GetPTPFlagStatus(ETH_PTP_FLAG_TSARU) == SET);
  }
  
  ETH_PTPUpdateMethodConfig(UpdateMethod);

  ETH_SetPTPTimeStampUpdate(ETH_PTP_PositiveTime, JAN_2018, 0);

  ETH_InitializePTPTimeStamp();
}

void updateClock(TimeInternal * offset)//参数  时间戳 以及偏差时间
{
	 static s32 observedDrift;
					s32 adj;
          s32 offsetNorm;
	        TimeInternal timeTmp;
	
    if (abs(offset -> nanoseconds ) > MAX_ADJ_OFFSET_NS )
    {	
				getTime(&timeTmp);
        subTime(&timeTmp, &timeTmp, offset);
        setTime(&timeTmp);	
    }
    else
    {
//      if( abs(offset -> nanoseconds ) > 200)
//			{
				offsetNorm = offset -> nanoseconds  ;
        /* the accumulator for the I component */
        observedDrift += offsetNorm >> 1 ;
        /* clamp the accumulator to ADJ_FREQ_MAX for sanity */
        if (observedDrift > ADJ_FREQ_MAX)
            observedDrift = ADJ_FREQ_MAX;
        else if (observedDrift < -ADJ_FREQ_MAX)
         observedDrift = -ADJ_FREQ_MAX;
         adj = offsetNorm + observedDrift;
         adjFreq(-adj);
//			}
		}

}




