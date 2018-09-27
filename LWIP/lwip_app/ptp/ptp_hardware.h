#ifndef _ptp_hardware_h_
#define _ptp_hardware_h_

#include "stm32f10x.h"
#include "stm32_eth.h"
#include "systime.h"


void ETH_PTPTime_UpdateOffset(struct ptptime_t * timeoffset);

void ETH_PTPTime_SetTime(struct ptptime_t * timestamp);


#endif
