#include "oled.h"
#include "i2c.h"
#include "delay.h"
#include "fonts.h"

// Display framebuffer

uint8_t OLED_Buffer[1024] = {0};

void OLED_WriteCmd (uint8_t cmd)
{
	// Send a command to the OLED controller
	
	I2C_Write(0x3C, 0x00, cmd);
}

void OLED_WriteData (uint8_t data)
{
	// Send display data to the OLED controller
	
	I2C_Write(0x3C, 0x40, data);
}

void OLED_Init (void)
{
	// Initialize the OLED controller
	
	delay(100);
	
	OLED_WriteCmd(0xAE);
	OLED_WriteCmd(0xD5);
	OLED_WriteCmd(0x80);
	OLED_WriteCmd(0xA8);
	OLED_WriteCmd(0x3F);
	OLED_WriteCmd(0xD3);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(0x40);
	OLED_WriteCmd(0x8D);
	OLED_WriteCmd(0x14);
	OLED_WriteCmd(0x20);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(0xA1);
	OLED_WriteCmd(0xC8);
	OLED_WriteCmd(0xDA);
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0x81);
	OLED_WriteCmd(0xCF);
	OLED_WriteCmd(0xD9);
	OLED_WriteCmd(0xF1);
	OLED_WriteCmd(0xDB);
	OLED_WriteCmd(0x40);
	OLED_WriteCmd(0xA4);
	OLED_WriteCmd(0xA6);
	OLED_WriteCmd(0x2E);
	OLED_WriteCmd(0xAF);
}



void OLED_Clear (void)
{
	// Clear the display buffer
	
	for (uint16_t i = 0; i < 1024; i++) OLED_Buffer[i] = 0x00;
}


void OLED_Update (void)
{
	// Transfer the display buffer to the OLED
	
	for (uint8_t i = 0; i < 8; i++)
	{
		OLED_WriteCmd(0xB0 + i);
		OLED_WriteCmd(0x00);
		OLED_WriteCmd(0x10);
		
		for (uint16_t j = 0; j < 128; j++)
		{
			OLED_WriteData(OLED_Buffer[i * 128 + j]);
		}
	}
}


void OLED_SetChar (uint8_t row, uint8_t col, uint8_t sym)
{
	// Draw a character in the display buffer
	
	if (row > 3) row = 0;
	if (col > 15) col = 0;
	
	row *= 2;
	col *= 8;
	
	char symbols[128] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789°% .:+-";
	uint16_t val;
	for (val = 0; val < 128; val++)
	{
		if (symbols[val] == sym) break;
		if (symbols[val] == '\0')
		{
			val = 64;
			break;
		}
	}
	
	uint16_t index = row * 128 + col;
	for (uint8_t i = 0; i < 8; i++)
	{
		OLED_Buffer[index + i] = FONT[val][i]; 
		OLED_Buffer[index + 128 + i] = FONT[val][8 + i]; 
	}
}


void OLED_Print (uint8_t row, const char *str)
{
	// Write a string to the display buffer
	
	if (row > 3) row = 0;
	if (str == 0) return;
	
	uint8_t col;
	for (col = 0; col < 16; col++)
	{
		if (str[col] == '\0') return;
		
		OLED_SetChar(row, col, str[col]);
	}
}	


void OLED_SetPage0 (MeteoData* meteo)
{
	// Draw the main weather data page

	uint8_t h = meteo->time / 3600;
	uint8_t m = (meteo->time % 3600) / 60;
	uint8_t s = meteo->time % 60;
	
	OLED_SetChar(0, 1, '0' + (h / 10));
	OLED_SetChar(0, 2, '0' + (h % 10));
	OLED_SetChar(0, 4, '0' + (m / 10));
	OLED_SetChar(0, 5, '0' + (m % 10));
	OLED_SetChar(0, 7, '0' + (s / 10));
	OLED_SetChar(0, 8, '0' + (s % 10));
	
	int32_t temp = meteo->temp;
	if (temp < 0) 
	{
		OLED_SetChar(1, 7, '-');
		temp *= (-1);
	}
	else 
	{
		OLED_SetChar(1, 7, '+');
	}
		
	uint16_t integ = temp / 100;
	uint16_t fract = temp % 100;	
	
	OLED_SetChar(1, 8, '0' + (integ / 10));
	OLED_SetChar(1, 9, '0' + (integ % 10));
	OLED_SetChar(1, 10, '.');
	OLED_SetChar(1, 11, '0' + (fract / 10));
	OLED_SetChar(1, 12, '0' + (fract % 10));

	if (meteo->press < 100000) OLED_SetChar(2, 7, ' ');
	else OLED_SetChar(2, 7, '0' + (meteo->press / 100000));
	OLED_SetChar(2, 8, '0' + ((meteo->press / 10000) % 10));
	OLED_SetChar(2, 9, '0' + ((meteo->press / 1000) % 10));
	OLED_SetChar(2, 10, '0' + ((meteo->press / 100) % 10));
	OLED_SetChar(2, 11, '0' + ((meteo->press / 10) % 10));
	OLED_SetChar(2, 12, '0' + (meteo->press % 10));
	
	integ = meteo->humid / 100;
	fract = meteo->humid % 100;

	OLED_SetChar(3, 7, '0' + (integ / 10));
	OLED_SetChar(3, 8, '0' + (integ % 10));
	OLED_SetChar(3, 9, '.');
	OLED_SetChar(3, 10, '0' + (fract / 10));
	OLED_SetChar(3, 11, '0' + (fract % 10));
}

void OLED_SetPage1 (MeteoData* meteo)
{
	// Draw the additional weather data page
	
	OLED_SetChar(1, 10, '0' + (meteo->uv / 10));
	OLED_SetChar(1, 11, '0' + (meteo->uv % 10));
	
	OLED_SetChar(2, 11, '0' + (meteo->zmbr / 10));
	OLED_SetChar(2, 12, '0' + (meteo->zmbr % 10));
	
	
}
