#ifndef _PARAM_H
#define _PARAM_H
#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"

#define MAX_TEMPERATURE 30 // High limit temprature
#define MIN_TEMPERATURE 16 // Low limit temprature

bool          WritePID(float kp, float ki, float kd);      // 3*4=12 byte
bool          WriteSetTemperature(unsigned char setpoint); // 1*1=4 byte
bool          ReadPID(float* kp, float* ki, float* kd);
unsigned char ReadSetTemperature(void);
void          ParamInit(void);

#endif
