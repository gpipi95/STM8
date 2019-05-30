#include "../core/delay.h"
#include "TM1638.h"

unsigned char tab[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40, 0xef };
void          LedDisplay(unsigned char data0, unsigned char data1, unsigned char data2, unsigned char data3,
                         unsigned char data4, unsigned char data5, unsigned char data6, unsigned char data7);

int main(void)
{
    unsigned char key_value;
    init_TM1638();
    LedDisplay(1, 2, 3, 4, 5, 6, 7, 8);
    delay_ms(1000);

    while (1) {
        key_value = Read_key();
        switch (key_value) {
        case 1:
            LedDisplay(1, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 2:
            LedDisplay(2, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 3:
            LedDisplay(3, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 4:
            LedDisplay(4, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 5:
            LedDisplay(5, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 6:
            LedDisplay(6, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 7:
            LedDisplay(7, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 8:
            LedDisplay(8, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 9:
            LedDisplay(9, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 10:
            LedDisplay(0, 1, 0, 0, 0, 0, 0, 0);
            break;
        case 11:
            LedDisplay(1, 1, 0, 0, 0, 0, 0, 0);
            break;
        case 12:
            LedDisplay(2, 1, 0, 0, 0, 0, 0, 0);
            break;
        case 13:
            LedDisplay(3, 1, 0, 0, 0, 0, 0, 0);
            break;
        case 14:
            LedDisplay(4, 1, 0, 0, 0, 0, 0, 0);
            break;
        case 15:
            LedDisplay(5, 1, 0, 0, 0, 0, 0, 0);
            break;
        case 16:
            LedDisplay(6, 1, 0, 0, 0, 0, 0, 0);
            break;
        default:
            break; //LedDisplay(0,0,0,0,0,0,0,0);
        }
    }
}

void LedDisplay(unsigned char ddata0, unsigned char ddata1, unsigned char ddata2, unsigned char ddata3,
                unsigned char ddata4, unsigned char ddata5, unsigned char ddata6, unsigned char ddata7)

{
    unsigned char writedata0, writedata1, writedata2, writedata3, writedata4, writedata5, writedata6, writedata7;
    unsigned char data0, data1, data2, data3, data4, data5, data6, data7;
    data0 = ddata4;
    data1 = ddata5;
    data2 = ddata6;
    data3 = ddata7;
    data4 = ddata0;
    data5 = ddata1;
    data6 = ddata2;
    data7 = ddata3;

    writedata0 = (tab[data0] & 0x01) + ((tab[data1] & 0x01) << 1) + ((tab[data2] & 0x01) << 2) + ((tab[data3] & 0x01) << 3) + ((tab[data4] & 0x01) << 4)
                 + ((tab[data5] & 0x01) << 5) + ((tab[data6] & 0x01) << 6) + ((tab[data7] & 0x01) << 7);

    writedata1 = ((tab[data0] & 0x02) >> 1) + ((tab[data1] & 0x02)) + ((tab[data2] & 0x02) << 1) + ((tab[data3] & 0x02) << 2) + ((tab[data4] & 0x02) << 3)
                 + ((tab[data5] & 0x02) << 4) + ((tab[data6] & 0x02) << 5) + ((tab[data7] & 0x02) << 6);

    writedata2 = ((tab[data0] & 0x04) >> 2) + ((tab[data1] & 0x04) >> 1) + ((tab[data2] & 0x04)) + ((tab[data3] & 0x04) << 1) + ((tab[data4] & 0x04) << 2)
                 + ((tab[data5] & 0x04) << 3) + ((tab[data6] & 0x04) << 4) + ((tab[data7] & 0x04) << 5);

    writedata3 = ((tab[data0] & 0x08) >> 3) + ((tab[data1] & 0x08) >> 2) + ((tab[data2] & 0x08) >> 1) + ((tab[data3] & 0x08)) + ((tab[data4] & 0x08) << 1)
                 + ((tab[data5] & 0x08) << 2) + ((tab[data6] & 0x08) << 3) + ((tab[data7] & 0x08) << 4);

    writedata4 = ((tab[data0] & 0x10) >> 4) + ((tab[data1] & 0x10) >> 3) + ((tab[data2] & 0x10) >> 2) + ((tab[data3] & 0x10) >> 1) + ((tab[data4] & 0x10))
                 + ((tab[data5] & 0x10) << 1) + ((tab[data6] & 0x10) << 2) + ((tab[data7] & 0x10) << 3);

    writedata5 = ((tab[data0] & 0x20) >> 5) + ((tab[data1] & 0x20) >> 4) + ((tab[data2] & 0x20) >> 3) + ((tab[data3] & 0x20) >> 2) + ((tab[data4] & 0x20) >> 1)
                 + ((tab[data5] & 0x20)) + ((tab[data6] & 0x20) << 1) + ((tab[data7] & 0x20) << 2);

    writedata6 = ((tab[data0] & 0x40) >> 6) + ((tab[data1] & 0x40) >> 5) + ((tab[data2] & 0x40) >> 4) + ((tab[data3] & 0x40) >> 3) + ((tab[data4] & 0x40) >> 2)
                 + ((tab[data5] & 0x40) >> 1) + ((tab[data6] & 0x40)) + ((tab[data7] & 0x40) << 1);

    writedata7 = ((tab[data0] & 0x80) >> 7) + ((tab[data1] & 0x80) >> 6) + ((tab[data2] & 0x80) >> 5) + ((tab[data3] & 0x80) >> 4) + ((tab[data4] & 0x80) >> 3)
                 + ((tab[data5] & 0x80) >> 2) + ((tab[data6] & 0x80) >> 1) + ((tab[data7] & 0x80));

    STB_Clear();
    Write_COM(0x88); //ÁÁ¶È
    STB_Set();
    delay_ms(5);
    STB_Clear();
    Write_COM(0x40); //Ğ´Êı¾İÃüÁî
    STB_Set();
    delay_ms(5);
    STB_Clear();
    TM1638_Write(0x00); //Ğ´µØÖ·ÃüÁî
    TM1638_Write(writedata0);
    TM1638_Write(0x00);
    TM1638_Write(writedata1);
    TM1638_Write(0x00);
    TM1638_Write(writedata2);
    TM1638_Write(0x00);
    TM1638_Write(writedata3);
    TM1638_Write(0x00);
    TM1638_Write(writedata4);
    TM1638_Write(0x00);
    TM1638_Write(writedata5);
    TM1638_Write(0x00);
    TM1638_Write(writedata6);
    TM1638_Write(0x00);
    TM1638_Write(writedata7);
    TM1638_Write(0x00);
    STB_Set();
}
