#include "STM8S103K3T6C.h"

void TIM2Init()
{
    TIM2_IER = 0x00;  // Disable all interrup of TIM2
    TIM2_EGR = 0x01;  // Re-initializes the counter and generates an update of the registers. Note that the prescaler
                      //counter is also cleared.
    TIM2_PSCR = 0x07; // 计数器时钟=主时钟/128=8MHz/128=62.5kHz
                      // 相当于计数器周期为16us

    //16*300=4800us=4.8ms
    TIM2_ARRH = 0x01; // 注意必须保证先写入高8位，再写入低8位
    TIM2_ARRL = 0x2C; // 设定重装载时的寄存器的高8位

    TIM2_CNTRH = 0x01; // 设定计数器的初值
    TIM2_CNTRL = 0x2C;

    TIM2_CR1 = 0x01; // b0 = 1,允许计数器工作
                     // b1 = 0,允许更新
    TIM2_IER = 0x01; // Update interrupt enable
}
