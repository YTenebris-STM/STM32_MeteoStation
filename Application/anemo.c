#include "anemo.h"
#include "stm32f10x.h"
#include "tim3.h"

#define CALIB_K 1
#define CALIB_B 0

#define ANEMO_SIZE 8
static uint16_t Anemo_Buffer[ANEMO_SIZE] = {0};
static uint8_t index = 0;
static uint8_t count = 0;

uint8_t Anemo_GetInstSpeed (uint16_t *instspeed)
{
	if (!is_measured) return 0;
	is_measured = 0;
	if (calm_flag) *instspeed = 0;
	else *instspeed = (CALIB_K * 1000 * 100 / period) + (CALIB_B * 100);
	return 1;
}

void Anemo_GetAvgSpeed (uint16_t *avgspeed)
{
	uint32_t result = 0;
	
	for (uint8_t i = 0; i < count; i++)
	{
		result += Anemo_Buffer[i];
	}
	
	*avgspeed = (uint16_t)(result / count);
}

void Anemo_AddToBuffer (uint16_t speed)
{
	// Add a measurement to the wind buffer
	
	Anemo_Buffer[index] = speed;
	index++;
	if (index >= ANEMO_SIZE) index = 0;
	if (count < ANEMO_SIZE) count++;
}

void Anemo_GetData (MeteoData* meteo)
{
	uint16_t speed;
	if (!Anemo_GetInstSpeed(&speed)) return;
	Anemo_AddToBuffer(speed);
	
	Anemo_GetAvgSpeed(&meteo->wind_speed);
}

