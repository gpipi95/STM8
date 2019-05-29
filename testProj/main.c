/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

/*----------------------------------------------------------
 *  test
 *--------------------------------------------------------*/
#include "../TM1638/TM1638.h"
#include "../core/STM8S103K3T6C.h"
#include "../core/delay.h"

void DelayMs(unsigned int time);
int  main()
{
    PD_DDR     = 0x08;
    PD_CR1     = 0x08; // push-pull
    PD_CR2     = 0x00;
    CLK_SWR    = 0xE1; // HSI selected as master clock source (reset value),16 MHz
    CLK_CKDIVR = 0x08; // fHSI= fHSI RC output/2 = 8MHz
    init_TM1638();
    while (1) {
        PD_ODR = PD_ODR | 0x08; // PD3 output to 1
        delay_ms(2000);         // delay 100MS
        PD_ODR = PD_ODR & 0xF7; // PD3 output 0
        delay_ms(1000);         // 100MS
    }
}
