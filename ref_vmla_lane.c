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

#ifndef INSN_NAME
#define INSN_NAME vmla
#define TEST_MSG "VMLA_LANE"
#endif

#define FNNAME1(NAME) void exec_ ## NAME ##_lane (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
#define DECL_VMLX_LANE(VAR)			\
  DECL_VARIABLE(VAR, int, 16, 4);		\
  DECL_VARIABLE(VAR, int, 32, 2);		\
  DECL_VARIABLE(VAR, uint, 16, 4);		\
  DECL_VARIABLE(VAR, uint, 32, 2);		\
  DECL_VARIABLE(VAR, float, 32, 2);		\
  DECL_VARIABLE(VAR, int, 16, 8);		\
  DECL_VARIABLE(VAR, int, 32, 4);		\
  DECL_VARIABLE(VAR, uint, 16, 8);		\
  DECL_VARIABLE(VAR, uint, 32, 4);		\
  DECL_VARIABLE(VAR, float, 32, 4)

  /* vector_res = vmlx_lane(vector, vector2, vector3, lane),
     then store the result.  */
#define TEST_VMLX_LANE1(INSN, Q, T1, T2, W, N, N2, L)			\
  VECT_VAR(vector_res, T1, W, N) =					\
    INSN##Q##_lane_##T2##W(VECT_VAR(vector, T1, W, N),			\
			   VECT_VAR(vector2, T1, W, N),			\
			   VECT_VAR(vector3, T1, W, N2),		\
			   L);						\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), \
		    VECT_VAR(vector_res, T1, W, N))

#define TEST_VMLX_LANE(INSN, Q, T1, T2, W, N, N2, V)	\
  TEST_VMLX_LANE1(INSN, Q, T1, T2, W, N, N2, V)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VMLX_LANE(vector);
  DECL_VMLX_LANE(vector2);
  DECL_VMLX_LANE(vector_res);

  DECL_VARIABLE(vector3, int, 16, 4);
  DECL_VARIABLE(vector3, int, 32, 2);
  DECL_VARIABLE(vector3, uint, 16, 4);
  DECL_VARIABLE(vector3, uint, 32, 2);
  DECL_VARIABLE(vector3, float, 32, 2);

  clean_results ();

  VLOAD(vector, buffer, , int, s, 16, 4);
  VLOAD(vector, buffer, , int, s, 32, 2);
  VLOAD(vector, buffer, , uint, u, 16, 4);
  VLOAD(vector, buffer, , uint, u, 32, 2);
  VLOAD(vector, buffer, q, int, s, 16, 8);
  VLOAD(vector, buffer, q, int, s, 32, 4);
  VLOAD(vector, buffer, q, uint, u, 16, 8);
  VLOAD(vector, buffer, q, uint, u, 32, 4);
  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);

  VDUP(vector2, , int, s, 16, 4, 0x55);
  VDUP(vector2, , int, s, 32, 2, 0x55);
  VDUP(vector2, , uint, u, 16, 4, 0x55);
  VDUP(vector2, , uint, u, 32, 2, 0x55);
  VDUP(vector2, , float, f, 32, 2, 55.3f);
  VDUP(vector2, q, int, s, 16, 8, 0x55);
  VDUP(vector2, q, int, s, 32, 4, 0x55);
  VDUP(vector2, q, uint, u, 16, 8, 0x55);
  VDUP(vector2, q, uint, u, 32, 4, 0x55);
  VDUP(vector2, q, float, f, 32, 4, 55.8f);

  VDUP(vector3, , int, s, 16, 4, 0xBB);
  VDUP(vector3, , int, s, 32, 2, 0xBB);
  VDUP(vector3, , uint, u, 16, 4, 0xBB);
  VDUP(vector3, , uint, u, 32, 2, 0xBB);
  VDUP(vector3, , float, f, 32, 2, 11.34f);

  /* Choose lane arbitrarily */
  TEST_VMLX_LANE(INSN_NAME, , int, s, 16, 4, 4, 2);
  TEST_VMLX_LANE(INSN_NAME, , int, s, 32, 2, 2, 1);
  TEST_VMLX_LANE(INSN_NAME, , uint, u, 16, 4, 4, 2);
  TEST_VMLX_LANE(INSN_NAME, , uint, u, 32, 2, 2, 1);
  TEST_VMLX_LANE(INSN_NAME, , float, f, 32, 2, 2, 1);
  TEST_VMLX_LANE(INSN_NAME, q, int, s, 16, 8, 4, 3);
  TEST_VMLX_LANE(INSN_NAME, q, int, s, 32, 4, 2, 1);
  TEST_VMLX_LANE(INSN_NAME, q, uint, u, 16, 8, 4, 2);
  TEST_VMLX_LANE(INSN_NAME, q, uint, u, 32, 4, 2, 1);
  TEST_VMLX_LANE(INSN_NAME, q, float, f, 32, 4, 2, 1);

  dump_results_hex (TEST_MSG);
}
