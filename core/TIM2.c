#include "STM8S103K3T6C.h"

void TIM2Init()
{
    TIM2_IER  = 0x00; // 禁止中断
    TIM2_EGR  = 0x01; // 允许产生更新事件
    TIM2_PSCR = 0x01; // 计数器时钟=主时钟/128=2MHZ/2
                      // 相当于计数器周期为1uS
                      // 设定重装载时的寄存器值
                      // 注意必须保证先写入高8位，再写入低8位
    TIM2_ARRH = 0xEA; // 设定重装载时的寄存器的高8位
    TIM2_ARRL = 0x60;

    TIM2_CNTRH = 0xEA; // 设定计数器的初值
    TIM2_CNTRL = 0x60;
    // 定时周期=1*60000=60000uS=60ms

    TIM2_CR1 = 0x01; // b0 = 1,允许计数器工作
                     // b1 = 0,允许更新
                     // 设置控制器，启动定时器
    TIM2_IER = 0x01; // 允许更新中断
}
