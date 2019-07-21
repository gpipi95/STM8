/*----------------------------------------------------------
 *  test
 *--------------------------------------------------------*/
//#include "../PID/PID_v1.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s_clk.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s_gpio.h"
#include "../TM1638/TM1638.h"
#include "../core/PrintfUtility.h"
#include "../core/TIM2.h"
#include "../core/delay.h"
#include "Control.h"
#include "Display.h"
#include "param.h"
#include "task.h"
#include <stdio.h>

void main(void)
{
    GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_3), GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3), GPIO_MODE_OUT_PP_LOW_SLOW);
    // HSI selected as master clock source (reset value),16 MHz
    // fHSI= fHSI RC output/2 = 8MHz, Warning: not like this!!!
    CLK_DeInit();
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    _asm("sim"); //先关闭总中断
    TIM2Init();
    TM1638Init();
    Printf_Init();
    _asm("rim"); //打开总中断
    DisplayTemperatureSetpoint(ReadTemperatureSetpoint());
    DisplayFanSpeedMode(GetFanSpeedMode());
    while (1) {
        BlinkPD3LedTask();
        ReadKeyboardTask();
        TemperatureTask();
    }
}
//putchar copies c to the user specified output stream.
//You must rewrite putchar in either C or assembly language to provide
//an interface to the output mechanism to the C library.
//Return Value
//putchar returns c. If a write error occurs, putchar returns EOF.
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1) {
    }
}
#endif

