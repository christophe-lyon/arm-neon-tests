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

#define TEST_MSG "VMULL_LANE"
void exec_vmull_lane (void)
{
  /* vector_res = vmull_lane(vector,vector2,lane), then store the result.  */
#define TEST_VMULL_LANE(T1, T2, W, W2, N, L)				\
  VECT_VAR(vector_res, T1, W2, N) =					\
    vmull##_lane_##T2##W(VECT_VAR(vector, T1, W, N),			\
			 VECT_VAR(vector2, T1, W, N),			\
			 L);						\
  vst1q_##T2##W2(VECT_VAR(result, T1, W2, N), VECT_VAR(vector_res, T1, W2, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, uint, 16, 4);
  DECL_VARIABLE(vector, uint, 32, 2);
  DECL_VARIABLE(vector2, int, 16, 4);
  DECL_VARIABLE(vector2, int, 32, 2);
  DECL_VARIABLE(vector2, uint, 16, 4);
  DECL_VARIABLE(vector2, uint, 32, 2);

  DECL_VARIABLE(vector_res, int, 32, 4);
  DECL_VARIABLE(vector_res, int, 64, 2);
  DECL_VARIABLE(vector_res, uint, 32, 4);
  DECL_VARIABLE(vector_res, uint, 64, 2);

  clean_results ();

  /* Initialize vector */
  VDUP(vector, , int, s, 16, 4, 0x1000);
  VDUP(vector, , int, s, 32, 2, 0x1000);
  VDUP(vector, , uint, u, 16, 4, 0x1000);
  VDUP(vector, , uint, u, 32, 2, 0x1000);

  /* Initialize vector2 */
  VDUP(vector2, , int, s, 16, 4, 0x4);
  VDUP(vector2, , int, s, 32, 2, 0x2);
  VDUP(vector2, , uint, u, 16, 4, 0x4);
  VDUP(vector2, , uint, u, 32, 2, 0x2);

  /* Choose lane arbitrarily */
  TEST_VMULL_LANE(int, s, 16, 32, 4, 2);
  TEST_VMULL_LANE(int, s, 32, 64, 2, 1);
  TEST_VMULL_LANE(uint, u, 16, 32, 4, 2);
  TEST_VMULL_LANE(uint, u, 32, 64, 2, 1);

  /* FIXME: only a subset of the result buffers are used, but we
     output all of them */
  dump_results_hex (TEST_MSG);
}
