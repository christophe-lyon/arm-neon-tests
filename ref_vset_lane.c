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

#define TEST_MSG "VSET_LANE/VSET_LANEQ"
void exec_vset_lane (void)
{
  /* vec=vset_lane(val, vec, lane), then store the result.  */
#define TEST_VSET_LANE_HERE(Q, T1, T2, W, N, V, L)			\
  VECT_VAR(vector, T1, W, N) =						\
    vset##Q##_lane_##T2##W(V,						\
			   VECT_VAR(vector, T1, W, N),			\
			   L);						\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);
  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);

  /* Choose value and lane arbitrarily  */
  TEST_VSET_LANE_HERE(, int, s, 8, 8, 0x11, 7);
  TEST_VSET_LANE_HERE(, int, s, 16, 4, 0x22, 3);
  TEST_VSET_LANE_HERE(, int, s, 32, 2, 0x33, 1);
  TEST_VSET_LANE_HERE(, int, s, 64, 1, 0x44, 0);
  TEST_VSET_LANE_HERE(, uint, u, 8, 8, 0x55, 6);
  TEST_VSET_LANE_HERE(, uint, u, 16, 4, 0x66, 2);
  TEST_VSET_LANE_HERE(, uint, u, 32, 2, 0x77, 1);
  TEST_VSET_LANE_HERE(, uint, u, 64, 1, 0x88, 0);
  TEST_VSET_LANE_HERE(, poly, p, 8, 8, 0x55, 6);
  TEST_VSET_LANE_HERE(, poly, p, 16, 4, 0x66, 2);
  TEST_VSET_LANE_HERE(, float, f, 32, 2, 33.2f, 1);

  TEST_VSET_LANE_HERE(q, int, s, 8, 16, 0x99, 15);
  TEST_VSET_LANE_HERE(q, int, s, 16, 8, 0xAA, 5);
  TEST_VSET_LANE_HERE(q, int, s, 32, 4, 0xBB, 3);
  TEST_VSET_LANE_HERE(q, int, s, 64, 2, 0xCC, 1);
  TEST_VSET_LANE_HERE(q, uint, u, 8, 16, 0xDD, 14);
  TEST_VSET_LANE_HERE(q, uint, u, 16, 8, 0xEE, 6);
  TEST_VSET_LANE_HERE(q, uint, u, 32, 4, 0xFF, 2);
  TEST_VSET_LANE_HERE(q, uint, u, 64, 2, 0x11, 1);
  TEST_VSET_LANE_HERE(q, poly, p, 8, 16, 0xDD, 14);
  TEST_VSET_LANE_HERE(q, poly, p, 16, 8, 0xEE, 6);
  TEST_VSET_LANE_HERE(q, float, f, 32, 4, 11.2f, 3);

  dump_results_hex (TEST_MSG);
}
