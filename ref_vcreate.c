/*

Copyright (c) 2009, 2010, 2011, 2013 STMicroelectronics
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

/* Template file for unary operator validation */

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#define INSN_NAME vcreate
#define TEST_MSG "VCREATE"

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* Basic test: y=vcreate(x), then store the result.  */
#define TEST_VCREATE(T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) = vcreate_##T2##W(VECT_VAR(val, T1, W, N)); \
  vst1_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

    /* With ARM RVCT, we need to declare variables before any executable
       statement  */

#define DECL_VAL(VAR, T1, W, N)			\
  uint64_t VECT_VAR(VAR, T1, W, N)

  DECL_VAL(val, int, 8, 8);
  DECL_VAL(val, int, 16, 4);
  DECL_VAL(val, int, 32, 2);
  DECL_VAL(val, int, 64, 1);
  DECL_VAL(val, float, 32, 2);
  DECL_VAL(val, uint, 8, 8);
  DECL_VAL(val, uint, 16, 4);
  DECL_VAL(val, uint, 32, 2);
  DECL_VAL(val, uint, 64, 1);
  DECL_VAL(val, poly, 8, 8);
  DECL_VAL(val, poly, 16, 4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_VAL(val, float, 16, 4);
#endif

  DECL_VARIABLE(vector_res, int, 8, 8);
  DECL_VARIABLE(vector_res, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 64, 1);
  DECL_VARIABLE(vector_res, float, 32, 2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_VARIABLE(vector_res, float, 16, 4);
#endif
  DECL_VARIABLE(vector_res, uint, 8, 8);
  DECL_VARIABLE(vector_res, uint, 16, 4);
  DECL_VARIABLE(vector_res, uint, 32, 2);
  DECL_VARIABLE(vector_res, uint, 64, 1);
  DECL_VARIABLE(vector_res, poly, 8, 8);
  DECL_VARIABLE(vector_res, poly, 16, 4);

  clean_results ();

  /* Initialize input values arbitrarily */
  VECT_VAR(val, int, 8, 8) = 0x123456789abcdef0LL;
  VECT_VAR(val, int, 16, 4) = 0x123456789abcdef0LL;
  VECT_VAR(val, int, 32, 2) = 0x123456789abcdef0LL;
  VECT_VAR(val, int, 64, 1) = 0x123456789abcdef0LL;
  VECT_VAR(val, float, 32, 2) = 0x123456789abcdef0LL;
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  VECT_VAR(val, float, 16, 4) = 0x123456789abcdef0LL;
#endif
  VECT_VAR(val, uint, 8, 8) = 0x123456789abcdef0ULL;
  VECT_VAR(val, uint, 16, 4) = 0x123456789abcdef0ULL;
  VECT_VAR(val, uint, 32, 2) = 0x123456789abcdef0ULL;
  VECT_VAR(val, uint, 64, 1) = 0x123456789abcdef0ULL;
  VECT_VAR(val, poly, 8, 8) = 0x123456789abcdef0ULL;
  VECT_VAR(val, poly, 16, 4) = 0x123456789abcdef0ULL;

  TEST_VCREATE(int, s, 8, 8);
  TEST_VCREATE(int, s, 16, 4);
  TEST_VCREATE(int, s, 32, 2);
  TEST_VCREATE(float, f, 32, 2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_VCREATE(float, f, 16, 4);
#endif
  TEST_VCREATE(int, s, 64, 1);
  TEST_VCREATE(uint, u, 8, 8);
  TEST_VCREATE(uint, u, 16, 4);
  TEST_VCREATE(uint, u, 32, 2);
  TEST_VCREATE(uint, u, 64, 1);
  TEST_VCREATE(poly, p, 8, 8);
  TEST_VCREATE(poly, p, 16, 4);

  dump_results_hex (TEST_MSG);
}
