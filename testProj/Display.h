#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"

void DisplayTemperatureSetpoint(unsigned char setpoint);
void DisplayFanSpeedMode(unsigned char fanSpeedMode);
void DisplayRealTemperature(short int temperature);
void DisplayFanValveState(unsigned char fanState, bool valveState);
#endif
