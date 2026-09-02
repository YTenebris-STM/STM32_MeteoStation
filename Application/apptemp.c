#include "apptemp.h"
#include "stm32f10x.h"
#include "math.h"

void AppTemp_GetData(MeteoData *meteo)
{
	// Calculate Steadman Apparent Temperature
	
	float temp = meteo->temp / 100.0f;
	float humid = meteo->humid / 100.0f;
	float wind_speed = meteo->wind_speed / 100.0f;
	float water_press = (humid / 100.0f) * 6.105f * expf(17.27f * temp / (temp + 237.7f));
	float apptemp = temp + 0.33f * water_press - 0.7f * wind_speed - 4.0f;
		
	if (apptemp >= 0.0f)
		meteo->apptemp = (int16_t)(apptemp * 10.0f + 0.5f);
	else
		meteo->apptemp = (int16_t)(apptemp * 10.0f - 0.5f);
}