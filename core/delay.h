#ifndef delay_H
#define delay_H

/* 
 *  * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 *   * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 *    * when tick = 1, because 0 equels 65535
 *     */
// todo: time is not correct check it.
#define F_CPU 8000000 // cpu frequence is 8MHz
static @ inline void delay_cycl(unsigned short ticks)
{
#define T_COUNT(x) (F_CPU * x / 1000000UL)*3  / 4
    // ldw X, __ticks ; insert automaticaly
    _asm("nop\n $N:\n decw X\n jrne $L\n nop\n ", ticks);
}

static @ inline void delay_us(const unsigned short us)
{
    delay_cycl((unsigned short)(T_COUNT(us)));
}

static @ inline void delay_ms(unsigned short ms)
{
    while (ms--) {
        delay_us(1000);
    }
}

#endif
