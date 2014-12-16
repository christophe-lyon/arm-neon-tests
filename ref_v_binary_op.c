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

/* Template file for binary operator validation */

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
  /* Basic test: y=OP(x,x), then store the result.  */
#define TEST_BINARY_OP1(INSN, Q, T1, T2, W, N)				\
  VECT_VAR(vector_res, T1, W, N) =					\
    INSN##Q##_##T2##W(VECT_VAR(vector, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

#define TEST_BINARY_OP(INSN, Q, T1, T2, W, N)				\
  TEST_BINARY_OP1(INSN, Q, T1, T2, W, N)				\

  /* With ARM RVCT, we need to declare variables before any executable
     statement  */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Fill input vector2 with arbitrary values */
  VDUP(vector2, , int, s, 8, 8, 2);
  VDUP(vector2, , int, s, 16, 4, -4);
  VDUP(vector2, , int, s, 32, 2, 3);
  VDUP(vector2, , int, s, 64, 1, 100);
  VDUP(vector2, , uint, u, 8, 8, 20);
  VDUP(vector2, , uint, u, 16, 4, 30);
  VDUP(vector2, , uint, u, 32, 2, 40);
  VDUP(vector2, , uint, u, 64, 1, 2);
  VDUP(vector2, q, int, s, 8, 16, -10);
  VDUP(vector2, q, int, s, 16, 8, -20);
  VDUP(vector2, q, int, s, 32, 4, -30);
  VDUP(vector2, q, int, s, 64, 2, 24);
  VDUP(vector2, q, uint, u, 8, 16, 12);
  VDUP(vector2, q, uint, u, 16, 8, 3);
  VDUP(vector2, q, uint, u, 32, 4, 55);
  VDUP(vector2, q, uint, u, 64, 2, 3);

  /* Apply a binary operator named INSN_NAME  */
  TEST_MACRO_ALL_VARIANTS_1_5(TEST_BINARY_OP, INSN_NAME);

  dump_results_hex (TEST_MSG);

#ifdef EXTRA_TESTS
  EXTRA_TESTS();
#endif
}
