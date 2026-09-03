#ifndef OLED_H
#define OLED_H

#include "stm32f10x.h"
#include "sys_data.h"

void OLED_Init (void);
void OLED_Clear (void);
void OLED_Update (void);
void OLED_Print (uint8_t row, const char *str);
void OLED_SetPage0 (SystemData* sys);
void OLED_SetPage1 (SystemData* sys);
void OLED_SetPage2 (SystemData* sys);

#endif