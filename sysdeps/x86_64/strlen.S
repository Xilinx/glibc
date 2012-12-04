/* Copyright (C) 2012 Free Software Foundation, Inc.

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

#include <sysdep.h> 

.text
ENTRY(strlen)

#ifdef AS_STRCAT
#define RETURN jmp .cpy_str
#elif defined(AS_STRNCAT)
#define RETURN jmp .cpy_str
mov %rdx,%r9
#else
#define RETURN ret
#endif

#ifdef AS_STRNLEN
test %rsi,%rsi
jne .l1
xor %rax,%rax
RETURN
.l1:

#define STRNLEN_PROLOG(lab)\
	mov %rsi,%r8;\
	andq $-64,%rax;\
	addq %rdi,%rsi;\
	subq %rax,%rsi;\
	addq %rcx,%r8;\
	testq %r9,%r8;\
	jne .##lab ;\
	bts %r8,%rdx;\
	.##lab##:
#else 
#define STRNLEN_PROLOG(lab) andq $-64,%rax
#endif

#define FIND_ZERO \
	pcmpeqb (%rax),%xmm8 ;\
	pcmpeqb 16(%rax),%xmm9;\
	pmovmskb	%xmm8, %r8d;\
	pcmpeqb 32(%rax),%xmm10;\
	pmovmskb	%xmm9, %edx;\
	pcmpeqb 48(%rax),%xmm11;\
	salq	$16, %rdx;\
	pmovmskb	%xmm10, %r10d;\
  pmovmskb	%xmm11, %ecx;\
	salq	$16, %rcx;\
	orq	%r8, %rdx;\
	orq	%r10, %rcx;\
	salq	$32, %rcx;\
	orq	%rcx, %rdx;


#define PROLOG(lab) \
	FIND_ZERO;\
	movq %rdi, %rcx;\
	xorq %rax,%rcx;\
	STRNLEN_PROLOG(lab);\
	sarq %cl,%rdx;\
  test %rdx, %rdx;\


	# strlen spends 99% time on first 80 characters which we optimize
	movq	%rdi, %rax
	pxor %xmm8,%xmm8
  movq  %rdi, %r8
	pxor %xmm9,%xmm9
  andq $4095, %r8
	pxor %xmm10,%xmm10
	pxor %xmm11,%xmm11
#ifdef AS_STRNLEN
	movq $-64, %r9
#endif
  cmpq $4032, %r8
#ifndef LINE_ALIGNED_START
	# we cannot unify this branch with next as it is ~6 cycles slower.
  ja .next
	andq $-16,%rax
	PROLOG(fall1)
	je	.L16
	bsfq	%rdx, %rax
	RETURN
	.next:
#endif

	andq $-64,%rax
	PROLOG(fall2)
	pxor %xmm11,%xmm11
	je	.L16
	bsfq	%rdx, %rax
	RETURN

	#.p2align 4,,10
	#.p2align 3
.L19:
	addq	$64, %rax
.L17:
	pxor %xmm8,%xmm8
	pxor %xmm9,%xmm9
	pxor %xmm10,%xmm10
	pxor %xmm11,%xmm11
#ifdef AS_STRNLEN
	testq %rsi,%rsi
	jne .fall3
	subq %rdi,%rax
	RETURN
	.fall3:
	FIND_ZERO
	testq %r9,%rsi
	jne .fall4
	bts %rsi,%rdx
	.fall4:
#else
	FIND_ZERO
#endif

	bsfq	%rdx, %rdx
	addq	%rdx, %rax
	subq	%rdi, %rax
	RETURN	

	#.p2align 4,,10
	#.p2align 3
.L16:
	#ifdef AS_STRNLEN
	addq %r9,%rsi
	testq %r9,%rsi
	je .L19
	#endif	
	#prefetcht0	576(%rax)
	movdqa	64(%rax), %xmm8
	pminub	80(%rax), %xmm8
	pminub	96(%rax), %xmm8
	pminub 112(%rax), %xmm8
	pcmpeqb	%xmm11, %xmm8
	pmovmskb	%xmm8, %edx
	testl	%edx, %edx
	jne	.L19
	subq	$-128, %rax
	#ifdef AS_STRNLEN
	addq %r9,%rsi
	testq %r9,%rsi
	je .L17
	#endif
	#prefetcht0	512(%rax)
	movdqa	  (%rax), %xmm8
	pminub	16(%rax), %xmm8
	pminub	32(%rax), %xmm8
	pminub  48(%rax), %xmm8
	pcmpeqb	%xmm11, %xmm8
	pmovmskb	%xmm8, %edx
	testl	%edx, %edx
	je	.L16
	jmp	.L17
END(strlen)
libc_hidden_builtin_def (strlen)
weak_alias(strlen,__strlen_sse2)
