#include "tim3.h"

volatile uint16_t period;
volatile uint8_t calm_flag; 
volatile uint8_t is_measured;

void TIM3_Init (void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	GPIOB->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
	GPIOB->CRL |= GPIO_CRL_CNF0_1;
	
	TIM3->PSC = 35999;
	TIM3->ARR = 9999;
	TIM3->CNT = 0;
	
	TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
	TIM3->CCMR2 |= TIM_CCMR2_CC3S_0;
	TIM3->CCER |= TIM_CCER_CC3E;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->DIER |= TIM_DIER_CC3IE;
	TIM3->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ(TIM3_IRQn);
}


void TIM3_IRQHandler (void)
{
	if (TIM3->SR & TIM_SR_CC3IF)
	{
		period = (TIM3->CCR3 + 1) / 2;
		calm_flag = 0;
		
		TIM3->CNT = 0;
		TIM3->SR &= ~TIM_SR_CC3IF;
		TIM3->SR &= ~TIM_SR_UIF;
		
		is_measured = 1;
	}
	
	if (TIM3->SR & TIM_SR_UIF)
	{
		calm_flag = 1;
		TIM3->SR &= ~TIM_SR_UIF;
		
		is_measured = 1;
	}
}


