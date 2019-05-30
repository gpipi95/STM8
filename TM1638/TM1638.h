#ifndef _TM1638_H
#define _TM1638_H
#include "../core/STM8S103K3T6C.h"

#define segA 0x40
#define segB 0x20
#define segC 0x10
#define segD 0x08
#define segE 0x04
#define segF 0x02
#define segG 0x01
#define segDP 0x80

#define ndp0 (segA + segB + segC + segD + segE + segF)
#define ndp1 (segB + segC)
#define ndp2 (segA + segB + segG + segE + segD)
#define ndp3 (segA + segB + segG + segC + segD)
#define ndp4 (segF + segG + segB + segC)
#define ndp5 (segA + segF + segG + segC + segD)
#define ndp6 (segA + segF + segG + segC + segD + segE)
#define ndp7 (segA + segB + segC)
#define ndp8 (segA + segB + segC + segD + segE + segF + segG)
#define ndp9 (segA + segB + segC + segD + segF + segG)
#define ndpA (segA + segB + segC + segE + segF + segG)
#define ndpB (segC + segD + segE + segF + segG)
#define ndpC (segA + segD + segE + segF)
#define ndpD (segB + segC + segD + segE + segG)
#define ndpE (segA + segD + segE + segF + segG)
#define ndpF (segA + segE + segF + segG)
#define wdp0 (ndp0 + segDP)
#define wdp1 (ndp1 + segDP)
#define wdp2 (ndp2 + segDP)
#define wdp3 (ndp3 + segDP)
#define wdp4 (ndp4 + segDP)
#define wdp5 (ndp5 + segDP)
#define wdp6 (ndp6 + segDP)
#define wdp7 (ndp7 + segDP)
#define wdp8 (ndp8 + segDP)
#define wdp9 (ndp9 + segDP)
#define wdpA (ndpA + segDP)
#define wdpB (ndpB + segDP)
#define wdpC (ndpC + segDP)
#define wdpD (ndpD + segDP)
#define wdpE (ndpE + segDP)
#define wdpF (ndpF + segDP)

//引脚定义, 51chip set
// todo: change the following register settings
#define DIO_Set() _asm("bset 0x5000, #1")                                             //  PA1
#define DIO_Clear() _asm("bres 0x5000, #1")                                           //  PA1
#define CLK_Set() _asm("bset 0x5000, #2")                                             //  PA2
#define CLK_Clear() _asm("bres 0x5000, #2")                                           //  PA2
#define STB_Set() _asm("bset 0x5000, #3")                                             //  PA3
#define STB_Clear() _asm("bres 0x5000, #3")                                           //  PA3
#define DIO_Input_Mode() _asm("bres 0x5002, #1\n bset 0x5003, #1\n bres 0x5004, #1")  // Set DIO to input without interrupt of pull up mode
#define DIO_Output_Mode() _asm("bset 0x5002, #1\n bset 0x5003, #1\n bres 0x5004, #1") // Set DIO to output without interrupt of pull up mode

static @ inline unsigned char DIO_Status(void) { return (PA_IDR & 0x02); }

void          TM1638_Write(unsigned char DATA); //写数据函数
unsigned char TM1638_Read(void);
unsigned char Read_key(void);
void          Write_COM(unsigned char cmd);
void          Write_DATA(unsigned char add, unsigned char DATA);
void          init_TM1638(void);

#endif
