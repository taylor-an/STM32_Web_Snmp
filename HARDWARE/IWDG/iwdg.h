#ifndef _IWDG_H
#define _IWDG_H
#include "sys.h"
#include "type.h"

void IWDG_Init(u8 pre, u16 rlr);
void IWDG_Feed(void);

#endif
