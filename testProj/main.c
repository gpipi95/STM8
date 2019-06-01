/*----------------------------------------------------------
 *  test
 *--------------------------------------------------------*/
#include "../TM1638/TM1638.h"
#include "../core/STM8S103K3T6C.h"
#include "../core/delay.h"

#define Test_TM1638

void DelayMs(unsigned int time);
int main()
{
#ifdef Test_TM1638
    unsigned char keyValue, i;
    unsigned char displayData[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
#endif
    PD_DDR = 0x08;
    PD_CR1 = 0x08; // push-pull
    PD_CR2 = 0x00;
    CLK_SWR = 0xE1;    // HSI selected as master clock source (reset value),16 MHz
    CLK_CKDIVR = 0x08; // fHSI= fHSI RC output/2 = 8MHz
    TM1638Init();
#ifdef Test_TM1638
    TM1638EightSymbolDisplay(displayData);
#endif
    while (1) {
#ifndef Test_TM1638
        PD_ODR = PD_ODR | 0x08; // PD3 output to 1
        delay_ms(2000);         // delay 100MS
        PD_ODR = PD_ODR & 0xF7; // PD3 output 0
        delay_ms(1000);         // 100MS
#endif

#ifdef Test_TM1638
        TM1638Readkey(&keyValue);
        if (keyValue != 0xFF) {
            for (i = 0; i < 8; ++i) {
                displayData[i] = 0;
            }
            if (keyValue < 10) {
                TM1638OneSymbolDisplay(0, keyValue);
                TM1638OneSymbolDisplay(1, 0);
            } else if (keyValue < 20) {
                TM1638OneSymbolDisplay(0, keyValue - 10);
                TM1638OneSymbolDisplay(1, 1);
            }
        }
#endif
    }
}
