#include <string.h>

void *memset(void *s, int c, size_t n)
{
  /* cast untyped void pointer into pointer to unsigned char. unsigned char
     is guaranteed to be able to alias any object type. */
  unsigned char *sptr = (unsigned char *)s;

  /* the value is converted to unsigned char, as per C standard. */
  unsigned char value = (unsigned char)c;

  /* set first n bytes of s pointer to c. */
  for(size_t i = 0; i < n; i++) *sptr++ = value;

  /* return pointer to s, as per C standard. */
  return s;
}
