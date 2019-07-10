#ifndef _OWIRE_H_
#define _OWIRE_H_

#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"
#include "../STM8S_StdPeriph_Driver/inc/stm8s_gpio.h"
#include "../core/utility.h"

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
#define W1_BUS_PORT GPIOF
#define W1_PIN GPIO_PIN_4

// input without pull up
#define W1_BUS_INPUT() GPIO_Init(W1_BUS_PORT, W1_PIN, GPIO_MODE_IN_PU_NO_IT)
// open drain output
#define W1_BUS_OUTPUT() GPIO_Init(W1_BUS_PORT, W1_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW)
#define W1_BUS_OUT_1() GPIO_WriteHigh(W1_BUS_PORT, W1_PIN)
#define W1_BUS_OUT_0() GPIO_WriteLow(W1_BUS_PORT, W1_PIN)
#define W1_READ_PIN() GPIO_ReadInputPin(W1_BUS_PORT, W1_PIN)

#endif
