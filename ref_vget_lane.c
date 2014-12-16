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

void exec_vget_lane (void)
{
  /* vec=vget_lane(vec, lane), then store the result.  */
#define TEST_VGET_LANE(Q, T1, T2, W, N, L)				\
  VAR(var, T1, W) = vget##Q##_lane_##T2##W(VECT_VAR(vector, T1, W, N), L); \
  fprintf(ref_file, "%s: %" PRIx##W "\n", "vget"STR(Q)"_lane_"STR(T2##W), VAR(var, T1, W))

  /* Special variant for poly* types, to clear sign bits in output.  */
#define TEST_VGET_LANE_POLY(Q, T1, T2, W, N, L)				\
  VAR(var, T1, W) = vget##Q##_lane_##T2##W(VECT_VAR(vector, T1, W, N), L); \
    fprintf(ref_file, "%s: %" PRIx##W "\n", "vget"STR(Q)"_lane_"STR(T2##W), \
	    (uint##W##_t)VAR(var, T1, W))

  /* Special variant for floating-point */
  union {
    uint32_t var_int32;
    float var_float32;
  } var_int32_float32;

#define TEST_VGET_LANE_F(Q, T1, T2, W, N, L)				\
  VAR(var, T1, W) = vget##Q##_lane_##T2##W(VECT_VAR(vector, T1, W, N), L); \
  var_int##W##_float##W.var_float##W = VAR(var, T1, W);		\
  fprintf(ref_file, "%s: %" PRIx##W "\n", "vget"STR(Q)"_lane_"STR(T2##W), var_int##W##_float##W.var_int##W)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);

  /* Scalar variables */
  VAR_DECL(var, int, 8);
  VAR_DECL(var, int, 16);
  VAR_DECL(var, int, 32);
  VAR_DECL(var, int, 64);
  VAR_DECL(var, uint, 8);
  VAR_DECL(var, uint, 16);
  VAR_DECL(var, uint, 32);
  VAR_DECL(var, uint, 64);
  VAR_DECL(var, poly, 8);
  VAR_DECL(var, poly, 16);
  VAR_DECL(var, float, 32);

  clean_results ();

  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);
  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);

  fprintf(ref_file, "\n%s output:\n", "VGET_LANE/VGETQ_LANE");

  /* Choose lane arbitrarily  */
  TEST_VGET_LANE(, int, s, 8, 8, 7);
  TEST_VGET_LANE(, int, s, 16, 4, 3);
  TEST_VGET_LANE(, int, s, 32, 2, 1);
  TEST_VGET_LANE(, int, s, 64, 1, 0);
  TEST_VGET_LANE(, uint, u, 8, 8, 6);
  TEST_VGET_LANE(, uint, u, 16, 4, 2);
  TEST_VGET_LANE(, uint, u, 32, 2, 1);
  TEST_VGET_LANE(, uint, u, 64, 1, 0);
  TEST_VGET_LANE_POLY(, poly, p, 8, 8, 6);
  TEST_VGET_LANE_POLY(, poly, p, 16, 4, 2);
  TEST_VGET_LANE_F(, float, f, 32, 2, 1);

  TEST_VGET_LANE(q, int, s, 8, 16, 15);
  TEST_VGET_LANE(q, int, s, 16, 8, 5);
  TEST_VGET_LANE(q, int, s, 32, 4, 3);
  TEST_VGET_LANE(q, int, s, 64, 2, 1);
  TEST_VGET_LANE(q, uint, u, 8, 16, 14);
  TEST_VGET_LANE(q, uint, u, 16, 8, 6);
  TEST_VGET_LANE(q, uint, u, 32, 4, 2);
  TEST_VGET_LANE(q, uint, u, 64, 2, 1);
  TEST_VGET_LANE_POLY(q, poly, p, 8, 16, 14);
  TEST_VGET_LANE_POLY(q, poly, p, 16, 8, 6);
  TEST_VGET_LANE_F(q, float, f, 32, 4, 3);

  fprintf(ref_file, "\n");
}
