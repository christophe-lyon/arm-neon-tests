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

#ifndef INSN_NAME
#define INSN_NAME vmla
#define TEST_MSG "VMLA"
#endif


#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
#define DECL_VMLX(T, W, N)			\
  DECL_VARIABLE(vector1, T, W, N);		\
  DECL_VARIABLE(vector2, T, W, N);		\
  DECL_VARIABLE(vector3, T, W, N);		\
  DECL_VARIABLE(vector_res, T, W, N)

  /* vector_res = OP(vector, vector3, vector4),
     then store the result.  */
#define TEST_VMLX1(INSN, Q, T1, T2, W, N)				\
  VECT_VAR(vector_res, T1, W, N) =                                      \
    INSN##Q##_##T2##W(VECT_VAR(vector1, T1, W, N),			\
		      VECT_VAR(vector2, T1, W, N),			\
		      VECT_VAR(vector3, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),                         \
		    VECT_VAR(vector_res, T1, W, N))

#define TEST_VMLX(INSN, Q, T1, T2, W, N)	\
  TEST_VMLX1(INSN, Q, T1, T2, W, N)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VMLX(int, 8, 8);
  DECL_VMLX(int, 16, 4);
  DECL_VMLX(int, 32, 2);
  DECL_VMLX(uint, 8, 8);
  DECL_VMLX(uint, 16, 4);
  DECL_VMLX(uint, 32, 2);
  DECL_VMLX(float, 32, 2);
  DECL_VMLX(int, 8, 16);
  DECL_VMLX(int, 16, 8);
  DECL_VMLX(int, 32, 4);
  DECL_VMLX(uint, 8, 16);
  DECL_VMLX(uint, 16, 8);
  DECL_VMLX(uint, 32, 4);
  DECL_VMLX(float, 32, 4);

  clean_results ();

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

  TEST_VDUP(vector2, , int, s, 8, 8, 0x11);
  TEST_VDUP(vector2, , int, s, 16, 4, 0x22);
  TEST_VDUP(vector2, , int, s, 32, 2, 0x33);
  TEST_VDUP(vector2, , uint, u, 8, 8, 0x44);
  TEST_VDUP(vector2, , uint, u, 16, 4, 0x55);
  TEST_VDUP(vector2, , uint, u, 32, 2, 0x66);
  TEST_VDUP(vector2, , float, f, 32, 2, 33.1);
  TEST_VDUP(vector2, q, int, s, 8, 16, 0x77);
  TEST_VDUP(vector2, q, int, s, 16, 8, 0x88);
  TEST_VDUP(vector2, q, int, s, 32, 4, 0x99);
  TEST_VDUP(vector2, q, uint, u, 8, 16, 0xAA);
  TEST_VDUP(vector2, q, uint, u, 16, 8, 0xBB);
  TEST_VDUP(vector2, q, uint, u, 32, 4, 0xCC);
  TEST_VDUP(vector2, q, float, f, 32, 4, 99.2);

  TEST_VDUP(vector3, , int, s, 8, 8, 0xFF);
  TEST_VDUP(vector3, , int, s, 16, 4, 0xEE);
  TEST_VDUP(vector3, , int, s, 32, 2, 0xDD);
  TEST_VDUP(vector3, , uint, u, 8, 8, 0xCC);
  TEST_VDUP(vector3, , uint, u, 16, 4, 0xBB);
  TEST_VDUP(vector3, , uint, u, 32, 2, 0xAA);
  TEST_VDUP(vector3, , float, f, 32, 2, 10.23);
  TEST_VDUP(vector3, q, int, s, 8, 16, 0x99);
  TEST_VDUP(vector3, q, int, s, 16, 8, 0x88);
  TEST_VDUP(vector3, q, int, s, 32, 4, 0x77);
  TEST_VDUP(vector3, q, uint, u, 8, 16, 0x66);
  TEST_VDUP(vector3, q, uint, u, 16, 8, 0x55);
  TEST_VDUP(vector3, q, uint, u, 32, 4, 0x44);
  TEST_VDUP(vector3, q, float, f, 32, 4, 77.8);

  TEST_VMLX(INSN_NAME, , int, s, 8, 8);
  TEST_VMLX(INSN_NAME, , int, s, 16, 4);
  TEST_VMLX(INSN_NAME, , int, s, 32, 2);
  TEST_VMLX(INSN_NAME, , uint, u, 8, 8);
  TEST_VMLX(INSN_NAME, , uint, u, 16, 4);
  TEST_VMLX(INSN_NAME, , uint, u, 32, 2);
  TEST_VMLX(INSN_NAME, , float, f, 32, 2);
  TEST_VMLX(INSN_NAME, q, int, s, 8, 16);
  TEST_VMLX(INSN_NAME, q, int, s, 16, 8);
  TEST_VMLX(INSN_NAME, q, int, s, 32, 4);
  TEST_VMLX(INSN_NAME, q, uint, u, 8, 16);
  TEST_VMLX(INSN_NAME, q, uint, u, 16, 8);
  TEST_VMLX(INSN_NAME, q, uint, u, 32, 4);
  TEST_VMLX(INSN_NAME, q, float, f, 32, 4);

  dump_results_hex (TEST_MSG);
}
