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

#ifndef INSN_NAME
#define INSN_NAME vtst
#define TEST_MSG "VTST/VTSTQ"
#endif

/* Can't use the standard ref_v_binary_op.c template because vtst has
   no 64 bits variant, and outputs are always of uint type */
#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* Basic test: y=OP(x,x), then store the result.  */
#define TEST_BINARY_OP1(INSN, Q, T1, T2, W, N)				\
  VECT_VAR(vector_res, uint, W, N) =					\
    INSN##Q##_##T2##W(VECT_VAR(vector, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N));			\
  vst1##Q##_u##W(VECT_VAR(result, uint, W, N),				\
		 VECT_VAR(vector_res, uint, W, N))

#define TEST_BINARY_OP(INSN, Q, T1, T2, W, N)				\
  TEST_BINARY_OP1(INSN, Q, T1, T2, W, N)				\

  /* With ARM RVCT, we need to declare variables before any executable
     statement  */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_UNSIGNED_VARIANTS(vector_res);


  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);

  /* Choose init value arbitrarily, will be used as comparison value */
  VDUP(vector2, , int, s, 8, 8, 15);
  VDUP(vector2, , int, s, 16, 4, 5);
  VDUP(vector2, , int, s, 32, 2, 1);
  VDUP(vector2, , uint, u, 8, 8, 15);
  VDUP(vector2, , uint, u, 16, 4, 5);
  VDUP(vector2, , uint, u, 32, 2, 1);
  VDUP(vector2, q, int, s, 8, 16, 15);
  VDUP(vector2, q, int, s, 16, 8, 5);
  VDUP(vector2, q, int, s, 32, 4, 1);
  VDUP(vector2, q, uint, u, 8, 16, 15);
  VDUP(vector2, q, uint, u, 16, 8, 5);
  VDUP(vector2, q, uint, u, 32, 4, 1);

#define TEST_MACRO_NO64BIT_VARIANT_1_5(MACRO, VAR, T1, T2)	\
  MACRO(VAR, , T1, T2, 8, 8);				\
  MACRO(VAR, , T1, T2, 16, 4);				\
  MACRO(VAR, , T1, T2, 32, 2);				\
  MACRO(VAR, q, T1, T2, 8, 16);				\
  MACRO(VAR, q, T1, T2, 16, 8);				\
  MACRO(VAR, q, T1, T2, 32, 4)

  /* Split the test, as both signed and unsigned variants output their
     result in an unsigned form (thus the same output variable is used
     in these tests) */
  TEST_MACRO_NO64BIT_VARIANT_1_5(TEST_BINARY_OP, INSN_NAME, int, s);
  dump_results_hex2 (TEST_MSG, " (signed input)");

  TEST_MACRO_NO64BIT_VARIANT_1_5(TEST_BINARY_OP, INSN_NAME, uint, u);
  dump_results_hex2 (TEST_MSG, " (unsigned input)");
}
