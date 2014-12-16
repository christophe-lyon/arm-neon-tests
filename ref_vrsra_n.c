/*

Copyright (c) 2009, 2010, 2011 STMicroelectronics
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

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#define TEST_MSG "VRSRA_N"
void exec_vrsra_n (void)
{
  /* Basic test: y=vrsra_n(x,v), then store the result.  */
#define TEST_VRSRA_N(Q, T1, T2, W, N, V)				\
  VECT_VAR(vector_res, T1, W, N) =					\
    vrsra##Q##_n_##T2##W(VECT_VAR(vector, T1, W, N),			\
			 VECT_VAR(vector2, T1, W, N),			\
			 V);						\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */

  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Choose arbitrary initialization values */
  VDUP(vector2, , int, s, 8, 8, 0x11);
  VDUP(vector2, , int, s, 16, 4, 0x22);
  VDUP(vector2, , int, s, 32, 2, 0x33);
  VDUP(vector2, , int, s, 64, 1, 0x44);
  VDUP(vector2, , uint, u, 8, 8, 0x55);
  VDUP(vector2, , uint, u, 16, 4, 0x66);
  VDUP(vector2, , uint, u, 32, 2, 0x77);
  VDUP(vector2, , uint, u, 64, 1, 0x88);

  VDUP(vector2, q, int, s, 8, 16, 0x11);
  VDUP(vector2, q, int, s, 16, 8, 0x22);
  VDUP(vector2, q, int, s, 32, 4, 0x33);
  VDUP(vector2, q, int, s, 64, 2, 0x44);
  VDUP(vector2, q, uint, u, 8, 16, 0x55);
  VDUP(vector2, q, uint, u, 16, 8, 0x66);
  VDUP(vector2, q, uint, u, 32, 4, 0x77);
  VDUP(vector2, q, uint, u, 64, 2, 0x88);

  /* Choose shift amount arbitrarily  */
  TEST_VRSRA_N(, int, s, 8, 8, 1);
  TEST_VRSRA_N(, int, s, 16, 4, 12);
  TEST_VRSRA_N(, int, s, 32, 2, 2);
  TEST_VRSRA_N(, int, s, 64, 1, 32);
  TEST_VRSRA_N(, uint, u, 8, 8, 2);
  TEST_VRSRA_N(, uint, u, 16, 4, 3);
  TEST_VRSRA_N(, uint, u, 32, 2, 5);
  TEST_VRSRA_N(, uint, u, 64, 1, 33);

  TEST_VRSRA_N(q, int, s, 8, 16, 1);
  TEST_VRSRA_N(q, int, s, 16, 8, 12);
  TEST_VRSRA_N(q, int, s, 32, 4, 2);
  TEST_VRSRA_N(q, int, s, 64, 2, 32);
  TEST_VRSRA_N(q, uint, u, 8, 16, 2);
  TEST_VRSRA_N(q, uint, u, 16, 8, 3);
  TEST_VRSRA_N(q, uint, u, 32, 4, 5);
  TEST_VRSRA_N(q, uint, u, 64, 2, 33);

  /* FIXME: only a few result buffers are used, but we output all of them */
  dump_results_hex (TEST_MSG);

  /* Initialize the accumulator with 0 */
  VDUP(vector, , int, s, 8, 8, 0);
  VDUP(vector, , int, s, 16, 4, 0);
  VDUP(vector, , int, s, 32, 2, 0);
  VDUP(vector, , int, s, 64, 1, 0);
  VDUP(vector, , uint, u, 8, 8, 0);
  VDUP(vector, , uint, u, 16, 4, 0);
  VDUP(vector, , uint, u, 32, 2, 0);
  VDUP(vector, , uint, u, 64, 1, 0);
  VDUP(vector, q, int, s, 8, 16, 0);
  VDUP(vector, q, int, s, 16, 8, 0);
  VDUP(vector, q, int, s, 32, 4, 0);
  VDUP(vector, q, int, s, 64, 2, 0);
  VDUP(vector, q, uint, u, 8, 16, 0);
  VDUP(vector, q, uint, u, 16, 8, 0);
  VDUP(vector, q, uint, u, 32, 4, 0);
  VDUP(vector, q, uint, u, 64, 2, 0);

  /* Initialize with max values to check overflow */
  VDUP(vector2, , int, s, 8, 8, 0x7F);
  VDUP(vector2, , int, s, 16, 4, 0x7FFF);
  VDUP(vector2, , int, s, 32, 2, 0x7FFFFFFF);
  VDUP(vector2, , int, s, 64, 1, 0x7FFFFFFFFFFFFFFFLL);
  VDUP(vector2, , uint, u, 8, 8, 0xFF);
  VDUP(vector2, , uint, u, 16, 4, 0xFFFF);
  VDUP(vector2, , uint, u, 32, 2, 0xFFFFFFFF);
  VDUP(vector2, , uint, u, 64, 1, 0xFFFFFFFFFFFFFFFFULL);
  VDUP(vector2, q, int, s, 8, 16, 0x7F);
  VDUP(vector2, q, int, s, 16, 8, 0x7FFF);
  VDUP(vector2, q, int, s, 32, 4, 0x7FFFFFFF);
  VDUP(vector2, q, int, s, 64, 2, 0x7FFFFFFFFFFFFFFFLL);
  VDUP(vector2, q, uint, u, 8, 16, 0xFF);
  VDUP(vector2, q, uint, u, 16, 8, 0xFFFF);
  VDUP(vector2, q, uint, u, 32, 4, 0xFFFFFFFF);
  VDUP(vector2, q, uint, u, 64, 2, 0xFFFFFFFFFFFFFFFFULL);

  /* Shift by 1 to check overflow with rounding constant  */
  TEST_VRSRA_N(, int, s, 8, 8, 1);
  TEST_VRSRA_N(, int, s, 16, 4, 1);
  TEST_VRSRA_N(, int, s, 32, 2, 1);
  TEST_VRSRA_N(, int, s, 64, 1, 1);
  TEST_VRSRA_N(, uint, u, 8, 8, 1);
  TEST_VRSRA_N(, uint, u, 16, 4, 1);
  TEST_VRSRA_N(, uint, u, 32, 2, 1);
  TEST_VRSRA_N(, uint, u, 64, 1, 1);
  TEST_VRSRA_N(q, int, s, 8, 16, 1);
  TEST_VRSRA_N(q, int, s, 16, 8, 1);
  TEST_VRSRA_N(q, int, s, 32, 4, 1);
  TEST_VRSRA_N(q, int, s, 64, 2, 1);
  TEST_VRSRA_N(q, uint, u, 8, 16, 1);
  TEST_VRSRA_N(q, uint, u, 16, 8, 1);
  TEST_VRSRA_N(q, uint, u, 32, 4, 1);
  TEST_VRSRA_N(q, uint, u, 64, 2, 1);

  dump_results_hex2 (TEST_MSG, " (checking overflow: shift by 1, positive input)");

  /* Shift by 3 to check overflow with rounding constant  */
  TEST_VRSRA_N(, int, s, 8, 8, 3);
  TEST_VRSRA_N(, int, s, 16, 4, 3);
  TEST_VRSRA_N(, int, s, 32, 2, 3);
  TEST_VRSRA_N(, int, s, 64, 1, 3);
  TEST_VRSRA_N(, uint, u, 8, 8, 3);
  TEST_VRSRA_N(, uint, u, 16, 4, 3);
  TEST_VRSRA_N(, uint, u, 32, 2, 3);
  TEST_VRSRA_N(, uint, u, 64, 1, 3);
  TEST_VRSRA_N(q, int, s, 8, 16, 3);
  TEST_VRSRA_N(q, int, s, 16, 8, 3);
  TEST_VRSRA_N(q, int, s, 32, 4, 3);
  TEST_VRSRA_N(q, int, s, 64, 2, 3);
  TEST_VRSRA_N(q, uint, u, 8, 16, 3);
  TEST_VRSRA_N(q, uint, u, 16, 8, 3);
  TEST_VRSRA_N(q, uint, u, 32, 4, 3);
  TEST_VRSRA_N(q, uint, u, 64, 2, 3);

  dump_results_hex2 (TEST_MSG, " (checking overflow: shift by 3, positive input)");

  /* Shift by max to check overflow with rounding constant  */
  TEST_VRSRA_N(, int, s, 8, 8, 8);
  TEST_VRSRA_N(, int, s, 16, 4, 16);
  TEST_VRSRA_N(, int, s, 32, 2, 32);
  TEST_VRSRA_N(, int, s, 64, 1, 64);
  TEST_VRSRA_N(, uint, u, 8, 8, 8);
  TEST_VRSRA_N(, uint, u, 16, 4, 16);
  TEST_VRSRA_N(, uint, u, 32, 2, 32);
  TEST_VRSRA_N(, uint, u, 64, 1, 64);
  TEST_VRSRA_N(q, int, s, 8, 16, 8);
  TEST_VRSRA_N(q, int, s, 16, 8, 16);
  TEST_VRSRA_N(q, int, s, 32, 4, 32);
  TEST_VRSRA_N(q, int, s, 64, 2, 64);
  TEST_VRSRA_N(q, uint, u, 8, 16, 8);
  TEST_VRSRA_N(q, uint, u, 16, 8, 16);
  TEST_VRSRA_N(q, uint, u, 32, 4, 32);
  TEST_VRSRA_N(q, uint, u, 64, 2, 64);

  dump_results_hex2 (TEST_MSG, " (checking overflow: shift by max, positive input)");
  /* Initialize with min values to check overflow */
  VDUP(vector2, , int, s, 8, 8, 0x80);
  VDUP(vector2, , int, s, 16, 4, 0x8000);
  VDUP(vector2, , int, s, 32, 2, 0x80000000);
  VDUP(vector2, , int, s, 64, 1, 0x8000000000000000LL);
  VDUP(vector2, q, int, s, 8, 16, 0x80);
  VDUP(vector2, q, int, s, 16, 8, 0x8000);
  VDUP(vector2, q, int, s, 32, 4, 0x80000000);
  VDUP(vector2, q, int, s, 64, 2, 0x8000000000000000ULL);

  /* Shift by 1 to check overflow with rounding constant  */
  TEST_VRSRA_N(, int, s, 8, 8, 1);
  TEST_VRSRA_N(, int, s, 16, 4, 1);
  TEST_VRSRA_N(, int, s, 32, 2, 1);
  TEST_VRSRA_N(, int, s, 64, 1, 1);
  TEST_VRSRA_N(q, int, s, 8, 16, 1);
  TEST_VRSRA_N(q, int, s, 16, 8, 1);
  TEST_VRSRA_N(q, int, s, 32, 4, 1);
  TEST_VRSRA_N(q, int, s, 64, 2, 1);

  dump_results_hex2 (TEST_MSG, " (checking overflow: shift by 1, negative input)");

  /* Shift by 3 to check overflow with rounding constant  */
  TEST_VRSRA_N(, int, s, 8, 8, 3);
  TEST_VRSRA_N(, int, s, 16, 4, 3);
  TEST_VRSRA_N(, int, s, 32, 2, 3);
  TEST_VRSRA_N(, int, s, 64, 1, 3);
  TEST_VRSRA_N(q, int, s, 8, 16, 3);
  TEST_VRSRA_N(q, int, s, 16, 8, 3);
  TEST_VRSRA_N(q, int, s, 32, 4, 3);
  TEST_VRSRA_N(q, int, s, 64, 2, 3);

  dump_results_hex2 (TEST_MSG, " (checking overflow: shift by max, negative input)");

  /* Shift by max to check overflow with rounding constant  */
  TEST_VRSRA_N(, int, s, 8, 8, 8);
  TEST_VRSRA_N(, int, s, 16, 4, 16);
  TEST_VRSRA_N(, int, s, 32, 2, 32);
  TEST_VRSRA_N(, int, s, 64, 1, 64);
  TEST_VRSRA_N(q, int, s, 8, 16, 8);
  TEST_VRSRA_N(q, int, s, 16, 8, 16);
  TEST_VRSRA_N(q, int, s, 32, 4, 32);
  TEST_VRSRA_N(q, int, s, 64, 2, 64);

  dump_results_hex2 (TEST_MSG, " (checking overflow: shift by max, negative input)");
}
