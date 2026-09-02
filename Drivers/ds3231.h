#ifndef DS3231_H
#define DS3231_H

#include "sys_data.h"

void RTC_Init (void);
void RTC_GetTime (MeteoData *meteo);

#endif