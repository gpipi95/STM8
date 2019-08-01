#ifndef _TM1638_H
#define _TM1638_H
#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s_gpio.h"
#include "../core/utility.h"

#define TM1638_PORT GPIOC

#define DIO_PIN GPIO_PIN_1
#define CLK_PIN GPIO_PIN_2
#define STB_PIN GPIO_PIN_3

// if no valid key return value is 0xFF
void TM1638Readkey(unsigned char* keyValue); /* read key values, 0xFF stands for invalid key */
void TM1638Init(void);
// ledName start from 0 to 7
void TM1638OneSymbolDisplay(unsigned char ledName, unsigned char symbol); /* display one led */
void TM1638EightSymbolDisplay(unsigned char* symbols);                    /* the content at the pointer must have 8 byte */

#endif
