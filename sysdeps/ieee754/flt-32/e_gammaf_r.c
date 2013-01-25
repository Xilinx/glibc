/* Implementation of gamma function according to ISO C.
   Copyright (C) 1997-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#include <math.h>
#include <math_private.h>


float
__ieee754_gammaf_r (float x, int *signgamp)
{
  /* We don't have a real gamma implementation now.  We'll use lgamma
     and the exp function.  But due to the required boundary
     conditions we must check some values separately.  */
  int32_t hx;

  GET_FLOAT_WORD (hx, x);

  if (__builtin_expect ((hx & 0x7fffffff) == 0, 0))
    {
      /* Return value for x == 0 is Inf with divide by zero exception.  */
      *signgamp = 0;
      return 1.0 / x;
    }
  if (__builtin_expect (hx < 0, 0)
      && (u_int32_t) hx < 0xff800000 && __rintf (x) == x)
    {
      /* Return value for integer x < 0 is NaN with invalid exception.  */
      *signgamp = 0;
      return (x - x) / (x - x);
    }
  if (__builtin_expect (hx == 0xff800000, 0))
    {
      /* x == -Inf.  According to ISO this is NaN.  */
      *signgamp = 0;
      return x - x;
    }

  /* XXX FIXME.  */
  return __ieee754_expf (__ieee754_lgammaf_r (x, signgamp));
}
strong_alias (__ieee754_gammaf_r, __gammaf_r_finite)
