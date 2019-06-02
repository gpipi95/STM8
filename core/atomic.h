#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include "../core/STM8S103K3T6C.h"
#define ATOMIC_BEGIN() GetInterruptState()
#define ATOMIC_END(x) SetInterruptState(x)

unsigned char @ inline GetInterruptState(void)
{
    return ((unsigned char)_asm("push cc\n pop a\n rim\n"));
}

void @ inline SetInterruptState(unsigned char istate)
{
    _asm("push a\n pop cc\n ", istate);
}

#endif
