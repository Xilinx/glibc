/* Copyright (C) 2012-2013 Free Software Foundation, Inc.

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

/* Used in linker - use only %xmm8-%xmm15. */

/* Long lived registers are
  %xmm11 - zero
  %rdi   - s

  strnlen(s,n):
  %r10  (s+n)&(~(64-1))
  %r11   s+n  
  strcat(s,src):
  %r10  src
  strncat(s,src,n):
  %r10  src
  %r11  n
 */


.text
ENTRY(strlen)

#ifdef AS_STRCAT
#define RETURN       jmp .cpy_str
#define RETURN_LAST
#elif defined(AS_STRNCAT)
#define RETURN       jmp .cpy_str
#define RETURN_LAST
#else
#define RETURN ret
#define RETURN_LAST ret
#endif

#define FIND_ZERO           \
  pcmpeqb   (%rax), %xmm8;  \
  pcmpeqb 16(%rax), %xmm9;  \
  pcmpeqb 32(%rax), %xmm10; \
  pcmpeqb 48(%rax), %xmm11; \
  pmovmskb  %xmm8,  %esi;   \
  pmovmskb  %xmm9,  %edx;   \
  pmovmskb  %xmm10, %r8d;   \
  pmovmskb  %xmm11, %ecx;   \
  salq  $16, %rdx;          \
  salq  $16, %rcx;          \
  orq  %rsi, %rdx;          \
  orq  %r8, %rcx;           \
  salq  $32, %rcx;          \
  orq  %rcx, %rdx;

#ifdef AS_STRNLEN
/* Do not read anything when n==0. */
  test %rsi,%rsi
  jne L(n_nonzero)
  xor %rax,%rax
  RETURN
L(n_nonzero):
#endif

/*Initialize long lived registers. */
#ifdef AS_STRCAT
  mov %rsi,%r10
#endif
#ifdef AS_STRNCAT
  mov %rsi,%r10
  mov %rdx,%r11
#endif
#ifdef AS_STRNLEN
  add  %rdi,%rsi
  mov  %rsi,%r10
  and  $-64,%r10 
  mov  %rsi,%r11
#endif


  pxor %xmm8,%xmm8
  pxor %xmm9,%xmm9
  pxor %xmm10,%xmm10
  pxor %xmm11,%xmm11
  movq  %rdi, %rax
  movq  %rdi, %rcx
  andq $4095, %rcx
  cmpq $4032, %rcx
  /* We cannot unify this branching as it would be ~6 cycles slower. */
  ja L(next)

#ifdef AS_STRNLEN
#define STRNLEN_PROLOG \
  mov   %r11,%rsi;     \
  subq  %rax,%rsi;     \
  andq  $-64,%rax;     \
  testq $-64,%rsi;     \
  je L(strnlen_ret) ;
#else 
#define STRNLEN_PROLOG  andq $-64,%rax;
#endif

#define PROLOG(lab) \
  FIND_ZERO;        \
  movq %rdi,%rcx;   \
  xorq %rax,%rcx;   \
  STRNLEN_PROLOG;   \
  sarq %cl, %rdx;   \
  test %rdx,%rdx;   \
  je L(lab);        \
  bsfq %rdx,%rax;   \
  RETURN

  andq $-16,%rax
  PROLOG(loop)

  L(next):
  andq $-64,%rax
  PROLOG(loop_init)

#ifdef AS_STRNLEN
  /*We must do this check to correctly handle strnlen(s,-1) */
  L(strnlen_ret):
  bts  %rsi ,%rdx
  sarq %cl  ,%rdx
  test %rdx ,%rdx
  je   L(loop_init)
  bsfq %rdx,%rax
  RETURN
#endif

L(loop_init):
  pxor %xmm9  ,%xmm9
  pxor %xmm10 ,%xmm10
  pxor %xmm11 ,%xmm11
#ifdef AS_STRNLEN
L(loop):

  addq $64 ,%rax
  cmpq %rax,%r10
  je   L(exit_end)

  movdqa    (%rax), %xmm8
  pminub  16(%rax), %xmm8
  pminub  32(%rax), %xmm8
  pminub  48(%rax), %xmm8
  pcmpeqb   %xmm11, %xmm8
  pmovmskb   %xmm8, %edx
  testl %edx, %edx
  jne L(exit)
  jmp L(loop)

  L(exit_end):
  cmp %rax  , %r11
  je L(first)
  pxor %xmm8, %xmm8
  FIND_ZERO
  L(first):
  bts   %r11, %rdx

  bsfq  %rdx, %rdx       
  addq  %rdx, %rax       
  subq  %rdi, %rax       
  RETURN  
 
  L(exit):
  pxor %xmm8, %xmm8
  FIND_ZERO

  bsfq  %rdx, %rdx       
  addq  %rdx, %rax       
  subq  %rdi, %rax       
  RETURN_LAST

#else
L(loop):

  movdqa  64(%rax), %xmm8
  pminub  80(%rax), %xmm8
  pminub  96(%rax), %xmm8
  pminub 112(%rax), %xmm8
  pcmpeqb   %xmm11, %xmm8
  pmovmskb   %xmm8, %edx
  testl  %edx, %edx
  jne  L(exit64)

  subq  $-128, %rax

  movdqa    (%rax), %xmm8
  pminub  16(%rax), %xmm8
  pminub  32(%rax), %xmm8
  pminub  48(%rax), %xmm8
  pcmpeqb   %xmm11, %xmm8
  pmovmskb   %xmm8, %edx
  testl  %edx, %edx
  jne  L(exit0)
  jmp  L(loop)

  L(exit64):
  addq  $64, %rax
  L(exit0):
  pxor %xmm8,%xmm8
  FIND_ZERO

  bsfq  %rdx, %rdx       
  addq  %rdx, %rax       
  subq  %rdi, %rax       
  RETURN_LAST
  
#endif  

#if !(defined(AS_STRCAT) || defined(AS_STRNCAT))
END(strlen)
libc_hidden_builtin_def (strlen)
weak_alias(strlen,__strlen_sse2)
#endif
