#include "stm32f10x.h"
#include "scheduler.h"
#include "i2c.h"
#include "tim2.h"
#include "oled.h"
#include "bme280.h"
#include "ds3231.h"
#include "adc.h"

#define NUM_PAGES 2

volatile uint8_t current_page = 0;
volatile uint8_t page_changed = 1;

void RCC_Init (void);
void GPIO_Init (void);
void Interrupt_PA0_Init (void);

int main (void)
{
	// Initialize peripherals and application modules
	
	RCC_Init();
	TIM2_Init();
	I2C_Init();
	OLED_Init();
	GPIO_Init();
	BME280_Init();
	RTC_Init();
	ADC_Init();
	Interrupt_PA0_Init();
	
	// Initialize the application scheduler
	
	Scheduler_Init();
	
	// Run the application scheduler

	while (1) 
	{		
		Scheduler_Run();
	}	
	return 0;
}

void RCC_Init (void) 
{
	// Configure system clock to 72 MHz
	
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY)) {};

	RCC->CFGR &= (uint32_t)((uint32_t) ~ (RCC_CFGR_PLLSRC |
																				RCC_CFGR_PLLXTPRE |
																				RCC_CFGR_PLLMULL));
	RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY)) {};
		
	RCC->CFGR &= ~RCC_CFGR_HPRE;
	RCC->CFGR |=  RCC_CFGR_HPRE_DIV1;
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |=  RCC_CFGR_PPRE1_DIV2;
	RCC->CFGR &= ~RCC_CFGR_PPRE2;
	RCC->CFGR |=  RCC_CFGR_PPRE2_DIV1;
	RCC->CFGR &= ~RCC_CFGR_ADCPRE;
	RCC->CFGR |=  RCC_CFGR_ADCPRE_DIV6;
		
	FLASH->ACR &= ~FLASH_ACR_PRFTBE;
	FLASH->ACR |=  FLASH_ACR_PRFTBE;
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=  FLASH_ACR_LATENCY_2;

	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |=  RCC_CFGR_SW_PLL;
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
}

void GPIO_Init (void) // GPIO Initialization (LED PB2, Button PA0)
{
	// Configure LED output and button input
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	GPIOB->CRL &= ~GPIO_CRL_MODE2_0;
	GPIOB->CRL |=  GPIO_CRL_MODE2_1;
	GPIOB->CRL &= ~GPIO_CRL_CNF2_0;
	GPIOB->CRL &= ~GPIO_CRL_CNF2_1;
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA->CRL &= ~GPIO_CRL_MODE0_0;
	GPIOA->CRL &= ~GPIO_CRL_MODE0_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF0_0;
	GPIOA->CRL |=  GPIO_CRL_CNF0_1;
  
	
}

void Interrupt_PA0_Init (void)
{
	// Configure EXTI interrupt for the user button
	
	EXTI->PR = EXTI_PR_PR0;
	EXTI->IMR |=  EXTI_IMR_MR0;
	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0_PA;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	EXTI->FTSR |= EXTI_FTSR_TR0;

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
}

void EXTI0_IRQHandler (void)
{
	// Set the page change flag when the button is pressed
	
	EXTI->PR = EXTI_PR_PR0;
	
	current_page++;
	if (current_page >= NUM_PAGES) current_page = 0;
	page_changed = 1;
	
}

