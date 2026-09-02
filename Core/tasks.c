#include "tasks.h"
#include "oled.h"
#include "bme280.h"
#include "zambretti.h"
#include "ultraviolet.h"
#include "ds3231.h"
#include "history.h"
#include "apptemp.h"
#include "anemo.h"

extern volatile uint8_t current_page;
extern volatile uint8_t page_changed;

void Task_Display (void)
{
	// Update the display according to the current page
	
	if (page_changed)
	{
		OLED_Clear();
		switch (current_page)
		{
			case 0 : 
				OLED_Print(0, " HH:MM:SS dd.mm ");
				OLED_Print(1, "Temp:  +12.34 °C");
				OLED_Print(2, "Press: 101225 Pa");
				OLED_Print(3, "Humid: 98.76  % ");
				break;
			case 1 :
				OLED_Print(0, "App temp: +01 °C");
				OLED_Print(1, "UV index: 23");
				OLED_Print(2, "Zambretti: 45");
				OLED_Print(3, "Wind: 67.8 mps");
				break;
				
		}
		
		page_changed = 0;
	}
	
	switch (current_page)
	{
		case 0 : 
			OLED_SetPage0(&meteo);
			break;
		case 1 :
			OLED_SetPage1(&meteo);
			break;
	}
	
	OLED_Update();
	
}


void Task_BME280 (void)
{
	// Read temperature, pressure and humidity
	
	BME280_GetData(&meteo.temp, &meteo.press, &meteo.humid);
}

void Task_UVI (void)
{
	// Read and update the UV index
	
	UV_GetData(&meteo.uv);
}

void Task_RTC (void)
{
	// Read and update the current time
	
	RTC_GetTime(&meteo.time);
}

void Task_Zambretti (void)
{
	// Calculate and update the Zambretti forecast
	
	HISTORY_Add(&meteo);
	Zambretti_GetData(&meteo);
}

void Task_LED (void)
{
	// Update the status LED
	
	GPIOB->ODR ^= GPIO_ODR_ODR2;
}

void Task_Anemo (void)
{
	Anemo_GetData(&meteo);
}


void Task_AppTemp (void)
{
	AppTemp_GetData(&meteo);
}








