#include "TM1638.h"
#include "../core/delay.h"

#define segG 0x40
#define segF 0x20
#define segE 0x10
#define segD 0x08
#define segC 0x04
#define segB 0x02
#define segA 0x01
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

#define DIO_Set() GPIO_WriteHigh(TM1638_PORT, DIO_PIN)  //  PA1
#define DIO_Clear() GPIO_WriteLow(TM1638_PORT, DIO_PIN) //  PA1
#define CLK_Set() GPIO_WriteHigh(TM1638_PORT, CLK_PIN)  //  PA2
#define CLK_Clear() GPIO_WriteLow(TM1638_PORT, CLK_PIN) //  PA2
#define STB_Set() GPIO_WriteHigh(TM1638_PORT, STB_PIN)  //  PA3
#define STB_Clear() GPIO_WriteLow(TM1638_PORT, STB_PIN) //  PA3

#define DIO_Input_Mode() GPIO_Init(TM1638_PORT, DIO_PIN, GPIO_MODE_IN_PU_NO_IT)       // Set DIO to input
#define DIO_Output_Mode() GPIO_Init(TM1638_PORT, DIO_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW) // Set DIO to output
#define DIO_Status() GPIO_ReadInputPin(TM1638_PORT, DIO_PIN)                          // read DIO bit

#define DISPLAY_CONTROL_COMMAND 0x8A
#define AUTO_INCREASE_COMMAND 0x40
#define DISPLAY_START_ADDRESS 0xC0
#define READ_KEY_COMMAND 0x42

void TM1638WriteByte(unsigned char data);  /* write DIO */
void TM1638ReadByte(unsigned char* data);  /* read DIO */
void TM1638SendCommand(unsigned char cmd); /* write command to tm1638 with STB change*/
void TM1638RefreshDisplayBuffer(void);
// display symbol's value must be less than 32
void TM1638ConvertDisplaySymbol(unsigned char pos, unsigned char symbol); /* convert symbol to display code */
// use map to find the keyName of keyboard
const unsigned char TM1638KeyMap[4][4] = {
    // 0x02 0x04 0x20 0x40
    { 9, 1, 10, 2 },  // 0 TM1638 key Byte 1
    { 11, 3, 12, 4 }, // 1 TM1638 key Byte 2
    { 13, 5, 14, 6 }, // 2 TM1638 key Byte 3
    { 15, 7, 16, 8 }  // 3 TM1638 key Byte 4
};
// digit to display code table
const unsigned char TM1638DigitToDisplayCodeMap[] = {
    // clang-format off
    ndp0, ndp1, ndp2, ndp3, ndp4, ndp5, ndp6, ndp7, //0-7
    ndp8, ndp9, ndpA, ndpB, ndpC, ndpD, ndpE, ndpF, //8-15
    wdp0, wdp1, wdp2, wdp3, wdp4, wdp5, wdp6, wdp7, //16-23
    wdp8, wdp9, wdpA, wdpB, wdpC, wdpD, wdpE, wdpF, //24-31
    segA, segB, segC, segD, segE, segF, segG, segDP, //32-39
    0x00 //40
    //0 , 1   , 2   , 3   , 4   , 5   , 6   , 7
    // clang-format on
};
// display buffer
// bit0 to bit7
// segA to segDP
unsigned char TM1638DisplayBuffer[8] = { segDP, segG, segF, segE, segD, segC, segB, segA };
//static unsigned char TM1638DisplayBuffer[8] = { segA, segB, segC, segD, segE, segF, segG, segDP };

void TM1638WriteByte(unsigned char data)
{
    unsigned char i;
    for (i = 0; i < 8; i++) {
        CLK_Clear();
        if (data & 0X01) {
            DIO_Set();
        } else {
            DIO_Clear();
        }
        data >>= 1;
        CLK_Set();
    }
}
void TM1638ReadByte(unsigned char* data)
{
    if (data != 0) {
        unsigned char i;
        DIO_Input_Mode(); // set to input mode

        (*data) = 0;
        for (i = 0; i < 8; i++) {
            (*data) >>= 1;
            CLK_Clear();
            if (DIO_Status()) {
                (*data) |= 0x80;
            }
            CLK_Set();
        }
        DIO_Output_Mode(); // set to output mode
    }
}
void TM1638SendCommand(unsigned char cmd)
{
    STB_Clear();
    TM1638WriteByte(cmd);
    STB_Set();
}

void TM1638Readkey(unsigned char* keyValue)
{
    if (keyValue != 0) {
        unsigned char c[4], i, hasKey, keyColumnIndex, keyRowIndex;
        *keyValue      = 0xFF; /* no valid key pressed */
        keyColumnIndex = 0;
        keyRowIndex    = 0;
        hasKey         = 0;

        STB_Clear();
        TM1638WriteByte(READ_KEY_COMMAND);
        delay_us(50);
        for (i = 0; i < 4; i++) {
            TM1638ReadByte(c + i);
            *(c + i) = *(c + i) & 0x77; /* clear 4 and 8 bit */
            if ((*(c + i) != 0)) {
                switch (*(c + i)) {
                case 0x02:
                    keyColumnIndex = 0;
                    hasKey         = 1;
                    break;
                case 0x04:
                    keyColumnIndex = 1;
                    hasKey         = 1;
                    break;
                case 0x20:
                    keyColumnIndex = 2;
                    hasKey         = 1;
                    break;
                case 0x40:
                    keyColumnIndex = 3;
                    hasKey         = 1;
                    break;
                default:
                    break;
                }
                keyRowIndex = i;
                if (hasKey) {
                    *keyValue = TM1638KeyMap[keyRowIndex][keyColumnIndex];
                    break;
                }
            }
        }
        STB_Set();
    }
}

void TM1638Init(void)
{
    // init displ data
    unsigned char displayData[] = { 0, 0, 40, 40, 40, 40, 40, 40 };
    // init IO state
    //    SET_BIT8(TM1638_PORT->DDR, DIO_PIN); // PA1-4 set to output
    //    SET_BIT8(TM1638_PORT->DDR, CLK_PIN); // PA1-4 set to output
    //    SET_BIT8(TM1638_PORT->DDR, STB_PIN); // PA1-4 set to output
    //
    //    SET_BIT8(TM1638_PORT->CR1, DIO_PIN); //  push-pull/pull up
    //    SET_BIT8(TM1638_PORT->CR1, CLK_PIN); //  push-pull/pull up
    //    SET_BIT8(TM1638_PORT->CR1, STB_PIN); //  push-pull/pull up
    //
    //    CLEAR_BIT8(TM1638_PORT->CR2, DIO_PIN); //  push-pull/pull up
    //    CLEAR_BIT8(TM1638_PORT->CR2, CLK_PIN); //  push-pull/pull up
    //    CLEAR_BIT8(TM1638_PORT->CR2, STB_PIN); //  push-pull/pull up

    GPIO_Init(TM1638_PORT, (GPIO_Pin_TypeDef)DIO_PIN | CLK_PIN | STB_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW); // PA1-4 set to output

    TM1638EightSymbolDisplay(displayData);
}

void TM1638ConvertDisplaySymbol(unsigned char pos, unsigned char symbol)
{
    if (pos < 8) /* max 8 led */
    {
        unsigned char i;
        if (symbol < 41) {
            symbol = TM1638DigitToDisplayCodeMap[symbol];
        }
        for (i = 0; i < 8; ++i) { /* 8 segments need to be changed */
            if (symbol & 0x01) {
                TM1638DisplayBuffer[i] |= 0x01 << pos; /* 1 to turn on segment */
            } else {
                TM1638DisplayBuffer[i] &= ~(0x01 << pos); /* 0 to trun off segment */
            }
            symbol >>= 1;
        }
    }
}

void TM1638EightSymbolDisplay(unsigned char* symbols)
{
    if (symbols != 0) {
        unsigned char i;
        for (i = 0; i < 8; ++i) {
            TM1638ConvertDisplaySymbol(i, symbols[i]);
        }
        TM1638RefreshDisplayBuffer();
    }
}

void TM1638RefreshDisplayBuffer(void)
{
    unsigned char i;
    TM1638SendCommand(DISPLAY_CONTROL_COMMAND);
    TM1638SendCommand(AUTO_INCREASE_COMMAND);
    STB_Clear();
    TM1638WriteByte(DISPLAY_START_ADDRESS);
    for (i = 0; i < 8; ++i) {
        TM1638WriteByte(TM1638DisplayBuffer[i]);
        TM1638WriteByte(0x00);
    }
    STB_Set();
}
void TM1638OneSymbolDisplay(unsigned char ledName, unsigned char symbol)
{
    if (ledName < 8) {
        TM1638ConvertDisplaySymbol(ledName, symbol);
        TM1638RefreshDisplayBuffer();
    }
}
