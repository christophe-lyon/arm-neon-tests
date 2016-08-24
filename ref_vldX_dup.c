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

void exec_vldX_dup (void)
{
  /* In this case, input variables are arrays of vectors */
#define DECL_VLDX_DUP(T1, W, N, X)					\
  VECT_ARRAY_TYPE(T1, W, N, X) VECT_ARRAY_VAR(vector, T1, W, N, X);	\
  VECT_VAR_DECL(result_bis_##X, T1, W, N)[X * N]

  /* We need to use a temporary result buffer (result_bis), because
     the one used for other tests is not large enough. A subset of the
     result data is moved from result_bis to result, and it is this
     subset which is used to check the actual behaviour. The next
     macro enables to move another chunk of data from result_bis to
     result.  */
#define TEST_VLDX_DUP(Q, T1, T2, W, N, X)				\
  VECT_ARRAY_VAR(vector, T1, W, N, X) =					\
    vld##X##Q##_dup_##T2##W(&VECT_VAR(buffer_dup, T1, W, N)[0]);	\
									\
  vst##X##Q##_##T2##W(VECT_VAR(result_bis_##X, T1, W, N),		\
		      VECT_ARRAY_VAR(vector, T1, W, N, X));		\
  memcpy(VECT_VAR(result, T1, W, N), VECT_VAR(result_bis_##X, T1, W, N), \
	 sizeof(VECT_VAR(result, T1, W, N)));


  /* Overwrite "result" with the contents of "result_bis"[Y] */
#define TEST_EXTRA_CHUNK(T1, W, N, X,Y)			\
  memcpy(VECT_VAR(result, T1, W, N),			\
	 &(VECT_VAR(result_bis_##X, T1, W, N)[Y*N]),	\
	 sizeof(VECT_VAR(result, T1, W, N)));

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
#define DECL_ALL_VLDX_DUP(X)			\
  DECL_VLDX_DUP(int, 8, 8, X);			\
  DECL_VLDX_DUP(int, 16, 4, X);			\
  DECL_VLDX_DUP(int, 32, 2, X);			\
  DECL_VLDX_DUP(int, 64, 1, X);			\
  DECL_VLDX_DUP(uint, 8, 8, X);			\
  DECL_VLDX_DUP(uint, 16, 4, X);		\
  DECL_VLDX_DUP(uint, 32, 2, X);		\
  DECL_VLDX_DUP(uint, 64, 1, X);		\
  DECL_VLDX_DUP(poly, 8, 8, X);			\
  DECL_VLDX_DUP(poly, 16, 4, X);		\
  DECL_VLDX_DUP(float, 32, 2, X)

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#define DECL_ALL_VLDX_DUP_FP16(X)		\
  DECL_VLDX_DUP(float, 16, 4, X)
#endif

#define TEST_ALL_VLDX_DUP(X)			\
  TEST_VLDX_DUP(, int, s, 8, 8, X);		\
  TEST_VLDX_DUP(, int, s, 16, 4, X);		\
  TEST_VLDX_DUP(, int, s, 32, 2, X);		\
  TEST_VLDX_DUP(, int, s, 64, 1, X);		\
  TEST_VLDX_DUP(, uint, u, 8, 8, X);		\
  TEST_VLDX_DUP(, uint, u, 16, 4, X);		\
  TEST_VLDX_DUP(, uint, u, 32, 2, X);		\
  TEST_VLDX_DUP(, uint, u, 64, 1, X);		\
  TEST_VLDX_DUP(, poly, p, 8, 8, X);		\
  TEST_VLDX_DUP(, poly, p, 16, 4, X);		\
  TEST_VLDX_DUP(, float, f, 32, 2, X)

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#define TEST_ALL_VLDX_DUP_FP16(X)		\
  TEST_VLDX_DUP(, float, f, 16, 4, X)
#endif

#define TEST_ALL_EXTRA_CHUNKS(X, Y)		\
  TEST_EXTRA_CHUNK(int, 8, 8, X, Y);		\
  TEST_EXTRA_CHUNK(int, 16, 4, X, Y);		\
  TEST_EXTRA_CHUNK(int, 32, 2, X, Y);		\
  TEST_EXTRA_CHUNK(int, 64, 1, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 8, 8, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 16, 4, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 32, 2, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 64, 1, X, Y);		\
  TEST_EXTRA_CHUNK(poly, 8, 8, X, Y);		\
  TEST_EXTRA_CHUNK(poly, 16, 4, X, Y);		\
  TEST_EXTRA_CHUNK(float, 32, 2, X, Y)

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#define TEST_ALL_EXTRA_CHUNKS_FP16(X, Y)	\
  TEST_EXTRA_CHUNK(float, 16, 4, X, Y)
#endif


  DECL_ALL_VLDX_DUP(2);
  DECL_ALL_VLDX_DUP(3);
  DECL_ALL_VLDX_DUP(4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_ALL_VLDX_DUP_FP16(2);
  DECL_ALL_VLDX_DUP_FP16(3);
  DECL_ALL_VLDX_DUP_FP16(4);
#endif

  /* Check vld2_dup/vld2q_dup */
  clean_results ();
#define TEST_MSG "VLD2_DUP/VLD2Q_DUP"
  TEST_ALL_VLDX_DUP(2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_VLDX_DUP_FP16(2);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 0");
  TEST_ALL_EXTRA_CHUNKS(2, 1);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_EXTRA_CHUNKS_FP16(2, 1);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 1");

  /* Check vld3_dup/vld3q_dup */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VLD3_DUP/VLD3Q_DUP"
  TEST_ALL_VLDX_DUP(3);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_VLDX_DUP_FP16(3);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 0");
  TEST_ALL_EXTRA_CHUNKS(3, 1);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_EXTRA_CHUNKS_FP16(3, 1);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 1");
  TEST_ALL_EXTRA_CHUNKS(3, 2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_EXTRA_CHUNKS_FP16(3, 2);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 2");

  /* Check vld4_dup/vld4q_dup */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VLD4_DUP/VLD4Q_DUP"
  TEST_ALL_VLDX_DUP(4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_VLDX_DUP_FP16(4);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 0");
  TEST_ALL_EXTRA_CHUNKS(4, 1);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_EXTRA_CHUNKS_FP16(4, 1);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 1");
  TEST_ALL_EXTRA_CHUNKS(4, 2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_EXTRA_CHUNKS_FP16(4, 2);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 2");
  TEST_ALL_EXTRA_CHUNKS(4, 3);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_EXTRA_CHUNKS_FP16(4, 3);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 3");
}
