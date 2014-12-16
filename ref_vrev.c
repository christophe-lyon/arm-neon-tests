/*

Copyright (c) 2009, 2010, 2011, 2013 STMicroelectronics
Written by Christophe Lyon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

/* Template file for unary operator validation */

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

void exec_vrev (void)
{
  /* Basic test: y=vrev(x), then store the result.  */
#define TEST_VREV(Q, T1, T2, W, N, W2)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vrev##W2##Q##_##T2##W(VECT_VAR(vector, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement  */

  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);
  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);

  /* Check vrev in each of the existing combinations  */
#define TEST_MSG "VREV16"
  TEST_VREV(, int, s, 8, 8, 16);
  TEST_VREV(, uint, u, 8, 8, 16);
  TEST_VREV(, poly, p, 8, 8, 16);
  TEST_VREV(q, int, s, 8, 16, 16);
  TEST_VREV(q, uint, u, 8, 16, 16);
  TEST_VREV(q, poly, p, 8, 16, 16);
  dump_results_hex (TEST_MSG);

#undef TEST_MSG
#define TEST_MSG "VREV32"
  TEST_VREV(, int, s, 8, 8, 32);
  TEST_VREV(, int, s, 16, 4, 32);
  TEST_VREV(, uint, u, 8, 8, 32);
  TEST_VREV(, uint, u, 16, 4, 32);
  TEST_VREV(, poly, p, 8, 8, 32);
  TEST_VREV(, poly, p, 16, 4, 32);
  TEST_VREV(q, int, s, 8, 16, 32);
  TEST_VREV(q, int, s, 16, 8, 32);
  TEST_VREV(q, uint, u, 8, 16, 32);
  TEST_VREV(q, uint, u, 16, 8, 32);
  TEST_VREV(q, poly, p, 8, 16, 32);
  TEST_VREV(q, poly, p, 16, 8, 32);
  dump_results_hex (TEST_MSG);

#undef TEST_MSG
#define TEST_MSG "VREV64"
  TEST_VREV(, int, s, 8, 8, 64);
  TEST_VREV(, int, s, 16, 4, 64);
  TEST_VREV(, int, s, 32, 2, 64);
  TEST_VREV(, uint, u, 8, 8, 64);
  TEST_VREV(, uint, u, 16, 4, 64);
  TEST_VREV(, uint, u, 32, 2, 64);
  TEST_VREV(, poly, p, 8, 8, 64);
  TEST_VREV(, poly, p, 16, 4, 64);
  TEST_VREV(q, int, s, 8, 16, 64);
  TEST_VREV(q, int, s, 16, 8, 64);
  TEST_VREV(q, int, s, 32, 4, 64);
  TEST_VREV(q, uint, u, 8, 16, 64);
  TEST_VREV(q, uint, u, 16, 8, 64);
  TEST_VREV(q, uint, u, 32, 4, 64);
  TEST_VREV(q, poly, p, 8, 16, 64);
  TEST_VREV(q, poly, p, 16, 8, 64);

  TEST_VREV(, float, f, 32, 2, 64);
  TEST_VREV(q, float, f, 32, 4, 64);

  dump_results_hex (TEST_MSG);
}
