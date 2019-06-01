#ifndef _OWIRE_H_
#define _OWIRE_H_

#include "../core/delay.h"
#include "atomic.h"

// Save and restore interrupt state
#define INTDE (1)

typedef enum {
    CONN_FAULT    = 1,
    W1_OK         = 2,
    NODEV_PRESENT = 3,
    LAST_DEV      = 4
} W1ErrorEnum;

// Init IO port and send reset to bus
// return 0 if devices not found
unsigned char W1Init(void);

// Read 1 byte from bus
unsigned char W1Read(void);

// Write 1 byte from bus
unsigned char W1Write(unsigned char data);

W1ErrorEnum W1Search(unsigned char cmd, unsigned char* p);
// Search next ROM id on bus id-by-id.
// write found id at the same place in a memory
// return 0 if error or found last id

//
unsigned char W1DowCRC8(unsigned char* p, unsigned char count);

//1-Wire port registers
#define W1_BUS_ODR PF_ODR
#define W1_BUS_IDR PF_IDR
#define W1_BUS_DDR PF_DDR
#define W1_BUS_CR1 PF_CR1
#define W1_BUS_CR2 PF_CR2
#define W1_PIN 4
#define W1_BUS_INPUT() _asm("bres W1_BUS_DDR, #W1_PIN\n")
#define W1_BUS_OUTPUT() _asm("bset W1_BUS_DDR, #W1_PIN\n")

//Standard 1-Wire commands
#define W1_READ_ROM 0x33
#define W1_MATCH_ROM 0x55
#define W1_SKIP_ROM 0xCC
#define W1_SRCH_ROM 0xF0
#define W1_STRT_CONV 0x44
#define W1_RD_SCRTPD 0xBE
#define W1_SRCH_ALRM 0xEC

/* time constants by Maxim IC
 * A - write '0'
 * C - write '1'
 * A+E - read sample
*/
#define W1SLOT (70) /* A + B */
#define A 6
#define B (W1SLOT - A)
#define C 60
#define D (W1SLOT - C)
//#define E   9
#define E 7
#define F (W1SLOT - E - A)
#define G 0
#define H 480
#define I 70
#define J 410

#define SFR(a) (unsigned char)(a)

#endif
