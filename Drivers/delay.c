#include "delay.h"
#include "tim2.h"

void delay (uint32_t ms)
{	
	// Wait for the specified number of milliseconds
	
	uint32_t now = TIM2_GetTick();
	while (TIM2_GetTick() - now < ms) __NOP();
}