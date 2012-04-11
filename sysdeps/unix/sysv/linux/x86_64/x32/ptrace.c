/* ptrace call for Linux/x32.
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
#include <sys/types.h>
#define ptrace __redirect_ptrace
#include <sys/ptrace.h>
#undef ptrace
#include <sys/syscall.h>
#include <sysdep.h>

#ifndef PTRACE_ARCH_PRCTL
#define PTRACE_ARCH_PRCTL      30
#endif

/* Since x32 ptrace stores 32bit base address of segment register %fs
   and %gs as unsigned 64bit value via ARCH_GET_FS and ARCH_GET_GS with
   PTRACE_ARCH_PRCTL, we use a local unsigned 64bit variable to hold
   the base address and copy it to ADDR after ptrace return.  */

long
ptrace (enum __ptrace_request request, pid_t pid, void *addr, void *data)
{
  long res, ret;
  unsigned long long base_addr;
  unsigned long *addr_saved;

  switch ((int) request)
    {
    case PTRACE_PEEKTEXT:
    case PTRACE_PEEKDATA:
    case PTRACE_PEEKUSER:
      /* Pass the address of return data to kernel.  */
      data = &ret;
      break;

    case PTRACE_ARCH_PRCTL:
      switch ((int) data)
	{
	case ARCH_GET_FS:
	case ARCH_GET_GS:
	  addr_saved = (unsigned long *) addr;
	  addr = &base_addr;
	  break;

	default:
	  break;
	}
      break;

    default:
      break;
    }

  res = INLINE_SYSCALL (ptrace, 4, request, pid, addr, data);
  if (res >= 0)
    switch ((int) request)
      {
      case PTRACE_PEEKTEXT:
      case PTRACE_PEEKDATA:
      case PTRACE_PEEKUSER:
	__set_errno (0);
	return ret;

      case PTRACE_ARCH_PRCTL:
	switch ((int) data)
	  {
	  case ARCH_GET_FS:
	  case ARCH_GET_GS:
	    *addr_saved = (unsigned long) base_addr;
	    break;

	  default:
	    break;
	  }
	break;

      default:
	break;
      }

  return res;
}
