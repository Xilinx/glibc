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

#include <sysdep-cancel.h>
#define _ERRNO_H	1
#include <bits/errno.h>
#include <kernel-features.h>

/* Clone the calling process, but without copying the whole address space.
   The calling process is suspended until the new process exits or is
   replaced by a call to `execve'.  Return -1 for errors, 0 to the new process,
   and the process ID of the new process to the old process.  */

ENTRY (__vfork)

#ifdef SHARED
	mfs	r4,rpc
	addik	r4,r4,_GLOBAL_OFFSET_TABLE_+8
	lwi	r3,r4,__libc_pthread_functions@GOT
#else
	.weak	pthread_create
	addik	r3,r0,pthread_create
#endif
	bnei	r3,HIDDEN_JUMPTARGET (__fork)
	addik	r12,r0,SYS_ify(vfork)
	brki	r14,8
	nop
	addik	r12,r0,-4095
	cmpu	r12,r12,r3
	bgei	r12,SYSCALL_ERROR_LABEL
	rtsd	r15,8
	nop

PSEUDO_END (__vfork)
libc_hidden_def (__vfork)

weak_alias (__vfork, vfork)
