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
  DECL_VARIABLE(vector, float, 32, 2);
  DECL_VARIABLE(vector, float, 32, 4);
  DECL_VARIABLE(vector2, float, 32, 2);
  DECL_VARIABLE(vector2, float, 32, 4);
  DECL_VARIABLE(vector_res, uint, 32, 2);
  DECL_VARIABLE(vector_res, uint, 32, 4);

  clean_results ();

  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);

  /* Choose init value arbitrarily, will be used for vector
     comparison */
  VDUP(vector2, , float, f, 32, 2, -16.0f);
  VDUP(vector2, q, float, f, 32, 4, -14.0f);

  fprintf(ref_file, "\n%s output:\n", TEST_MSG);
  fprintf(gcc_tests_file, "\n%s output:\n", TEST_MSG);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);

  TEST_VCOMP(INSN_NAME, q, float, f, uint, 32, 4);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);

  VDUP(vector2, , float, f, 32, 2, -10.0f);
  VDUP(vector2, q, float, f, 32, 4, 10.0f);

  fprintf(ref_file, "\n%s output:\n", TEST_MSG);
  fprintf(gcc_tests_file, "\n%s output:\n", TEST_MSG);
  TEST_VCOMP(INSN_NAME, , float, f, uint, 32, 2);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);

  TEST_VCOMP(INSN_NAME, q, float, f, uint, 32, 4);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);
}
