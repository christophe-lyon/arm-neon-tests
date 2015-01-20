/*

Copyright (c) 2009, 2010, 2011, 2012 STMicroelectronics
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

#define INSN vqshlu
#define TEST_MSG "VQSHLU_N/VQSHLUQ_N"

#define FNNAME1(NAME) void exec_ ## NAME ## _n(void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN)
{
  /* Basic test: v2=vqshlu_n(v1,v), then store the result.  */
#define TEST_VQSHLU_N2(INSN, Q, T1, T2, T3, T4, W, N, V)		\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, T3, W, N));		\
  VECT_VAR(vector_res, T3, W, N) =					\
    INSN##Q##_n_##T2##W(VECT_VAR(vector, T1, W, N),			\
			V);						\
    vst1##Q##_##T4##W(VECT_VAR(result, T3, W, N),			\
		      VECT_VAR(vector_res, T3, W, N));			\
    dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##Q##_n_##T2##W),	\
			       xSTR(T1), W, N)

  /* Two auxliary macros are necessary to expand INSN */
#define TEST_VQSHLU_N1(INSN, Q, T1, T2, T3, T4, W, N, V)	\
  TEST_VQSHLU_N2(INSN, Q, T1, T2, T3, T4, W, N, V)

#define TEST_VQSHLU_N(Q, T1, T2, T3, T4, W, N, V)		\
  TEST_VQSHLU_N1(INSN, Q, T1, T2, T3, T4, W, N, V)


  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Fill input vector with negative values, to check saturation on limits */
  VDUP(vector, , int, s, 8, 8, -1);
  VDUP(vector, , int, s, 16, 4, -2);
  VDUP(vector, , int, s, 32, 2, -3);
  VDUP(vector, , int, s, 64, 1, -4);
  VDUP(vector, q, int, s, 8, 16, -1);
  VDUP(vector, q, int, s, 16, 8, -2);
  VDUP(vector, q, int, s, 32, 4, -3);
  VDUP(vector, q, int, s, 64, 2, -4);

  /* Choose shift amount arbitrarily  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (negative input)");
  TEST_VQSHLU_N(, int, s, uint, u, 8, 8, 2);
  TEST_VQSHLU_N(, int, s, uint, u, 16, 4, 1);
  TEST_VQSHLU_N(, int, s, uint, u, 32, 2, 1);
  TEST_VQSHLU_N(, int, s, uint, u, 64, 1, 2);

  TEST_VQSHLU_N(q, int, s, uint, u, 8, 16, 2);
  TEST_VQSHLU_N(q, int, s, uint, u, 16, 8, 1);
  TEST_VQSHLU_N(q, int, s, uint, u, 32, 4, 1);
  TEST_VQSHLU_N(q, int, s, uint, u, 64, 2, 2);

  /* FIXME: only a few result buffers are used, but we output all of them */
  dump_results_hex2 (TEST_MSG, " (negative input)");

  /* Fill input vector with max value, to check saturation on limits */
  VDUP(vector, , int, s, 8, 8, 0x7F);
  VDUP(vector, , int, s, 16, 4, 0x7FFF);
  VDUP(vector, , int, s, 32, 2, 0x7FFFFFFF);
  VDUP(vector, , int, s, 64, 1, 0x7FFFFFFFFFFFFFFFLL);
  VDUP(vector, q, int, s, 8, 16, 0x7F);
  VDUP(vector, q, int, s, 16, 8, 0x7FFF);
  VDUP(vector, q, int, s, 32, 4, 0x7FFFFFFF);
  VDUP(vector, q, int, s, 64, 2, 0x7FFFFFFFFFFFFFFFULL);

  /* shift by 1  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (check cumulative saturation: shift by 1)");
  TEST_VQSHLU_N(, int, s, uint, u, 8, 8, 1);
  TEST_VQSHLU_N(, int, s, uint, u, 16, 4, 1);
  TEST_VQSHLU_N(, int, s, uint, u, 32, 2, 1);
  TEST_VQSHLU_N(, int, s, uint, u, 64, 1, 1);

  TEST_VQSHLU_N(q, int, s, uint, u, 8, 16, 1);
  TEST_VQSHLU_N(q, int, s, uint, u, 16, 8, 1);
  TEST_VQSHLU_N(q, int, s, uint, u, 32, 4, 1);
  TEST_VQSHLU_N(q, int, s, uint, u, 64, 2, 1);

  dump_results_hex2 (TEST_MSG, " (check cumulative saturation: shift by 1)");

  /* shift by 2 to force saturation  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (check cumulative saturation: shift by 2)");
  TEST_VQSHLU_N(, int, s, uint, u, 8, 8, 2);
  TEST_VQSHLU_N(, int, s, uint, u, 16, 4, 2);
  TEST_VQSHLU_N(, int, s, uint, u, 32, 2, 2);
  TEST_VQSHLU_N(, int, s, uint, u, 64, 1, 2);

  TEST_VQSHLU_N(q, int, s, uint, u, 8, 16, 2);
  TEST_VQSHLU_N(q, int, s, uint, u, 16, 8, 2);
  TEST_VQSHLU_N(q, int, s, uint, u, 32, 4, 2);
  TEST_VQSHLU_N(q, int, s, uint, u, 64, 2, 2);

  dump_results_hex2 (TEST_MSG, " (check cumulative saturation: shift by 2)");

  /* Fill input vector with positive values, to check normal case */
  VDUP(vector, , int, s, 8, 8, 1);
  VDUP(vector, , int, s, 16, 4, 2);
  VDUP(vector, , int, s, 32, 2, 3);
  VDUP(vector, , int, s, 64, 1, 4);
  VDUP(vector, q, int, s, 8, 16, 5);
  VDUP(vector, q, int, s, 16, 8, 6);
  VDUP(vector, q, int, s, 32, 4, 7);
  VDUP(vector, q, int, s, 64, 2, 8);

  /* shift arbitrary amount  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_VQSHLU_N(, int, s, uint, u, 8, 8, 1);
  TEST_VQSHLU_N(, int, s, uint, u, 16, 4, 2);
  TEST_VQSHLU_N(, int, s, uint, u, 32, 2, 3);
  TEST_VQSHLU_N(, int, s, uint, u, 64, 1, 4);

  TEST_VQSHLU_N(q, int, s, uint, u, 8, 16, 5);
  TEST_VQSHLU_N(q, int, s, uint, u, 16, 8, 6);
  TEST_VQSHLU_N(q, int, s, uint, u, 32, 4, 7);
  TEST_VQSHLU_N(q, int, s, uint, u, 64, 2, 8);

  dump_results_hex (TEST_MSG);
}
