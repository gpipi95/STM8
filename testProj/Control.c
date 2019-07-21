#include "Control.h"
#include "Display.h"
#include "param.h"

static unsigned char FanSpeedMode = FAN_SPEED_AUTO; // fan speed mode automatic

void IncreaseTemperatureSetpoint(char step)
{
    WriteTemperatureSetpoint(ReadTemperatureSetpoint() + step);
    DisplayTemperatureSetpoint(ReadTemperatureSetpoint());
}

void ChangeFanMode(void)
{
    switch (FanSpeedMode) {
    case FAN_SPEED_AUTO:
        FanSpeedMode = FAN_SPEED_LOW;
        break;
    case FAN_SPEED_LOW:
        FanSpeedMode = FAN_SPEED_MIDDLE;
        break;
    case FAN_SPEED_MIDDLE:
        FanSpeedMode = FAN_SPEED_HIGH;
        break;
    case FAN_SPEED_HIGH:
        FanSpeedMode = FAN_SPEED_AUTO;
        break;
    default:
        break;
    }
    DisplayFanSpeedMode(FanSpeedMode);
}

unsigned char GetFanSpeedMode(void)
{
    return FanSpeedMode;
}
