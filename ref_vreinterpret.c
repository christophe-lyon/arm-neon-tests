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
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#define TEST_MSG "VREINTERPRET/VREINTERPRETQ"

void exec_vreinterpret (void)
{
  int i;

  /* Basic test: y=vreinterpret(x), then store the result.  */
#define TEST_VREINTERPRET(Q, T1, T2, W, N, TS1, TS2, WS, NS)		\
  VECT_VAR(vector_res, T1, W, N) =					\
    vreinterpret##Q##_##T2##W##_##TS2##WS(VECT_VAR(vector, TS1, WS, NS)); \
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),				\
		    VECT_VAR(vector_res, T1, W, N));			\
  DUMP(TEST_MSG, T1, W, N, PRIx##W);

#define TEST_VREINTERPRET_FP(Q, T1, T2, W, N, TS1, TS2, WS, NS)		\
  VECT_VAR(vector_res, T1, W, N) =					\
    vreinterpret##Q##_##T2##W##_##TS2##WS(VECT_VAR(vector, TS1, WS, NS)); \
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),				\
		    VECT_VAR(vector_res, T1, W, N));			\
  DUMP_FP(TEST_MSG, T1, W, N, PRIx##W);

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  clean_results ();


  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(TEST_VLOAD, vector, buffer);
  TEST_VLOAD(vector, buffer, , float, f, 32, 2);
  TEST_VLOAD(vector, buffer, q, float, f, 32, 4);

  /* The same result buffers are used multiple times, so output them
     before overwriting them  */
  fprintf(ref_file, "\n%s output:\n", TEST_MSG);


  /* vreinterpret_s8_xx */
  TEST_VREINTERPRET(, int, s, 8, 8, int, s, 16, 4);
  TEST_VREINTERPRET(, int, s, 8, 8, int, s, 32, 2);
  TEST_VREINTERPRET(, int, s, 8, 8, int, s, 64, 1);
  TEST_VREINTERPRET(, int, s, 8, 8, uint, u, 8, 8);
  TEST_VREINTERPRET(, int, s, 8, 8, uint, u, 16, 4);
  TEST_VREINTERPRET(, int, s, 8, 8, uint, u, 32, 2);
  TEST_VREINTERPRET(, int, s, 8, 8, uint, u, 64, 1);

  /* vreinterpret_s16_xx */
  TEST_VREINTERPRET(, int, s, 16, 4, int, s, 8, 8);
  TEST_VREINTERPRET(, int, s, 16, 4, int, s, 32, 2);
  TEST_VREINTERPRET(, int, s, 16, 4, int, s, 64, 1);
  TEST_VREINTERPRET(, int, s, 16, 4, uint, u, 8, 8);
  TEST_VREINTERPRET(, int, s, 16, 4, uint, u, 16, 4);
  TEST_VREINTERPRET(, int, s, 16, 4, uint, u, 32, 2);
  TEST_VREINTERPRET(, int, s, 16, 4, uint, u, 64, 1);

  /* vreinterpret_s32_xx */
  TEST_VREINTERPRET(, int, s, 32, 2, int, s, 8, 8);
  TEST_VREINTERPRET(, int, s, 32, 2, int, s, 16, 4);
  TEST_VREINTERPRET(, int, s, 32, 2, int, s, 64, 1);
  TEST_VREINTERPRET(, int, s, 32, 2, uint, u, 8, 8);
  TEST_VREINTERPRET(, int, s, 32, 2, uint, u, 16, 4);
  TEST_VREINTERPRET(, int, s, 32, 2, uint, u, 32, 2);
  TEST_VREINTERPRET(, int, s, 32, 2, uint, u, 64, 1);

  /* vreinterpret_s64_xx */
  TEST_VREINTERPRET(, int, s, 64, 1, int, s, 8, 8);
  TEST_VREINTERPRET(, int, s, 64, 1, int, s, 16, 4);
  TEST_VREINTERPRET(, int, s, 64, 1, int, s, 32, 2);
  TEST_VREINTERPRET(, int, s, 64, 1, uint, u, 8, 8);
  TEST_VREINTERPRET(, int, s, 64, 1, uint, u, 16, 4);
  TEST_VREINTERPRET(, int, s, 64, 1, uint, u, 32, 2);
  TEST_VREINTERPRET(, int, s, 64, 1, uint, u, 64, 1);

  /* vreinterpret_u8_xx */
  TEST_VREINTERPRET(, uint, u, 8, 8, int, s, 8, 8);
  TEST_VREINTERPRET(, uint, u, 8, 8, int, s, 16, 4);
  TEST_VREINTERPRET(, uint, u, 8, 8, int, s, 32, 2);
  TEST_VREINTERPRET(, uint, u, 8, 8, int, s, 64, 1);
  TEST_VREINTERPRET(, uint, u, 8, 8, uint, u, 16, 4);
  TEST_VREINTERPRET(, uint, u, 8, 8, uint, u, 32, 2);
  TEST_VREINTERPRET(, uint, u, 8, 8, uint, u, 64, 1);

  /* vreinterpret_u16_xx */
  TEST_VREINTERPRET(, uint, u, 16, 4, int, s, 8, 8);
  TEST_VREINTERPRET(, uint, u, 16, 4, int, s, 16, 4);
  TEST_VREINTERPRET(, uint, u, 16, 4, int, s, 32, 2);
  TEST_VREINTERPRET(, uint, u, 16, 4, int, s, 64, 1);
  TEST_VREINTERPRET(, uint, u, 16, 4, uint, u, 8, 8);
  TEST_VREINTERPRET(, uint, u, 16, 4, uint, u, 32, 2);
  TEST_VREINTERPRET(, uint, u, 16, 4, uint, u, 64, 1);

  /* vreinterpret_u32_xx */
  TEST_VREINTERPRET(, uint, u, 32, 2, int, s, 8, 8);
  TEST_VREINTERPRET(, uint, u, 32, 2, int, s, 16, 4);
  TEST_VREINTERPRET(, uint, u, 32, 2, int, s, 32, 2);
  TEST_VREINTERPRET(, uint, u, 32, 2, int, s, 64, 1);
  TEST_VREINTERPRET(, uint, u, 32, 2, uint, u, 8, 8);
  TEST_VREINTERPRET(, uint, u, 32, 2, uint, u, 16, 4);
  TEST_VREINTERPRET(, uint, u, 32, 2, uint, u, 64, 1);

  /* vreinterpret_u64_xx */
  TEST_VREINTERPRET(, uint, u, 64, 1, int, s, 8, 8);
  TEST_VREINTERPRET(, uint, u, 64, 1, int, s, 16, 4);
  TEST_VREINTERPRET(, uint, u, 64, 1, int, s, 32, 2);
  TEST_VREINTERPRET(, uint, u, 64, 1, int, s, 64, 1);
  TEST_VREINTERPRET(, uint, u, 64, 1, uint, u, 8, 8);
  TEST_VREINTERPRET(, uint, u, 64, 1, uint, u, 16, 4);
  TEST_VREINTERPRET(, uint, u, 64, 1, uint, u, 32, 2);

  /* vreinterpretq_s8_xx */
  TEST_VREINTERPRET(q, int, s, 8, 16, int, s, 16, 8);
  TEST_VREINTERPRET(q, int, s, 8, 16, int, s, 32, 4);
  TEST_VREINTERPRET(q, int, s, 8, 16, int, s, 64, 2);
  TEST_VREINTERPRET(q, int, s, 8, 16, uint, u, 8, 16);
  TEST_VREINTERPRET(q, int, s, 8, 16, uint, u, 16, 8);
  TEST_VREINTERPRET(q, int, s, 8, 16, uint, u, 32, 4);
  TEST_VREINTERPRET(q, int, s, 8, 16, uint, u, 64, 2);

  /* vreinterpretq_s16_xx */
  TEST_VREINTERPRET(q, int, s, 16, 8, int, s, 8, 16);
  TEST_VREINTERPRET(q, int, s, 16, 8, int, s, 32, 4);
  TEST_VREINTERPRET(q, int, s, 16, 8, int, s, 64, 2);
  TEST_VREINTERPRET(q, int, s, 16, 8, uint, u, 8, 16);
  TEST_VREINTERPRET(q, int, s, 16, 8, uint, u, 16, 8);
  TEST_VREINTERPRET(q, int, s, 16, 8, uint, u, 32, 4);
  TEST_VREINTERPRET(q, int, s, 16, 8, uint, u, 64, 2);

  /* vreinterpretq_s32_xx */
  TEST_VREINTERPRET(q, int, s, 32, 4, int, s, 8, 16);
  TEST_VREINTERPRET(q, int, s, 32, 4, int, s, 16, 8);
  TEST_VREINTERPRET(q, int, s, 32, 4, int, s, 64, 2);
  TEST_VREINTERPRET(q, int, s, 32, 4, uint, u, 8, 16);
  TEST_VREINTERPRET(q, int, s, 32, 4, uint, u, 16, 8);
  TEST_VREINTERPRET(q, int, s, 32, 4, uint, u, 32, 4);
  TEST_VREINTERPRET(q, int, s, 32, 4, uint, u, 64, 2);

  /* vreinterpretq_s64_xx */
  TEST_VREINTERPRET(q, int, s, 64, 2, int, s, 8, 16);
  TEST_VREINTERPRET(q, int, s, 64, 2, int, s, 16, 8);
  TEST_VREINTERPRET(q, int, s, 64, 2, int, s, 32, 4);
  TEST_VREINTERPRET(q, int, s, 64, 2, uint, u, 8, 16);
  TEST_VREINTERPRET(q, int, s, 64, 2, uint, u, 16, 8);
  TEST_VREINTERPRET(q, int, s, 64, 2, uint, u, 32, 4);
  TEST_VREINTERPRET(q, int, s, 64, 2, uint, u, 64, 2);

  /* vreinterpretq_u16_xx */
  TEST_VREINTERPRET(q, uint, u, 16, 8, int, s, 8, 16);
  TEST_VREINTERPRET(q, uint, u, 16, 8, int, s, 16, 8);
  TEST_VREINTERPRET(q, uint, u, 16, 8, int, s, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 16, 8, int, s, 64, 2);
  TEST_VREINTERPRET(q, uint, u, 16, 8, uint, u, 8, 16);
  TEST_VREINTERPRET(q, uint, u, 16, 8, uint, u, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 16, 8, uint, u, 64, 2);

  /* vreinterpretq_u32_xx */
  TEST_VREINTERPRET(q, uint, u, 32, 4, int, s, 8, 16);
  TEST_VREINTERPRET(q, uint, u, 32, 4, int, s, 16, 8);
  TEST_VREINTERPRET(q, uint, u, 32, 4, int, s, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 32, 4, int, s, 64, 2);
  TEST_VREINTERPRET(q, uint, u, 32, 4, uint, u, 8, 16);
  TEST_VREINTERPRET(q, uint, u, 32, 4, uint, u, 16, 8);
  TEST_VREINTERPRET(q, uint, u, 32, 4, uint, u, 64, 2);

  /* vreinterpretq_u64_xx */
  TEST_VREINTERPRET(q, uint, u, 64, 2, int, s, 8, 16);
  TEST_VREINTERPRET(q, uint, u, 64, 2, int, s, 16, 8);
  TEST_VREINTERPRET(q, uint, u, 64, 2, int, s, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 64, 2, int, s, 64, 2);
  TEST_VREINTERPRET(q, uint, u, 64, 2, uint, u, 8, 16);
  TEST_VREINTERPRET(q, uint, u, 64, 2, uint, u, 16, 8);
  TEST_VREINTERPRET(q, uint, u, 64, 2, uint, u, 32, 4);

  /* vreinterpretq_u8_xx */
  TEST_VREINTERPRET(q, uint, u, 8, 16, int, s, 8, 16);
  TEST_VREINTERPRET(q, uint, u, 8, 16, int, s, 16, 8);
  TEST_VREINTERPRET(q, uint, u, 8, 16, int, s, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 8, 16, int, s, 64, 2);
  TEST_VREINTERPRET(q, uint, u, 8, 16, uint, u, 16, 8);
  TEST_VREINTERPRET(q, uint, u, 8, 16, uint, u, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 8, 16, uint, u, 64, 2);

  /* vreinterpret_f32_xx */
  TEST_VREINTERPRET_FP(, float, f, 32, 2, int, s, 8, 8);
  TEST_VREINTERPRET_FP(, float, f, 32, 2, int, s, 16, 4);
  TEST_VREINTERPRET_FP(, float, f, 32, 2, int, s, 32, 2);
  TEST_VREINTERPRET_FP(, float, f, 32, 2, int, s, 64, 1);
  TEST_VREINTERPRET_FP(, float, f, 32, 2, uint, u, 8, 8);
  TEST_VREINTERPRET_FP(, float, f, 32, 2, uint, u, 16, 4);
  TEST_VREINTERPRET_FP(, float, f, 32, 2, uint, u, 32, 2);
  TEST_VREINTERPRET_FP(, float, f, 32, 2, uint, u, 64, 1);

  /* vreinterpretq_f32_xx */
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, int, s, 8, 16);
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, int, s, 16, 8);
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, int, s, 32, 4);
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, int, s, 64, 2);
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, uint, u, 8, 16);
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, uint, u, 16, 8);
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, uint, u, 32, 4);
  TEST_VREINTERPRET_FP(q, float, f, 32, 4, uint, u, 64, 2);

  /* vreinterpret_xx_f32 */
  TEST_VREINTERPRET(, int, s, 8, 8, float, f, 32, 2);
  TEST_VREINTERPRET(, int, s, 16, 4, float, f, 32, 2);
  TEST_VREINTERPRET(, int, s, 32, 2, float, f, 32, 2);
  TEST_VREINTERPRET(, int, s, 64, 1, float, f, 32, 2);
  TEST_VREINTERPRET(, uint, u, 8, 8, float, f, 32, 2);
  TEST_VREINTERPRET(, uint, u, 16, 4, float, f, 32, 2);
  TEST_VREINTERPRET(, uint, u, 32, 2, float, f, 32, 2);
  TEST_VREINTERPRET(, uint, u, 64, 1, float, f, 32, 2);

  /* vreinterpretq_xx_f32 */
  TEST_VREINTERPRET(q, int, s, 8, 16, float, f, 32, 4);
  TEST_VREINTERPRET(q, int, s, 16, 8, float, f, 32, 4);
  TEST_VREINTERPRET(q, int, s, 32, 4, float, f, 32, 4);
  TEST_VREINTERPRET(q, int, s, 64, 2, float, f, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 8, 16, float, f, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 16, 8, float, f, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 32, 4, float, f, 32, 4);
  TEST_VREINTERPRET(q, uint, u, 64, 2, float, f, 32, 4);
}
