#ifndef _TM1638_H
#define _TM1638_H
//#include "reg51.h"                            /* 51 chip set */
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
#define wdp0 (segA + segB + segC + segD + segE + segF + segDP)
#define wdp1 (segB + segC + segDP)
#define wdp2 (segA + segB + segG + segE + segD + segDP)
#define wdp3 (segA + segB + segG + segC + segD + segDP)
#define wdp4 (segF + segG + segB + segC + segDP)
#define wdp5 (segA + segF + segG + segC + segD + segDP)
#define wdp6 (segA + segF + segG + segC + segD + segE + segDP)
#define wdp7 (segA + segB + segC + segDP)
#define wdp8 (segA + segB + segC + segD + segE + segF + segG + segDP)
#define wdp9 (segA + segB + segC + segD + segF + segG + segDP)
#define wdpA (segA + segB + segC + segE + segF + segG + segDP)
#define wdpB (segC + segD + segE + segF + segG + segDP)
#define wdpC (segA + segD + segE + segF + segDP)
#define wdpD (segB + segC + segD + segE + segG + segDP)
#define wdpE (segA + segD + segE + segF + segG + segDP)
#define wdpF (segA + segE + segF + segG + segDP)

//引脚定义, 51chip set
//sbit DIO = P1 ^ 0;
//sbit CLK = P1 ^ 1;
//sbit STB = P1 ^ 2;
// todo: change the following register settings
inline void          DIO_Set() { PD_ODR = PD_ODR | 0x08; }
inline void          DIO_Clear() { PD_ODR = PD_ODR | 0x08; }
inline void          CLK_Set() { PD_ODR = PD_ODR | 0x08; }
inline void          CLK_Clear() { PD_ODR = PD_ODR | 0x08; }
inline void          STB_Set() { PD_ODR = PD_ODR | 0x08; }
inline void          STB_Clear() { PD_ODR = PD_ODR | 0x08; }
inline unsigned char DIO_Status() { return PD_ODR & 0x08; }

void          TM1638_Write(unsigned char DATA); //写数据函数
unsigned char TM1638_Read(void);
unsigned char Read_key(void);
void          Write_COM(unsigned char cmd);
void          Write_DATA(unsigned char add, unsigned char DATA);
void          init_TM1638(void);

#endif
