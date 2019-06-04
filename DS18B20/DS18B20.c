#include "DS18B20.h"
#include "../core/STM8S103K3T6C.h"
#include "../core/delay.h"
#include "owire.h"

unsigned char DS18B20Init(void)
{
    return 1;
}
unsigned char DS18B20ConvertTemp(void)
{
    unsigned char result = 0;
    if (W1Init()) { // found the device
        W1WriteByte(DS18B20_SKIP_ROM);
        W1WriteByte(DS18B20_START_CONVERT);
        result = 1;
    }
    return result;
}
unsigned char DS18B20ReadTemp(unsigned char* temp)
{
    unsigned char i = 0;
    if (temp != 0 && W1Init()) { // found the device
        W1WriteByte(DS18B20_SKIP_ROM);
        W1WriteByte(DS18B20_READ_SCRTPD);
        for (i = 0; i < 9; ++i) {
            *(temp + i) = W1ReadByte();
        }
    }
    return i ? 0x01 : 0x00;
}
