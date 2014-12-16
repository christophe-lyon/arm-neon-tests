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

#define TEST_MSG "VEXT/VEXTQ"
void exec_vext (void)
{
  /* vector_res = vext(vector1,vector2,offset), then store the result.  */
#define TEST_VEXT(Q, T1, T2, W, N, V)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vext##Q##_##T2##W(VECT_VAR(vector1, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N),			\
		      V);						\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector1);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();

  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector1, buffer);
  VLOAD(vector1, buffer, , float, f, 32, 2);
  VLOAD(vector1, buffer, q, float, f, 32, 4);

  /* Choose arbitrary initialization values */
  VDUP(vector2, , int, s, 8, 8, 0x11);
  VDUP(vector2, , int, s, 16, 4, 0x22);
  VDUP(vector2, , int, s, 32, 2, 0x33);
  VDUP(vector2, , int, s, 64, 1, 0x44);
  VDUP(vector2, , uint, u, 8, 8, 0x55);
  VDUP(vector2, , uint, u, 16, 4, 0x66);
  VDUP(vector2, , uint, u, 32, 2, 0x77);
  VDUP(vector2, , uint, u, 64, 1, 0x88);
  VDUP(vector2, , poly, p, 8, 8, 0x55);
  VDUP(vector2, , poly, p, 16, 4, 0x66);
  VDUP(vector2, , float, f, 32, 2, 33.6f);

  VDUP(vector2, q, int, s, 8, 16, 0x11);
  VDUP(vector2, q, int, s, 16, 8, 0x22);
  VDUP(vector2, q, int, s, 32, 4, 0x33);
  VDUP(vector2, q, int, s, 64, 2, 0x44);
  VDUP(vector2, q, uint, u, 8, 16, 0x55);
  VDUP(vector2, q, uint, u, 16, 8, 0x66);
  VDUP(vector2, q, uint, u, 32, 4, 0x77);
  VDUP(vector2, q, uint, u, 64, 2, 0x88);
  VDUP(vector2, q, poly, p, 8, 16, 0x55);
  VDUP(vector2, q, poly, p, 16, 8, 0x66);
  VDUP(vector2, q, float, f, 32, 4, 33.2f);

  /* Choose arbitrary extract offsets */
  TEST_VEXT(, int, s, 8, 8, 7);
  TEST_VEXT(, int, s, 16, 4, 3);
  TEST_VEXT(, int, s, 32, 2, 1);
  TEST_VEXT(, int, s, 64, 1, 0);
  TEST_VEXT(, uint, u, 8, 8, 6);
  TEST_VEXT(, uint, u, 16, 4, 2);
  TEST_VEXT(, uint, u, 32, 2, 1);
  TEST_VEXT(, uint, u, 64, 1, 0);
  TEST_VEXT(, poly, p, 8, 8, 6);
  TEST_VEXT(, poly, p, 16, 4, 2);
  TEST_VEXT(, float, f, 32, 2, 1);

  TEST_VEXT(q, int, s, 8, 16, 14);
  TEST_VEXT(q, int, s, 16, 8, 7);
  TEST_VEXT(q, int, s, 32, 4, 3);
  TEST_VEXT(q, int, s, 64, 2, 1);
  TEST_VEXT(q, uint, u, 8, 16, 12);
  TEST_VEXT(q, uint, u, 16, 8, 6);
  TEST_VEXT(q, uint, u, 32, 4, 3);
  TEST_VEXT(q, uint, u, 64, 2, 1);
  TEST_VEXT(q, poly, p, 8, 16, 12);
  TEST_VEXT(q, poly, p, 16, 8, 6);
  TEST_VEXT(q, float, f, 32, 4, 3);

  dump_results_hex (TEST_MSG);
}
