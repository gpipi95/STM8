#include "../core/delay.h"
#include "TM1638.h"

int main(void)
{
    unsigned char keyValue, i;
    unsigned char displayData[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    TM1638Init();
    TM1638EightSymbolDisplay(displayData);
    while (1) {
        TM1638Readkey(&keyValue);
        if (keyValue != 0xFF) {
            for (i = 0; i < 8; ++i) {
                displayData[i] = 0;
            }
            if (keyValue < 10) {
                TM1638OneSymbolDisplay(0, keyValue);
                TM1638OneSymbolDisplay(1, 0);
            } else if (keyValue < 20) {
                TM1638OneSymbolDisplay(0, keyValue - 10);
                TM1638OneSymbolDisplay(1, 1);
            }
        }
    }
}

