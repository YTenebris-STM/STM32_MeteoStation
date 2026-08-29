#include "tim2.h"

volatile uint32_t system_ms = 0;

void TIM2_Init (void)
{
	// Configure TIM2 to generate a 1 ms system tick
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->CR1 = TIM_CR1_URS;
	
	// Set the timer period to 1 ms
	
	TIM2->PSC = 7199;
	TIM2->ARR = 9;
	TIM2->CNT = 0;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->DIER |= TIM_DIER_UIE;

	
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler (void)
{
	if (TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		system_ms++;
	}
}

uint32_t TIM2_GetTick (void)
{
	return system_ms;
}