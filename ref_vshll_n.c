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

#define TEST_MSG "VSHLL_N"
void exec_vshll_n (void)
{
  /* Basic test: v2=vshll_n(v1,v), then store the result.  */
#define TEST_VSHLL_N(T1, T2, W, W2, N, V)				\
  VECT_VAR(vector_res, T1, W2, N) =					\
    vshll##_n_##T2##W(VECT_VAR(vector, T1, W, N),			\
		      V);						\
  vst1q##_##T2##W2(VECT_VAR(result, T1, W2, N), VECT_VAR(vector_res, T1, W2, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */

  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Choose shift amount arbitrarily  */
  TEST_VSHLL_N(int, s, 8, 16, 8, 1);
  TEST_VSHLL_N(int, s, 16, 32, 4, 1);
  TEST_VSHLL_N(int, s, 32, 64, 2, 3);
  TEST_VSHLL_N(uint, u, 8, 16, 8, 2);
  TEST_VSHLL_N(uint, u, 16, 32, 4, 4);
  TEST_VSHLL_N(uint, u, 32, 64, 2, 3);

  dump_results_hex (TEST_MSG);
}
