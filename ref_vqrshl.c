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

#define INSN vqrshl
#define TEST_MSG "VQRSHL/VQRSHLQ"

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN)
{
  /* Basic test: v3=vqrshl(v1,v2), then store the result.  */
#define TEST_VQRSHL2(INSN, T3, Q, T1, T2, W, N)			\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, T1, W, N));	\
  VECT_VAR(vector_res, T1, W, N) =				\
    INSN##Q##_##T2##W(VECT_VAR(vector, T1, W, N),		\
		      VECT_VAR(vector_shift, T3, W, N));	\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),			\
		    VECT_VAR(vector_res, T1, W, N));		\
  dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##Q##_##T2##W),	\
			   xSTR(T1), W, N)

  /* Two auxliary macros are necessary to expand INSN */
#define TEST_VQRSHL1(INSN, T3, Q, T1, T2, W, N)	\
  TEST_VQRSHL2(INSN, T3, Q, T1, T2, W, N)

#define TEST_VQRSHL(T3, Q, T1, T2, W, N)	\
  TEST_VQRSHL1(INSN, T3, Q, T1, T2, W, N)


  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  DECL_VARIABLE_SIGNED_VARIANTS(vector_shift);

  clean_results ();

  /* Fill input vector with 0, to check saturation on limits */
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
  /* Use values equal or one-less-than the type width to check
     behaviour on limits */
  VDUP(vector_shift, , int, s, 8, 8, 7);
  VDUP(vector_shift, , int, s, 16, 4, 15);
  VDUP(vector_shift, , int, s, 32, 2, 31);
  VDUP(vector_shift, , int, s, 64, 1, 63);
  VDUP(vector_shift, q, int, s, 8, 16, 8);
  VDUP(vector_shift, q, int, s, 16, 8, 16);
  VDUP(vector_shift, q, int, s, 32, 4, 32);
  VDUP(vector_shift, q, int, s, 64, 2, 64);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (with input = 0)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
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

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (input 0 and negative shift amount)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG, " (input 0 and negative shift amount)");

  /* Test again, with predefined input values */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Choose init value arbitrarily, will be used as shift amount */
  VDUP(vector_shift, , int, s, 8, 8, 1);
  VDUP(vector_shift, , int, s, 16, 4, 3);
  VDUP(vector_shift, , int, s, 32, 2, 8);
  VDUP(vector_shift, , int, s, 64, 1, 3);
  VDUP(vector_shift, q, int, s, 8, 16, 10);
  VDUP(vector_shift, q, int, s, 16, 8, 12);
  VDUP(vector_shift, q, int, s, 32, 4, 31);
  VDUP(vector_shift, q, int, s, 64, 2, 63);

  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex (TEST_MSG);

  /* Use negative shift amounts */
  VDUP(vector_shift, , int, s, 8, 8, -2);
  VDUP(vector_shift, , int, s, 16, 4, -2);
  VDUP(vector_shift, , int, s, 32, 2, -3);
  VDUP(vector_shift, , int, s, 64, 1, -4);
  VDUP(vector_shift, q, int, s, 8, 16, -7);
  VDUP(vector_shift, q, int, s, 16, 8, -11);
  VDUP(vector_shift, q, int, s, 32, 4, -13);
  VDUP(vector_shift, q, int, s, 64, 2, -20);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (negative shift amount)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG, " (negative shift amount)");


  /* Fill input vector with max value, to check saturation on limits */
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

  /* Use -1 shift amount to check cumulative saturation with round_const */
  VDUP(vector_shift, , int, s, 8, 8, -1);
  VDUP(vector_shift, , int, s, 16, 4, -1);
  VDUP(vector_shift, , int, s, 32, 2, -1);
  VDUP(vector_shift, , int, s, 64, 1, -1);
  VDUP(vector_shift, q, int, s, 8, 16, -1);
  VDUP(vector_shift, q, int, s, 16, 8, -1);
  VDUP(vector_shift, q, int, s, 32, 4, -1);
  VDUP(vector_shift, q, int, s, 64, 2, -1);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (checking cumulative saturation: shift by -1)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG,
		     " (checking cumulative saturation: shift by -1)");


  /* Use -3 shift amount to check cumulative saturation with round_const */
  VDUP(vector_shift, , int, s, 8, 8, -3);
  VDUP(vector_shift, , int, s, 16, 4, -3);
  VDUP(vector_shift, , int, s, 32, 2, -3);
  VDUP(vector_shift, , int, s, 64, 1, -3);
  VDUP(vector_shift, q, int, s, 8, 16, -3);
  VDUP(vector_shift, q, int, s, 16, 8, -3);
  VDUP(vector_shift, q, int, s, 32, 4, -3);
  VDUP(vector_shift, q, int, s, 64, 2, -3);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (checking cumulative saturation: shift by -3)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG,
		     " (checking cumulative saturation: shift by -3)");


  /* Use large shift amount  */
  VDUP(vector_shift, , int, s, 8, 8, 10);
  VDUP(vector_shift, , int, s, 16, 4, 20);
  VDUP(vector_shift, , int, s, 32, 2, 40);
  VDUP(vector_shift, , int, s, 64, 1, 70);
  VDUP(vector_shift, q, int, s, 8, 16, 10);
  VDUP(vector_shift, q, int, s, 16, 8, 20);
  VDUP(vector_shift, q, int, s, 32, 4, 40);
  VDUP(vector_shift, q, int, s, 64, 2, 70);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (checking cumulative saturation: large shift amount)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG,
		     " (checking cumulative saturation: large shift amount)");


  /* Fill input vector with negative values, to check saturation on limits */
  VDUP(vector, , int, s, 8, 8, 0x80);
  VDUP(vector, , int, s, 16, 4, 0x8000);
  VDUP(vector, , int, s, 32, 2, 0x80000000);
  VDUP(vector, , int, s, 64, 1, 0x8000000000000000LL);
  VDUP(vector, q, int, s, 8, 16, 0x80);
  VDUP(vector, q, int, s, 16, 8, 0x8000);
  VDUP(vector, q, int, s, 32, 4, 0x80000000);
  VDUP(vector, q, int, s, 64, 2, 0x8000000000000000LL);

  /* Use large shift amount  */
  VDUP(vector_shift, , int, s, 8, 8, 10);
  VDUP(vector_shift, , int, s, 16, 4, 20);
  VDUP(vector_shift, , int, s, 32, 2, 40);
  VDUP(vector_shift, , int, s, 64, 1, 70);
  VDUP(vector_shift, q, int, s, 8, 16, 10);
  VDUP(vector_shift, q, int, s, 16, 8, 20);
  VDUP(vector_shift, q, int, s, 32, 4, 40);
  VDUP(vector_shift, q, int, s, 64, 2, 70);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (checking cumulative saturation: large shift amount with negative input)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG,
		     " (checking cumulative saturation: large shift amount with negative input)");


  /* Fill input vector with negative and positive values, to check
   * saturation on limits */
  VDUP(vector, , int, s, 8, 8, 0x7F);
  VDUP(vector, , int, s, 16, 4, 0x7FFF);
  VDUP(vector, , int, s, 32, 2, 0x7FFFFFFF);
  VDUP(vector, , int, s, 64, 1, 0x7FFFFFFFFFFFFFFFLL);
  VDUP(vector, q, int, s, 8, 16, 0x80);
  VDUP(vector, q, int, s, 16, 8, 0x8000);
  VDUP(vector, q, int, s, 32, 4, 0x80000000);
  VDUP(vector, q, int, s, 64, 2, 0x8000000000000000LL);

  /* Use large negative shift amount  */
  VDUP(vector_shift, , int, s, 8, 8, -10);
  VDUP(vector_shift, , int, s, 16, 4, -20);
  VDUP(vector_shift, , int, s, 32, 2, -40);
  VDUP(vector_shift, , int, s, 64, 1, -70);
  VDUP(vector_shift, q, int, s, 8, 16, -10);
  VDUP(vector_shift, q, int, s, 16, 8, -20);
  VDUP(vector_shift, q, int, s, 32, 4, -40);
  VDUP(vector_shift, q, int, s, 64, 2, -70);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (checking cumulative saturation: large negative shift amount)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG,
		     " (checking cumulative saturation: large negative shift amount)");


  /* Fill input vector with 0, to check saturation in case of large
   * shift amount */
  VDUP(vector, , int, s, 8, 8, 0);
  VDUP(vector, , int, s, 16, 4, 0);
  VDUP(vector, , int, s, 32, 2, 0);
  VDUP(vector, , int, s, 64, 1, 0);
  VDUP(vector, q, int, s, 8, 16, 0);
  VDUP(vector, q, int, s, 16, 8, 0);
  VDUP(vector, q, int, s, 32, 4, 0);
  VDUP(vector, q, int, s, 64, 2, 0);

  /* Use large shift amount  */
  VDUP(vector_shift, , int, s, 8, 8, -10);
  VDUP(vector_shift, , int, s, 16, 4, -20);
  VDUP(vector_shift, , int, s, 32, 2, -40);
  VDUP(vector_shift, , int, s, 64, 1, -70);
  VDUP(vector_shift, q, int, s, 8, 16, -10);
  VDUP(vector_shift, q, int, s, 16, 8, -20);
  VDUP(vector_shift, q, int, s, 32, 4, -40);
  VDUP(vector_shift, q, int, s, 64, 2, -70);

  fprintf(ref_file, "\n%s cumulative saturation output:\n",
	  TEST_MSG " (checking cumulative saturation: large shift amount with 0 input)");
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VQRSHL, int);
  dump_results_hex2 (TEST_MSG,
		     " (checking cumulative saturation: large shift amount with 0 input)");
}
