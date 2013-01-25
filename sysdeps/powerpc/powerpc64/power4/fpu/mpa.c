
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

const mp_no mpone = {1, {1.0, 1.0}};
const mp_no mptwo = {1, {1.0, 2.0}};

/* Compare mantissa of two multiple precision numbers regardless of the sign
   and exponent of the numbers.  */
static int
mcr (const mp_no *x, const mp_no *y, int p)
{
  long i;
  long p2 = p;
  for (i = 1; i <= p2; i++)
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
  long i;

  if (X[0] == ZERO)
    {
      if (Y[0] == ZERO)
	i = 0;
      else
	i = -1;
    }
  else if (Y[0] == ZERO)
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

/* Copy multiple precision number X into Y.  They could be the same
   number.  */
void
__cpy (const mp_no *x, mp_no *y, int p)
{
  long i;

  EY = EX;
  for (i = 0; i <= p; i++)
    Y[i] = X[i];

  return;
}

/* Convert a multiple precision number *X into a double precision
   number *Y, normalized case  (|x| >= 2**(-1022))).  */
static void
norm (const mp_no *x, double *y, int p)
{
#define R  RADIXI
  long i;
  double a, c, u, v, z[5];
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
      for (a = ONE, z[1] = X[1]; z[1] < TWO23;)
	{
	  a *= TWO;
	  z[1] *= TWO;
	}

      for (i = 2; i < 5; i++)
	{
	  z[i] = X[i] * a;
	  u = (z[i] + CUTTER) - CUTTER;
	  if (u > z[i])
	    u -= RADIX;
	  z[i] -= u;
	  z[i - 1] += u * RADIXI;
	}

      u = (z[3] + TWO71) - TWO71;
      if (u > z[3])
	u -= TWO19;
      v = z[3] - u;

      if (v == TWO18)
	{
	  if (z[4] == ZERO)
	    {
	      for (i = 5; i <= p; i++)
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
	  else
	    z[3] += ONE;
	}

      c = (z[1] + R * (z[2] + R * z[3])) / a;
    }

  c *= X[0];

  for (i = 1; i < EX; i++)
    c *= RADIX;
  for (i = 1; i > EX; i--)
    c *= RADIXI;

  *y = c;
  return;
#undef R
}

/* Convert a multiple precision number *X into a double precision
   number *Y, Denormal case  (|x| < 2**(-1022))).  */
static void
denorm (const mp_no *x, double *y, int p)
{
  long i, k;
  long p2 = p;
  double c, u, z[5];

#define R RADIXI
  if (EX < -44 || (EX == -44 && X[1] < TWO5))
    {
      *y = ZERO;
      return;
    }

  if (p2 == 1)
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
  else if (p2 == 2)
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
      for (i = k + 1; i <= p2; i++)
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
  return;

#undef R
}

/* Convert multiple precision number *X into double precision number *Y.  The
   result is correctly rounded to the nearest/even.  */
void
__mp_dbl (const mp_no *x, double *y, int p)
{
  if (X[0] == ZERO)
    {
      *y = ZERO;
      return;
    }

  if (EX > -42)
    norm (x, y, p);
  else if (EX == -42 && X[1] >= TWO10)
    norm (x, y, p);
  else
    denorm (x, y, p);
}

/* Get the multiple precision equivalent of X into *Y.  If the precision is too
   small, the result is truncated.  */
void
__dbl_mp (double x, mp_no *y, int p)
{
  long i, n;
  long p2 = p;
  double u;

  /* Sign.  */
  if (x == ZERO)
    {
      Y[0] = ZERO;
      return;
    }
  else if (x > ZERO)
    Y[0] = ONE;
  else
    {
      Y[0] = MONE;
      x = -x;
    }

  /* Exponent.  */
  for (EY = ONE; x >= RADIX; EY += ONE)
    x *= RADIXI;
  for (; x < ONE; EY -= ONE)
    x *= RADIX;

  /* Digits.  */
  n = MIN (p2, 4);
  for (i = 1; i <= n; i++)
    {
      u = (x + TWO52) - TWO52;
      if (u > x)
	u -= ONE;
      Y[i] = u;
      x -= u;
      x *= RADIX;
    }
  for (; i <= p2; i++)
    Y[i] = ZERO;
  return;
}

/* Add magnitudes of *X and *Y assuming that abs (*X) >= abs (*Y) > 0.  The
   sign of the sum *Z is not changed.  X and Y may overlap but not X and Z or
   Y and Z.  No guard digit is used.  The result equals the exact sum,
   truncated.  */
static void
add_magnitudes (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  long i, j, k;
  long p2 = p;

  EZ = EX;

  i = p2;
  j = p2 + EY - EX;
  k = p2 + 1;

  if (j < 1)
    {
      __cpy (x, z, p);
      return;
    }
  else
    Z[k] = ZERO;

  for (; j > 0; i--, j--)
    {
      Z[k] += X[i] + Y[j];
      if (Z[k] >= RADIX)
	{
	  Z[k] -= RADIX;
	  Z[--k] = ONE;
	}
      else
	Z[--k] = ZERO;
    }

  for (; i > 0; i--)
    {
      Z[k] += X[i];
      if (Z[k] >= RADIX)
	{
	  Z[k] -= RADIX;
	  Z[--k] = ONE;
	}
      else
	Z[--k] = ZERO;
    }

  if (Z[1] == ZERO)
    {
      for (i = 1; i <= p2; i++)
	Z[i] = Z[i + 1];
    }
  else
    EZ += ONE;
}

/* Subtract the magnitudes of *X and *Y assuming that abs (*x) > abs (*y) > 0.
   The sign of the difference *Z is not changed.  X and Y may overlap but not X
   and Z or Y and Z.  One guard digit is used.  The error is less than one
   ULP.  */
static void
sub_magnitudes (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  long i, j, k;
  long p2 = p;

  EZ = EX;

  if (EX == EY)
    {
      i = j = k = p2;
      Z[k] = Z[k + 1] = ZERO;
    }
  else
    {
      j = EX - EY;
      if (j > p2)
	{
	  __cpy (x, z, p);
	  return;
	}
      else
	{
	  i = p2;
	  j = p2 + 1 - j;
	  k = p2;
	  if (Y[j] > ZERO)
	    {
	      Z[k + 1] = RADIX - Y[j--];
	      Z[k] = MONE;
	    }
	  else
	    {
	      Z[k + 1] = ZERO;
	      Z[k] = ZERO;
	      j--;
	    }
	}
    }

  for (; j > 0; i--, j--)
    {
      Z[k] += (X[i] - Y[j]);
      if (Z[k] < ZERO)
	{
	  Z[k] += RADIX;
	  Z[--k] = MONE;
	}
      else
	Z[--k] = ZERO;
    }

  for (; i > 0; i--)
    {
      Z[k] += X[i];
      if (Z[k] < ZERO)
	{
	  Z[k] += RADIX;
	  Z[--k] = MONE;
	}
      else
	Z[--k] = ZERO;
    }

  for (i = 1; Z[i] == ZERO; i++);
  EZ = EZ - i + 1;
  for (k = 1; i <= p2 + 1;)
    Z[k++] = Z[i++];
  for (; k <= p2;)
    Z[k++] = ZERO;

  return;
}

/* Add *X and *Y and store the result in *Z.  X and Y may overlap, but not X
   and Z or Y and Z.  One guard digit is used.  The error is less than one
   ULP.  */
void
__add (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  int n;

  if (X[0] == ZERO)
    {
      __cpy (y, z, p);
      return;
    }
  else if (Y[0] == ZERO)
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
	Z[0] = ZERO;
    }
  return;
}

/* Subtract *Y from *X and return the result in *Z.  X and Y may overlap but
   not X and Z or Y and Z.  One guard digit is used.  The error is less than
   one ULP.  */
void
__sub (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  int n;

  if (X[0] == ZERO)
    {
      __cpy (y, z, p);
      Z[0] = -Z[0];
      return;
    }
  else if (Y[0] == ZERO)
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
	Z[0] = ZERO;
    }
  return;
}

/* Multiply *X and *Y and store result in *Z.  X and Y may overlap but not X
   and Z or Y and Z.  For P in [1, 2, 3], the exact result is truncated to P
   digits.  In case P > 3 the error is bounded by 1.001 ULP.  */
void
__mul (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  long i, i1, i2, j, k, k2;
  long p2 = p;
  double u, zk, zk2;

  /* Is z=0? */
  if (X[0] * Y[0] == ZERO)
    {
      Z[0] = ZERO;
      return;
    }

  /* Multiply, add and carry */
  k2 = (p2 < 3) ? p2 + p2 : p2 + 3;
  zk = Z[k2] = ZERO;
  for (k = k2; k > 1;)
    {
      if (k > p2)
	{
	  i1 = k - p2;
	  i2 = p2 + 1;
	}
      else
	{
	  i1 = 1;
	  i2 = k;
	}
#if 1
      /* Rearrange this inner loop to allow the fmadd instructions to be
         independent and execute in parallel on processors that have
         dual symmetrical FP pipelines.  */
      if (i1 < (i2 - 1))
	{
	  /* Make sure we have at least 2 iterations.  */
	  if (((i2 - i1) & 1L) == 1L)
	    {
	      /* Handle the odd iterations case.  */
	      zk2 = x->d[i2 - 1] * y->d[i1];
	    }
	  else
	    zk2 = 0.0;
	  /* Do two multiply/adds per loop iteration, using independent
	     accumulators; zk and zk2.  */
	  for (i = i1, j = i2 - 1; i < i2 - 1; i += 2, j -= 2)
	    {
	      zk += x->d[i] * y->d[j];
	      zk2 += x->d[i + 1] * y->d[j - 1];
	    }
	  zk += zk2;		/* Final sum.  */
	}
      else
	{
	  /* Special case when iterations is 1.  */
	  zk += x->d[i1] * y->d[i1];
	}
#else
      /* The original code.  */
      for (i = i1, j = i2 - 1; i < i2; i++, j--)
	zk += X[i] * Y[j];
#endif

      u = (zk + CUTTER) - CUTTER;
      if (u > zk)
	u -= RADIX;
      Z[k] = zk - u;
      zk = u * RADIXI;
      --k;
    }
  Z[k] = zk;

  /* Is there a carry beyond the most significant digit?  */
  if (Z[1] == ZERO)
    {
      for (i = 1; i <= p2; i++)
	Z[i] = Z[i + 1];
      EZ = EX + EY - 1;
    }
  else
    EZ = EX + EY;

  Z[0] = X[0] * Y[0];
  return;
}

/* Invert *X and store in *Y.  Relative error bound:
   - For P = 2: 1.001 * R ^ (1 - P)
   - For P = 3: 1.063 * R ^ (1 - P)
   - For P > 3: 2.001 * R ^ (1 - P)

   *X = 0 is not permissible.  */
void
__inv (const mp_no *x, mp_no *y, int p)
{
  long i;
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
  return;
}

/* Divide *X by *Y and store result in *Z.  X and Y may overlap but not X and Z
   or Y and Z.  Relative error bound:
   - For P = 2: 2.001 * R ^ (1 - P)
   - For P = 3: 2.063 * R ^ (1 - P)
   - For P > 3: 3.001 * R ^ (1 - P)

   *X = 0 is not permissible.  */
void
__dvd (const mp_no *x, const mp_no *y, mp_no *z, int p)
{
  mp_no w;

  if (X[0] == ZERO)
    Z[0] = ZERO;
  else
    {
      __inv (y, &w, p);
      __mul (x, &w, z, p);
    }
  return;
}
