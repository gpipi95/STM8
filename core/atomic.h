#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"

extern unsigned char InterruptStateSave;
#define ATOMIC_BEGIN() InterruptStateSave = GetInterruptState()
#define ATOMIC_END() SetInterruptState(InterruptStateSave)

unsigned char @ inline GetInterruptState(void)
{
    return ((unsigned char)_asm("push cc\n pop a\n rim\n"));
}

void @ inline SetInterruptState(unsigned char istate)
{
    _asm("push a\n pop cc\n ", istate);
}

#endif
