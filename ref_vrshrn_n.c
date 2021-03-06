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

#define TEST_MSG "VRSHRN_N"
void exec_vrshrn_n (void)
{
  /* Basic test: v2=vrshrn_n(v1,v), then store the result.  */
#define TEST_VRSHRN_N(T1, T2, W, N, W2, V)				\
  VECT_VAR(vector_res, T1, W2, N) =					\
    vrshrn_n_##T2##W(VECT_VAR(vector, T1, W, N),			\
		     V);						\
  vst1_##T2##W2(VECT_VAR(result, T1, W2, N), VECT_VAR(vector_res, T1, W2, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */

  /* vector is twice as large as vector_res */
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);
  DECL_VARIABLE(vector, int, 64, 2);
  DECL_VARIABLE(vector, uint, 16, 8);
  DECL_VARIABLE(vector, uint, 32, 4);
  DECL_VARIABLE(vector, uint, 64, 2);

  DECL_VARIABLE(vector_res, int, 8, 8);
  DECL_VARIABLE(vector_res, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 2);
  DECL_VARIABLE(vector_res, uint, 8, 8);
  DECL_VARIABLE(vector_res, uint, 16, 4);
  DECL_VARIABLE(vector_res, uint, 32, 2);

  clean_results ();

  /* Fill input vector with 0, to check behavior on limits */
  VDUP(vector, q, int, s, 16, 8, 0);
  VDUP(vector, q, int, s, 32, 4, 0);
  VDUP(vector, q, int, s, 64, 2, 0);
  VDUP(vector, q, uint, u, 16, 8, 0);
  VDUP(vector, q, uint, u, 32, 4, 0);
  VDUP(vector, q, uint, u, 64, 2, 0);

  /* Choose shift amount arbitrarily  */
  TEST_VRSHRN_N(int, s, 16, 8, 8, 1);
  TEST_VRSHRN_N(int, s, 32, 4, 16, 1);
  TEST_VRSHRN_N(int, s, 64, 2, 32, 2);
  TEST_VRSHRN_N(uint, u, 16, 8, 8, 2);
  TEST_VRSHRN_N(uint, u, 32, 4, 16, 3);
  TEST_VRSHRN_N(uint, u, 64, 2, 32, 3);

  dump_results_hex2 (TEST_MSG, " (with input = 0)");

  /* Test again, with predefined input values */
  VLOAD(vector, buffer, q, int, s, 16, 8);
  VLOAD(vector, buffer, q, int, s, 32, 4);
  VLOAD(vector, buffer, q, int, s, 64, 2);
  VLOAD(vector, buffer, q, uint, u, 16, 8);
  VLOAD(vector, buffer, q, uint, u, 32, 4);
  VLOAD(vector, buffer, q, uint, u, 64, 2);

  /* Choose shift amount arbitrarily  */
  TEST_VRSHRN_N(int, s, 16, 8, 8, 1);
  TEST_VRSHRN_N(int, s, 32, 4, 16, 1);
  TEST_VRSHRN_N(int, s, 64, 2, 32, 2);
  TEST_VRSHRN_N(uint, u, 16, 8, 8, 2);
  TEST_VRSHRN_N(uint, u, 32, 4, 16, 3);
  TEST_VRSHRN_N(uint, u, 64, 2, 32, 3);

  /* FIXME: only a few result buffers are used, but we output all of them */
  dump_results_hex (TEST_MSG);

  /* Fill input arbitrary values */
  VDUP(vector, q, int, s, 16, 8, 30);
  VDUP(vector, q, int, s, 32, 4, 0);
  VDUP(vector, q, int, s, 64, 2, 0);
  VDUP(vector, q, uint, u, 16, 8, 0xFFF0);
  VDUP(vector, q, uint, u, 32, 4, 0xFFFFFFF0);
  VDUP(vector, q, uint, u, 64, 2, 0);

  /* Choose shift amount arbitrarily  */
  TEST_VRSHRN_N(int, s, 16, 8, 8, 7);
  TEST_VRSHRN_N(int, s, 32, 4, 16, 14);
  TEST_VRSHRN_N(int, s, 64, 2, 32, 31);
  TEST_VRSHRN_N(uint, u, 16, 8, 8, 7);
  TEST_VRSHRN_N(uint, u, 32, 4, 16, 16);
  TEST_VRSHRN_N(uint, u, 64, 2, 32, 3);

  /* FIXME: only a few result buffers are used, but we output all of them */
  dump_results_hex2 (TEST_MSG, " (with large shift amount)");
}
