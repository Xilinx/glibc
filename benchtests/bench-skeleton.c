/* Skeleton for benchmark programs.
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

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

int
main (int argc, char **argv)
{
  int i, j;
  int64_t total = 0, max = 0, min = 0x7fffffffffffffff;
  struct timespec start, end;

  memset (&start, 0, sizeof (start));
  memset (&end, 0, sizeof (end));

  for (i = 0; i < ITER; i++)
    {
      for (j = 0; j < NUM_SAMPLES; j++)
	{
	  clock_gettime (CLOCK_MONOTONIC_RAW, &start);
	  BENCH_FUNC(j);
	  clock_gettime (CLOCK_MONOTONIC_RAW, &end);

	  int64_t cur = (end.tv_nsec - start.tv_nsec
			 + (end.tv_sec - start.tv_sec) * 1000000000);

	  if (cur > max)
	    max = cur;

	  if (cur < min)
	    min = cur;

	  total += cur;
	}
    }

  printf (FUNCNAME ": TOTAL: %"PRId64"ns, MAX: %"PRId64"ns, "
	  "MIN: %"PRId64"ns, AVG: %lfns\n", total, max, min,
	  (double) total / (ITER * NUM_SAMPLES));
}
