/* Multiple versions of llrintf.
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

/* Redefine lrint/__lrint so that the compiler won't complain about the type
   mismatch with the IFUNC selector in strong_alias below.  */
#undef lrintf
#undef __lrintf
#define lrintf lrintf_copysign
#define __lrintf __lrintf_copysign
#include <math.h>
#undef lrintf
#undef __lrintf

#include "init-arch.h"

/* The double-precision implementation also works for the single one.  */
extern __typeof (__llrintf) __llrint_ppc64 attribute_hidden;
extern __typeof (__llrintf) __llrint_power6x attribute_hidden;

libc_ifunc (__llrintf,
	    (hwcap & PPC_FEATURE_POWER6_EXT)
	    ? __llrint_power6x
            : __llrint_ppc64);

weak_alias (__llrintf, llrintf)
strong_alias (__llrintf, __lrintf)
weak_alias (__lrintf, lrintf)
