#ifndef BME280_H
#define BME280_H

#include "stm32f10x.h"

void BME280_Init (void);
void BME280_GetData (int32_t* temp, uint32_t* press, uint32_t* humid);

#endif