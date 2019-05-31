#include "TM1638.h"
#include "../core/delay.h"
// use map to find the keyName of keyboard
const static unsigned char TM1638KeyMap[4][4] = {
    // 0x02 0x04 0x20 0x40
    { 9, 1, 10, 2 },  // 0 TM1638 key Byte 1
    { 11, 3, 12, 4 }, // 1 TM1638 key Byte 2
    { 13, 5, 14, 6 }, // 2 TM1638 key Byte 3
    { 15, 7, 16, 8 }  // 3 TM1638 key Byte 4
};
// digit to display code table
const static unsigned char TM1638DigitToDisplayCodeMap[32] = {
    // clang-format off
    ndp0, ndp1, ndp2, ndp3, ndp4, ndp5, ndp6, ndp7,
    ndp8, ndp9, ndpA, ndpB, ndpC, ndpD, ndpE, ndpF,
    wdp0, wdp1, wdp2, wdp3, wdp4, wdp5, wdp6, wdp7,
    wdp8, wdp9, wdpA, wdpB, wdpC, wdpD, wdpE, wdpF
    // clang-format on
};
// display buffer
// bit0 to bit7
// segA to segDP
static unsigned char TM1638DisplayBuffer[8] = { segDP, segG, segF, segE, segD, segC, segB, segA };
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
        TM1638WriteByte(0x42);
        delay_us(100);
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
    unsigned char displayData[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    // init IO state
    PA_DDR = 0x0F; // PA1-4 set to output
    PA_CR1 = 0x0F; // push-pull
    PA_CR2 = 0x00;
    TM1638EightSymbolDisplay(displayData);
}

void TM1638ConvertDisplaySymbol(unsigned char pos, unsigned char symbol)
{
    if (pos < 8) /* max 8 led */
    {
        unsigned char i;
        if (symbol < 32) {
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
    TM1638SendCommand(0x8A);
    TM1638SendCommand(0x40);
    STB_Clear();
    TM1638WriteByte(0xC0);
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
