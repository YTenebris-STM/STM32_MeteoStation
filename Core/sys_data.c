#include "sys_data.h"

MeteoData meteo;
TimeData time;
SystemData sys = 
{
	.time = &time,
	.meteo = &meteo
};
