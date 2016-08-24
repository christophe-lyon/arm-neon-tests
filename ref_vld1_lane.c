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

#define TEST_MSG "VLD1_LANE/VLD1_LANEQ"
void exec_vld1_lane (void)
{
  /* Fill vector_src with 0xAA, then load 1 lane */
#define TEST_VLD1_LANE(Q, T1, T2, W, N, L)				\
  memset (VECT_VAR(buffer_src, T1, W, N), 0xAA, W/8*N);			\
  VECT_VAR(vector_src, T1, W, N) =					\
    vld1##Q##_##T2##W(VECT_VAR(buffer_src, T1, W, N));			\
  VECT_VAR(vector, T1, W, N) =						\
    vld1##Q##_lane_##T2##W(VECT_VAR(buffer, T1, W, N),			\
			   VECT_VAR(vector_src, T1, W, N), L);		\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_VARIABLE(vector, float, 16, 4);
  DECL_VARIABLE(vector, float, 16, 8);
#endif
  DECL_VARIABLE_ALL_VARIANTS(vector_src);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_VARIABLE(vector_src, float, 16, 4);
  DECL_VARIABLE(vector_src, float, 16, 8);
#endif

  ARRAY(buffer_src, int, 8, 8);
  ARRAY(buffer_src, int, 16, 4);
  ARRAY(buffer_src, int, 32, 2);
  ARRAY(buffer_src, int, 64, 1);
  ARRAY(buffer_src, uint, 8, 8);
  ARRAY(buffer_src, uint, 16, 4);
  ARRAY(buffer_src, uint, 32, 2);
  ARRAY(buffer_src, uint, 64, 1);
  ARRAY(buffer_src, poly, 8, 8);
  ARRAY(buffer_src, poly, 16, 4);
  ARRAY(buffer_src, float, 32, 2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  ARRAY(buffer_src, float, 16, 4);
#endif

  ARRAY(buffer_src, int, 8, 16);
  ARRAY(buffer_src, int, 16, 8);
  ARRAY(buffer_src, int, 32, 4);
  ARRAY(buffer_src, int, 64, 2);
  ARRAY(buffer_src, uint, 8, 16);
  ARRAY(buffer_src, uint, 16, 8);
  ARRAY(buffer_src, uint, 32, 4);
  ARRAY(buffer_src, uint, 64, 2);
  ARRAY(buffer_src, poly, 8, 16);
  ARRAY(buffer_src, poly, 16, 8);
  ARRAY(buffer_src, float, 32, 4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  ARRAY(buffer_src, float, 16, 8);
#endif

  clean_results ();

  /* Choose lane arbitrarily  */
  TEST_VLD1_LANE(, int, s, 8, 8, 6);
  TEST_VLD1_LANE(, int, s, 16, 4, 3);
  TEST_VLD1_LANE(, int, s, 32, 2, 1);
  TEST_VLD1_LANE(, int, s, 64, 1, 0);
  TEST_VLD1_LANE(, uint, u, 8, 8, 7);
  TEST_VLD1_LANE(, uint, u, 16, 4, 3);
  TEST_VLD1_LANE(, uint, u, 32, 2, 1);
  TEST_VLD1_LANE(, uint, u, 64, 1, 0);
  TEST_VLD1_LANE(, poly, p, 8, 8, 7);
  TEST_VLD1_LANE(, poly, p, 16, 4, 3);
  TEST_VLD1_LANE(, float, f, 32, 2, 1);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_VLD1_LANE(, float, f, 16, 4, 2);
#endif

  TEST_VLD1_LANE(q, int, s, 8, 16, 15);
  TEST_VLD1_LANE(q, int, s, 16, 8, 5);
  TEST_VLD1_LANE(q, int, s, 32, 4, 2);
  TEST_VLD1_LANE(q, int, s, 64, 2, 1);
  TEST_VLD1_LANE(q, uint, u, 8, 16, 12);
  TEST_VLD1_LANE(q, uint, u, 16, 8, 6);
  TEST_VLD1_LANE(q, uint, u, 32, 4, 2);
  TEST_VLD1_LANE(q, uint, u, 64, 2, 0);
  TEST_VLD1_LANE(q, poly, p, 8, 16, 12);
  TEST_VLD1_LANE(q, poly, p, 16, 8, 6);
  TEST_VLD1_LANE(q, float, f, 32, 4, 2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_VLD1_LANE(q, float, f, 16, 8, 5);
#endif

#ifndef __CC_ARM
  /* Check runtime assertions. With RVCT, the check is performed at
     compile-time */
  //  TEST_VLD1_LANE(, int, s, 64, 1, 1);
#endif

  dump_results_hex (TEST_MSG);
}
