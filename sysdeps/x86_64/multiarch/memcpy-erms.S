/* memcpy with Enhanced REP MOVSB/STOSB
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

#if !defined NOT_IN_libc \
    && (defined SHARED \
        || defined USE_AS_MEMMOVE \
	|| !defined USE_MULTIARCH)

#  include "asm-syntax.h"

#  ifndef MEMCPY
#   define MEMCPY	__memcpy_erms
#   define MEMCPY_CHK	__memcpy_chk_erms
#  endif

	.section .text.erms,"ax",@progbits
# if !defined USE_AS_BCOPY
ENTRY (MEMCPY_CHK)
	cmpq	%rdx, %rcx
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (MEMCPY_CHK)
# endif

ENTRY (MEMCPY)
	mov	%rdi, %rax
	mov	%rdx, %rcx
# ifdef USE_AS_MEMPCPY
	add	%rdx, %rax
# endif

# ifdef USE_AS_MEMMOVE
	cmp	%rsi, %rdi
	ja	L(copy_backward)
	je	L(bwd_write_0bytes)
# endif

	rep movsb
	ret

# ifdef USE_AS_MEMMOVE
L(copy_backward):
	lea	-1(%rdi,%rdx), %rdi
	lea	-1(%rsi,%rdx), %rsi
	std
	rep movsb
	cld
L(bwd_write_0bytes):
	ret
# endif

END (MEMCPY)

#endif
