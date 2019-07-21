#ifndef _CONTROL_H
#define _CONTROL_H

#define FAN_SPEED_AUTO 0
#define FAN_SPEED_LOW 1
#define FAN_SPEED_MIDDLE 2
#define FAN_SPEED_HIGH 3

#define HIGH_THRESHOLD 10       // unit is degree, difference is bigger than this turn fan speed to high
#define LOW_THRESHOLD 2         // difference below this turn to low fan speed
#define CLOSE_FAN_THRESHOLD 0.5 // below this turn off fan

void          IncreaseTemperatureSetpoint(char step);
void          ChangeFanMode(void);
unsigned char GetFanSpeedMode(void);
#endif
