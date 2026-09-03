#ifndef HISTORY_H
#define HISTORY_H

#include "stm32f10x.h"
#include "sys_data.h"

void HISTORY_Add (SystemData* sys);
MeteoData* HISTORY_GetOldest (void);
MeteoData* HISTORY_GetNewest (void);
uint8_t HISTORY_IsFull (void);

#endif