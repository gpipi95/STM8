/*----------------------------------------------------------
 *  test
 *--------------------------------------------------------*/
#include "../DS18B20/DS18B20.h"
#include "../DS18B20/owire.h"
#include "../TM1638/TM1638.h"
#include "../core/STM8S103K3T6C.h"
#include "../core/TIM2.h"
#include "../core/delay.h"

#define Test_TM1638
int main()
{
#ifdef Test_TM1638
    unsigned char keyValue;
#endif
    short int     temperature;
    unsigned char temp;
#ifndef Test_TM1638
    PD_DDR |= 0x01 << 3;
    PD_CR1 |= 0x01 << 3; // push-pull
    PD_CR2 = 0x00;
#endif
    CLK_SWR    = 0xE1; // HSI selected as master clock source (reset value),16 MHz
    CLK_CKDIVR = 0x08; // fHSI= fHSI RC output/2 = 8MHz

    _asm("sim"); //先关闭总中断
    TIM2Init();
    TM1638Init();
    _asm("rim"); //打开总中断
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
            if (keyValue < 10) {
                switch (keyValue) {
                case 1:
                    TM1638OneSymbolDisplay(1, DS18B20ConvertTemp());
                    TM1638OneSymbolDisplay(0, 1);
                    break;
                case 2:
                    temperature = DS18B20GetTemp();

                    if (temperature < 0) {
                        temp = 38; // display minus flag
                    } else if (CAST_UC((temperature / 1000) % 10) == 0) {
                        temp = 40; // turn off led
                    } else {
                        temp = CAST_UC((temperature / 1000) % 10);
                    }
                    TM1638OneSymbolDisplay(7, temp);
                    TM1638OneSymbolDisplay(6, CAST_UC((temperature / 100) % 10));
                    TM1638OneSymbolDisplay(5, CAST_UC((temperature / 10) % 10) + 16);
                    TM1638OneSymbolDisplay(4, CAST_UC(temperature % 10));

                    TM1638OneSymbolDisplay(0, 2);
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
