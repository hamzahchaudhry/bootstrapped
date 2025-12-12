#include <stdio.h>

#include <kernel/keyboard.h>
#include <kernel/kshell.h>
#include <kernel/tty.h>

/* kernel entry point, called from _start in boot.s. */
void kernel_main(void)
{
  /* initialize terminal interface */
  terminal_initialize();
  printf("You're in bootstrapped!\n");
  printf("Type keys to see scancode->ASCII polling:\n");

  /* run the shell */
  kshell_run();
}
