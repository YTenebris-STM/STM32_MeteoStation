#include "history.h"

#define HISTORY_SIZE 37

// Circular buffer storing recent weather measurements

static MeteoData HISTORY_Buffer[HISTORY_SIZE] = {0};
static uint8_t index = 0;
static uint8_t count = 0;

void HISTORY_Add (SystemData* sys)
{
	// Add a measurement to the circular history buffer
	
	HISTORY_Buffer[index] = *sys->meteo;
	index++;
	if (index >= HISTORY_SIZE) index = 0;
	if (count < HISTORY_SIZE) count++;
}

MeteoData* HISTORY_GetOldest (void)
{
	// Get the oldest stored measurement
	
	return &HISTORY_Buffer[index];
}

MeteoData* HISTORY_GetNewest (void)
{
	// Get the newest stored measurement
	
	if (index == 0) return &HISTORY_Buffer[HISTORY_SIZE - 1];
	return &HISTORY_Buffer[index-1];
}

uint8_t HISTORY_IsFull (void)
{
	// Check whether the history buffer is full
	
	return count >= HISTORY_SIZE;
}