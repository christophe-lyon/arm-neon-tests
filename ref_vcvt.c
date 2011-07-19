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

#define TEST_MSG "VCVT/VCVTQ"
void exec_vcvt (void)
{
  int i;

  /* Basic test: y=vcvt(x), then store the result.  */
#define TEST_VCVT(Q, T1, T2, W, N, TS1, TS2)			\
  VECT_VAR(vector_res, T1, W, N) =				\
    vcvt##Q##_##T2##W##_##TS2##W(VECT_VAR(vector, TS1, W, N));	\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),			\
		    VECT_VAR(vector_res, T1, W, N));		\
  DUMP(TEST_MSG, T1, W, N, PRIx##W);

#define TEST_VCVT_FP(Q, T1, T2, W, N, TS1, TS2)			\
  VECT_VAR(vector_res, T1, W, N) =				\
    vcvt##Q##_##T2##W##_##TS2##W(VECT_VAR(vector, TS1, W, N));	\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),			\
		    VECT_VAR(vector_res, T1, W, N));		\
  DUMP_FP(TEST_MSG, T1, W, N, PRIx##W);

#define TEST_VCVT_N(Q, T1, T2, W, N, TS1, TS2, V)			\
  VECT_VAR(vector_res, T1, W, N) =					\
    vcvt##Q##_n_##T2##W##_##TS2##W(VECT_VAR(vector, TS1, W, N), V);	\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),				\
		    VECT_VAR(vector_res, T1, W, N));			\
  DUMP(TEST_MSG, T1, W, N, PRIx##W);

#define TEST_VCVT_N_FP(Q, T1, T2, W, N, TS1, TS2, V)			\
  VECT_VAR(vector_res, T1, W, N) =					\
    vcvt##Q##_n_##T2##W##_##TS2##W(VECT_VAR(vector, TS1, W, N), V);	\
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

  /* Make sure some elements have a fractional part, to exercise
     integer conversions */
  TEST_VSET_LANE(vector, , float, f, 32, 2, 0, -15.3f);
  TEST_VSET_LANE(vector, , float, f, 32, 2, 1, 5.3f);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 2, -15.3f);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 3, 5.3f);

  /* The same result buffers are used multiple times, so we output
     them before overwriting them  */
  fprintf(ref_file, "\n%s output:\n", TEST_MSG);

  /* vcvt_f32_xx */
  TEST_VCVT_FP(, float, f, 32, 2, int, s);
  TEST_VCVT_FP(, float, f, 32, 2, uint, u);

  /* vcvtq_f32_xx */
  TEST_VCVT_FP(q, float, f, 32, 4, int, s);
  TEST_VCVT_FP(q, float, f, 32, 4, uint, u);

  /* vcvt_xx_f32 */
  TEST_VCVT(, int, s, 32, 2, float, f);
  TEST_VCVT(, uint, u, 32, 2, float, f);

  /* vcvtq_xx_f32 */
  TEST_VCVT(q, int, s, 32, 4, float, f);
  TEST_VCVT(q, uint, u, 32, 4, float, f);

  /* The same result buffers are used multiple times, so output them
     before overwriting them  */
#undef TEST_MSG
#define TEST_MSG "VCVT_N/VCVTQ_N"
  fprintf(ref_file, "\n%s output:\n", TEST_MSG);

  /* vcvt_n_f32_xx */
  TEST_VCVT_N_FP(, float, f, 32, 2, int, s, 2);
  TEST_VCVT_N_FP(, float, f, 32, 2, uint, u, 7);

  /* vcvtq_n_f32_xx */
  TEST_VCVT_N_FP(q, float, f, 32, 4, int, s, 30);
  TEST_VCVT_N_FP(q, float, f, 32, 4, uint, u, 12);

  /* vcvt_n_xx_f32 */
  TEST_VCVT_N(, int, s, 32, 2, float, f, 20);
  TEST_VCVT_N(, uint, u, 32, 2, float, f, 2);

  /* vcvtq_n_xx_f32 */
  TEST_VCVT_N(q, int, s, 32, 4, float, f, 13);
  TEST_VCVT_N(q, uint, u, 32, 4, float, f, 1);

  /* Check rounding */
#undef TEST_MSG
#define TEST_MSG "VCVT/VCVTQ"
  fprintf(ref_file, "\n%s output:\n", TEST_MSG " (check rounding)");
  TEST_VDUP(vector, , float, f, 32, 2, 10.4f);
  TEST_VDUP(vector, q, float, f, 32, 4, 125.9f);
  /* vcvt_xx_f32 */
  TEST_VCVT(, int, s, 32, 2, float, f);
  TEST_VCVT(, uint, u, 32, 2, float, f);
  /* vcvtq_xx_f32 */
  TEST_VCVT(q, int, s, 32, 4, float, f);
  TEST_VCVT(q, uint, u, 32, 4, float, f);

#undef TEST_MSG
#define TEST_MSG "VCVT_N/VCVTQ_N"
  fprintf(ref_file, "\n%s output:\n", TEST_MSG " (check rounding)");
  /* vcvt_n_xx_f32 */
  TEST_VCVT_N(, int, s, 32, 2, float, f, 20);
  TEST_VCVT_N(, uint, u, 32, 2, float, f, 20);
  /* vcvtq_n_xx_f32 */
  TEST_VCVT_N(q, int, s, 32, 4, float, f, 13);
  TEST_VCVT_N(q, uint, u, 32, 4, float, f, 13);

#undef TEST_MSG
#define TEST_MSG "VCVT_N/VCVTQ_N"
  fprintf(ref_file, "\n%s output:\n", TEST_MSG " (check saturation)");
  /* vcvt_n_xx_f32 */
  TEST_VCVT_N(, int, s, 32, 2, float, f, 31);
  /* vcvtq_n_xx_f32 */
  TEST_VCVT_N(q, int, s, 32, 4, float, f, 31);
}
