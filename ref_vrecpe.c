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
#include <math.h>

#define TEST_MSG "VRECPE/VRECPEQ"
void exec_vrecpe(void)
{
  int i;

  /* Basic test: y=vrecpe(x), then store the result.  */
#define TEST_VRECPE(Q, T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vrecpe##Q##_##T2##W(VECT_VAR(vector, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),				\
		    VECT_VAR(vector_res, T1, W, N))

    /* With ARM RVCT, we need to declare variables before any executable
       statement  */

  /* No need for 64 bits variants */
  DECL_VARIABLE(vector, uint, 32, 2);
  DECL_VARIABLE(vector, uint, 32, 4);
  DECL_VARIABLE(vector, float, 32, 2);
  DECL_VARIABLE(vector, float, 32, 4);

  DECL_VARIABLE(vector_res, uint, 32, 2);
  DECL_VARIABLE(vector_res, uint, 32, 4);
  DECL_VARIABLE(vector_res, float, 32, 2);
  DECL_VARIABLE(vector_res, float, 32, 4);

  clean_results ();

  /* Choose init value arbitrarily */
  VDUP(vector, , uint, u, 32, 2, 0x12345678);
  VDUP(vector, , float, f, 32, 2, 1.9f);
  VDUP(vector, q, uint, u, 32, 4, 0xABCDEF10);
  VDUP(vector, q, float, f, 32, 4, 125.0f);

  /* Apply the operator */
  TEST_VRECPE(, uint, u, 32, 2);
  TEST_VRECPE(, float, f, 32, 2);
  TEST_VRECPE(q, uint, u, 32, 4);
  TEST_VRECPE(q, float, f, 32, 4);

  fprintf (ref_file, "\n%s %s output:\n", TEST_MSG, " (positive input)");
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 4, PRIx32);

  /* Choose init value arbitrarily */
  VDUP(vector, , uint, u, 32, 2, 0xFFFFFFFF);
  VDUP(vector, , float, f, 32, 2, -10.0f);
  VDUP(vector, q, uint, u, 32, 4, 0x89081234);
  VDUP(vector, q, float, f, 32, 4, -125.0f);

  /* Apply the operator */
  TEST_VRECPE(, uint, u, 32, 2);
  TEST_VRECPE(, float, f, 32, 2);
  TEST_VRECPE(q, uint, u, 32, 4);
  TEST_VRECPE(q, float, f, 32, 4);

  fprintf (ref_file, "\n%s %s output:\n", TEST_MSG, " (negative input)");
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 4, PRIx32);

  /* Test FP variants with special input values (NaN, infinity) */
  VDUP(vector, , float, f, 32, 2, NAN);
  VDUP(vector, q, float, f, 32, 4, HUGE_VALF);

  /* Apply the operator */
  TEST_VRECPE(, float, f, 32, 2);
  TEST_VRECPE(q, float, f, 32, 4);

  fprintf (ref_file, "\n%s %s output:\n", TEST_MSG, " FP special (NaN, infinity)");
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 4, PRIx32);

  /* Test FP variants with special input values (zero, large value) */
  VDUP(vector, , float, f, 32, 2, 0.0f);
  VDUP(vector, q, float, f, 32, 4, 9.0e37f);

  /* Apply the operator */
  TEST_VRECPE(, float, f, 32, 2);
  TEST_VRECPE(q, float, f, 32, 4);

  fprintf (ref_file, "\n%s %s output:\n", TEST_MSG, " FP special (zero, large value)");
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 4, PRIx32);

  /* Test FP variants with special input values (-0, -infinity) */
  VDUP(vector, , float, f, 32, 2, -0.0f);
  VDUP(vector, q, float, f, 32, 4, -HUGE_VALF);

  /* Apply the operator */
  TEST_VRECPE(, float, f, 32, 2);
  TEST_VRECPE(q, float, f, 32, 4);

  fprintf (ref_file, "\n%s %s output:\n", TEST_MSG, " FP special (-0, -infinity)");
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 4, PRIx32);

  /* Test FP variants with special input values (large negative value) */
  VDUP(vector, , float, f, 32, 2, -9.0e37f);

  /* Apply the operator */
  TEST_VRECPE(, float, f, 32, 2);

  fprintf (ref_file, "\n%s %s output:\n", TEST_MSG, " FP special (large negative value)");
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
}
