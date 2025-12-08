#include <string.h>

void *memmove(void *dest, const void *src, size_t n)
{
  /* cast untyped void pointers into pointers to unsigned char.
     unsigned char is guaranteed to be able to alias any object type. */
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;

  /* if dest and src are the same, or n == 0, there is nothing to do. */
  if(d == s || n == 0) return dest;

  /* if dest starts BEFORE src in memory, safely copy forwards.
     even if the regions overlap, writing to dest will not overwrite any
     src bytes that we have not yet read. */
  if(d < s)
    {
      for(size_t i = 0; i < n; i++) d[i] = s[i];
    }
  else
    {
      /* otherwise, dest starts AT or AFTER src in memory.
         copying forwards could overwrite src bytes before they are copied.
         copy backwards to prevent this. */
      for(size_t i = n; i != 0; i--) d[i - 1] = s[i - 1];
    }

  /* return original destination pointer, as per the C standard. */
  return dest;
}
