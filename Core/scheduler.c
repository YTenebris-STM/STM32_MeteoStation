#include "scheduler.h"
#include "stm32f10x.h"
#include "tim2.h"
#include "tasks.h"

// Task execution periods in milliseconds

#define DISPLAY_PERIOD 1000
#define BME280_PERIOD 1000
#define UV_PERIOD 1000
#define RTC_PERIOD 1000
#define ZAMBRETTI_PERIOD 300000
#define LED_PERIOD 100
#define APPTEMP_PERIOD 5000
#define ANEMO_PERIOD 1000

static uint32_t display_time;
static uint32_t bme280_time;
static uint32_t uv_time;
static uint32_t rtc_time;
static uint32_t zambretti_time;
static uint32_t led_time;
static uint32_t apptemp_time;
static uint32_t anemo_time;

extern volatile uint8_t page_changed;

void Scheduler_Init (void)
{
	// Initialize task timers
	
	uint32_t now = TIM2_GetTick();

	display_time = now;
	bme280_time = now;
	uv_time = now;
	rtc_time = now;
	zambretti_time = now;
	led_time = now;
}


void Scheduler_Run (void)
{
	uint32_t now = TIM2_GetTick();
	
	// Run tasks when their execution periods expire
	
	if (now - display_time >= DISPLAY_PERIOD)
	{	
		display_time += 1000;
		Task_Display();
	}
		
	if (now - bme280_time >= BME280_PERIOD)
	{	
		bme280_time += 1000;
		Task_BME280();
	}
	
	if (now - uv_time >= UV_PERIOD)
	{	
		uv_time += 1000;
		Task_UVI();
	}
		
	if (now - rtc_time >= RTC_PERIOD)
	{	
		rtc_time += 1000;
		Task_RTC();
	}
	
	if (now - zambretti_time >= ZAMBRETTI_PERIOD)
	{	
		zambretti_time += 300000;
		Task_Zambretti();
	}
		
	if (now - led_time >= LED_PERIOD)
	{	
		led_time += 100;
		Task_LED();
	}
	
	// Update the display immediately after a page change
	
	if (page_changed)
	{
		Task_Display();
		display_time = now;
	}
	
	if (now - apptemp_time >= APPTEMP_PERIOD)
	{	
		apptemp_time += 100;
		Task_AppTemp();
	}
	
	if (now - anemo_time >= ANEMO_PERIOD)
	{	
		anemo_time += 100;
		Task_Anemo();
	}
		
}