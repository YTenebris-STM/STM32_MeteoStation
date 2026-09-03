#ifndef BME280_H
#define BME280_H

#include "sys_data.h"

void BME280_Init (void);
void BME280_GetData (SystemData* sys);

#endif