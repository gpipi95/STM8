/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

/*----------------------------------------------------------
 *  test
 *--------------------------------------------------------*/
#include "../TM1638/TM1638.h"
#include "../core/STM8S103K3T6C.h"

void DelayMs(unsigned int time);
int  main()
{
    PD_DDR = 0x08;
    PD_CR1 = 0x08; // push-pull
    PD_CR2 = 0x00;
    while (1) {
        PD_ODR = PD_ODR | 0x08; // PD3 output to 1
        DelayMs(1000);          // delay 100MS
        PD_ODR = PD_ODR & 0xF7; // PD3 output 0
        DelayMs(1000);          // 100MS
    }
}

void DelayMs(unsigned int time)
{
    unsigned char i;
    while (time != 0) {
        for (i = 0; i < 250; i++) {
        }
        for (i = 0; i < 75; i++) {
        }
        time--;
    }
}
