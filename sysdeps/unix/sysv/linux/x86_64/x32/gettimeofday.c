/* The gettimeofday system call.  Linux/x32 version.
   Copyright (C) 2012 Free Software Foundation, Inc.
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

#ifdef SHARED
# include <dl-vdso.h>
libc_ifunc_vdso (__gettimeofday, __vdso_gettimeofday)
#else
# include <errno.h>
# include <sysdep.h>
# include <sys/time.h>

int
__gettimeofday (struct timeval *tv, struct timezone *tz)
{
  return INLINE_SYSCALL (gettimeofday, 2, tv, tz);
}
#endif

weak_alias (__gettimeofday, gettimeofday)
strong_alias (__gettimeofday, __gettimeofday_internal)
