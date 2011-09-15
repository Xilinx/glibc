/* memset with Enhanced REP MOVSB/STOSB
   Copyright (C) 2011 Free Software Foundation, Inc.
   Contributed by Intel Corporation.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sysdep.h>

#ifndef NOT_IN_libc

	.text
# ifdef SHARED
ENTRY (__memset_chk_erms)
	cmpq	%rdx, %rcx
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (__memset_chk_erms)
# endif

ENTRY (__memset_erms)
# ifdef USE_AS_BZERO_P
	mov	%rsi, %rcx
	xor	%eax, %eax
# else
	mov	%rdx, %rcx
	movzbl	%sil, %eax
	mov	%rdi, %rdx
# endif
	rep stosb
# ifndef USE_AS_BZERO_P
	mov %rdx, %rax
# endif
	ret
END (__memset_erms)

#endif
