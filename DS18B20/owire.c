#include "owire.h"
#include "../core/atomic.h"
#include "../core/delay.h"

unsigned char iStateSave; /* interrupt state */

/* do CRC8 table */
const unsigned char W1_CRC8_TABLE[] = {
    // clang-format off
    0,   94,  188, 226, 97,  63,  221, 131, 194, 156, 126, 32,  163, 253, 31,  65,  // 0
    157, 195, 33,  127, 252, 162, 64,  30,  95,  1,   227, 189, 62,  96,  130, 220, // 16
    35,  125, 159, 193, 66,  28,  254, 160, 225, 191, 93,  3,   128, 222, 60,  98,  // 32
    190, 224, 2,   92,  223, 129, 99,  61,  124, 34,  192, 158, 29,  67,  161, 255, // 48
    70,  24,  250, 164, 39,  121, 155, 197, 132, 218, 56,  102, 229, 187, 89,  7,   // 64
    219, 133, 103, 57,  186, 228, 6,   88,  25,  71,  165, 251, 120, 38,  196, 154, // 80
    101, 59,  217, 135, 4,   90,  184, 230, 167, 249, 27,  69,  198, 152, 122, 36,  // 96
    248, 166, 68,  26,  153, 199, 37,  123, 58,  100, 134, 216, 91,  5,   231, 185, // 112
    140, 210, 48,  110, 237, 179, 81,  15,  78,  16,  242, 172, 47,  113, 147, 205, //128
    17,  79,  173, 243, 112, 46,  204, 146, 211, 141, 111, 49,  178, 236, 14,  80,  // 144
    175, 241, 19,  77,  206, 144, 114, 44,  109, 51,  209, 143, 12,  82,  176, 238, //160
    50,  108, 142, 208, 83,  13,  239, 177, 240, 174, 76,  18,  145, 207, 45,  115, // 176
    202, 148, 118, 40,  171, 245, 23,  73,  8,   86,  180, 234, 105, 55,  213, 139, // 192
    87,  9,   235, 181, 54,  104, 138, 212, 149, 203, 41,  119, 244, 170, 72,  22,  // 208
    233, 183, 85,  11,  136, 214, 52,  106, 43,  117, 151, 201, 74,  20,  246, 168, // 224
    116, 42,  200, 150, 21,  75,  169, 247, 182, 232, 10,  84,  215, 137, 107, 53   // 240
};
// clang-format on

/**
 * @brief read 1 bit from bus
 *
 * @return the bit readed
 */
unsigned char W1ReadBit(void)
{
    unsigned char out;
#if INTDE
    iStateSave = ATOMIC_BEGIN();
#endif
    W1_BUS_OUTPUT();
    W1_BUS_OUT_0(); // low
    delay_us(2);
    W1_BUS_OUT_1(); // high
    W1_BUS_INPUT();
    delay_us(10);
    out = CAST_UC((W1_BUS_IDR & 1 << W1_PIN) ? 0x01 : 0x00);
    delay_us(58);
#if INTDE
    ATOMIC_END(iStateSave);
#endif
    return (out);
}

/**
 * @brief write one bit to bus
 *
 * @param bit 0 to write 0 else to write 1
 */
void W1WriteBit(unsigned char bit)
{
#if INTDE
    iStateSave = ATOMIC_BEGIN();
#endif
    if (bit == 0) {
        // Write '0' bit
        W1_BUS_OUTPUT();
        W1_BUS_OUT_0(); // Drives low
        delay_us(70);
        W1_BUS_OUT_1(); // Releases the bus
        delay_us(5);
    } else {
        // Write '1' bit
        W1_BUS_OUTPUT();
        W1_BUS_OUT_0(); // Drives low
        delay_us(5);
        W1_BUS_OUT_1(); // Releases the bus
        delay_us(70);
    }
#if INTDE
    ATOMIC_END(iStateSave);
#endif
}

/**
 * @brief Init IO port and send reset to bus
 *
 * @return return 0 if devices not found, 1 if device is found.
 */
unsigned char W1Init(void)
{
    unsigned char presence;

#if INTDE
    iStateSave = ATOMIC_BEGIN();
#endif
    /* Init IO and reset presence pulse */
    // select Open drain output, fast mode
    W1_BUS_OUTPUT();
    W1_BUS_OUT_0();
    delay_us(480);
    W1_BUS_OUT_1(); // release the bus
                    //    W1_BUS_INPUT(); // change to input mode, rising edge
    delay_us(80);
    presence = CAST_UC(!(W1_BUS_IDR & 1 << W1_PIN) ? 0x01 : 0x00);
    delay_us(380);
#if INTDE
    ATOMIC_END(iStateSave);
#endif
    return presence;
}

unsigned char W1ReadByte(void)
{
    unsigned char out = 0x00;
    unsigned char j;

    for (j = 0; j < 8; j++) {
        out >>= 1;
        out |= CAST_UC(W1ReadBit() ? 0x80 : 0);
    }
    return (out);
}

void W1WriteByte(unsigned char data)
{
    unsigned char i;

    for (i = 0; i < 8; i++) {
        if ((data & (0x01 << i)) != 0) {
            W1WriteBit(1);
        } else {
            W1WriteBit(0);
        }
    }
}

unsigned char W1DowCRC8(unsigned char* p, unsigned char count)
{
    unsigned char i;
    unsigned char crc = 0;

    i = 0;
    do {
        crc = W1_CRC8_TABLE[crc ^ *(p + i)];
    } while (++i < count);
    return (crc);
}

