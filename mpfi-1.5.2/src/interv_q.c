/* interv_q.c -- Build an interval from two rational numbers.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2010,
                     Spaces project, Inria Lorraine
                     and Salsa project, INRIA Rocquencourt,
                     and Arenaire project, Inria Rhone-Alpes, France
                     and Lab. ANO, USTL (Univ. of Lille),  France

This file is part of the MPFI Library.

The MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPFI Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpfi-impl.h"

int
mpfi_interv_q (mpfi_ptr a, mpq_srcptr b, mpq_srcptr c)
{
  int inexact_left, inexact_right, inexact=0;

  if ( mpq_cmp (b, c) <= 0) {
    inexact_left = mpfr_set_q (&(a->left), b, MPFI_RNDD);
    inexact_right = mpfr_set_q (&(a->right), c, MPFI_RNDU);
  }
  else {
    inexact_left = mpfr_set_q (&(a->left), c, MPFI_RNDD);
    inexact_right = mpfr_set_q (&(a->right), b, MPFI_RNDU);
  }
  /* a cannot be a NaN, mpfr_set_q never returns a NaN */
  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  return inexact;
}
