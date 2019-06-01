#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include "../core/STM8S103K3T6C.h"
#define ATOMIC_BEGIN() getInterruptState()
#define ATOMIC_END(x) setInterruptState(x)

unsigned char @ inline getInterruptState(void)
{
    return ((unsigned char)_asm("push cc\n pop a\n rim\n"));
}

void @ inline setInterruptState(unsigned char istate)
{
    _asm("push a\n pop cc\n ", istate);
}

#endif
