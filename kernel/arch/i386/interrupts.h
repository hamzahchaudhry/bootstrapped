#ifndef ARCH_i386_INTERRUPTS_H
#define ARCH_i386_INTERRUPTS_H

#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

typedef struct
{
  uint16_t isr_low;   /* lower 16 bits of ISR's address */
  uint16_t kernel_cs; /* GDT segment selector CPU will load into CS before
                         calling ISR */
  uint8_t reserved;   /* set to zero */
  uint8_t attributes; /* type and attributes */
  uint16_t isr_high;  /* higher 16 bits of ISR's address */
} __attribute__((packed)) idt_entry_t;

typedef struct
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idtr_t;

#endif
