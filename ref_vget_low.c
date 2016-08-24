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

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#define TEST_MSG "VGET_LOW"
void exec_vget_low (void)
{
  /* Basic test: vec64=vget_low(vec128), then store the result.  */
#define TEST_VGET_LOW(T1, T2, W, N, N2)					\
  VECT_VAR(vector64, T1, W, N) =					\
    vget_low_##T2##W(VECT_VAR(vector128, T1, W, N2));			\
  vst1_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector64, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_64BITS_VARIANTS(vector64);
  DECL_VARIABLE_128BITS_VARIANTS(vector128);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_VARIABLE(vector64, float, 16, 4);
  DECL_VARIABLE(vector128, float, 16, 8);
#endif

  TEST_MACRO_128BITS_VARIANTS_2_5(VLOAD, vector128, buffer);
  VLOAD(vector128, buffer, q, float, f, 32, 4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  VLOAD(vector128, buffer, q, float, f, 16, 8);
#endif

  clean_results ();

  TEST_VGET_LOW(int, s, 8, 8, 16);
  TEST_VGET_LOW(int, s, 16, 4, 8);
  TEST_VGET_LOW(int, s, 32, 2, 4);
  TEST_VGET_LOW(int, s, 64, 1, 2);
  TEST_VGET_LOW(uint, u, 8, 8, 16);
  TEST_VGET_LOW(uint, u, 16, 4, 8);
  TEST_VGET_LOW(uint, u, 32, 2, 4);
  TEST_VGET_LOW(uint, u, 64, 1, 2);
  TEST_VGET_LOW(poly, p, 8, 8, 16);
  TEST_VGET_LOW(poly, p, 16, 4, 8);
  TEST_VGET_LOW(float, f, 32, 2, 4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_VGET_LOW(float, f, 16, 4, 8);
#endif

  dump_results_hex (TEST_MSG);
}
