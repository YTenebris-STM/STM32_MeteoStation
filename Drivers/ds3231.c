#include "ds3231.h"
#include "i2c.h"

static const char* const week[7] = 
{
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"	
};

static uint8_t BIN_To_BCD(uint8_t value)
{
	return ((value / 10) << 4) | (value % 10);
}


static uint8_t BCD_To_BIN(uint8_t value)
{
	return ((value >> 4) * 10) + (value & 0x0F);
}

void RTC_WriteData (uint8_t reg, uint8_t data)
{
	// Send data to DS3231
	
	I2C_Write(0x68, reg, data);
}

void RTC_WriteTime (uint8_t reg, uint8_t time)
{
	// Send time to DS3231
	
	I2C_Write(0x68, reg, BIN_To_BCD(time));
}

uint8_t RTC_ReadTime (uint8_t reg)
{
	// Read time from DS3231
	
	return BCD_To_BIN(I2C_Read(0x68, reg));
}

void RTC_Init(void)
{
	/*
	RTC_WriteData(0x00, 0x00);  // seconds
	RTC_WriteData(0x01, 0x00);  // minutes
	RTC_WriteData(0x02, 0x22);  // hours
	RTC_WriteData(0x03, 0x03);  // day of the week
	RTC_WriteData(0x04, 0x02);  // day
	RTC_WriteData(0x05, 0x09);  // month
	RTC_WriteData(0x06, 0x26);  // year
	*/

	RTC_WriteData(0x0F, 0x00);
}


void RTC_GetTime(SystemData* sys)
{
	sys->time->seconds = RTC_ReadTime(0x00);
	sys->time->minutes = RTC_ReadTime(0x01);
	sys->time->hours   = RTC_ReadTime(0x02);
	sys->time->week    = week[RTC_ReadTime(0x03) - 1];
	sys->time->day     = RTC_ReadTime(0x04);
	sys->time->month   = RTC_ReadTime(0x05);
	sys->time->year    = RTC_ReadTime(0x06);
}