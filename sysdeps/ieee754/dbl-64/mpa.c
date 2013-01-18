/*
 * IBM Accurate Mathematical Library
 * written by International Business Machines Corp.
 * Copyright (C) 2001-2013 Free Software Foundation, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
/************************************************************************/
/*  MODULE_NAME: mpa.c                                                  */
/*                                                                      */
/*  FUNCTIONS:                                                          */
/*               mcr                                                    */
/*               acr                                                    */
/*               cpy                                                    */
/*               norm                                                   */
/*               denorm                                                 */
/*               mp_dbl                                                 */
/*               dbl_mp                                                 */
/*               add_magnitudes                                         */
/*               sub_magnitudes                                         */
/*               add                                                    */
/*               sub                                                    */
/*               mul                                                    */
/*               inv                                                    */
/*               dvd                                                    */
/*                                                                      */
/* Arithmetic functions for multiple precision numbers.                 */
/* Relative errors are bounded                                          */
/************************************************************************/


#include "endian.h"
#include "mpa.h"
#include <sys/param.h>
#include <alloca.h>

#ifndef SECTION
# define SECTION
#endif

#ifndef NO__CONST
const mp_no mpone = {1, {1.0, 1.0}};
const mp_no mptwo = {1, {1.0, 2.0}};
#endif

#ifndef NO___ACR
/* Compare mantissa of two multiple precision numbers regardless of the sign
   and exponent of the numbers.  */
static int
mcr (const mp_no *x, const mp_no *y, int p)
{
  int i;
  for (i = 1; i <= p; i++)
    {
      if (X[i] == Y[i])
	continue;
      else if (X[i] > Y[i])
	return 1;
      else
	return -1;
    }
  return 0;
}

/* Compare the absolute values of two multiple precision numbers.  */
int
__acr (const mp_no *x, const mp_no *y, int p)
{
  int i;

  if (X[0] == 0)
    {
      if (Y[0] == 0)
	i = 0;
      else
	i = -1;
    }
  else if (Y[0] == 0)
    i = 1;
  else
    {
      if (EX > EY)
	i = 1;
      else if (EX < EY)
	i = -1;
      else
	i = mcr (x, y, p);
    }

  return i;
}
#endif

#ifndef NO___CPY
/* Copy multiple precision number X into Y.  They could be the same
   number.  */
void
__cpy (const mp_no *x, mp_no *y, int p)
{
  EY = EX;
  for (int i = 0; i <= p; i++)
    Y[i] = X[i];
}
#endif

#ifndef NO___MP_DBL
/* Convert a multiple precision number *X into a double precision
   number *Y, normalized case  (|x| >= 2**(-1022))).  */
static void
norm (const mp_no *x, double *y, int p)
{
#define R  RADIXI
  int i;
  double c;
  int a, z[5];

  if (p < 5)
    {
      if (p == 1)
	c = X[1];
      else if (p == 2)
	c = X[1] + R * X[2];
      else if (p == 3)
	c = X[1] + R * (X[2] + R * X[3]);
      else if (p == 4)
	c = (X[1] + R * X[2]) + R * R * (X[3] + R * X[4]);
    }
  else
    {
      for (a = 1, z[1] = X[1]; z[1] < TWOPOW(23); )
	{
	  a *= 2;
	  z[1] *= 2;
	}

      for (i = 2; i < 5; i++)
	{
	  int64_t tmp = (int64_t) X[i] * a;

	  z[i] = tmp % I_RADIX;
	  z[i-1] += tmp / I_RADIX;
	}

      if (__glibc_unlikely ((z[3] & (TWOPOW(18) - 1)) == 0))
	{
	  if (z[4] == 0)
	    {
	      for (i = 5; i <= p; i++)
		{
		  if (X[i] == 0)
		    continue;
		  else
		    {
		      z[3]++;
		      break;
		    }
		}
	    }
	  else
	    z[3]++;
	}

      c = (z[1] + R * (z[2] + R * z[3])) / a;
    }

  c *= X[0];

  for (i = 1; i < EX; i++)
    c *= RADIX;
  for (i = 1; i > EX; i--)
    c *= RADIXI;

  *y = c;
#undef R
}

/* Convert a multiple precision number *X into a double precision
   number *Y, Denormal case  (|x| < 2**(-1022))).  */
static void
denorm (const mp_no *x, double *y, int p)
{
  int i, k;
  double c, u, z[5];

#define R  RADIXI
  if (EX < -44 || (EX == -44 && X[1] < TWO5))
    {
      *y = ZERO;
      return;
    }

  if (p == 1)
    {
      if (EX == -42)
	{
	  z[1] = X[1] + TWO10;
	  z[2] = ZERO;
	  z[3] = ZERO;
	  k = 3;
	}
      else if (EX == -43)
	{
	  z[1] = TWO10;
	  z[2] = X[1];
	  z[3] = ZERO;
	  k = 2;
	}
      else
	{
	  z[1] = TWO10;
	  z[2] = ZERO;
	  z[3] = X[1];
	  k = 1;
	}
    }
  else if (p == 2)
    {
      if (EX == -42)
	{
	  z[1] = X[1] + TWO10;
	  z[2] = X[2];
	  z[3] = ZERO;
	  k = 3;
	}
      else if (EX == -43)
	{
	  z[1] = TWO10;
	  z[2] = X[1];
	  z[3] = X[2];
	  k = 2;
	}
      else
	{
	  z[1] = TWO10;
	  z[2] = ZERO;
	  z[3] = X[1];
	  k = 1;
	}
    }
  else
    {
      if (EX == -42)
	{
	  z[1] = X[1] + TWO10;
	  z[2] = X[2];
	  k = 3;
	}
      else if (EX == -43)
	{
	  z[1] = TWO10;
	  z[2] = X[1];
	  k = 2;
	}
      else
	{
	  z[1] = TWO10;
	  z[2] = ZERO;
	  k = 1;
	}
      z[3] = X[k];
    }

  u = (z[3] + TWO57) - TWO57;
  if (u > z[3])
    u -= TWO5;

  if (u == z[3])
    {
      for (i = k + 1; i <= p; i++)
	{
	  if (X[i] == ZERO)
	    continue;
	  else
	    {
	      z[3] += ONE;
	      break;
	    }
	}
    }

  c = X[0] * ((z[1] + R * (z[2] + R * z[3])) - TWO10);

  *y = c * TWOM1032;
#undef R
}

/* Convert multiple precision number *X into double precision number *Y.  The
   result is correctly rounded to the nearest/even.  */
void
__mp_dbl (const mp_no *x, double *y, int p)
{
  if (X[0] == 0)
    {
      *y = ZERO;
      return;
    }

  if (__glibc_likely (EX > -42 || (EX == -42 && X[1] >= TWOPOW(10))))
    norm (x, y, p);
  else
    denorm (x, y, p);
}
#endif

/* Get the multiple precision equivalent of X into *Y.  If the precision is too
   small, the result is truncated.  */
void
SECTION
__dbl_mp (double x, mp_no *y, int p)
{
  int i, n;

  /* Sign.  */
  if (x == ZERO)
    {
      Y[0] = 0;
      return;
    }
  else if (x > ZERO)
    Y[0] = 1;
  else
    {
      Y[0] = -1;
      x = -x;
    }

  /* Exponent.  */
  for (EY = 1; x >= RADIX; EY++)
    x *= RADIXI;
  for (; x < ONE; EY--)
    x *= RADIX;

  /* Digits.  */
  n = MIN (p, 4);
  for (i = 1; i <= n; i++)
    {
      Y[i] = x;
      x = (x - Y[i]) * RADIX;
    }
  for (; i <= p; i++)
    Y[i] = 0;
}

/* Add magnitudes of *X and *Y assuming that abs (*X) >= abs (*Y) > 0.  The
   sign of the sum *Z is not changed.  X and Y may overlap but not X and Z or
   Y and Z.  No guard digit is used.  The result equals the exact sum,
   truncated.  */
static void
SECTION
add_magnitudes (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  int i, j, k;

  EZ = EX;

  i = p;
  j = p + EY - EX;
  k = p + 1;

  if (j < 1)
    {
      __cpy (x, z, p);
      return;
    }
  else
    Z[k] = 0;

  for (; j > 0; i--, j--)
    {
      int tmp = Z[k] + X[i] + Y[j];
      Z[k] = tmp % I_RADIX;
      Z[--k] = tmp / I_RADIX;
    }

  for (; i > 0; i--)
    {
      int tmp = Z[k] + X[i];
      Z[k] = tmp % I_RADIX;
      Z[--k] = tmp / I_RADIX;
    }

  if (Z[1] == 0)
    {
      for (i = 1; i <= p; i++)
	Z[i] = Z[i + 1];
    }
  else
    EZ++;
}

/* Subtract the magnitudes of *X and *Y assuming that abs (*x) > abs (*y) > 0.
   The sign of the difference *Z is not changed.  X and Y may overlap but not X
   and Z or Y and Z.  One guard digit is used.  The error is less than one
   ULP.  */
static void
SECTION
sub_magnitudes (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  int i, j, k;

  EZ = EX;

  if (EX == EY)
    {
      i = j = k = p;
      Z[k] = Z[k + 1] = 0;
    }
  else
    {
      j = EX - EY;
      if (j > p)
	{
	  __cpy (x, z, p);
	  return;
	}
      else
	{
	  int tmp;

	  i = p;
	  j = p + 1 - j;
	  k = p;

	  tmp = I_RADIX - Y[j];
	  Z[k + 1] = tmp % I_RADIX;
	  Z[k] = tmp / I_RADIX - 1;
	  j--;
	}
    }

  for (; j > 0; i--, j--)
    {
      int tmp = I_RADIX + Z[k] + X[i] - Y[j];

      Z[k] = tmp % I_RADIX;
      Z[--k] = tmp / I_RADIX - 1;
    }

  for (; i > 0; i--)
    {
      int tmp = I_RADIX + Z[k] + X[i];

      Z[k] = tmp % I_RADIX;
      Z[--k] = tmp / I_RADIX - 1;
    }

  for (i = 1; Z[i] == 0; i++);

  EZ = EZ - i + 1;
  for (k = 1; i <= p + 1;)
    Z[k++] = Z[i++];
  for (; k <= p;)
    Z[k++] = 0;
}

/* Add *X and *Y and store the result in *Z.  X and Y may overlap, but not X
   and Z or Y and Z.  One guard digit is used.  The error is less than one
   ULP.  */
void
SECTION
__add (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  int n;

  if (X[0] == 0)
    {
      __cpy (y, z, p);
      return;
    }
  else if (Y[0] == 0)
    {
      __cpy (x, z, p);
      return;
    }

  if (X[0] == Y[0])
    {
      if (__acr (x, y, p) > 0)
	{
	  add_magnitudes (x, y, z, p);
	  Z[0] = X[0];
	}
      else
	{
	  add_magnitudes (y, x, z, p);
	  Z[0] = Y[0];
	}
    }
  else
    {
      if ((n = __acr (x, y, p)) == 1)
	{
	  sub_magnitudes (x, y, z, p);
	  Z[0] = X[0];
	}
      else if (n == -1)
	{
	  sub_magnitudes (y, x, z, p);
	  Z[0] = Y[0];
	}
      else
	Z[0] = 0;
    }
}

/* Subtract *Y from *X and return the result in *Z.  X and Y may overlap but
   not X and Z or Y and Z.  One guard digit is used.  The error is less than
   one ULP.  */
void
SECTION
__sub (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  int n;

  if (X[0] == 0)
    {
      __cpy (y, z, p);
      Z[0] = -Z[0];
      return;
    }
  else if (Y[0] == 0)
    {
      __cpy (x, z, p);
      return;
    }

  if (X[0] != Y[0])
    {
      if (__acr (x, y, p) > 0)
	{
	  add_magnitudes (x, y, z, p);
	  Z[0] = X[0];
	}
      else
	{
	  add_magnitudes (y, x, z, p);
	  Z[0] = -Y[0];
	}
    }
  else
    {
      if ((n = __acr (x, y, p)) == 1)
	{
	  sub_magnitudes (x, y, z, p);
	  Z[0] = X[0];
	}
      else if (n == -1)
	{
	  sub_magnitudes (y, x, z, p);
	  Z[0] = -Y[0];
	}
      else
	Z[0] = 0;
    }
}

/* Multiply *X and *Y and store result in *Z.  X and Y may overlap but not X
   and Z or Y and Z.  For P in [1, 2, 3], the exact result is truncated to P
   digits.  In case P > 3 the error is bounded by 1.001 ULP.  */
void
SECTION
__mul (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  int i, j, k, ip, ip2;
  int64_t zk;
  int64_t *diag;

  /* Is z=0?  */
  if (__glibc_unlikely (X[0] * Y[0] == 0))
    {
      Z[0] = 0;
      return;
    }

  /* We need not iterate through all X's and Y's since it's pointless to
     multiply zeroes.  Here, both are zero...  */
  for (ip2 = p; ip2 > 0; ip2--)
    if (X[ip2] | Y[ip2])
      break;

  /* ... and here, at least one of them is still zero.  */
  for (ip = ip2; ip > 0; ip--)
    if (X[ip] * Y[ip])
      break;

  /* Multiply, add and carry.  */
  k = (__glibc_unlikely (p < 3)) ? p + p : p + 3;

  /* For X with precision IP and Y with precision IP2, the term X[I]*Y[K-I] is
     non-zero only when the ranges of non-zero values overlap.  This happens
     only for values of K <= IP + IP2.  Note that we go from 1..K-1, which is
     why we come down to IP + IP2 + 1 and not just IP + IP2.  */
  while (k > ip + ip2 + 1)
    Z[k--] = 0;

  zk = Z[k] = 0;

  /* This gives us additional precision if required.  This is only executed
     when P < IP1 + IP2 + 1, i.e. at least one of the numbers have precision
     of greater than or equal to half of what's required (P).  Anything less
     and we're just wasting our time since we'll be spinning around
     multiplying zeroes.  */
  while (k > p)
    {
      for (i = k - p, j = p; i < p + 1; i++, j--)
	zk += (int64_t) X[i] * Y[j];

      Z[k--] = zk % I_RADIX;
      zk /= I_RADIX;
    }

  /* Precompute sums of diagonal elements so that we can directly use them
     later.  See the next comment to know we why need them.  */
  diag = alloca (k * sizeof (int64_t));
  diag[0] = 0;
  for (i = 1; i <= ip; i++)
    {
      diag[i] = X[i] * Y[i];
      diag[i] += diag[i - 1];
    }
  while (i < k)
    diag[i++] = diag[ip];

  /* The real deal.  Mantissa digit Z[k] is the sum of all X[i] * Y[j] where i
     goes from 1 -> k - 1 and j goes the same range in reverse.  To reduce the
     number of multiplications, we halve the range and if k is an even number,
     add the diagonal element X[k/2]Y[k/2].  Through the half range, we compute
     X[i] * Y[j] as (X[i] + X[j]) * (Y[i] + Y[j]) - X[i] * Y[i] - X[j] * Y[j].

     This reduction tells us that we're summing two things, the first term
     through the half range and the negative of the sum of the product of all
     terms of X and Y in the full range.  i.e.

     SUM(X[i] * Y[i]) for k terms.  This is precalculated above for each k in
     a single loop so that it completes in O(n) time and can hence be directly
     used in the loop below.  */
  while (k > 1)
    {
      int lim = k / 2;

      if (k % 2 == 0)
        {
	  /* We want to add this only once, but since we subtract it in the sum
	     of products above, we add twice.  */
          zk += 2 * X[lim] * Y[lim];
	  lim--;
	}

      for (i = 1, j = k - 1; i <= lim; i++, j--)
	zk += (int64_t) (X[i] + X[j]) * (Y[i] + Y[j]);

      zk -= diag[k - 1];

      Z[k--] = zk % I_RADIX;
      zk /= I_RADIX;
    }
  Z[k] = zk;

  EZ = EX + EY;
  /* Is there a carry beyond the most significant digit?  */
  if (__glibc_unlikely (Z[1] == 0))
    {
      for (i = 1; i <= p; i++)
	Z[i] = Z[i + 1];
      EZ--;
    }

  Z[0] = X[0] * Y[0];
}

/* Invert *X and store in *Y.  Relative error bound:
   - For P = 2: 1.001 * R ^ (1 - P)
   - For P = 3: 1.063 * R ^ (1 - P)
   - For P > 3: 2.001 * R ^ (1 - P)

   *X = 0 is not permissible.  */
static void
SECTION
__inv (const mp_no *x, mp_no *y, int p)
{
  int i;
  double t;
  mp_no z, w;
  static const int np1[] =
    { 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
  };

  __cpy (x, &z, p);
  z.e = 0;
  __mp_dbl (&z, &t, p);
  t = ONE / t;
  __dbl_mp (t, y, p);
  EY -= EX;

  for (i = 0; i < np1[p]; i++)
    {
      __cpy (y, &w, p);
      __mul (x, &w, y, p);
      __sub (&mptwo, y, &z, p);
      __mul (&w, &z, y, p);
    }
}

/* Divide *X by *Y and store result in *Z.  X and Y may overlap but not X and Z
   or Y and Z.  Relative error bound:
   - For P = 2: 2.001 * R ^ (1 - P)
   - For P = 3: 2.063 * R ^ (1 - P)
   - For P > 3: 3.001 * R ^ (1 - P)

   *X = 0 is not permissible.  */
void
SECTION
__dvd (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  mp_no w;

  if (X[0] == 0)
    Z[0] = 0;
  else
    {
      __inv (y, &w, p);
      __mul (x, &w, z, p);
    }
}
