/* Set the default attributes to be used by pthread_create in the process.
   Copyright (C) 2013 Free Software Foundation, Inc.
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
#include <stdlib.h>
#include <pthreadP.h>
#include <assert.h>

int
pthread_attr_set_default_np (const pthread_attr_t *in)
{
  struct pthread_attr *real_in;

  assert (sizeof (*in) >= sizeof (struct pthread_attr));
  real_in = (struct pthread_attr *) in;

  /* Catch invalid values.  */
  int policy = real_in->schedpolicy;

  if (policy != SCHED_OTHER && policy != SCHED_FIFO && policy != SCHED_RR)
    return EINVAL;

  struct sched_param *param = &real_in->schedparam;

  if (param->sched_priority > 0)
    {
      int min = sched_get_priority_min (policy);
      int max = sched_get_priority_max (policy);

      if (min == -1 || max == -1
	  || param->sched_priority > max || param->sched_priority < min)
	return EINVAL;
    }

  /* stacksize == 0 is fine.  It means that we don't change the current
     value.  */
  if (real_in->stacksize != 0 && real_in->stacksize < PTHREAD_STACK_MIN)
    return EINVAL;

  /* Everything is fine, so the values.  */
  default_attr.schedparam = real_in->schedparam;
  default_attr.schedpolicy = real_in->schedpolicy;
  default_attr.flags = real_in->flags;
  default_attr.guardsize = real_in->guardsize;
  default_attr.stackaddr = real_in->stackaddr;
  default_attr.cpuset = real_in->cpuset;
  default_attr.cpusetsize = real_in->cpusetsize;

  /* Mantain stacksize as a non-zero value.  This is a computed fallback that
     we get on library initialization, so we don't want to overwrite it unless
     there is a valid value to replace it.  */
  if (real_in->stacksize > 0)
    default_attr.stacksize = real_in->stacksize;

  return 0;
}
