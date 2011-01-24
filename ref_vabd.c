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

#define TEST_MSG "VABD/VABDQ"
void exec_vabd (void)
{
  /* Basic test: v4=vabd(v1,v2), then store the result.  */
#define TEST_VABD(Q, T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vabd##Q##_##T2##W(VECT_VAR(vector1, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector_res, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
#define DECL_VABD_VAR(VAR)			\
  DECL_VARIABLE(VAR, int, 8, 8);		\
  DECL_VARIABLE(VAR, int, 16, 4);		\
  DECL_VARIABLE(VAR, int, 32, 2);		\
  DECL_VARIABLE(VAR, uint, 8, 8);		\
  DECL_VARIABLE(VAR, uint, 16, 4);		\
  DECL_VARIABLE(VAR, uint, 32, 2);		\
  DECL_VARIABLE(VAR, float, 32, 2);		\
  DECL_VARIABLE(VAR, int, 8, 16);		\
  DECL_VARIABLE(VAR, int, 16, 8);		\
  DECL_VARIABLE(VAR, int, 32, 4);		\
  DECL_VARIABLE(VAR, uint, 8, 16);		\
  DECL_VARIABLE(VAR, uint, 16, 8);		\
  DECL_VARIABLE(VAR, uint, 32, 4);		\
  DECL_VARIABLE(VAR, float, 32, 4)

  DECL_VABD_VAR(vector1);
  DECL_VABD_VAR(vector2);
  DECL_VABD_VAR(vector_res);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  TEST_VLOAD(vector1, buffer, , int, s, 8, 8);
  TEST_VLOAD(vector1, buffer, , int, s, 16, 4);
  TEST_VLOAD(vector1, buffer, , int, s, 32, 2);
  TEST_VLOAD(vector1, buffer, , uint, u, 8, 8);
  TEST_VLOAD(vector1, buffer, , uint, u, 16, 4);
  TEST_VLOAD(vector1, buffer, , uint, u, 32, 2);
  TEST_VLOAD(vector1, buffer, , float, f, 32, 2);
  TEST_VLOAD(vector1, buffer, q, int, s, 8, 16);
  TEST_VLOAD(vector1, buffer, q, int, s, 16, 8);
  TEST_VLOAD(vector1, buffer, q, int, s, 32, 4);
  TEST_VLOAD(vector1, buffer, q, uint, u, 8, 16);
  TEST_VLOAD(vector1, buffer, q, uint, u, 16, 8);
  TEST_VLOAD(vector1, buffer, q, uint, u, 32, 4);
  TEST_VLOAD(vector1, buffer, q, float, f, 32, 4);

  /* Choose init value arbitrarily */
  TEST_VDUP(vector2, , int, s, 8, 8, 1);
  TEST_VDUP(vector2, , int, s, 16, 4, -13);
  TEST_VDUP(vector2, , int, s, 32, 2, 8);
  TEST_VDUP(vector2, , uint, u, 8, 8, 1);
  TEST_VDUP(vector2, , uint, u, 16, 4, 13);
  TEST_VDUP(vector2, , uint, u, 32, 2, 8);
  TEST_VDUP(vector2, , float, f, 32, 2, 8.3);
  TEST_VDUP(vector2, q, int, s, 8, 16, 10);
  TEST_VDUP(vector2, q, int, s, 16, 8, -12);
  TEST_VDUP(vector2, q, int, s, 32, 4, 32);
  TEST_VDUP(vector2, q, uint, u, 8, 16, 10);
  TEST_VDUP(vector2, q, uint, u, 16, 8, 12);
  TEST_VDUP(vector2, q, uint, u, 32, 4, 32);
  TEST_VDUP(vector2, q, float, f, 32, 4, 32.12);

  TEST_VABD(, int, s, 8, 8);
  TEST_VABD(, int, s, 16, 4);
  TEST_VABD(, int, s, 32, 2);
  TEST_VABD(, uint, u, 8, 8);
  TEST_VABD(, uint, u, 16, 4);
  TEST_VABD(, uint, u, 32, 2);
  TEST_VABD(, float, f, 32, 2);
  TEST_VABD(q, int, s, 8, 16);
  TEST_VABD(q, int, s, 16, 8);
  TEST_VABD(q, int, s, 32, 4);
  TEST_VABD(q, uint, u, 8, 16);
  TEST_VABD(q, uint, u, 16, 8);
  TEST_VABD(q, uint, u, 32, 4);
  TEST_VABD(q, float, f, 32, 4);

  dump_results_hex (TEST_MSG);
}
