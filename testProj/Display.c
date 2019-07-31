#include "Display.h"
#include "../TM1638/TM1638.h"

void DisplayTemperatureSetpoint(unsigned char setpoint)
{
    unsigned char ten = setpoint / 10;
    if (ten == 0) {
        TM1638OneSymbolDisplay(1, 40);
    } else {
        TM1638OneSymbolDisplay(1, setpoint / 10);
    }
    TM1638OneSymbolDisplay(0, setpoint % 10);
}

void DisplayFanSpeedMode(unsigned char fanSpeedMode)
{
    TM1638OneSymbolDisplay(2, fanSpeedMode);
}
void DisplayRealTemperature(short int temperature)
{
    unsigned char temp;
    if (temperature < 0) {
        temp = 38; // display minus flag
    } else if (CAST_UC((temperature / 1000) % 10) == 0) {
        temp = 40; // turn off led
    } else {
        temp = CAST_UC((temperature / 1000) % 10);
    }
    TM1638OneSymbolDisplay(7, temp);
    if (temp == 40) {
        if (CAST_UC((temperature / 100) % 10) != 0) {
            temp = CAST_UC((temperature / 100) % 10);
        }
    }
    TM1638OneSymbolDisplay(6, temp);
    TM1638OneSymbolDisplay(5, CAST_UC((temperature / 10) % 10) + 16);
    TM1638OneSymbolDisplay(4, CAST_UC(temperature % 10));
}

void DisplayFanValveState(unsigned char fanState, bool valveState)
{
    unsigned char offset = 0;
    if (valveState) {
        offset = 16;
    }
    TM1638OneSymbolDisplay(3, fanState + offset);
}
