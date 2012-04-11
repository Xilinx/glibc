/* Copyright (C) 2012 Free Software Foundation, Inc.
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

#ifndef _LINUX_X32_SYSDEP_H
#define _LINUX_X32_SYSDEP_H 1

/* There is some commonality.  */
#include <sysdeps/unix/sysv/linux/x86_64/sysdep.h>
#include <sysdeps/x86_64/x32/sysdep.h>

#ifdef __ASSEMBLER__

# ifdef SYSCALL_RETURN_INT64
#  define SYSCALL_SET_ERROR_RETURN orq $-1, %rax
# else
#  define SYSCALL_SET_ERROR_RETURN orl $-1, %eax
# endif

# ifndef PIC
/* Nothing here.  */
# elif RTLD_PRIVATE_ERRNO
#  undef SYSCALL_ERROR_HANDLER
#  define SYSCALL_ERROR_HANDLER			\
0:						\
  lea rtld_errno(%rip), %ecx;			\
  xorl %edx, %edx;				\
  subl %eax, %edx;				\
  movl %edx, (%rcx);				\
  SYSCALL_SET_ERROR_RETURN;			\
  jmp L(pseudo_end);
# else
#  undef SYSCALL_ERROR_HANDLER
#  define SYSCALL_ERROR_HANDLER			\
0:						\
  movq SYSCALL_ERROR_ERRNO@GOTTPOFF(%rip), %rcx;\
  xorl %edx, %edx;				\
  subl %eax, %edx;				\
  movl %edx, %fs:(%rcx);			\
  SYSCALL_SET_ERROR_RETURN;			\
  jmp L(pseudo_end);
# endif	/* PIC */

#endif	/* __ASSEMBLER__ */

#endif /* linux/x86_64/x32/sysdep.h */
