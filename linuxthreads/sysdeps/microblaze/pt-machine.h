/* Machine-dependent pthreads configuration and inline functions.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef _PT_MACHINE_H
#define _PT_MACHINE_H   1

#ifndef PT_EI
# define PT_EI extern inline __attribute__ ((always_inline))
#endif

extern long int testandset (int *spinlock);
extern int __compare_and_swap (long int *p, long int oldval, long int newval);

#if 0
/* Use compare_and_swap and test one implementation not two - the same as ppc */
#define IMPLEMENT_TAS_WITH_CAS
#define HAS_COMPARE_AND_SWAP

PT_EI int
__compare_and_swap (long int *p, long int oldval, long int newval)
{
	int t, test;

/* NOTE MS: Microblaze version below 7.20.a has no lwx/swx instruction
 * and they behave as lw/sw. lw/sw are not able to setup carry bit that's why
 * code has to automatically do it by addc. */
	__asm__ __volatile__(
	"	addc	r0, r0, r0;"	/* clean carry bit*/\
	"1:	lwx	%0, %4, r0;"	/* atomic_add_return */\
	"	cmp	%1, %0, %2;"	/* compare signed values current/old */\
	"	bnei	%1, 2f;"	/* if is not equal - jump over */\
	"	swx	%3, %4, r0;"	/* save newval */\
	"	addic	%1, r0, 0;"	/* check Carry if saving was OK */\
	"	bnei	%1, 1b;"	/* jump if swx wasn't successful */\
	"2:"
		: "=&r" (t),
		"=&r" (test)
		: "r" (oldval),
		"r" (newval),
		"r" (p)
		: "cc", "memory");

	return t == oldval;
}
#else
PT_EI long int
testandset (int *spinlock)
{
	int t, test;

/* NOTE MS: Microblaze version below 7.20.a has no lwx/swx instruction
 * and they behave as lw/sw. lw/sw are not able to setup carry bit that's why
 * code has to automatically do it by addc. */
	__asm__ __volatile__(
	"	addc	r0, r0, r0;"	/* clean carry bit*/\
	"1:	lwx	%0, %3, r0;"	/* atomic_add_return*/\
	"	swx	%2, %3, r0;"	/* save value back*/\
	"	addic	%1, r0, 0;"	/* check Carry if saving was OK*/\
	"	bnei	%1, 1b;"	/* jump if swx wasn't successful*/\
		: "=&r" (t),
		"=&r" (test)
		: "r" (1),
		"r" (spinlock)
		: "cc", "memory");

	return t;
}
#endif

/* Get some notion of the current stack.  Need not be exactly the top
   of the stack, just something somewhere in the current frame.  */
#define CURRENT_STACK_FRAME  stack_pointer
register char * stack_pointer __asm__ ("%r1");


#endif /* pt-machine.h */
