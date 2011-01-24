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

#ifdef __arm__
#include <arm_neon.h>
#else
#error Target not supported
#endif

#include "stm-arm-neon-ref.h"

#define TEST_MSG "VABA/VABAQ"
void exec_vaba (void)
{
  /* Basic test: v4=vaba(v1,v2,v3), then store the result.  */
#define TEST_VABA(Q, T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vaba##Q##_##T2##W(VECT_VAR(vector1, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N),			\
		      VECT_VAR(vector3, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
#define DECL_VABA_VAR(VAR)			\
  DECL_VARIABLE(VAR, int, 8, 8);		\
  DECL_VARIABLE(VAR, int, 16, 4);		\
  DECL_VARIABLE(VAR, int, 32, 2);		\
  DECL_VARIABLE(VAR, uint, 8, 8);		\
  DECL_VARIABLE(VAR, uint, 16, 4);		\
  DECL_VARIABLE(VAR, uint, 32, 2);		\
  DECL_VARIABLE(VAR, int, 8, 16);		\
  DECL_VARIABLE(VAR, int, 16, 8);		\
  DECL_VARIABLE(VAR, int, 32, 4);		\
  DECL_VARIABLE(VAR, uint, 8, 16);		\
  DECL_VARIABLE(VAR, uint, 16, 8);		\
  DECL_VARIABLE(VAR, uint, 32, 4)

  DECL_VABA_VAR(vector1);
  DECL_VABA_VAR(vector2);
  DECL_VABA_VAR(vector3);
  DECL_VABA_VAR(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_VLOAD(vector1, buffer, , int, s, 8, 8);
  TEST_VLOAD(vector1, buffer, , int, s, 16, 4);
  TEST_VLOAD(vector1, buffer, , int, s, 32, 2);
  TEST_VLOAD(vector1, buffer, , uint, u, 8, 8);
  TEST_VLOAD(vector1, buffer, , uint, u, 16, 4);
  TEST_VLOAD(vector1, buffer, , uint, u, 32, 2);
  TEST_VLOAD(vector1, buffer, q, int, s, 8, 16);
  TEST_VLOAD(vector1, buffer, q, int, s, 16, 8);
  TEST_VLOAD(vector1, buffer, q, int, s, 32, 4);
  TEST_VLOAD(vector1, buffer, q, uint, u, 8, 16);
  TEST_VLOAD(vector1, buffer, q, uint, u, 16, 8);
  TEST_VLOAD(vector1, buffer, q, uint, u, 32, 4);


  /* Choose init value arbitrarily */
  TEST_VDUP(vector2, , int, s, 8, 8, 1);
  TEST_VDUP(vector2, , int, s, 16, 4, -13);
  TEST_VDUP(vector2, , int, s, 32, 2, 8);
  TEST_VDUP(vector2, , uint, u, 8, 8, 1);
  TEST_VDUP(vector2, , uint, u, 16, 4, 13);
  TEST_VDUP(vector2, , uint, u, 32, 2, 8);
  TEST_VDUP(vector2, q, int, s, 8, 16, 10);
  TEST_VDUP(vector2, q, int, s, 16, 8, -12);
  TEST_VDUP(vector2, q, int, s, 32, 4, 32);
  TEST_VDUP(vector2, q, uint, u, 8, 16, 10);
  TEST_VDUP(vector2, q, uint, u, 16, 8, 12);
  TEST_VDUP(vector2, q, uint, u, 32, 4, 32);

  /* Choose init value arbitrarily */
  TEST_VDUP(vector3, , int, s, 8, 8, -5);
  TEST_VDUP(vector3, , int, s, 16, 4, 25);
  TEST_VDUP(vector3, , int, s, 32, 2, -40);
  TEST_VDUP(vector3, , uint, u, 8, 8, 100);
  TEST_VDUP(vector3, , uint, u, 16, 4, 2340);
  TEST_VDUP(vector3, , uint, u, 32, 2, 0xffffffff);
  TEST_VDUP(vector3, q, int, s, 8, 16, -100);
  TEST_VDUP(vector3, q, int, s, 16, 8, -3000);
  TEST_VDUP(vector3, q, int, s, 32, 4, 10000);
  TEST_VDUP(vector3, q, uint, u, 8, 16, 2);
  TEST_VDUP(vector3, q, uint, u, 16, 8, 3);
  TEST_VDUP(vector3, q, uint, u, 32, 4, 4);

  TEST_VABA(, int, s, 8, 8);
  TEST_VABA(, int, s, 16, 4);
  TEST_VABA(, int, s, 32, 2);
  TEST_VABA(, uint, u, 8, 8);
  TEST_VABA(, uint, u, 16, 4);
  TEST_VABA(, uint, u, 32, 2);
  TEST_VABA(q, int, s, 8, 16);
  TEST_VABA(q, int, s, 16, 8);
  TEST_VABA(q, int, s, 32, 4);
  TEST_VABA(q, uint, u, 8, 16);
  TEST_VABA(q, uint, u, 16, 8);
  TEST_VABA(q, uint, u, 32, 4);

  dump_results_hex (TEST_MSG);
}
