/* Verify that pthread_attr_[gs]et_default work correctly.

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

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static size_t stacksize = 1024 * 1024;

void *
verify_result (pthread_attr_t *attr)
{
  int ret;
  size_t stack;

  if ((ret = pthread_attr_getstacksize (attr, &stack)) != 0)
    {
      printf ("pthread_attr_getstacksize failed: %s\n", strerror (ret));
      return (void *) (uintptr_t) 1;
    }

  if (stacksize != stack)
    {
      printf ("pthread_attr_set_default failed for stacksize (%zu, %zu)\n",
	      stacksize, stack);
      return (void *) (uintptr_t) 1;
    }

  return NULL;
}

void *
thr (void *unused)
{
  pthread_attr_t attr;
  int ret;

  /* To verify that the set_default_np worked.  */
  puts ("verifying pthread_attr_get_default_np");
  if ((ret = pthread_attr_get_default_np (&attr)) != 0)
    {
      printf ("pthread_attr_get_default_np failed: %s\n", strerror (ret));
      return (void *) (uintptr_t) 1;
    }

  if (verify_result (&attr))
    return (void *) (uintptr_t) 1;

  /* To verify that the attributes actually got applied.  */
  puts ("verifying pthread_getattr_np");
  if ((ret = pthread_getattr_np (pthread_self (), &attr)) != 0)
    {
      printf ("pthread_getattr_np failed: %s\n", strerror (ret));
      return (void *) (uintptr_t) 1;
    }

  return verify_result (&attr);
}

int
run_threads (void)
{
  pthread_t t;
  pthread_attr_t attr;
  int ret, i;
  void *tret;

  if ((ret = pthread_attr_init (&attr)) != 0)
    {
      printf ("pthread_attr_init failed: %s\n", strerror (ret));
      return 1;
    }

  if ((ret = pthread_attr_setstacksize (&attr, stacksize)) != 0)
    {
      printf ("pthread_attr_setstacksize failed: %s\n", strerror (ret));
      return 1;
    }

  if ((ret = pthread_attr_set_default_np (&attr)) != 0)
    {
      printf ("pthread_attr_set_default_np failed: %s\n", strerror (ret));
      return 1;
    }

  /* Run twice to ensure that the attributes do not get overwritten in the
     first run somehow.  */
  for (i = 0; i < 2; i++)
    {
      if ((ret = pthread_create (&t, NULL, thr, NULL)) != 0)
	{
	  printf ("pthread_create failed: %s\n", strerror (ret));
	  return 1;
	}

      if ((ret = pthread_join (t, &tret)) != 0)
	{
	  printf ("pthread_join failed: %s\n", strerror (ret));
	  return 1;
	}

      if (tret != NULL)
	{
	  puts ("Thread failed\n");
	  return 1;
	}
    }

  return 0;
}

int
do_test (void)
{
  long pagesize = sysconf (_SC_PAGESIZE);

  if (pagesize < 0)
    {
      printf ("sysconf failed: %s\n", strerror (errno));
      return 1;
    }

  /* Perturb the size by a page so that we're not aligned on the 64K boundary.
     pthread_create does this perturbation on x86 to avoid causing the 64k
     aliasing conflict.  We want to prevent pthread_create from doing that
     since it is not consistent for all architectures.  */
  stacksize += pagesize;

  /* Run twice to ensure that we don't give a false positive.  */
  puts ("First iteration");
  int ret = run_threads ();

  if (ret)
    return 1;

  puts ("Second iteration");
  stacksize *= 2;
  return run_threads ();
}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
