#include <stddef.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
  /* cast untyped void pointers into pointers to unsigned char.
     unsigned char is guaranteed to be able to alias any object type,
     and memcmp compares bytes as unsigned chars. */
  const unsigned char *s1ptr = (const unsigned char *)s1;
  const unsigned char *s2ptr = (const unsigned char *)s2;

  /* if n == 0, there is nothing to compare, so they are equal */
  if(n == 0) return 0;

  for(size_t i = 0; i < n; i++, s1ptr++, s2ptr++)
    {
      if(*s1ptr < *s2ptr)
        return -1; /* first differing byte in s1 is smaller */
      else if(*s1ptr > *s2ptr)
        return 1; /* first differing byte in s1 is larger */
      /* else equal, keep going */
    }

  /* all n bytes were equal */
  return 0;
}
