#include "ds3231.h"

void RTC_SetTime (uint32_t seconds);

void RTC_Init (void)
{
	// Configure the RTC to use the LSE oscillator
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
	PWR->CR |= PWR_CR_DBP;
	RCC->BDCR |= RCC_BDCR_LSEON;
	while (!(RCC->BDCR & RCC_BDCR_LSERDY)) {};
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
	RCC->BDCR |= RCC_BDCR_RTCEN;

	while (!(RTC->CRL & RTC_CRL_RTOFF));
	RTC->CRL |= RTC_CRL_CNF;
		
	// Configure the RTC prescaler for 1 Hz counting
		
	RTC->PRLH = 0;
	RTC->PRLL = 32767;
	RTC->CRL &= ~RTC_CRL_CNF;
	while (!(RTC->CRL & RTC_CRL_RTOFF));
		
	// Set the initial time for debugging

	RTC_SetTime(66600);
}

void RTC_SetTime (uint32_t seconds)
{
	while (!(RTC->CRL & RTC_CRL_RTOFF));
	RTC->CRL |= RTC_CRL_CNF;
	RTC->CNTH = seconds >> 16;
	RTC->CNTL = seconds & 0xFFFF;
	RTC->CRL &= ~RTC_CRL_CNF;
}

void RTC_GetTime (uint32_t* time)
{
	// Read the current RTC counter value
	
	uint16_t tmp_l, tmp_h;
	
	do {
		tmp_h = RTC->CNTH;
		tmp_l = RTC->CNTL;
	} while (tmp_h != RTC->CNTH);

	*time = ((uint32_t)tmp_h << 16) | tmp_l;
}
