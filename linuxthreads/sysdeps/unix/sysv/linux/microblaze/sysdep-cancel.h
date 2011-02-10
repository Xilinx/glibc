/* The GNU C Library is free software; you can redistribute it and/or
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
#ifndef __ASSEMBLER__
# include <linuxthreads/internals.h>
#endif

#if !defined NOT_IN_libc || defined IS_IN_libpthread || defined IS_IN_librt

#if !defined(IS_IN_librt) || !defined(PIC)
#define AC_STACK_SIZE	16	/* space for r15, async_cancel arg and 2 temp words */
#define AC_SET_GOT	/* empty */
#define AC_RESTORE_GOT	/* empty */
#else /* __librt_async_cancel is defined in libc, so we have to call via PLT */
#define AC_STACK_SIZE	20	/* extra 4 bytes for r20 */
#define AC_SET_GOT	\
    swi   r20,r1,AC_STACK_SIZE-4;		\
    mfs   r20,rpc;				\
    addik r20,r20,_GLOBAL_OFFSET_TABLE_+8;
#define AC_RESTORE_GOT	\
    lwi   r20,r1,AC_STACK_SIZE-4;
#endif

# undef PSEUDO
# define PSEUDO(name, syscall_name, args)	\
  .text;					\
  ENTRY (name)					\
    SINGLE_THREAD_P;				\
    bnei r12,L(pseudo_cancel);			\
    DO_CALL (syscall_name, args);		\
    addik r4,r0,-4095;				\
    cmpu  r4,r4,r3;				\
    bgei  r4,SYSCALL_ERROR_LABEL;		\
    rtsd  r15,8;				\
    nop;					\
L(pseudo_cancel):				\
    addik r1,r1,-AC_STACK_SIZE;			\
    swi   r15,r1,0;				\
    AC_SET_GOT					\
    DOCARGS_##args				\
    CENABLE;					\
    swi   r3,r1,8;				\
    UNDOCARGS_##args				\
    DO_CALL (syscall_name, args);		\
    swi   r3,r1,12;				\
    lwi   r5,r1,8;				\
    CDISABLE;					\
    lwi   r3,r1,12;				\
    lwi   r15,r1,0;				\
    AC_RESTORE_GOT				\
    addik r1,r1,AC_STACK_SIZE;			\
    addik r4,r0,-4095;				\
    cmpu  r4,r4,r3;				\
    bgei  r4,SYSCALL_ERROR_LABEL;		\
    rtsd  r15,8;				\
    nop;					\

/* 
 * Macros to save/restore syscall arguments across CENABLE
 * The arguments are saved into the caller's stack (original r1 + 4)
 */

#define DOCARGS_0
#define DOCARGS_1	swi   r5,r1,AC_STACK_SIZE + 4;
#define DOCARGS_2	swi   r6,r1,AC_STACK_SIZE + 8; DOCARGS_1
#define DOCARGS_3	swi   r7,r1,AC_STACK_SIZE + 12; DOCARGS_2
#define DOCARGS_4	swi   r8,r1,AC_STACK_SIZE + 16; DOCARGS_3
#define DOCARGS_5	swi   r9,r1,AC_STACK_SIZE + 20; DOCARGS_4
#define DOCARGS_6	swi   r10,r1,AC_STACK_SIZE + 24; DOCARGS_5

#define UNDOCARGS_0
#define UNDOCARGS_1	lwi   r5,r1,AC_STACK_SIZE + 4;
#define UNDOCARGS_2	UNDOCARGS_1 lwi   r6,r1,AC_STACK_SIZE + 8;
#define UNDOCARGS_3	UNDOCARGS_2 lwi   r7,r1,AC_STACK_SIZE + 12;
#define UNDOCARGS_4	UNDOCARGS_3 lwi   r8,r1,AC_STACK_SIZE + 16;
#define UNDOCARGS_5	UNDOCARGS_4 lwi   r9,r1,AC_STACK_SIZE + 20;
#define UNDOCARGS_6	UNDOCARGS_5 lwi   r10,r1,AC_STACK_SIZE + 24;

/* FIXME: do we need a separate call format (via PLT) for PIC??? */
# ifdef IS_IN_libpthread
#   define CENABLE	brlid r15,__pthread_enable_asynccancel; addk r0,r0,r0
#   define CDISABLE	brlid r15,__pthread_disable_asynccancel; addk r0,r0,r0
# elif !defined NOT_IN_libc
#   define CENABLE	brlid r15,__libc_enable_asynccancel; addk r0,r0,r0
#   define CDISABLE	brlid r15,__libc_disable_asynccancel; addk r0,r0,r0
# else
#   ifdef PIC
#     define CENABLE	brlid r15,__librt_enable_asynccancel@PLT; addk r0,r0,r0
#     define CDISABLE	brlid r15,__librt_disable_asynccancel@PLT; addk r0,r0,r0
#   else
#     define CENABLE	brlid r15,__librt_enable_asynccancel; addk r0,r0,r0
#     define CDISABLE	brlid r15,__librt_disable_asynccancel; addk r0,r0,r0
#   endif /* PIC */
# endif

# if !defined NOT_IN_libc
#  define __local_multiple_threads __libc_multiple_threads
# elif defined IS_IN_libpthread
#  define __local_multiple_threads __pthread_multiple_threads
# else
#  define __local_multiple_threads __librt_multiple_threads
# endif

# ifndef __ASSEMBLER__
extern int __local_multiple_threads attribute_hidden;
#  define SINGLE_THREAD_P __builtin_expect (__local_multiple_threads == 0, 1)
# else
#  if !defined PIC
#   define SINGLE_THREAD_P lwi r12,r0,__local_multiple_threads;
#  else
#   define SINGLE_THREAD_P \
    mfs   r12,rpc;				\
    addik r12,r12,_GLOBAL_OFFSET_TABLE_+8;	\
    lwi   r12,r12,__local_multiple_threads@GOT;	\
    lwi   r12,r12,0;
#  endif
# endif

#elif !defined __ASSEMBLER__

/* This code should never be used but we define it anyhow.  */
# define SINGLE_THREAD_P (1)

#endif
