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

#ifndef INSN_NAME
#define INSN_NAME vqdmlal_n
#define TEST_MSG "VQDMLAL_N"
#endif

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* vector_res = vqdmlxl_n(vector, vector3, val),
     then store the result.  */
#define TEST_VQDMLXL_N1(INSN, T1, T2, W, W2, N, V)		\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, T1, W, N));	\
  VECT_VAR(vector_res, T1, W, N) =				\
    INSN##_##T2##W2(VECT_VAR(vector, T1, W, N),			\
		    VECT_VAR(vector3, T1, W2, N),		\
		    V);						\
  vst1q_##T2##W(VECT_VAR(result, T1, W, N),			\
		VECT_VAR(vector_res, T1, W, N));		\
  dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##_##T2##W2),	\
			   xSTR(T1), W, N)

#define TEST_VQDMLXL_N(INSN, T1, T2, W, W2, N, V)	\
  TEST_VQDMLXL_N1(INSN, T1, T2, W, W2, N, V)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE(vector, int, 32, 4);
  DECL_VARIABLE(vector3, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 4);

  DECL_VARIABLE(vector, int, 64, 2);
  DECL_VARIABLE(vector3, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 64, 2);

  clean_results ();

  VLOAD(vector, buffer, q, int, s, 32, 4);
  VLOAD(vector, buffer, q, int, s, 64, 2);

  VDUP(vector3, , int, s, 16, 4, 0x55);
  VDUP(vector3, , int, s, 32, 2, 0x55);

  /* Choose val arbitrarily */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_VQDMLXL_N(INSN_NAME, int, s, 32, 16, 4, 0x22);
  TEST_VQDMLXL_N(INSN_NAME, int, s, 64, 32, 2, 0x33);

  dump_results_hex (TEST_MSG);

  VDUP(vector3, , int, s, 16, 4, 0x8000);
  VDUP(vector3, , int, s, 32, 2, 0x80000000);
  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (check mul cumulative saturation)");
  TEST_VQDMLXL_N(INSN_NAME, int, s, 32, 16, 4, 0x8000);
  TEST_VQDMLXL_N(INSN_NAME, int, s, 64, 32, 2, 0x80000000);
  dump_results_hex2 (TEST_MSG, " (check mul cumulative saturation)");
}
