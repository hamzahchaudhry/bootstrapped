#include <stdio.h>

#include <kernel/interrupts.h>
#include <kernel/keyboard.h>
#include <kernel/kshell.h>
#include <kernel/pic.h>
#include <kernel/tty.h>

/* kernel entry point, called from _start in boot.s */
void kernel_main(void)
{
  /* initialize interrupts */
  idt_init();
  PIC_remap(0x20, 0x28);
  IRQ_clear_mask(1);
  __asm__ volatile("sti"); /* set interrupt flag */

  /* initialize terminal interface */
  terminal_initialize();
  printf("You're in bootstrapped!\n");
  kshell_run();
}
