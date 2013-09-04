/* C-version of memmove for using when Intel MPX is enabled
   in order to prosess with a buffer of pointers correctly.
   Copyright (C) 2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifdef __CHKP__

# include <stddef.h>

void *
__memmove (void *dst, const void *src, size_t n)
{
  const char *s = src;
  char *d = dst;
  void *ret = dst;
  size_t offset_src = ((size_t) s) & (sizeof(size_t) - 1);
  size_t offset_dst = ((size_t) d) & (sizeof(size_t) - 1);

  if (offset_src != offset_dst)
  {
    if (s < d)
    {
      /* backward copying */
      d += n;
      s += n;
      while (n--)
        *--d = *--s;
    }
    else
      /* forward copying */
      while (n--)
        *d++ = *s++;
  }
  else
  {
    if (s < d)
    {
      offset_src = (offset_src + (size_t)src) & (sizeof(size_t) - 1);
      /* backward copying */
      d += n;
      s += n;
      while (n-- && offset_src--)
        *--d = *--s;
      n++;
      if (!n) return ret;
      void **d1 = (void **)d;
      void **s1 = (void **)s;
      while (n >= sizeof(void *))
      {
        n -= sizeof(void *);
        *--d1 = *--s1;
      }
      s = (char *)s1;
      d = (char *)d1;
      while (n--)
        *--d = *--s;
    }
    else
    {
      if (offset_src) offset_src = sizeof(size_t) - offset_src;
      /* forward copying */
      while (n-- && offset_src--)
        *d++ = *s++;
      n++;
      if (!n) return ret;
      void **d1 = (void **)d;
      void **s1 = (void **)s;
      while (n >= sizeof(void *))
      {
        n -= sizeof(void *);
        *d1++ = *s1++;
      }
      s = (char *)s1;
      d = (char *)d1;
      while (n--)
        *d++ = *s++;
    }
  }
  return ret;
}

weak_alias (__memmove, __libc_memmove)
weak_alias (__memmove, __GI_memmove)
weak_alias (__memmove, memmove)

# if defined SHARED && !defined NOT_IN_libc
#  include <shlib-compat.h>
#  if SHLIB_COMPAT (libc, GLIBC_2_2_5, GLIBC_2_14)
compat_symbol (libc, memmove, memcpy, GLIBC_2_2_5);
#  endif
# endif

weak_alias (__memmove, mpx_memmove_nochk)
#endif
