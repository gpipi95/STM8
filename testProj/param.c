#include "param.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"

#define START_OF_PID 0x4000
#define START_OF_TEMPERATURE 0x400C

void _startReadWrite(void)
{
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
}

bool _writeFloat(float data, uint16_t address)
{
    unsigned char* unsignedChar_ptr = (unsigned char*)&data;
    unsigned char  i, value;
    bool           result = TRUE;

    _startReadWrite();
    for (i = 0; i < sizeof(float); ++i) {
        FLASH_ProgramByte((address + i), *(unsignedChar_ptr + i));
        value  = FLASH_ReadByte(address + i);
        result = result && (value == *(unsignedChar_ptr + i));
    }
    return result;
}

float _readFloat(uint16_t address)
{
    float          data;
    unsigned char  i;
    unsigned char* unsignedChar_ptr = (unsigned char*)&data;
    _startReadWrite();

    for (i = 0; i < sizeof(float); ++i) {
        *(unsignedChar_ptr + i) = FLASH_ReadByte(address + i);
    }
    return data;
}

bool WritePID(float kp, float ki, float kd)
{
    bool result = _writeFloat(kp, START_OF_PID);
    result      = result && _writeFloat(ki, START_OF_PID + sizeof(float));
    result      = result && _writeFloat(kd, START_OF_PID + 2 * sizeof(float));
    return result;
}

void ReadPID(float* kp, float* ki, float* kd)
{
    *kp = _readFloat(START_OF_PID);
    *ki = _readFloat(START_OF_PID + sizeof(float));
    *kd = _readFloat(START_OF_PID + 2 * sizeof(float));
}

unsigned char ReadTemperatureSetpoint(void)
{
    unsigned char value;
    _startReadWrite();
    value = FLASH_ReadByte(START_OF_TEMPERATURE);
    // value valid check
    if (value > MAX_TEMPERATURE) {
        value = MAX_TEMPERATURE;
    } else if (value < MIN_TEMPERATURE) {
        value = MIN_TEMPERATURE;
    }
    return value;
}

bool WriteTemperatureSetpoint(unsigned char setpoint)
{
    unsigned char value;
    _startReadWrite();
    // value valid check
    if (setpoint > MAX_TEMPERATURE) {
        setpoint = MAX_TEMPERATURE;
    } else if (setpoint < MIN_TEMPERATURE) {
        setpoint = MIN_TEMPERATURE;
    }

    FLASH_ProgramByte(START_OF_TEMPERATURE, setpoint);
    value = FLASH_ReadByte(START_OF_TEMPERATURE);
    return value == setpoint;
}
