#ifndef SYS_DATA_H
#define SYS_DATA_H

#include "stm32f10x.h"

// Configure weather data structure

typedef struct 
{
	int32_t  temp;         // °C * 100
	uint32_t press;        // Pa
	uint32_t humid;        // % * 100
	uint32_t time;         // seconds
	uint16_t uv;           // index (0...11)
	uint8_t  zmbr;         // index (1...26)
	uint16_t  wind_speed;   // mps * 100
	int16_t  apptemp;      // °C * 10
} MeteoData;

extern MeteoData meteo;

#endif