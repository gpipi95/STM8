/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */
#include "STM8S103K3.h"

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
    unsigned char       interrupt_instruction;
    interrupt_handler_t interrupt_handler;
};

@far @interrupt void TIM2Handle(void)
{
    static unsigned char counter = 255;
    TIM2_SR1                     = 0x00; // 清除更新标??
    if (counter-- == 0) {
        counter = 255;
        PD_ODR  = PD_ODR ^ 0x08; // LED驱动信号取反
    }                            //LED闪烁频率=2MHZ/2/60000/2=8.3
    return;
}
@far @interrupt void NonHandledInterrupt(void)
{
    /* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
    return;
}

extern void _stext(); /* startup routine */

struct interrupt_vector const _vectab[] = {
    { 0x82, (interrupt_handler_t)_stext }, /* reset */
    { 0x82, NonHandledInterrupt },         /* trap  */
    { 0x82, NonHandledInterrupt },         /* irq0  */
    { 0x82, NonHandledInterrupt },         /* irq1  */
    { 0x82, NonHandledInterrupt },         /* irq2  */
    { 0x82, NonHandledInterrupt },         /* irq3  */
    { 0x82, NonHandledInterrupt },         /* irq4  */
    { 0x82, NonHandledInterrupt },         /* irq5  */
    { 0x82, NonHandledInterrupt },         /* irq6  */
    { 0x82, NonHandledInterrupt },         /* irq7  */
    { 0x82, NonHandledInterrupt },         /* irq8  */
    { 0x82, NonHandledInterrupt },         /* irq9  */
    { 0x82, NonHandledInterrupt },         /* irq10 */
    { 0x82, NonHandledInterrupt },         /* irq11 */
    { 0x82, NonHandledInterrupt },         /* irq12 */
    { 0x82, TIM2Handle },                  /* irq13 */
    { 0x82, NonHandledInterrupt },         /* irq14 */
    { 0x82, NonHandledInterrupt },         /* irq15 */
    { 0x82, NonHandledInterrupt },         /* irq16 */
    { 0x82, NonHandledInterrupt },         /* irq17 */
    { 0x82, NonHandledInterrupt },         /* irq18 */
    { 0x82, NonHandledInterrupt },         /* irq19 */
    { 0x82, NonHandledInterrupt },         /* irq20 */
    { 0x82, NonHandledInterrupt },         /* irq21 */
    { 0x82, NonHandledInterrupt },         /* irq22 */
    { 0x82, NonHandledInterrupt },         /* irq23 */
    { 0x82, NonHandledInterrupt },         /* irq24 */
    { 0x82, NonHandledInterrupt },         /* irq25 */
    { 0x82, NonHandledInterrupt },         /* irq26 */
    { 0x82, NonHandledInterrupt },         /* irq27 */
    { 0x82, NonHandledInterrupt },         /* irq28 */
    { 0x82, NonHandledInterrupt },         /* irq29 */
};
