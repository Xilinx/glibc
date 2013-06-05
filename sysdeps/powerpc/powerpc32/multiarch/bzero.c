/* Multiple versions of bzero.
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

/* Define multiple versions only for definition in libc.  */
#ifndef NOT_IN_libc
# include <string.h>
# include <strings.h>
# include "init-arch.h"

extern __typeof (memset) __memset_ppc32 attribute_hidden;
extern __typeof (memset) __memset_power4 attribute_hidden;
extern __typeof (memset) __memset_power6 attribute_hidden;
extern __typeof (memset) __memset_power7 attribute_hidden;

__typeof(bzero) __bzero_ppc32 attribute_hidden;
void
__bzero_ppc32 (void *s, size_t n)
{
  __memset_ppc32 (s, 0, n);
}

__typeof(bzero) __bzero_power4 attribute_hidden;
void
__bzero_power4 (void *s, size_t n)
{
  __memset_power4 (s, 0, n);
}

__typeof(bzero) __bzero_power6 attribute_hidden;
void
__bzero_power6 (void *s, size_t n)
{
  __memset_power6 (s, 0, n);
}

__typeof(bzero) __bzero_power7 attribute_hidden;
void
__bzero_power7 (void *s, size_t n)
{
  __memset_power7 (s, 0, n);
}

libc_ifunc (__bzero,
            (hwcap & PPC_FEATURE_HAS_VSX)
            ? __bzero_power7 :
	      (hwcap & PPC_FEATURE_ARCH_2_05)
		? __bzero_power6 :
		  (hwcap & PPC_FEATURE_POWER4)
		? __bzero_power4
            : __bzero_ppc32);

weak_alias (__bzero, bzero)
#endif
