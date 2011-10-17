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

#ifdef __arm__
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"
#include <math.h>


#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  int i;

  /* Basic test: y=vcomp(x,x), then store the result.  */
#define TEST_VCOMP1(INSN, Q, T1, T2, T3, W, N)				\
  VECT_VAR(vector_res, T3, W, N) =					\
    INSN##Q##_##T2##W(VECT_VAR(vector, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N));			\
  vst1##Q##_u##W(VECT_VAR(result, T3, W, N), VECT_VAR(vector_res, T3, W, N))

#define TEST_VCOMP(INSN, Q, T1, T2, T3, W, N)				\
  TEST_VCOMP1(INSN, Q, T1, T2, T3, W, N)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  /* No need for 64 bits elements */
  DECL_VARIABLE(vector, int, 8, 8);
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, uint, 8, 8);
  DECL_VARIABLE(vector, uint, 16, 4);
  DECL_VARIABLE(vector, uint, 32, 2);
  DECL_VARIABLE(vector, float, 32, 2);
  DECL_VARIABLE(vector, int, 8, 16);
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);
  DECL_VARIABLE(vector, uint, 8, 16);
  DECL_VARIABLE(vector, uint, 16, 8);
  DECL_VARIABLE(vector, uint, 32, 4);
  DECL_VARIABLE(vector, float, 32, 4);

  DECL_VARIABLE(vector2, int, 8, 8);
  DECL_VARIABLE(vector2, int, 16, 4);
  DECL_VARIABLE(vector2, int, 32, 2);
  DECL_VARIABLE(vector2, uint, 8, 8);
  DECL_VARIABLE(vector2, uint, 16, 4);
  DECL_VARIABLE(vector2, uint, 32, 2);
  DECL_VARIABLE(vector2, float, 32, 2);
  DECL_VARIABLE(vector2, int, 8, 16);
  DECL_VARIABLE(vector2, int, 16, 8);
  DECL_VARIABLE(vector2, int, 32, 4);
  DECL_VARIABLE(vector2, uint, 8, 16);
  DECL_VARIABLE(vector2, uint, 16, 8);
  DECL_VARIABLE(vector2, uint, 32, 4);
  DECL_VARIABLE(vector2, float, 32, 4);

  DECL_VARIABLE(vector_res, uint, 8, 8);
  DECL_VARIABLE(vector_res, uint, 16, 4);
  DECL_VARIABLE(vector_res, uint, 32, 2);
  DECL_VARIABLE(vector_res, uint, 8, 16);
  DECL_VARIABLE(vector_res, uint, 16, 8);
  DECL_VARIABLE(vector_res, uint, 32, 4);

  clean_results ();

  /* There is no 64 bits variant, we can't use the generic initializer  */
  TEST_VLOAD(vector, buffer, , int, s, 8, 8);
  TEST_VLOAD(vector, buffer, , int, s, 16, 4);
  TEST_VLOAD(vector, buffer, , int, s, 32, 2);
  TEST_VLOAD(vector, buffer, , uint, u, 8, 8);
  TEST_VLOAD(vector, buffer, , uint, u, 16, 4);
  TEST_VLOAD(vector, buffer, , uint, u, 32, 2);
  TEST_VLOAD(vector, buffer, , float, f, 32, 2);

  TEST_VLOAD(vector, buffer, q, int, s, 8, 16);
  TEST_VLOAD(vector, buffer, q, int, s, 16, 8);
  TEST_VLOAD(vector, buffer, q, int, s, 32, 4);
  TEST_VLOAD(vector, buffer, q, uint, u, 8, 16);
  TEST_VLOAD(vector, buffer, q, uint, u, 16, 8);
  TEST_VLOAD(vector, buffer, q, uint, u, 32, 4);
  TEST_VLOAD(vector, buffer, q, float, f, 32, 4);

  /* Choose init value arbitrarily, will be used for vector
     comparison */
  TEST_VDUP(vector2, , int, s, 8, 8, -10);
  TEST_VDUP(vector2, , int, s, 16, 4, -14);
  TEST_VDUP(vector2, , int, s, 32, 2, -16);
  TEST_VDUP(vector2, , uint, u, 8, 8, 0xF3);
  TEST_VDUP(vector2, , uint, u, 16, 4, 0xFFF2);
  TEST_VDUP(vector2, , uint, u, 32, 2, 0xFFFFFFF1);
  TEST_VDUP(vector2, , float, f, 32, 2, -15.0f);

  TEST_VDUP(vector2, q, int, s, 8, 16, -4);
  TEST_VDUP(vector2, q, int, s, 16, 8, -10);
  TEST_VDUP(vector2, q, int, s, 32, 4, -14);
  TEST_VDUP(vector2, q, uint, u, 8, 16, 0xF4);
  TEST_VDUP(vector2, q, uint, u, 16, 8, 0xFFF6);
  TEST_VDUP(vector2, q, uint, u, 32, 4, 0xFFFFFFF2);
  TEST_VDUP(vector2, q, float, f, 32, 4, -14.0f);

  /* The same result buffers are used multiple times, so output them
     before overwriting them  */
  fprintf(ref_file, "\n%s output:\n", TEST_MSG);
  TEST_VCOMP(INSN_NAME, , int, s, uint, 8, 8);
  TEST_VCOMP(INSN_NAME, , int, s, uint, 16, 4);
  TEST_VCOMP(INSN_NAME, , int, s, uint, 32, 2);
  DUMP(TEST_MSG, uint, 8, 8, PRIx8);
  DUMP(TEST_MSG, uint, 16, 4, PRIx16);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);

  TEST_VCOMP(INSN_NAME, , uint, u, uint, 8, 8);
  TEST_VCOMP(INSN_NAME, , uint, u, uint, 16, 4);
  TEST_VCOMP(INSN_NAME, , uint, u, uint, 32, 2);
  DUMP(TEST_MSG, uint, 8, 8, PRIx8);
  DUMP(TEST_MSG, uint, 16, 4, PRIx16);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);

  TEST_VCOMP(INSN_NAME, q, int, s, uint, 8, 16);
  TEST_VCOMP(INSN_NAME, q, int, s, uint, 16, 8);
  TEST_VCOMP(INSN_NAME, q, int, s, uint, 32, 4);
  DUMP(TEST_MSG, uint, 8, 16, PRIx8);
  DUMP(TEST_MSG, uint, 16, 8, PRIx16);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);

  TEST_VCOMP(INSN_NAME, q, uint, u, uint, 8, 16);
  TEST_VCOMP(INSN_NAME, q, uint, u, uint, 16, 8);
  TEST_VCOMP(INSN_NAME, q, uint, u, uint, 32, 4);
  DUMP(TEST_MSG, uint, 8, 16, PRIx8);
  DUMP(TEST_MSG, uint, 16, 8, PRIx16);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);

  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);

  TEST_VCOMP(INSN_NAME, q, float, f, uint, 32, 4);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);

  /* Extra tests to have 100% coverage on all the variants */
  TEST_VDUP(vector2, , uint, u, 32, 2, 0xFFFFFFF0);
  TEST_VCOMP(INSN_NAME, , uint, u, uint, 32, 2);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);

  TEST_VDUP(vector2, , int, s, 32, 2, -15);
  TEST_VCOMP(INSN_NAME, , int, s, uint, 32, 2);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);

  TEST_VDUP(vector2, , float, f, 32, 2, -16.0f);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);


  /* Extra FP tests with special values (NaN, ....) */
  TEST_VDUP(vector, , float, f, 32, 2, 1.0);
  TEST_VDUP(vector2, , float, f, 32, 2, NAN);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG " FP special (NaN)", uint, 32, 2, PRIx32);

  TEST_VDUP(vector, , float, f, 32, 2, 1.0);
  TEST_VDUP(vector2, , float, f, 32, 2, -NAN);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG " FP special (-NaN)", uint, 32, 2, PRIx32);

  TEST_VDUP(vector, , float, f, 32, 2, NAN);
  TEST_VDUP(vector2, , float, f, 32, 2, 1.0);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG " FP special (NaN)", uint, 32, 2, PRIx32);

  TEST_VDUP(vector, , float, f, 32, 2, 1.0);
  TEST_VDUP(vector2, , float, f, 32, 2, HUGE_VALF);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG " FP special (inf)", uint, 32, 2, PRIx32);

  TEST_VDUP(vector, , float, f, 32, 2, 1.0);
  TEST_VDUP(vector2, , float, f, 32, 2, -HUGE_VALF);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG " FP special (-inf)", uint, 32, 2, PRIx32);

  TEST_VDUP(vector, , float, f, 32, 2, HUGE_VALF);
  TEST_VDUP(vector2, , float, f, 32, 2, 1.0);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG " FP special (inf)", uint, 32, 2, PRIx32);

  TEST_VDUP(vector, , float, f, 32, 2, -0.0);
  TEST_VDUP(vector2, , float, f, 32, 2, 0.0);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG " FP special (-0.0)", uint, 32, 2, PRIx32);
}
