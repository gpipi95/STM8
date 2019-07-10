#include "DS18B20.h"
//#include "../core/STM8S103K3T6C.h"
#include "../core/delay.h"
#include "owire.h"

unsigned char DS18B20_SCRTPD[9];
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
unsigned char DS18B20ReadSCRTPD()
{
    unsigned char i = 0;
    if (W1Init()) { // found the device
        W1WriteByte(DS18B20_SKIP_ROM);
        W1WriteByte(DS18B20_READ_SCRTPD);
        for (i = 0; i < 9; ++i) {
            *(DS18B20_SCRTPD + i) = W1ReadByte();
        }
    }
    return *(DS18B20_SCRTPD + 8) == W1DowCRC8(DS18B20_SCRTPD, 8);
}

bool DS18B20GetTemp(short int* temp)
{
    unsigned char degree, belowDegree;
    degree      = 0;
    belowDegree = 0;
    if (DS18B20ReadSCRTPD()) {
        degree      = ((*(DS18B20_SCRTPD + 1)) << 4) & 0x7F;
        degree      = degree + (((*DS18B20_SCRTPD) >> 4) & 0x0F);
        belowDegree = ((*DS18B20_SCRTPD) & 0x0F) * 10 / 16;
        *temp       = -1 * (((*(DS18B20_SCRTPD + 1)) & 0x08) >> 3) + degree * 10 + belowDegree;
        return TRUE;
    }
    return FALSE;
}

