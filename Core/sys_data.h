#ifndef SYS_DATA_H
#define SYS_DATA_H

#include "stm32f10x.h"

// Configure weather data structure

typedef struct 
{
	int32_t  temp;
	uint32_t press;
	uint32_t humid;
	uint32_t time;
	uint16_t uv;
	uint8_t  zmbr;
} MeteoData;

extern MeteoData meteo;

#endif