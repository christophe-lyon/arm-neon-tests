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

void exec_vstX_lane (void)
{
  /* In this case, input variables are arrays of vectors */
#define DECL_VSTX_LANE(T1, W, N, X)					\
  VECT_ARRAY_TYPE(T1, W, N, X) VECT_ARRAY_VAR(vector, T1, W, N, X);	\
  VECT_ARRAY_TYPE(T1, W, N, X) VECT_ARRAY_VAR(vector_src, T1, W, N, X);	\
  VECT_VAR_DECL(result_bis_##X, T1, W, N)[X * N]

  /* We need to use a temporary result buffer (result_bis), because
     the one used for other tests is not large enough. A subset of the
     result data is moved from result_bis to result, and it is this
     subset which is used to check the actual behaviour. The next
     macro enables to move another chunk of data from result_bis to
     result.  */
#define TEST_VSTX_LANE(Q, T1, T2, W, N, X, L)				\
  memset (VECT_VAR(buffer_src, T1, W, N), 0xAA,				\
	  sizeof(VECT_VAR(buffer_src, T1, W, N)));			\
  memset (VECT_VAR(result_bis_##X, T1, W, N), 0,			\
	  sizeof(VECT_VAR(result_bis_##X, T1, W, N)));			\
									\
  VECT_ARRAY_VAR(vector_src, T1, W, N, X) =				\
    vld##X##Q##_##T2##W(VECT_VAR(buffer_src, T1, W, N));		\
									\
  VECT_ARRAY_VAR(vector, T1, W, N, X) =					\
    /* Use dedicated init buffer, of size X */				\
    vld##X##Q##_lane_##T2##W(VECT_VAR(buffer_vld##X##_lane, T1, W, X),	\
			     VECT_ARRAY_VAR(vector_src, T1, W, N, X),	\
			     L);					\
  vst##X##Q##_lane_##T2##W(VECT_VAR(result_bis_##X, T1, W, N),		\
			   VECT_ARRAY_VAR(vector, T1, W, N, X),		\
			   L);						\
  memcpy(VECT_VAR(result, T1, W, N), VECT_VAR(result_bis_##X, T1, W, N), \
	 sizeof(VECT_VAR(result, T1, W, N)));

  /* Overwrite "result" with the contents of "result_bis"[Y] */
#define TEST_EXTRA_CHUNK(T1, W, N, X, Y)		\
  memcpy(VECT_VAR(result, T1, W, N),			\
	 &(VECT_VAR(result_bis_##X, T1, W, N)[Y*N]),	\
	 sizeof(VECT_VAR(result, T1, W, N)));

  /* With ARM RVCT, we need to declare variables before any executable
     statement */

  /* We need all variants in 64 bits, but there is no 64x2 variant */
#define DECL_ALL_VSTX_LANE(X)			\
  DECL_VSTX_LANE(int, 8, 8, X);			\
  DECL_VSTX_LANE(int, 16, 4, X);		\
  DECL_VSTX_LANE(int, 32, 2, X);		\
  DECL_VSTX_LANE(uint, 8, 8, X);		\
  DECL_VSTX_LANE(uint, 16, 4, X);		\
  DECL_VSTX_LANE(uint, 32, 2, X);		\
  DECL_VSTX_LANE(poly, 8, 8, X);		\
  DECL_VSTX_LANE(poly, 16, 4, X);		\
  DECL_VSTX_LANE(float, 32, 2, X);		\
  DECL_VSTX_LANE(int, 16, 8, X);		\
  DECL_VSTX_LANE(int, 32, 4, X);		\
  DECL_VSTX_LANE(uint, 16, 8, X);		\
  DECL_VSTX_LANE(uint, 32, 4, X);		\
  DECL_VSTX_LANE(poly, 16, 8, X);		\
  DECL_VSTX_LANE(float, 32, 4, X)

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#define DECL_ALL_VSTX_LANE_FP16(X)		\
  DECL_VSTX_LANE(float, 16, 4, X);		\
  DECL_VSTX_LANE(float, 16, 8, X)
#endif

#define DUMMY_ARRAY(V, T, W, N, L) VECT_VAR_DECL(V,T,W,N)[N*L]

  /* Use the same lanes regardless of the size of the array (X), for
     simplicity */
#define TEST_ALL_VSTX_LANE(X)			\
  TEST_VSTX_LANE(, int, s, 8, 8, X, 7);		\
  TEST_VSTX_LANE(, int, s, 16, 4, X, 2);	\
  TEST_VSTX_LANE(, int, s, 32, 2, X, 0);	\
  TEST_VSTX_LANE(, float, f, 32, 2, X, 0);	\
  TEST_VSTX_LANE(, uint, u, 8, 8, X, 4);	\
  TEST_VSTX_LANE(, uint, u, 16, 4, X, 3);	\
  TEST_VSTX_LANE(, uint, u, 32, 2, X, 1);	\
  TEST_VSTX_LANE(, poly, p, 8, 8, X, 4);	\
  TEST_VSTX_LANE(, poly, p, 16, 4, X, 3);	\
  TEST_VSTX_LANE(q, int, s, 16, 8, X, 6);	\
  TEST_VSTX_LANE(q, int, s, 32, 4, X, 2);	\
  TEST_VSTX_LANE(q, uint, u, 16, 8, X, 5);	\
  TEST_VSTX_LANE(q, uint, u, 32, 4, X, 0);	\
  TEST_VSTX_LANE(q, poly, p, 16, 8, X, 5);	\
  TEST_VSTX_LANE(q, float, f, 32, 4, X, 2)

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#define TEST_ALL_VSTX_LANE_FP16(X)		\
  TEST_VSTX_LANE(, float, f, 16, 4, X, 3);	\
  TEST_VSTX_LANE(q, float, f, 16, 8, X, 6)
#endif

#define TEST_ALL_EXTRA_CHUNKS(X, Y)		\
  TEST_EXTRA_CHUNK(int, 8, 8, X, Y);		\
  TEST_EXTRA_CHUNK(int, 16, 4, X, Y);		\
  TEST_EXTRA_CHUNK(int, 32, 2, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 8, 8, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 16, 4, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 32, 2, X, Y);		\
  TEST_EXTRA_CHUNK(poly, 8, 8, X, Y);		\
  TEST_EXTRA_CHUNK(poly, 16, 4, X, Y);		\
  TEST_EXTRA_CHUNK(float, 32, 2, X, Y);		\
  TEST_EXTRA_CHUNK(int, 16, 8, X, Y);		\
  TEST_EXTRA_CHUNK(int, 32, 4, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 16, 8, X, Y);		\
  TEST_EXTRA_CHUNK(uint, 32, 4, X, Y);		\
  TEST_EXTRA_CHUNK(poly, 16, 8, X, Y);		\
  TEST_EXTRA_CHUNK(float, 32, 4, X, Y)

#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
#define TEST_ALL_EXTRA_CHUNKS_FP16(X, Y)	\
  TEST_EXTRA_CHUNK(float, 16, 4, X, Y);		\
  TEST_EXTRA_CHUNK(float, 16, 8, X, Y)
#endif

  /* Declare the temporary buffers / variables */
  DECL_ALL_VSTX_LANE(2);
  DECL_ALL_VSTX_LANE(3);
  DECL_ALL_VSTX_LANE(4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DECL_ALL_VSTX_LANE_FP16(2);
  DECL_ALL_VSTX_LANE_FP16(3);
  DECL_ALL_VSTX_LANE_FP16(4);
#endif

  /* Define dummy input arrays, large enough for x4 vectors */
  DUMMY_ARRAY(buffer_src, int, 8, 8, 4);
  DUMMY_ARRAY(buffer_src, int, 16, 4, 4);
  DUMMY_ARRAY(buffer_src, int, 32, 2, 4);
  DUMMY_ARRAY(buffer_src, uint, 8, 8, 4);
  DUMMY_ARRAY(buffer_src, uint, 16, 4, 4);
  DUMMY_ARRAY(buffer_src, uint, 32, 2, 4);
  DUMMY_ARRAY(buffer_src, poly, 8, 8, 4);
  DUMMY_ARRAY(buffer_src, poly, 16, 4, 4);
  DUMMY_ARRAY(buffer_src, float, 32, 2, 4);
  DUMMY_ARRAY(buffer_src, int, 16, 8, 4);
  DUMMY_ARRAY(buffer_src, int, 32, 4, 4);
  DUMMY_ARRAY(buffer_src, uint, 16, 8, 4);
  DUMMY_ARRAY(buffer_src, uint, 32, 4, 4);
  DUMMY_ARRAY(buffer_src, poly, 16, 8, 4);
  DUMMY_ARRAY(buffer_src, float, 32, 4, 4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  DUMMY_ARRAY(buffer_src, float, 16, 4, 4);
  DUMMY_ARRAY(buffer_src, float, 16, 8, 4);
#endif

  /* Check vst2_lane/vst2q_lane */
  clean_results ();
#define TEST_MSG "VST2_LANE/VST2Q_LANE"
  TEST_ALL_VSTX_LANE(2);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_VSTX_LANE_FP16(2);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 0");

  TEST_ALL_EXTRA_CHUNKS(2, 1);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_EXTRA_CHUNKS_FP16(2, 1);
#endif
  dump_results_hex2 (TEST_MSG, " chunk 1");

  /* Check vst3_lane/vst3q_lane */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VST3_LANE/VST3Q_LANE"
  TEST_ALL_VSTX_LANE(3);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_VSTX_LANE_FP16(3);
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

  /* Check vst4_lane/vst4q_lane */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VST4_LANE/VST4Q_LANE"
  TEST_ALL_VSTX_LANE(4);
#if defined(__ARM_FP16_FORMAT_IEEE) && ( ((__ARM_FP & 0x2) != 0) || ((__ARM_NEON_FP16_INTRINSICS & 1) != 0) )
  TEST_ALL_VSTX_LANE_FP16(4);
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
