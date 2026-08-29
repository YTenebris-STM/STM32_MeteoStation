#ifndef TASKS_H
#define TASKS_H

#include "stm32f10x.h"

void Task_Display (void);
void Task_BME280 (void);
void Task_UVI (void);
void Task_RTC (void);
void Task_Zambretti (void);
void Task_LED (void);

#endif