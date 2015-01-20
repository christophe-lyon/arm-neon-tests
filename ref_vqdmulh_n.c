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

#define INSN vqdmulh
#define TEST_MSG "VQDMULH_N"
#define FNNAME1(NAME) void exec_ ## NAME ## _n (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN)
{
  int i;

  /* vector_res = vqdmulh_n(vector,val), then store the result.  */
#define TEST_VQDMULH_N2(INSN, Q, T1, T2, W, N, L)		\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, T1, W, N));	\
  VECT_VAR(vector_res, T1, W, N) =				\
    INSN##Q##_n_##T2##W(VECT_VAR(vector, T1, W, N),		\
			L);					\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),			\
		    VECT_VAR(vector_res, T1, W, N));		\
  dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##Q##_n_##T2##W),	\
			   xSTR(T1), W, N)

  /* Two auxliary macros are necessary to expand INSN */
#define TEST_VQDMULH_N1(INSN, Q, T1, T2, W, N, L)	\
  TEST_VQDMULH_N2(INSN, Q, T1, T2, W, N, L)

#define TEST_VQDMULH_N(Q, T1, T2, W, N, L)	\
  TEST_VQDMULH_N1(INSN, Q, T1, T2, W, N, L)


  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);

  DECL_VARIABLE(vector_res, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 16, 8);
  DECL_VARIABLE(vector_res, int, 32, 4);

  clean_results ();


  /* Initialize vector */
  VDUP(vector, , int, s, 16, 4, 0x1000);
  VDUP(vector, , int, s, 32, 2, 0x100023);
  VDUP(vector, q, int, s, 16, 8, 0x1000);
  VDUP(vector, q, int, s, 32, 4, 0x100045);

  /* Choose multiplier arbitrarily */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_VQDMULH_N(, int, s, 16, 4, 0xCF);
  TEST_VQDMULH_N(, int, s, 32, 2, 0x2344);
  TEST_VQDMULH_N(q, int, s, 16, 8, 0x80);
  TEST_VQDMULH_N(q, int, s, 32, 4, 0x5422);

  fprintf(ref_file, "\n%s output:\n", TEST_MSG);
  fprintf(gcc_tests_file, "\n%s output:\n", TEST_MSG);
  DUMP(TEST_MSG, int, 16, 4, PRIx16);
  DUMP(TEST_MSG, int, 32, 2, PRIx32);
  DUMP(TEST_MSG, int, 16, 8, PRIx16);
  DUMP(TEST_MSG, int, 32, 4, PRIx32);



  VDUP(vector, , int, s, 16, 4, 0x8000);
  VDUP(vector, , int, s, 32, 2, 0x80000000);
  VDUP(vector, q, int, s, 16, 8, 0x8000);
  VDUP(vector, q, int, s, 32, 4, 0x80000000);
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (check mul cumulative saturation)");
  TEST_VQDMULH_N(, int, s, 16, 4, 0x8000);
  TEST_VQDMULH_N(, int, s, 32, 2, 0x80000000);
  TEST_VQDMULH_N(q, int, s, 16, 8, 0x8000);
  TEST_VQDMULH_N(q, int, s, 32, 4, 0x80000000);
  dump_results_hex2 (TEST_MSG, " (check mul cumulative saturation)");
}
