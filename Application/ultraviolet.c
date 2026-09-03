#include "ultraviolet.h"
#include "adc.h"
#include "stm32f10x.h"

void UV_GetData (SystemData* sys)
{
	// Read and convert the UV sensor value
	
	uint32_t data = ADC_Read();
	data = (data * 33) / 4095;
	sys->meteo->uv = (uint16_t) data;
}

