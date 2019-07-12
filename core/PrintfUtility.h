#ifndef PRINTFUTILITY_H
#define PRINTFUTILITY_H
// for cosmic compiler
#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"
#include <stdio.h>

#define PUTCHAR_PROTOTYPE char putchar(char c)
#define GETCHAR_PROTOTYPE char getchar(void)

void Printf_Init(void);

#endif

