#include "Control.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s_gpio.h"
#include "../core/PrintfUtility.h"
#include "Display.h"
#include "param.h"

unsigned char FanSpeedMode  = FAN_SPEED_AUTO_MODE; // default fan speed mode is automatic
unsigned char FanSpeedState = FAN_SPEED_CLOSED;    // default fan speed state is close
bool          ValveState    = FALSE;               // default Valve state is closed

void IncreaseTemperatureSetpoint(char step)
{
    WriteTemperatureSetpoint(ReadTemperatureSetpoint() + step);
}

void ChangeFanMode(void)
{
    switch (FanSpeedMode) {
    case FAN_SPEED_AUTO_MODE:
        FanSpeedMode = FAN_SPEED_LOW_MODE;
        ChangeFanOutput(FAN_SPEED_LOW);
        break;
    case FAN_SPEED_LOW_MODE:
        FanSpeedMode = FAN_SPEED_MIDDLE_MODE;
        ChangeFanOutput(FAN_SPEED_MIDDLE);
        break;
    case FAN_SPEED_MIDDLE_MODE:
        FanSpeedMode = FAN_SPEED_HIGH_MODE;
        ChangeFanOutput(FAN_SPEED_HIGH);
        break;
    case FAN_SPEED_HIGH_MODE:
        FanSpeedMode = FAN_SPEED_AUTO_MODE;
        break;
    default:
        break;
    }
}

unsigned char GetFanSpeedMode(void)
{
    return FanSpeedMode;
}

void InitFanControl(void)
{
    // set all output mode to push-pull low speed
    GPIO_Init(CONTROL_PORT, (GPIO_Pin_TypeDef)(FAN_HIGH_IO | FAN_MIDDLE_IO | FAN_LOW_IO | VALVE_OPEN_IO | VALVE_CLOSE_IO), GPIO_MODE_OUT_PP_HIGH_SLOW);
}

bool ChangeFanOutput(unsigned char state)
{
    if (state != FanSpeedState) {
        switch (state) {
        case FAN_SPEED_LOW:
            FanSpeedState = FAN_SPEED_LOW;
            GPIO_WriteHigh(CONTROL_PORT, FAN_LOW_IO);
            GPIO_WriteLow(CONTROL_PORT, (GPIO_Pin_TypeDef)(FAN_HIGH_IO | FAN_MIDDLE_IO));
            break;
        case FAN_SPEED_MIDDLE:
            FanSpeedState = FAN_SPEED_MIDDLE;
            GPIO_WriteHigh(CONTROL_PORT, FAN_MIDDLE_IO);
            GPIO_WriteLow(CONTROL_PORT, (GPIO_Pin_TypeDef)(FAN_HIGH_IO | FAN_LOW_IO));
            break;
        case FAN_SPEED_HIGH:
            FanSpeedState = FAN_SPEED_HIGH;
            GPIO_WriteHigh(CONTROL_PORT, FAN_HIGH_IO);
            GPIO_WriteLow(CONTROL_PORT, (GPIO_Pin_TypeDef)(FAN_LOW_IO | FAN_MIDDLE_IO));
            break;
        default:
            FanSpeedState = FAN_SPEED_CLOSED;
            GPIO_WriteLow(CONTROL_PORT, (GPIO_Pin_TypeDef)(FAN_HIGH_IO | FAN_MIDDLE_IO | FAN_LOW_IO));
            break;
        }
        return TRUE;
    }
    return FALSE;
}

void ChangeValveOutput(bool state)
{
    if (state) {
        ValveState = TRUE;
        GPIO_WriteHigh(CONTROL_PORT, VALVE_OPEN_IO);
        GPIO_WriteLow(CONTROL_PORT, VALVE_CLOSE_IO);
    } else {
        ValveState = FALSE;
        GPIO_WriteHigh(CONTROL_PORT, VALVE_CLOSE_IO);
        GPIO_WriteLow(CONTROL_PORT, VALVE_OPEN_IO);
    }
}

bool GetValveState(void)
{
    return ValveState;
}

unsigned char GetFanSpeedState(void)
{
    return FanSpeedState;
}

bool ControlFanSpeed(short int temperature)
{
    bool result = FALSE;
    if (FanSpeedMode == FAN_SPEED_AUTO_MODE) {
        float diff = (float)ReadTemperatureSetpoint() - (float)temperature / 10;

        diff = diff > 0 ? diff : -(diff);

        if (diff < LOW_THRESHOLD) {
            result = ChangeFanOutput(FAN_SPEED_LOW);
        } else if (diff < MIDDLE_THRESHOLD) {
            result = ChangeFanOutput(FAN_SPEED_MIDDLE);
        } else {
            result = ChangeFanOutput(FAN_SPEED_HIGH);
        }
    }
    return result;
}

