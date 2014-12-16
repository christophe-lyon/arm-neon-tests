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

#define TEST_MSG "VRSHL/VRSHLQ"
void exec_vrshl (void)
{
  /* Basic test: v3=vrshl(v1,v2), then store the result.  */
#define TEST_VRSHL(T3, Q, T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vrshl##Q##_##T2##W(VECT_VAR(vector, T1, W, N),			\
		       VECT_VAR(vector_shift, T3, W, N));		\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  DECL_VARIABLE_SIGNED_VARIANTS(vector_shift);

  clean_results ();

  /* Fill input vector with 0, to check behavior on limits */
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

  /* Choose init value arbitrarily, will be used as shift amount */
  /* Use values equal to one-less-than the type width to check
     behaviour on limits */
  VDUP(vector_shift, , int, s, 8, 8, 7);
  VDUP(vector_shift, , int, s, 16, 4, 15);
  VDUP(vector_shift, , int, s, 32, 2, 31);
  VDUP(vector_shift, , int, s, 64, 1, 63);
  VDUP(vector_shift, q, int, s, 8, 16, 7);
  VDUP(vector_shift, q, int, s, 16, 8, 15);
  VDUP(vector_shift, q, int, s, 32, 4, 31);
  VDUP(vector_shift, q, int, s, 64, 2, 63);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (with input = 0)");

  /* Use negative shift amounts */
  VDUP(vector_shift, , int, s, 8, 8, -1);
  VDUP(vector_shift, , int, s, 16, 4, -2);
  VDUP(vector_shift, , int, s, 32, 2, -3);
  VDUP(vector_shift, , int, s, 64, 1, -4);
  VDUP(vector_shift, q, int, s, 8, 16, -7);
  VDUP(vector_shift, q, int, s, 16, 8, -11);
  VDUP(vector_shift, q, int, s, 32, 4, -13);
  VDUP(vector_shift, q, int, s, 64, 2, -20);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (input 0 and negative shift amount)");

  /* Test again, with predefined input values */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Choose init value arbitrarily, will be used as shift amount */
  VDUP(vector_shift, , int, s, 8, 8, 1);
  VDUP(vector_shift, , int, s, 16, 4, 3);
  VDUP(vector_shift, , int, s, 32, 2, 8);
  VDUP(vector_shift, , int, s, 64, 1, -3);
  VDUP(vector_shift, q, int, s, 8, 16, 10);
  VDUP(vector_shift, q, int, s, 16, 8, 12);
  VDUP(vector_shift, q, int, s, 32, 4, 32);
  VDUP(vector_shift, q, int, s, 64, 2, 63);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex (TEST_MSG);


  /* Use negative shift amounts */
  VDUP(vector_shift, , int, s, 8, 8, -1);
  VDUP(vector_shift, , int, s, 16, 4, -2);
  VDUP(vector_shift, , int, s, 32, 2, -3);
  VDUP(vector_shift, , int, s, 64, 1, -4);
  VDUP(vector_shift, q, int, s, 8, 16, -7);
  VDUP(vector_shift, q, int, s, 16, 8, -11);
  VDUP(vector_shift, q, int, s, 32, 4, -13);
  VDUP(vector_shift, q, int, s, 64, 2, -20);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (negative shift amount)");

  /* Fill input vector with max value, to check behavior on limits */
  VDUP(vector, , int, s, 8, 8, 0x7F);
  VDUP(vector, , int, s, 16, 4, 0x7FFF);
  VDUP(vector, , int, s, 32, 2, 0x7FFFFFFF);
  VDUP(vector, , int, s, 64, 1, 0x7FFFFFFFFFFFFFFFLL);
  VDUP(vector, , uint, u, 8, 8, 0xFF);
  VDUP(vector, , uint, u, 16, 4, 0xFFFF);
  VDUP(vector, , uint, u, 32, 2, 0xFFFFFFFF);
  VDUP(vector, , uint, u, 64, 1, 0xFFFFFFFFFFFFFFFFULL);
  VDUP(vector, q, int, s, 8, 16, 0x7F);
  VDUP(vector, q, int, s, 16, 8, 0x7FFF);
  VDUP(vector, q, int, s, 32, 4, 0x7FFFFFFF);
  VDUP(vector, q, int, s, 64, 2, 0x7FFFFFFFFFFFFFFFLL);
  VDUP(vector, q, uint, u, 8, 16, 0xFF);
  VDUP(vector, q, uint, u, 16, 8, 0xFFFF);
  VDUP(vector, q, uint, u, 32, 4, 0xFFFFFFFF);
  VDUP(vector, q, uint, u, 64, 2, 0xFFFFFFFFFFFFFFFFULL);

  /* Use -1 shift amount to check overflow with round_const */
  VDUP(vector_shift, , int, s, 8, 8, -1);
  VDUP(vector_shift, , int, s, 16, 4, -1);
  VDUP(vector_shift, , int, s, 32, 2, -1);
  VDUP(vector_shift, , int, s, 64, 1, -1);
  VDUP(vector_shift, q, int, s, 8, 16, -1);
  VDUP(vector_shift, q, int, s, 16, 8, -1);
  VDUP(vector_shift, q, int, s, 32, 4, -1);
  VDUP(vector_shift, q, int, s, 64, 2, -1);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (checking round_const overflow: shift by -1)");

  /* Use -3 shift amount to check overflow with round_const */
  VDUP(vector_shift, , int, s, 8, 8, -3);
  VDUP(vector_shift, , int, s, 16, 4, -3);
  VDUP(vector_shift, , int, s, 32, 2, -3);
  VDUP(vector_shift, , int, s, 64, 1, -3);
  VDUP(vector_shift, q, int, s, 8, 16, -3);
  VDUP(vector_shift, q, int, s, 16, 8, -3);
  VDUP(vector_shift, q, int, s, 32, 4, -3);
  VDUP(vector_shift, q, int, s, 64, 2, -3);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (checking round_const overflow: shift by -3)");

  /* Use negative shift amount as large as input vector width */
  VDUP(vector_shift, , int, s, 8, 8, -8);
  VDUP(vector_shift, , int, s, 16, 4, -16);
  VDUP(vector_shift, , int, s, 32, 2, -32);
  VDUP(vector_shift, , int, s, 64, 1, -64);
  VDUP(vector_shift, q, int, s, 8, 16, -8);
  VDUP(vector_shift, q, int, s, 16, 8, -16);
  VDUP(vector_shift, q, int, s, 32, 4, -32);
  VDUP(vector_shift, q, int, s, 64, 2, -64);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (checking negative shift amount as large as input vector width)");

  /* Test large shift amount */
  VDUP(vector_shift, , int, s, 8, 8, 10);
  VDUP(vector_shift, , int, s, 16, 4, 20);
  VDUP(vector_shift, , int, s, 32, 2, 33);
  VDUP(vector_shift, , int, s, 64, 1, 65);
  VDUP(vector_shift, q, int, s, 8, 16, 9);
  VDUP(vector_shift, q, int, s, 16, 8, 16);
  VDUP(vector_shift, q, int, s, 32, 4, 32);
  VDUP(vector_shift, q, int, s, 64, 2, 64);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (large shift amount)");

  /* Test large negative shift amount */
  VDUP(vector_shift, , int, s, 8, 8, -10);
  VDUP(vector_shift, , int, s, 16, 4, -20);
  VDUP(vector_shift, , int, s, 32, 2, -33);
  VDUP(vector_shift, , int, s, 64, 1, -65);
  VDUP(vector_shift, q, int, s, 8, 16, -9);
  VDUP(vector_shift, q, int, s, 16, 8, -16);
  VDUP(vector_shift, q, int, s, 32, 4, -32);
  VDUP(vector_shift, q, int, s, 64, 2, -64);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VRSHL, int);

  dump_results_hex2 (TEST_MSG, " (large negative shift amount)");
}
