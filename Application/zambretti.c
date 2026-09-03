#include "zambretti.h"
#include "stm32f10x.h"
#include "history.h"
#include "math.h"

#define ALTITUDE 152.0f

float Zambretti_GetSLP (MeteoData* meteo)
{
	// Calculate sea-level pressure from station pressure
	
	float abs_temp = (meteo->temp / 100.0f) + 273.15f;
	float factor = 1.0f - (0.0065f * ALTITUDE) / (abs_temp + 0.0065f * ALTITUDE);
	float P0 = meteo->press / 100.0f * powf(factor, -5.257f);
	
	return P0;
}

void Zambretti_GetData (SystemData* sys)
{
	// Calculate and update the Zambretti forecast
	
	if (!HISTORY_IsFull())
	{
		sys->meteo->zmbr = 0;
		return;
	}
	
	// Calculate the pressure trend from recent measurements
	
	float P0 = Zambretti_GetSLP(HISTORY_GetNewest());
	float trend = P0 - Zambretti_GetSLP(HISTORY_GetOldest());
	
	// Calculate the Zambretti weather forecast index
	
	float z;

	if (trend < -1.6f)
	{
		z = 130.0f - P0 / 8.1f;
	}
	else if (trend > 1.6f)
	{
		z = 179.0f - (2.0f * P0) / 12.9f;
	}
	else
	{
		z = 147.0f - (5.0f * P0) / 37.6f;
	}

	if (z < 1.0f) z = 1.0f;
	if (z > 26.0f) z = 26.0f;
	
	sys->meteo->zmbr = (uint8_t)(z + 0.5f);
}