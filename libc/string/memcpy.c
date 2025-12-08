#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
  /* cast untyped void pointers into pointers to unsigned char.
     unsigned char is guaranteed to be able to alias any object type. */
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;

  for(size_t i = 0; i < n; i++) *d++ = *s++;

  /* return original destination pointer, as per the C standard. */
  return dest;
}
