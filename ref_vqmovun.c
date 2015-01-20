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

#define INSN_NAME vqmovun
#define TEST_MSG "VQMOVUN"

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* Basic test: y=OP(x), then store the result.  */
#define TEST_UNARY_OP1(INSN, T1, T2, W, W2, N)			\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, T1, W, N));	\
  VECT_VAR(vector_res, T1, W, N) =				\
    INSN##_s##W2(VECT_VAR(vector, int, W2, N));			\
  vst1##_##T2##W(VECT_VAR(result, T1, W, N),			\
		 VECT_VAR(vector_res, T1, W, N));		\
  dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##_s##W2),	\
			   xSTR(T1), W, N)

#define TEST_UNARY_OP(INSN, T1, T2, W, W2, N)	\
  TEST_UNARY_OP1(INSN, T1, T2, W, W2, N)	\

  /* With ARM RVCT, we need to declare variables before any executable
     statement  */

  /* No need for 64 bits variants */
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);
  DECL_VARIABLE(vector, int, 64, 2);

  DECL_VARIABLE(vector_res, uint, 8, 8);
  DECL_VARIABLE(vector_res, uint, 16, 4);
  DECL_VARIABLE(vector_res, uint, 32, 2);

  clean_results ();

  /* Fill input vector with arbitrary values */
  VDUP(vector, q, int, s, 16, 8, 0x34);
  VDUP(vector, q, int, s, 32, 4, 0x5678);
  VDUP(vector, q, int, s, 64, 2, 0x12345678);

  /* Apply a unary operator named INSN_NAME  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_UNARY_OP(INSN_NAME, uint, u, 8, 16, 8);
  TEST_UNARY_OP(INSN_NAME, uint, u, 16, 32, 4);
  TEST_UNARY_OP(INSN_NAME, uint, u, 32, 64, 2);

  dump_results_hex (TEST_MSG);

  /* Fill input vector with negative values */
  VDUP(vector, q, int, s, 16, 8, 0x8234);
  VDUP(vector, q, int, s, 32, 4, 0x87654321);
  VDUP(vector, q, int, s, 64, 2, 0x8765432187654321LL);

  /* Apply a unary operator named INSN_NAME  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (negative input)");
  TEST_UNARY_OP(INSN_NAME, uint, u, 8, 16, 8);
  TEST_UNARY_OP(INSN_NAME, uint, u, 16, 32, 4);
  TEST_UNARY_OP(INSN_NAME, uint, u, 32, 64, 2);

  dump_results_hex2 (TEST_MSG, " (negative input)");
}
