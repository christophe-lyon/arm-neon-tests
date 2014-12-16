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

#define TEST_MSG "VBSL/VBSLQ"
void exec_vbsl (void)
{
  /* Basic test: y=vbsl(unsigned_vec,x,x), then store the result.  */
#define TEST_VBSL(T3, Q, T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vbsl##Q##_##T2##W(VECT_VAR(vector_first, T3, W, N),			\
		      VECT_VAR(vector, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  DECL_VARIABLE_UNSIGNED_VARIANTS(vector_first);

  clean_results ();

  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);
  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);

  /* Choose init value arbitrarily, will be used for vector
     comparison. As we want different values for each type variant, we
     can't use generic initialization macros.  */
  VDUP(vector2, , int, s, 8, 8, -10);
  VDUP(vector2, , int, s, 16, 4, -14);
  VDUP(vector2, , int, s, 32, 2, -30);
  VDUP(vector2, , int, s, 64, 1, -33);
  VDUP(vector2, , uint, u, 8, 8, 0xF3);
  VDUP(vector2, , uint, u, 16, 4, 0xFFF2);
  VDUP(vector2, , uint, u, 32, 2, 0xFFFFFFF0);
  VDUP(vector2, , uint, u, 64, 1, 0xFFFFFFF3);
  VDUP(vector2, , float, f, 32, 2, -30.3f);
  VDUP(vector2, , poly, p, 8, 8, 0xF3);
  VDUP(vector2, , poly, p, 16, 4, 0xFFF2);

  VDUP(vector2, q, int, s, 8, 16, -10);
  VDUP(vector2, q, int, s, 16, 8, -14);
  VDUP(vector2, q, int, s, 32, 4, -30);
  VDUP(vector2, q, int, s, 64, 2, -33);
  VDUP(vector2, q, uint, u, 8, 16, 0xF3);
  VDUP(vector2, q, uint, u, 16, 8, 0xFFF2);
  VDUP(vector2, q, uint, u, 32, 4, 0xFFFFFFF0);
  VDUP(vector2, q, uint, u, 64, 2, 0xFFFFFFF3);
  VDUP(vector2, q, poly, p, 8, 16, 0xF3);
  VDUP(vector2, q, poly, p, 16, 8, 0xFFF2);
  VDUP(vector2, q, float, f, 32, 4, -30.4f);

  VDUP(vector_first, , uint, u, 8, 8, 0xF4);
  VDUP(vector_first, , uint, u, 16, 4, 0xFFF6);
  VDUP(vector_first, , uint, u, 32, 2, 0xFFFFFFF2);
  VDUP(vector_first, , uint, u, 64, 1, 0xFFFFFFF2);
  VDUP(vector_first, q, uint, u, 8, 16, 0xF4);
  VDUP(vector_first, q, uint, u, 16, 8, 0xFFF6);
  VDUP(vector_first, q, uint, u, 32, 4, 0xFFFFFFF2);
  VDUP(vector_first, q, uint, u, 64, 2, 0xFFFFFFF2);

  TEST_MACRO_ALL_VARIANTS_1_5(TEST_VBSL, uint);
  TEST_VBSL(uint, , poly, p, 8, 8);
  TEST_VBSL(uint, , poly, p, 16, 4);
  TEST_VBSL(uint, q, poly, p, 8, 16);
  TEST_VBSL(uint, q, poly, p, 16, 8);
  TEST_VBSL(uint, , float, f, 32, 2);
  TEST_VBSL(uint, q, float, f, 32, 4);

  dump_results_hex (TEST_MSG);
}
