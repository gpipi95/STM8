#ifndef _OWIRE_H_
#define _OWIRE_H_

#include "../core/utility.h"

// Save and restore interrupt state
#define INTDE (1)

/**
 * @brief Init IO port and send reset to bus
 *
 * @return return 0 if devices not found, 1 if device is found.
 */
unsigned char W1Init(void);
/**
 * @brief Read 1 byte from bus
 *
 * @return the data readed
 */
unsigned char W1ReadByte(void);

/**
 * @brief Write 1 byte to bus
 *
 * @param data the data to be writed
 *
 * @return always return 1
 */
void W1WriteByte(unsigned char data);

/**
 * @brief CRC8 Calculate
 *
 * @param p unsigned char content pointer
 * @param count lenght of the content
 *
 * @return crc8 result
 */
unsigned char W1DowCRC8(unsigned char* p, unsigned char count);

//1-Wire port registers
#define W1_BUS_ODR PF_ODR
#define W1_BUS_IDR PF_IDR
#define W1_BUS_DDR PF_DDR
#define W1_BUS_CR1 PF_CR1
#define W1_BUS_CR2 PF_CR2
#define W1_PIN 4

// input without pull up
#define W1_BUS_INPUT()              \
    CLEAR_BIT8(W1_BUS_DDR, W1_PIN); \
    CLEAR_BIT8(W1_BUS_CR1, W1_PIN); \
    CLEAR_BIT8(W1_BUS_CR2, W1_PIN)
// open drain output
#define W1_BUS_OUTPUT()             \
    SET_BIT8(W1_BUS_DDR, W1_PIN);   \
    CLEAR_BIT8(W1_BUS_CR1, W1_PIN); \
    CLEAR_BIT8(W1_BUS_CR2, W1_PIN)

#define W1_BUS_OUT_1() SET_BIT8(W1_BUS_ODR, W1_PIN)
#define W1_BUS_OUT_0() CLEAR_BIT8(W1_BUS_ODR, W1_PIN)

#define W1_READ_PIN() \
    CAST_UC(W1_BUS_IDR & 1 << W1_PIN)

#endif
