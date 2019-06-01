#ifndef _TM1638_H
#define _TM1638_H
#include "../core/STM8S103K3T6C.h"
#include "../core/utility.h"

#define TM1638_PORT_ODR PA_ODR
#define TM1638_PORT_IDR PA_IDR
#define TM1638_PORT_DDR PA_DDR
#define TM1638_PORT_CR1 PA_CR1
#define TM1638_PORT_CR2 PA_CR2
#define DIO_PIN 1
#define CLK_PIN 2
#define STB_PIN 3

// if no valid key return value is 0xFF
void TM1638Readkey(unsigned char* keyValue); /* read key values, 0xFF stands for invalid key */
void TM1638Init(void);
// ledName start from 0 to 7
void TM1638OneSymbolDisplay(unsigned char ledName, unsigned char symbol); /* display one led */
void TM1638EightSymbolDisplay(unsigned char* symbols);                    /* the content at the pointer must have 8 byte */

#endif
