#ifndef DS3231_H
#define DS3231_H

#include "stm32f10x.h"

void RTC_Init (void);
void RTC_GetTime (uint32_t* time);

#endif