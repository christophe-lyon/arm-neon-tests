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

#define TEST_MSG "VDUP_LANE/VDUP_LANEQ"
void exec_vdup_lane (void)
{
  /* Basic test: vec1=vdup_lane(vec2, lane), then store the result.  */
#define TEST_VDUP_LANE(Q, T1, T2, W, N, N2, L)				\
  VECT_VAR(vector_res, T1, W, N) =					\
    vdup##Q##_lane_##T2##W(VECT_VAR(vector, T1, W, N2), L);		\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  /* Input vector can only have 64 bits */
  DECL_VARIABLE_64BITS_VARIANTS(vector);

  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  TEST_MACRO_64BITS_VARIANTS_2_5(VLOAD, vector, buffer);
  VLOAD(vector, buffer, , float, f, 32, 2);

  /* Choose lane arbitrarily  */
  TEST_VDUP_LANE(, int, s, 8, 8, 8, 1);
  TEST_VDUP_LANE(, int, s, 16, 4, 4, 2);
  TEST_VDUP_LANE(, int, s, 32, 2, 2, 1);
  TEST_VDUP_LANE(, int, s, 64, 1, 1, 0);
  TEST_VDUP_LANE(, uint, u, 8, 8, 8, 7);
  TEST_VDUP_LANE(, uint, u, 16, 4, 4, 3);
  TEST_VDUP_LANE(, uint, u, 32, 2, 2, 1);
  TEST_VDUP_LANE(, uint, u, 64, 1, 1, 0);
  TEST_VDUP_LANE(, poly, p, 8, 8, 8, 7);
  TEST_VDUP_LANE(, poly, p, 16, 4, 4, 3);
  TEST_VDUP_LANE(, float, f, 32, 2, 2, 1);

  TEST_VDUP_LANE(q, int, s, 8, 16, 8, 2);
  TEST_VDUP_LANE(q, int, s, 16, 8, 4, 3);
  TEST_VDUP_LANE(q, int, s, 32, 4, 2, 1);
  TEST_VDUP_LANE(q, int, s, 64, 2, 1, 0);
  TEST_VDUP_LANE(q, uint, u, 8, 16, 8, 5);
  TEST_VDUP_LANE(q, uint, u, 16, 8, 4, 1);
  TEST_VDUP_LANE(q, uint, u, 32, 4, 2, 0);
  TEST_VDUP_LANE(q, uint, u, 64, 2, 1, 0);
  TEST_VDUP_LANE(q, poly, p, 8, 16, 8, 5);
  TEST_VDUP_LANE(q, poly, p, 16, 8, 4, 1);
  TEST_VDUP_LANE(q, float, f, 32, 4, 2, 1);

  dump_results_hex (TEST_MSG);
}
