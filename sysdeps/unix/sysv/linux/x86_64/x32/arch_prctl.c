/* arch_prctl call for Linux/x32.
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

#include <errno.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sysdep.h>

/* Since x32 arch_prctl stores 32bit base address of segment register %fs
   and %gs as unsigned 64bit value via ARCH_GET_FS and ARCH_GET_GS, we
   use a local unsigned 64bit variable to hold the base address and copy
   it to ADDR after arch_prctl return.  */

int
__arch_prctl (int code, unsigned long *addr)
{
  int res;
  unsigned long long base_addr;
  unsigned long *addr_saved;

  switch (code)
    {
    case ARCH_GET_FS:
    case ARCH_GET_GS:
      addr_saved = addr;
      addr = &base_addr;
      break;

    default:
      break;
    }

  res = INLINE_SYSCALL (arch_prctl, 2, code, addr);
  if (res == 0)
    switch (code)
      {
      case ARCH_GET_FS:
      case ARCH_GET_GS:
	*addr_saved = (unsigned long) base_addr;
	break;

      default:
	break;
      }

  return res;
}

weak_alias (__arch_prctl, arch_prctl);
