#ifndef TIM3_H
#define TIM3_H

#include "stm32f10x.h"

extern volatile uint16_t period;
extern volatile uint8_t calm_flag; 
extern volatile uint8_t is_measured;

void TIM3_Init (void);

#endif