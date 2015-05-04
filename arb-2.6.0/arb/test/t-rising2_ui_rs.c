/*=============================================================================

    This file is part of arb.

    arb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    arb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arb; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "arith.h"
#include "arb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("rising2_ui_rs....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000; iter++)
    {
        arb_t a, u, v, u2, v2;
        fmpz *f;
        arb_ptr g;
        ulong n;
        long i, prec;

        arb_init(a);
        arb_init(u);
        arb_init(v);
        arb_init(u2);
        arb_init(v2);

        arb_randtest(a, state, 1 + n_randint(state, 4000), 10);
        arb_randtest(u, state, 1 + n_randint(state, 4000), 10);
        arb_randtest(v, state, 1 + n_randint(state, 4000), 10);
        n = n_randint(state, 120);

        f = _fmpz_vec_init(n + 1);
        g = _arb_vec_init(n + 1);

        prec = 2 + n_randint(state, 4000);
        arb_rising2_ui_rs(u, v, a, n, 0, prec);

        arith_stirling_number_1u_vec(f, n, n + 1);
        for (i = 0; i <= n; i++)
            arb_set_fmpz(g + i, f + i);
        _arb_poly_evaluate(u2, g, n + 1, a, prec);

        _arb_poly_derivative(g, g, n + 1, prec);
        _arb_poly_evaluate(v2, g, n, a, prec);

        if (!arb_overlaps(u, u2) || !arb_overlaps(v, v2))
        {
            printf("FAIL: overlap\n\n");
            printf("n = %lu\n", n);
            printf("a = "); arb_printd(a, 15); printf("\n\n");
            printf("u = "); arb_printd(u, 15); printf("\n\n");
            printf("u2 = "); arb_printd(u2, 15); printf("\n\n");
            printf("v = "); arb_printd(v, 15); printf("\n\n");
            printf("v2 = "); arb_printd(v2, 15); printf("\n\n");
            abort();
        }

        arb_set(u2, a);
        arb_rising2_ui_rs(u2, v, u2, n, 0, prec);

        if (!arb_equal(u2, u))
        {
            printf("FAIL: aliasing 1\n\n");
            printf("a = "); arb_printd(a, 15); printf("\n\n");
            printf("u = "); arb_printd(u, 15); printf("\n\n");
            printf("u2 = "); arb_printd(u2, 15); printf("\n\n");
            printf("n = %lu\n", n);
            abort();
        }

        arb_set(v2, a);
        arb_rising2_ui_rs(u, v2, v2, n, 0, prec);

        if (!arb_equal(v2, v))
        {
            printf("FAIL: aliasing 2\n\n");
            printf("a = "); arb_printd(a, 15); printf("\n\n");
            printf("v = "); arb_printd(v, 15); printf("\n\n");
            printf("v2 = "); arb_printd(v2, 15); printf("\n\n");
            printf("n = %lu\n", n);
            abort();
        }

        arb_clear(a);
        arb_clear(u);
        arb_clear(v);
        arb_clear(u2);
        arb_clear(v2);
        _fmpz_vec_clear(f, n + 1);
        _arb_vec_clear(g, n + 1);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
