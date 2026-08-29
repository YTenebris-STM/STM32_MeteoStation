#include "adc.h"

void ADC_Init (void)
{	
	// Configure the analog input for the UV sensor
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA->CRL &= ~GPIO_CRL_MODE1;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	
	// Configure ADC1 for single-channel conversion
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	ADC1->CR1 = 0;
	ADC1->CR2 = 0;
	ADC1->CR2 |= ADC_CR2_EXTTRIG;
	ADC1->CR2 |= ADC_CR2_EXTSEL;
	ADC1->SQR1 &= ~ADC_SQR1_L;
	
	ADC1->SQR3 = ADC_SQR3_SQ1_0;
	ADC1->SMPR2 |= ADC_SMPR2_SMP1;
	ADC1->CR2 |= ADC_CR2_ADON;
	
	for (uint8_t i = 0; i < 10; i++) __NOP();	
	ADC1->CR2 |= ADC_CR2_CAL;
	while (ADC1->CR2 & ADC_CR2_CAL);
}

uint16_t ADC_Read (void)
{
	ADC1->CR2 |= ADC_CR2_SWSTART; 
	while (!(ADC1->SR & ADC_SR_EOC));
	return ADC1->DR;
}





