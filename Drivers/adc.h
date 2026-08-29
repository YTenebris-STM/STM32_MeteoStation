#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

void ADC_Init (void);
uint16_t ADC_Read (void);

#endif