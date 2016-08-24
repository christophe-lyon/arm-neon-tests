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
#include <math.h>

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

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#define TEST_VCVT_FP16(T1, T2, W1, W2, N)			\
  VECT_VAR(vector_res, T1, W1, N) =				\
    vcvt_##T2##W1##_##T2##W2(VECT_VAR(vector, T1, W2, N));	\
  vst1q_##T2##W1(VECT_VAR(result, T1, W1, N),			\
		 VECT_VAR(vector_res, T1, W1, N));		\
  DUMP_FP(TEST_MSG, T1, W1, N, PRIx##W1);

#define TEST_VCVT_2FP16(T1, T2, W1, W2, N)			\
  VECT_VAR(vector_res, T1, W1, N) =				\
    vcvt_##T2##W1##_##T2##W2(VECT_VAR(vector, T1, W2, N));	\
  vst1_##T2##W1(VECT_VAR(result, T1, W1, N),			\
		 VECT_VAR(vector_res, T1, W1, N));		\
  DUMP_FP16(TEST_MSG, T1, W1, N, PRIx##W1);
#endif

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
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_VARIABLE(vector_init, uint, 16, 4);
  DECL_VARIABLE(vector_init, uint, 16, 8);
  DECL_VARIABLE(vector, float, 16, 4);
  DECL_VARIABLE(vector, float, 16, 8);
  DECL_VARIABLE(vector_res, float, 16, 4);
  DECL_VARIABLE(vector_res, float, 16, 8);
#endif

  clean_results ();


  /* Initialize input "vector" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector, buffer);
  VLOAD(vector, buffer, , float, f, 32, 2);
  VLOAD(vector, buffer, q, float, f, 32, 4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  VLOAD(vector, buffer, , float, f, 16, 4);
  VLOAD(vector, buffer, q, float, f, 16, 8);
#endif

  /* Make sure some elements have a fractional part, to exercise
     integer conversions */
  TEST_VSET_LANE(vector, , float, f, 32, 2, 0, -15.3f);
  TEST_VSET_LANE(vector, , float, f, 32, 2, 1, 5.3f);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 2, -15.3f);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 3, 5.3f);

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  /* FP16 tests.  */
  /* There is no vdup_n_f16, so we need another initialization
     method.  */
  /* Use all lanes with values of different types: first, a "standard"
     positive number, a "standard" negative one, and +0 and -0.  */
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 0, 0x4b90 /* 15.125 */);
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 1, 0xcb90 /* -15.125 */);
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 2, 0 /* 0 */);
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 3, 0x8000 /* -0 */);
  VECT_VAR(vector, float, 16, 4) =
    vreinterpret_f16_u16(VECT_VAR(vector_init, uint, 16, 4));
#endif

  /* The same result buffers are used multiple times, so we output
     them before overwriting them  */
  fprintf(ref_file, "\n%s output:\n", TEST_MSG);
  fprintf(gcc_tests_file, "\n%s output:\n", TEST_MSG);

  /* vcvt_f32_xx */
  TEST_VCVT_FP(, float, f, 32, 2, int, s);
  TEST_VCVT_FP(, float, f, 32, 2, uint, u);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_VCVT_FP16(float, f, 32, 16, 4);
#endif

  /* vcvtq_f32_xx */
  TEST_VCVT_FP(q, float, f, 32, 4, int, s);
  TEST_VCVT_FP(q, float, f, 32, 4, uint, u);

  /* vcvt_xx_f32 */
  TEST_VCVT(, int, s, 32, 2, float, f);
  TEST_VCVT(, uint, u, 32, 2, float, f);

  TEST_VSET_LANE(vector, q, float, f, 32, 4, 0, 0.0f);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 1, -0.0f);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 2, 15.12f);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 3, -15.12f);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_VCVT_2FP16(float, f, 16, 32, 4);
#endif

  /* vcvtq_xx_f32 */
  TEST_VCVT(q, int, s, 32, 4, float, f);
  TEST_VCVT(q, uint, u, 32, 4, float, f);

  /* The same result buffers are used multiple times, so output them
     before overwriting them  */
#undef TEST_MSG
#define TEST_MSG "VCVT_N/VCVTQ_N"
  fprintf(ref_file, "\n%s output:\n", TEST_MSG);
  fprintf(gcc_tests_file, "\n%s output:\n", TEST_MSG);

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
  VDUP(vector, , float, f, 32, 2, 10.4f);
  VDUP(vector, q, float, f, 32, 4, 125.9f);
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

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#undef TEST_MSG
#define TEST_MSG "VCVT FP16"
  fprintf(ref_file, "\n%s output:\n", TEST_MSG " (check fp16-fp32 inf/nan/denormal)");
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 0, 0x0390 /* DENORMAL */);
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 1, 0x7c00 /* inf */);
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 2, 0x7e00 /* nan */);
  TEST_VSET_LANE(vector_init, , uint, u, 16, 4, 3, 0xfc00 /* -inf */);
  VECT_VAR(vector, float, 16, 4) =
    vreinterpret_f16_u16(VECT_VAR(vector_init, uint, 16, 4));
  TEST_VCVT_FP16(float, f, 32, 16, 4);

  fprintf(ref_file, "\n%s output:\n", TEST_MSG " (check fp32-fp16 inf/nan)");
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 0, NAN);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 1, HUGE_VALF);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 2, -HUGE_VALF);
  TEST_VSET_LANE(vector, q, float, f, 32, 4, 3, -0.0f);
  TEST_VCVT_2FP16(float, f, 16, 32, 4);
#endif
}
