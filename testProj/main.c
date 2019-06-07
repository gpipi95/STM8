/*----------------------------------------------------------
 *  test
 *--------------------------------------------------------*/
#include "../TM1638/TM1638.h"
#include "../core/STM8S103K3T6C.h"
#include "../core/TIM2.h"
#include "../core/delay.h"
#include "helper.h"

int main()
{
    PD_DDR |= 0x01 << 3;
    PD_CR1 |= 0x01 << 3; // push-pull
    PD_CR2 = 0x00;

    CLK_SWR    = 0xE1; // HSI selected as master clock source (reset value),16 MHz
    CLK_CKDIVR = 0x08; // fHSI= fHSI RC output/2 = 8MHz

    _asm("sim"); //先关闭总中断
    TIM2Init();
    TM1638Init();
    _asm("rim"); //打开总中断
    while (1) {
        BlinkPD3LedTask();
        ReadKeyboardTask();
        GetDisplayTempTask();
    }
}
