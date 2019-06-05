/*----------------------------------------------------------
 *  test
 *--------------------------------------------------------*/
#include "../DS18B20/DS18B20.h"
#include "../DS18B20/owire.h"
#include "../TM1638/TM1638.h"
#include "../core/STM8S103K3T6C.h"
#include "../core/delay.h"

#define Test_TM1638
int main()
{
#ifdef Test_TM1638
    unsigned char keyValue, i;
    unsigned char displayData[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
#endif
    unsigned char temp[9]; // temperature

#ifndef Test_TM1638
    PD_DDR = 0x10;
    PD_CR1 = 0x00; // push-pull
    PD_CR2 = 0x00;
#endif
    CLK_SWR    = 0xE1; // HSI selected as master clock source (reset value),16 MHz
    CLK_CKDIVR = 0x08; // fHSI= fHSI RC output/2 = 8MHz
    TM1638Init();
#ifdef Test_TM1638
    TM1638EightSymbolDisplay(displayData);
#endif
    while (1) {
#ifndef Test_TM1638
        PF_ODR = PF_ODR | 0x10; // PD3 output to 1
        delay_us(10);           // delay 100MS
        PF_ODR = PF_ODR & 0xEF; // PD3 output 0
        delay_us(10);           // 100MS
#endif

#ifdef Test_TM1638
        TM1638Readkey(&keyValue);
        if (keyValue != 0xFF) {
            for (i = 0; i < 8; ++i) {
                displayData[i] = 0;
            }
            if (keyValue < 10) {
                //                TM1638OneSymbolDisplay(0, keyValue);
                //                TM1638OneSymbolDisplay(1, 0);
                switch (keyValue) {
                case 1:
                    DS18B20Init();
                    TM1638OneSymbolDisplay(2, 1);
                    break;
                case 2:
                    TM1638OneSymbolDisplay(3, DS18B20ConvertTemp());
                    TM1638OneSymbolDisplay(2, 2);
                    break;
                case 3:
                    TM1638OneSymbolDisplay(3, DS18B20ReadTemp(temp));

                    TM1638OneSymbolDisplay(7, (*(temp + 1)) & 0x07);
                    TM1638OneSymbolDisplay(6, (*temp) >> 4);
                    TM1638OneSymbolDisplay(5, (*temp) & 0x0F);
                    TM1638OneSymbolDisplay(4, (*(temp + 1)) >> 3);

                    if (*(temp + 8) == W1DowCRC8(temp, 8)) {
                        TM1638OneSymbolDisplay(1, 1);
                    } else {
                        TM1638OneSymbolDisplay(1, 0);
                    }
                    TM1638OneSymbolDisplay(2, 3);
                    break;
                default:
                    TM1638OneSymbolDisplay(2, 0);
                    break;
                }
            } else if (keyValue < 20) {
                TM1638OneSymbolDisplay(0, keyValue - 10);
                TM1638OneSymbolDisplay(1, 1);
            }
        }
#endif
    }
}
