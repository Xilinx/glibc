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
  struct pthread_attr *real_in, attrs;
  int ret;

  assert (sizeof (*in) >= sizeof (struct pthread_attr));
  real_in = (struct pthread_attr *) in;

  /* Catch invalid values.  */
  int policy = real_in->schedpolicy;

  ret = check_sched_policy_attr (policy);
  if (ret)
    return ret;

  struct sched_param *param = &real_in->schedparam;

  if (param->sched_priority > 0)
    {
      ret = check_sched_priority_attr (param->sched_priority, policy);

      if (ret)
        return ret;
    }

  ret = check_cpuset_attr (real_in->cpuset, real_in->cpusetsize);
  if (ret)
    return ret;

  /* stacksize == 0 is fine.  It means that we don't change the current
     value.  */
  if (real_in->stacksize != 0)
    {
      ret = check_stacksize_attr (real_in->stacksize);
      if (ret)
        return ret;
    }

  /* Having a default stack address is wrong.  */
  if (real_in->flags & ATTR_FLAG_STACKADDR)
    return EINVAL;

  attrs = *real_in;

  /* Mantain stacksize as a non-zero value.  This is a computed fallback that
     we get on library initialization, so we don't want to overwrite it unless
     there is a valid value to replace it.  */
  if (real_in->stacksize > 0)
    attrs.stacksize = real_in->stacksize;

  /* Now take the lock because we start writing into
     __default_pthread_attr.  */
  lll_lock (__default_pthread_attr_lock, LLL_PRIVATE);

  /* Free the cpuset if the input is 0.  Otherwise copy in the cpuset
     contents.  */
  if (attrs.cpuset == NULL || attrs.cpusetsize == 0)
    free (__default_pthread_attr.cpuset);
  else if (attrs.cpusetsize != __default_pthread_attr.cpusetsize)
    {
      /* This may look wrong at first sight, but it isn't.  We're freeing
	 __default_pthread_attr.cpuset and allocating to attrs.cpuset because
	 we'll copy over all of attr to __default_pthread_attr later.  */
      void *newp = (cpu_set_t *) realloc (__default_pthread_attr.cpuset,
					  attrs.cpusetsize);

      if (newp == NULL)
	{
	  ret = ENOMEM;
	  goto out;
	}

      attrs.cpuset = newp;
    }
  else
    attrs.cpuset = __default_pthread_attr.cpuset;

  memcpy (attrs.cpuset, real_in->cpuset, real_in->cpusetsize);
  __default_pthread_attr = attrs;

 out:
  lll_unlock (__default_pthread_attr_lock, LLL_PRIVATE);
  return ret;
}
