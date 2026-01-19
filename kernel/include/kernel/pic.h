#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

#include <stdint.h>

void PIC_sendEOI(uint8_t irq);
void PIC_remap(int offset1, int offset2);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);

#endif
