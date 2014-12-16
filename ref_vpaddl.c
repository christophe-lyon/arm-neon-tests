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

#define INSN_NAME vpaddl
#define TEST_MSG "VPADDL/VPADDLQ"

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* Basic test: y=OP(x), then store the result.  */
#define TEST_VPADDL1(INSN, Q, T1, T2, W, N, W2, N2)	\
  VECT_VAR(vector_res, T1, W2, N2) =			\
    INSN##Q##_##T2##W(VECT_VAR(vector, T1, W, N));	\
  vst1##Q##_##T2##W2(VECT_VAR(result, T1, W2, N2),	\
		    VECT_VAR(vector_res, T1, W2, N2))

#define TEST_VPADDL(INSN, Q, T1, T2, W, N, W2, N2)	\
  TEST_VPADDL1(INSN, Q, T1, T2, W, N, W2, N2)

    /* With ARM RVCT, we need to declare variables before any executable
       statement  */

  /* No need for 64 bits variants */
  DECL_VARIABLE(vector, int, 8, 8);
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, uint, 8, 8);
  DECL_VARIABLE(vector, uint, 16, 4);
  DECL_VARIABLE(vector, uint, 32, 2);
  DECL_VARIABLE(vector, int, 8, 16);
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);
  DECL_VARIABLE(vector, uint, 8, 16);
  DECL_VARIABLE(vector, uint, 16, 8);
  DECL_VARIABLE(vector, uint, 32, 4);

  DECL_VARIABLE(vector_res, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 64, 1);
  DECL_VARIABLE(vector_res, uint, 16, 4);
  DECL_VARIABLE(vector_res, uint, 32, 2);
  DECL_VARIABLE(vector_res, uint, 64, 1);
  DECL_VARIABLE(vector_res, int, 16, 8);
  DECL_VARIABLE(vector_res, int, 32, 4);
  DECL_VARIABLE(vector_res, int, 64, 2);
  DECL_VARIABLE(vector_res, uint, 16, 8);
  DECL_VARIABLE(vector_res, uint, 32, 4);
  DECL_VARIABLE(vector_res, uint, 64, 2);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  VLOAD(vector, buffer, , int, s, 8, 8);
  VLOAD(vector, buffer, , int, s, 16, 4);
  VLOAD(vector, buffer, , int, s, 32, 2);
  VLOAD(vector, buffer, , uint, u, 8, 8);
  VLOAD(vector, buffer, , uint, u, 16, 4);
  VLOAD(vector, buffer, , uint, u, 32, 2);
  VLOAD(vector, buffer, q, int, s, 8, 16);
  VLOAD(vector, buffer, q, int, s, 16, 8);
  VLOAD(vector, buffer, q, int, s, 32, 4);
  VLOAD(vector, buffer, q, uint, u, 8, 16);
  VLOAD(vector, buffer, q, uint, u, 16, 8);
  VLOAD(vector, buffer, q, uint, u, 32, 4);

  /* Apply a unary operator named INSN_NAME  */
  TEST_VPADDL(INSN_NAME, , int, s, 8, 8, 16, 4);
  TEST_VPADDL(INSN_NAME, , int, s, 16, 4, 32, 2);
  TEST_VPADDL(INSN_NAME, , int, s, 32, 2, 64, 1);
  TEST_VPADDL(INSN_NAME, , uint, u, 8, 8, 16, 4);
  TEST_VPADDL(INSN_NAME, , uint, u, 16, 4, 32, 2);
  TEST_VPADDL(INSN_NAME, , uint, u, 32, 2, 64, 1);
  TEST_VPADDL(INSN_NAME, q, int, s, 8, 16, 16, 8);
  TEST_VPADDL(INSN_NAME, q, int, s, 16, 8, 32, 4);
  TEST_VPADDL(INSN_NAME, q, int, s, 32, 4, 64, 2);
  TEST_VPADDL(INSN_NAME, q, uint, u, 8, 16, 16, 8);
  TEST_VPADDL(INSN_NAME, q, uint, u, 16, 8, 32, 4);
  TEST_VPADDL(INSN_NAME, q, uint, u, 32, 4, 64, 2);

  dump_results_hex (TEST_MSG);
}
