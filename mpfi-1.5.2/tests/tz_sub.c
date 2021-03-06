/* tz_sub.c -- Test mpfi_z_sub.

Copyright 2010, 2011, 
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

#include "mpfi-tests.h"

void
check_overflow ()
{
  mpfi_t interval;
  mpfi_t got;
  mpz_t z;
  int inex;

  mpfi_init2 (interval, 53);
  mpfi_init2 (got, 53);
  mpz_init (z);

  /* right overflow: 1023 - [-Max, 0] = [1023, +oo] */
  mpz_set_ui (z, 1023);
  mpfr_set_inf (&(interval->left), -1);
  mpfr_nextabove (&(interval->left));
  mpfr_set_ui (&(interval->right), 0, MPFI_RNDU);

  inex = mpfi_z_sub (got, z, interval);

  if (MPFI_LEFT_IS_INEXACT (inex) || mpfr_cmp_z (&(got->left), z) != 0
      || !MPFI_RIGHT_IS_INEXACT (inex) || !mpfr_inf_p (&(got->right))) {
    printf ("Error: mpfi_fr_sub (rop, z, op) does not correctly handle "
            "overflow.\n  z = ");
    mpz_out_str (stdout, 16, z);
    printf ("\nop = ");
    mpfi_out_str (stdout, 16, 0, interval);
    printf ("\nrop = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nreturn value = %d\n", inex);
    exit (1);
  }

  /* left overflow: -1023 - [0, Max] = [-oo, -1023] */
  mpz_neg (z, z);
  mpfi_neg (interval, interval);

  inex = mpfi_z_sub (got, z, interval);

  if (!MPFI_LEFT_IS_INEXACT (inex)
      || !mpfr_inf_p (&(got->left))
      || MPFI_RIGHT_IS_INEXACT (inex)
      || mpfr_cmp_si (&(got->right), -1023) != 0) {
    printf ("Error: mpfi_fr_sub (rop, z, op) does not correctly handle "
            "overflow.\n  z = ");
    mpz_out_str (stdout, 16, z);
    printf ("\nop = ");
    mpfi_out_str (stdout, 16, 0, interval);
    printf ("\nrop = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nreturn value = %d\n", inex);
    exit (1);
  }

  mpz_clear (z);
  mpfi_clear (interval);
  mpfi_clear (got);
}

#ifndef HAVE_MPFR_Z_SUB
/* fake non-existing function */
int
mpfr_z_sub (mpfr_ptr x, mpz_srcptr z, mpfr_srcptr y, mp_rnd_t rnd)
{
  int ret;

  /* invert rounding mode */
  if (rnd == MPFI_RNDU)
    rnd = MPFI_RNDD;
  else if (rnd == MPFI_RNDD)
    rnd = MPFI_RNDU;
  ret = mpfr_sub_z (x, y, z, rnd);
  mpfr_neg (x, x, MPFI_RNDU);
  return -ret;
}
#endif /* HAVE_MPFR_Z_SUB */

int
main (int argc, char **argv)
{
  struct mpfi_function_t i_z_sub;

  mpfi_fun_init_IZI (&i_z_sub, mpfi_z_sub, mpfr_z_sub);
  test_start ();

  check_data (&i_z_sub, "z_sub.dat");
  check_random (&i_z_sub, 2, 1000, 10);
  check_overflow ();

  test_end ();
  mpfi_fun_clear (&i_z_sub);

  return 0;
}
