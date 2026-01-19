#include <stdbool.h>
#include <stdint.h>

#include <kernel/interrupts.h>

#include "interrupts.h"

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static bool vectors[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;

extern void *isr_stub_table[];
extern void irq_stub_1(void);
__attribute__((noreturn)) void exception_handler(void)
{
  __asm__ volatile("cli; hlt"); /* completely hangs computer */
  __builtin_unreachable();
}

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
  idt_entry_t *descriptor = &idt[vector];

  descriptor->isr_low = (uint32_t)isr & 0xFFFF;
  descriptor->kernel_cs = 0x08;
  descriptor->attributes = flags;
  descriptor->isr_high = (uint32_t)isr >> 16;
  descriptor->reserved = 0;
}

void idt_init()
{
  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

  for (uint8_t vector = 0; vector < 32; vector++)
  {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    vectors[vector] = true;
  }

  /* keyboard */
  idt_set_descriptor(0x21, (void *)irq_stub_1, 0x8E);
  vectors[0x21] = true;

  __asm__ volatile("lidt %0" : : "m"(idtr)); /* load new IDT */
}
