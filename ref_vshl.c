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

#define TEST_MSG "VSHL/VSHLQ"
void exec_vshl (void)
{
  /* Basic test: v3=vshl(v1,v2), then store the result.  */
#define TEST_VSHL(T3, Q, T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vshl##Q##_##T2##W(VECT_VAR(vector, T1, W, N),			\
		      VECT_VAR(vector_shift, T3, W, N));		\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  DECL_VARIABLE_SIGNED_VARIANTS(vector_shift);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Choose init value arbitrarily, will be used as shift amount */
  VDUP(vector_shift, , int, s, 8, 8, 1);
  VDUP(vector_shift, , int, s, 16, 4, 3);
  VDUP(vector_shift, , int, s, 32, 2, 8);
  VDUP(vector_shift, , int, s, 64, 1, 3);
  VDUP(vector_shift, q, int, s, 8, 16, 5);
  VDUP(vector_shift, q, int, s, 16, 8, 12);
  VDUP(vector_shift, q, int, s, 32, 4, 30);
  VDUP(vector_shift, q, int, s, 64, 2, 63);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VSHL, int);

  dump_results_hex (TEST_MSG);

  /* Test large shift amount */
  VDUP(vector_shift, , int, s, 8, 8, 8);
  VDUP(vector_shift, , int, s, 16, 4, 16);
  VDUP(vector_shift, , int, s, 32, 2, 32);
  VDUP(vector_shift, , int, s, 64, 1, 64);
  VDUP(vector_shift, q, int, s, 8, 16, 8);
  VDUP(vector_shift, q, int, s, 16, 8, 17);
  VDUP(vector_shift, q, int, s, 32, 4, 33);
  VDUP(vector_shift, q, int, s, 64, 2, 65);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VSHL, int);

  dump_results_hex2 (TEST_MSG, " (large shift amount)");


  /* Test negative shift amount */
  VDUP(vector_shift, , int, s, 8, 8, -1);
  VDUP(vector_shift, , int, s, 16, 4, -1);
  VDUP(vector_shift, , int, s, 32, 2, -2);
  VDUP(vector_shift, , int, s, 64, 1, -4);
  VDUP(vector_shift, q, int, s, 8, 16, -2);
  VDUP(vector_shift, q, int, s, 16, 8, -5);
  VDUP(vector_shift, q, int, s, 32, 4, -3);
  VDUP(vector_shift, q, int, s, 64, 2, -5);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VSHL, int);

  dump_results_hex2 (TEST_MSG, " (negative shift amount)");
}
