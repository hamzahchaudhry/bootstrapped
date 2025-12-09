#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int c)
{
#if defined(__is_libk)
  unsigned char uc = (unsigned char)c;
  terminal_write((const char *)&uc, sizeof(uc));
#else
  // TODO: Implement stdio and the write system call.
#endif
  return c;
}
