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


long double
__ieee754_gammal_r (long double x, int *signgamp)
{
  /* We don't have a real gamma implementation now.  We'll use lgamma
     and the exp function.  But due to the required boundary
     conditions we must check some values separately.  */
  u_int32_t es, hx, lx;

  GET_LDOUBLE_WORDS (es, hx, lx, x);

  if (__builtin_expect (((es & 0x7fff) | hx | lx) == 0, 0))
    {
      /* Return value for x == 0 is Inf with divide by zero exception.  */
      *signgamp = 0;
      return 1.0 / x;
    }
  if (__builtin_expect (es == 0xffffffff && ((hx & 0x7fffffff) | lx) == 0, 0))
    {
      /* x == -Inf.  According to ISO this is NaN.  */
      *signgamp = 0;
      return x - x;
    }
  if (__builtin_expect ((es & 0x7fff) == 0x7fff, 0)
      && ((hx & 0x7fffffff) | lx) != 0)
    {
      /* NaN, return it.  */
      *signgamp = 0;
      return x;
    }
  if (__builtin_expect ((es & 0x8000) != 0, 0)
      && x < 0xffffffff && __rintl (x) == x)
    {
      /* Return value for integer x < 0 is NaN with invalid exception.  */
      *signgamp = 0;
      return (x - x) / (x - x);
    }

  /* XXX FIXME.  */
  return __ieee754_expl (__ieee754_lgammal_r (x, signgamp));
}
strong_alias (__ieee754_gammal_r, __gammal_r_finite)
