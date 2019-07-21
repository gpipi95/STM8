#include "Display.h"
#include "../TM1638/TM1638.h"

void DisplayTemperatureSetpoint(unsigned char setpoint)
{
    TM1638OneSymbolDisplay(1, setpoint / 10);
    TM1638OneSymbolDisplay(0, setpoint % 10);
}

void DisplayFanSpeedMode(unsigned char fanSpeedMode)
{
    TM1638OneSymbolDisplay(2, fanSpeedMode);
}
