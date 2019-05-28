#include "TM1638.h"
#include "../core/delay.h"

void TM1638_Write(unsigned char DATA) //写数据函数
{
    unsigned char i;
    for (i = 0; i < 8; i++) {
        CLK_Clear();
        if (DATA & 0X01) {
            DIO_Set();
        } else {
            DIO_Clear();
        }
        DATA >>= 1;
        CLK_Set();
    }
}
unsigned char TM1638_Read(void) //读数据函数
{
    unsigned char i;
    unsigned char temp = 0;
    DIO_Set(); //设置为输入
    for (i = 0; i < 8; i++) {
        temp >>= 1;
        CLK_Clear();
        if (DIO_Status()) {
            temp |= 0x80;
        }
        CLK_Set();
    }
    return temp;
}
void Write_COM(unsigned char cmd) //发送命令字
{
    STB_Clear();
    TM1638_Write(cmd);
    STB_Set();
}

unsigned char Read_key(void)
{
    unsigned char c[4], i, key_value = 0;
    STB_Clear();
    TM1638_Write(0x42);
    delay_ms(100);
    for (i = 0; i < 4; i++) {
        c[i] = TM1638_Read();
    }
    STB_Set(); //4个字节数据合成一个字节
    if (c[0] == 0x04) {
        key_value = 1;
    }
    if (c[0] == 0x40) {
        key_value = 2;
    }
    if (c[1] == 0x04) {
        key_value = 3;
    }
    if (c[1] == 0x40) {
        key_value = 4;
    }
    if (c[2] == 0x04) {
        key_value = 5;
    }
    if (c[2] == 0x40) {
        key_value = 6;
    }
    if (c[3] == 0x04) {
        key_value = 7;
    }
    if (c[3] == 0x40) {
        key_value = 8;
    }
    if (c[0] == 0x02) {
        key_value = 9;
    }
    if (c[0] == 0x20) {
        key_value = 10;
    }
    if (c[1] == 0x02) {
        key_value = 11;
    }
    if (c[1] == 0x20) {
        key_value = 12;
    }
    if (c[2] == 0x02) {
        key_value = 13;
    }
    if (c[2] == 0x20) {
        key_value = 14;
    }
    if (c[3] == 0x02) {
        key_value = 15;
    }
    if (c[3] == 0x20) {
        key_value = 16;
    }
    return (key_value);
}

void init_TM1638(void)
{
    unsigned char i;
    Write_COM(0x8a); //亮度
    Write_COM(0x8a); //亮度
    Write_COM(0x40); //写数据命令
    STB_Clear();
    TM1638_Write(0xc0); //写地址命令
    for (i = 0; i < 16; i++) {
        TM1638_Write(0xff);
    }
    STB_Set();
}

