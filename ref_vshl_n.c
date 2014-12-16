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

#define TEST_MSG "VSHL_N"
void exec_vshl_n (void)
{
  /* Basic test: v2=vshl_n(v1,v), then store the result.  */
#define TEST_VSHL_N(Q, T1, T2, W, N, V)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vshl##Q##_n_##T2##W(VECT_VAR(vector, T1, W, N),			\
			V);						\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */

  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Choose shift amount arbitrarily  */
  TEST_VSHL_N(, int, s, 8, 8, 1);
  TEST_VSHL_N(, int, s, 16, 4, 1);
  TEST_VSHL_N(, int, s, 32, 2, 3);
  TEST_VSHL_N(, int, s, 64, 1, 2);
  TEST_VSHL_N(, uint, u, 8, 8, 2);
  TEST_VSHL_N(, uint, u, 16, 4, 4);
  TEST_VSHL_N(, uint, u, 32, 2, 3);
  TEST_VSHL_N(, uint, u, 64, 1, 1);

  TEST_VSHL_N(q, int, s, 8, 16, 5);
  TEST_VSHL_N(q, int, s, 16, 8, 1);
  TEST_VSHL_N(q, int, s, 32, 4, 2);
  TEST_VSHL_N(q, int, s, 64, 2, 2);
  TEST_VSHL_N(q, uint, u, 8, 16, 2);
  TEST_VSHL_N(q, uint, u, 16, 8, 3);
  TEST_VSHL_N(q, uint, u, 32, 4, 2);
  TEST_VSHL_N(q, uint, u, 64, 2, 1);

  dump_results_hex (TEST_MSG);
}
