#include <stdio.h>

#include <kernel/keyboard.h>
#include <kernel/tty.h>

/* kernel entry point, called from _start in boot.s. */
void kernel_main(void)
{
  /* initialize terminal interface */
  terminal_initialize();
  printf("You're in bootstrapped!\n");
  printf("Type keys to see scancode->ASCII polling:\n");

  /* simple busy-wait poll */
  // TODO: replace with IRQ system.
  while(1)
    {
      unsigned char c = keyboard_poll();
      if(c) putchar(c);
    }
}
