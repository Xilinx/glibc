/* Copyright (C) 1991-2013 Free Software Foundation, Inc.
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

#include <stddef.h>
#include <string.h>
#include <memcopy.h>
#include <bp-checks.h>

#undef strcpy

/* Copy SRC to DEST.  */
char *
strcpy (dest, src)
     char *dest;
     const char *src;
{
  char c;
  char *__unbounded s = (char *__unbounded) CHECK_BOUNDS_LOW (src);
  const ptrdiff_t off = CHECK_BOUNDS_LOW (dest) - s - 1;
  size_t n;

  do
    {
      c = *s++;
      s[off] = c;
    }
  while (c != '\0');

  n = s - src;
  (void) CHECK_BOUNDS_HIGH (src + n);
  (void) CHECK_BOUNDS_HIGH (dest + n);

  return dest;
}
libc_hidden_builtin_def (strcpy)
