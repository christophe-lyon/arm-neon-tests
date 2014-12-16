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

#ifndef INSN_NAME
#define INSN_NAME vmax
#define TEST_MSG "VMAX/VMAXQ"
#endif

/* Can't use the standard ref_v_binary_op.c template because vmax has
   no 64 bits variant */
#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  int i;

  /* Basic test: y=OP(x,x), then store the result.  */
#define TEST_BINARY_OP1(INSN, Q, T1, T2, W, N)				\
  VECT_VAR(vector_res, T1, W, N) =                                      \
    INSN##Q##_##T2##W(VECT_VAR(vector, T1, W, N),                       \
                      VECT_VAR(vector2, T1, W, N));                     \
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

#define TEST_BINARY_OP(INSN, Q, T1, T2, W, N)   \
  TEST_BINARY_OP1(INSN, Q, T1, T2, W, N)        \

  /* With ARM RVCT, we need to declare variables before any executable
     statement  */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);
#ifndef NO_FLOAT_VARIANT
  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);
#endif

  /* Choose init value arbitrarily, will be used as comparison value */
  VDUP(vector2, , int, s, 8, 8, -13);
  VDUP(vector2, , int, s, 16, 4, -14);
  VDUP(vector2, , int, s, 32, 2, -16);
  VDUP(vector2, , uint, u, 8, 8, 0xf3);
  VDUP(vector2, , uint, u, 16, 4, 0xfff1);
  VDUP(vector2, , uint, u, 32, 2, 0xfffffff0);
  VDUP(vector2, q, int, s, 8, 16, -12);
  VDUP(vector2, q, int, s, 16, 8, -13);
  VDUP(vector2, q, int, s, 32, 4, -15);
  VDUP(vector2, q, uint, u, 8, 16, 0xf9);
  VDUP(vector2, q, uint, u, 16, 8, 0xfff2);
  VDUP(vector2, q, uint, u, 32, 4, 0xfffffff1);
#ifndef NO_FLOAT_VARIANT
  VDUP(vector2, , float, f, 32, 2, -15.5f);
  VDUP(vector2, q, float, f, 32, 4, -14.5f);
#endif

#ifndef NO_FLOAT_VARIANT
#define FLOAT_VARIANT(MACRO, VAR)			\
  MACRO(VAR, , float, f, 32, 2);			\
  MACRO(VAR, q, float, f, 32, 4)
#else
#define FLOAT_VARIANT(MACRO, VAR)
#endif

#define TEST_MACRO_NO64BIT_VARIANT_1_5(MACRO, VAR)	\
  MACRO(VAR, , int, s, 8, 8);				\
  MACRO(VAR, , int, s, 16, 4);				\
  MACRO(VAR, , int, s, 32, 2);				\
  MACRO(VAR, , uint, u, 8, 8);				\
  MACRO(VAR, , uint, u, 16, 4);				\
  MACRO(VAR, , uint, u, 32, 2);				\
  MACRO(VAR, q, int, s, 8, 16);				\
  MACRO(VAR, q, int, s, 16, 8);				\
  MACRO(VAR, q, int, s, 32, 4);				\
  MACRO(VAR, q, uint, u, 8, 16);			\
  MACRO(VAR, q, uint, u, 16, 8);			\
  MACRO(VAR, q, uint, u, 32, 4);			\
  FLOAT_VARIANT(MACRO, VAR)

  /* Apply a binary operator named INSN_NAME  */
  TEST_MACRO_NO64BIT_VARIANT_1_5(TEST_BINARY_OP, INSN_NAME);

  dump_results_hex (TEST_MSG);


#ifndef NO_FLOAT_VARIANT
  /* Extra FP tests with special values (NaN, ....) */
  VDUP(vector, q, float, f, 32, 4, 1.0f);
  VDUP(vector2, q, float, f, 32, 4, NAN);
  TEST_BINARY_OP(INSN_NAME, q, float, f, 32, 4);
  DUMP_FP(TEST_MSG " FP special (NaN)", float, 32, 4, PRIx32);

  VDUP(vector, q, float, f, 32, 4, -NAN);
  VDUP(vector2, q, float, f, 32, 4, 1.0f);
  TEST_BINARY_OP(INSN_NAME, q, float, f, 32, 4);
  DUMP_FP(TEST_MSG " FP special (-NaN)", float, 32, 4, PRIx32);

  VDUP(vector, q, float, f, 32, 4, 1.0f);
  VDUP(vector2, q, float, f, 32, 4, HUGE_VALF);
  TEST_BINARY_OP(INSN_NAME, q, float, f, 32, 4);
  DUMP_FP(TEST_MSG " FP special (inf)", float, 32, 4, PRIx32);

  VDUP(vector, q, float, f, 32, 4, -HUGE_VALF);
  VDUP(vector2, q, float, f, 32, 4, 1.0f);
  TEST_BINARY_OP(INSN_NAME, q, float, f, 32, 4);
  DUMP_FP(TEST_MSG " FP special (-inf)", float, 32, 4, PRIx32);

  VDUP(vector, q, float, f, 32, 4, 0.0f);
  VDUP(vector2, q, float, f, 32, 4, -0.0f);
  TEST_BINARY_OP(INSN_NAME, q, float, f, 32, 4);
  DUMP_FP(TEST_MSG " FP special (-0.0)", float, 32, 4, PRIx32);

  VDUP(vector, q, float, f, 32, 4, -0.0f);
  VDUP(vector2, q, float, f, 32, 4, 0.0f);
  TEST_BINARY_OP(INSN_NAME, q, float, f, 32, 4);
  DUMP_FP(TEST_MSG " FP special (-0.0)", float, 32, 4, PRIx32);
#endif
}
