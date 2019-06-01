#include "DS18B20.h"
#include "../core/STM8S103K3T6C.h"
#include "../core/delay.h"

void DS18B20Init(void)
{
    PF_DDR |= 0x10; // PF4 set to output
    PF_CR1 |= 0x10; // push-pull / pull up mode
    PF_CR2 &= 0xEF;
}
