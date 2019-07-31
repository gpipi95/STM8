#ifndef _PARAM_H
#define _PARAM_H
#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"

#define MAX_TEMPERATURE 35 // High limit temprature
#define MIN_TEMPERATURE 5  // Low limit temprature

bool          WritePID(float kp, float ki, float kd);      // 3*4=12 byte
bool          WriteSetTemperature(unsigned char setpoint); // 1*1=4 byte
void          ReadPID(float* kp, float* ki, float* kd);
unsigned char ReadTemperatureSetpoint(void);
bool          WriteTemperatureSetpoint(unsigned char setpoint);

#endif
