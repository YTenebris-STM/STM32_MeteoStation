#include "bme280.h"
#include "i2c.h"
#include "delay.h"

typedef struct 
{
	uint16_t dig_T1;
	int16_t  dig_T2;
	int16_t  dig_T3;

	uint16_t dig_P1;
	int16_t  dig_P2;
	int16_t  dig_P3;
	int16_t  dig_P4;
	int16_t  dig_P5;
	int16_t  dig_P6;
	int16_t  dig_P7;
	int16_t  dig_P8;
	int16_t  dig_P9;

	uint8_t  dig_H1;
	int16_t  dig_H2;
	uint8_t  dig_H3;
	int16_t  dig_H4;
	int16_t  dig_H5;
	int8_t   dig_H6;
} BME280_Calib;

static BME280_Calib calib;

void BME280_Init (void)
{
	// Read factory calibration data
	
	calib.dig_T1 = (I2C_Read(0x76, 0x89) << 8) | I2C_Read(0x76, 0x88);
	calib.dig_T2 = (I2C_Read(0x76, 0x8B) << 8) | I2C_Read(0x76, 0x8A);
	calib.dig_T3 = (I2C_Read(0x76, 0x8D) << 8) | I2C_Read(0x76, 0x8C);

	calib.dig_P1 = (I2C_Read(0x76, 0x8F) << 8) | I2C_Read(0x76, 0x8E);
	calib.dig_P2 = (I2C_Read(0x76, 0x91) << 8) | I2C_Read(0x76, 0x90);
	calib.dig_P3 = (I2C_Read(0x76, 0x93) << 8) | I2C_Read(0x76, 0x92);	
	calib.dig_P4 = (I2C_Read(0x76, 0x95) << 8) | I2C_Read(0x76, 0x94);
	calib.dig_P5 = (I2C_Read(0x76, 0x97) << 8) | I2C_Read(0x76, 0x96);
	calib.dig_P6 = (I2C_Read(0x76, 0x99) << 8) | I2C_Read(0x76, 0x98);
	calib.dig_P7 = (I2C_Read(0x76, 0x9B) << 8) | I2C_Read(0x76, 0x9A);
	calib.dig_P8 = (I2C_Read(0x76, 0x9D) << 8) | I2C_Read(0x76, 0x9C);
	calib.dig_P9 = (I2C_Read(0x76, 0x9F) << 8) | I2C_Read(0x76, 0x9E);

	calib.dig_H1 = I2C_Read(0x76, 0xA1);
	calib.dig_H2 = (I2C_Read(0x76, 0xE2) << 8) | I2C_Read(0x76, 0xE1);
	calib.dig_H3 = I2C_Read(0x76, 0xE3);
	calib.dig_H4 = (I2C_Read(0x76, 0xE4) << 4) | (I2C_Read(0x76, 0xE5) & 0x0F);
	calib.dig_H5 = (I2C_Read(0x76, 0xE6) << 4) | (I2C_Read(0x76, 0xE5) >> 4);
	calib.dig_H6 = (int8_t)I2C_Read(0x76, 0xE7);
	
	// Configure BME280 for forced-mode measurements

	I2C_Write(0x76, 0xF2, 0x05);
	I2C_Write(0x76, 0xF4, 0xB7);
	
	I2C_Write(0x76, 0xF5, 0xA0);
	delay(100);

}

void BME280_GetRawData (int32_t* temp, int32_t* press, int32_t* humid)
{
	// Read raw sensor data
	
	uint8_t data[8];
	for (uint8_t i = 0; i < 8; i++) 
	{
		data[i] = I2C_Read(0x76, 0xF7 + i);
	}

	*press = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | (data[2] >> 4);
	*temp = ((uint32_t)data[3] << 12) | ((uint32_t)data[4] << 4) | (data[5] >> 4);
	*humid = ((uint32_t)data[6] << 8) | data[7];
}

int32_t BME280_GetTemp (int32_t raw, int32_t* temp)
{
	// Compensate raw temperature data using factory calibration
	
	int32_t var1 = ((((raw >> 3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
	int32_t var2 = (((((raw >> 4) - ((int32_t)calib.dig_T1)) * ((raw >> 4) - 
								 ((int32_t)calib.dig_T1))) >> 12) * ((int32_t)calib.dig_T3)) >> 14;
	int32_t t_fine = var1 + var2;

	*temp = (t_fine * 5 + 128) >> 8;
	
	return t_fine;
}

void BME280_GetPress (int32_t raw, int32_t t_fine, uint32_t* press)
{
	// Compensate raw pressure data using factory calibration
	
	int64_t var1, var2, p;
	
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calib.dig_P6;
	var2 = var2 + ((var1 * (int64_t)calib.dig_P5) << 17);
	var2 = var2 + (((int64_t)calib.dig_P4) << 35);

	var1 = ((var1 * var1 * (int64_t)calib.dig_P3) >> 8) + ((var1 * (int64_t)calib.dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1) * ((int64_t)calib.dig_P1)) >> 33;

	if (var1 == 0) return;

	p = 1048576 - raw;
	p = (((p << 31) - var2) * 3125) / var1;

	var1 = (((int64_t)calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)calib.dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)calib.dig_P7) << 4);
	
	*press = p / 256;
}

void BME280_GetHumid (int32_t hum_raw, int32_t t_fine, uint32_t* humid)
{
	// Compensate raw humidity data using factory calibration
	
	int32_t var1 = t_fine - ((int32_t)76800);
	var1 = (((((hum_raw << 14) - (((int32_t)calib.dig_H4) << 20) - (((int32_t)calib.dig_H5) * var1)) + 
					((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)calib.dig_H6)) >> 10) * 
					(((var1 * ((int32_t)calib.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + 
					((int32_t)2097152)) * ((int32_t)calib.dig_H2) + 8192) >> 14));
	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)calib.dig_H1)) >> 4));
	var1 = (var1 < 0 ? 0 : var1);
	var1 = (var1 > 419430400 ? 419430400 : var1);
	
	*humid = ((uint32_t)(var1 >> 12) * 100) / 1024;
}

void BME280_GetData (int32_t* temp, uint32_t* press, uint32_t* humid)
{
	// Read sensor data and update the measurement structure
	
	int32_t raw_temp, raw_press, raw_humid;
	BME280_GetRawData(&raw_temp, &raw_press, &raw_humid);
	
	int32_t t_fine = BME280_GetTemp(raw_temp, temp);
	BME280_GetPress(raw_press, t_fine, press);
	BME280_GetHumid(raw_humid, t_fine, humid);
}