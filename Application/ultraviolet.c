#include "ultraviolet.h"
#include "adc.h"

void UV_GetData (uint16_t* uv)
{
	// Read and convert the UV sensor value
	
	uint32_t data = ADC_Read();
	data = (data * 33) / 4095;
	*uv = (uint16_t) data;
}

