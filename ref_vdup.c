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

#define TEST_MSG "VDUP/VDUPQ"
void exec_vdup (void)
{
  int i;

  /* Basic test: vec=vdup(x), then store the result.  */
#define TEST_VDUP(Q, T1, T2, W, N)					\
  VECT_VAR(vector, T1, W, N) =						\
    vdup##Q##_n_##T2##W(VECT_VAR(buffer_dup, T1, W, N)[i]);		\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector, T1, W, N))

  /* Basic test: vec=vmov(x), then store the result.  */
#define TEST_VMOV(Q, T1, T2, W, N)					\
  VECT_VAR(vector, T1, W, N) =						\
    vmov##Q##_n_##T2##W(VECT_VAR(buffer_dup, T1, W, N)[i]);		\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N), VECT_VAR(vector, T1, W, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_ALL_VARIANTS(vector);

  for (i=0; i< 3; i++) {
    clean_results ();

    TEST_VDUP(, int, s, 8, 8);
    TEST_VDUP(, int, s, 16, 4);
    TEST_VDUP(, int, s, 32, 2);
    TEST_VDUP(, int, s, 64, 1);
    TEST_VDUP(, uint, u, 8, 8);
    TEST_VDUP(, uint, u, 16, 4);
    TEST_VDUP(, uint, u, 32, 2);
    TEST_VDUP(, uint, u, 64, 1);
    TEST_VDUP(, poly, p, 8, 8);
    TEST_VDUP(, poly, p, 16, 4);
    TEST_VDUP(, float, f, 32, 2);

    TEST_VDUP(q, int, s, 8, 16);
    TEST_VDUP(q, int, s, 16, 8);
    TEST_VDUP(q, int, s, 32, 4);
    TEST_VDUP(q, int, s, 64, 2);
    TEST_VDUP(q, uint, u, 8, 16);
    TEST_VDUP(q, uint, u, 16, 8);
    TEST_VDUP(q, uint, u, 32, 4);
    TEST_VDUP(q, uint, u, 64, 2);
    TEST_VDUP(q, poly, p, 8, 16);
    TEST_VDUP(q, poly, p, 16, 8);
    TEST_VDUP(q, float, f, 32, 4);

    dump_results_hex (TEST_MSG);
  }

#undef TEST_MSG
#define TEST_MSG "VMOV/VMOVQ"
  for (i=0; i< 3; i++) {
    clean_results ();

    TEST_VMOV(, int, s, 8, 8);
    TEST_VMOV(, int, s, 16, 4);
    TEST_VMOV(, int, s, 32, 2);
    TEST_VMOV(, int, s, 64, 1);
    TEST_VMOV(, uint, u, 8, 8);
    TEST_VMOV(, uint, u, 16, 4);
    TEST_VMOV(, uint, u, 32, 2);
    TEST_VMOV(, uint, u, 64, 1);
    TEST_VMOV(, poly, p, 8, 8);
    TEST_VMOV(, poly, p, 16, 4);
    TEST_VMOV(, float, f, 32, 2);

    TEST_VMOV(q, int, s, 8, 16);
    TEST_VMOV(q, int, s, 16, 8);
    TEST_VMOV(q, int, s, 32, 4);
    TEST_VMOV(q, int, s, 64, 2);
    TEST_VMOV(q, uint, u, 8, 16);
    TEST_VMOV(q, uint, u, 16, 8);
    TEST_VMOV(q, uint, u, 32, 4);
    TEST_VMOV(q, uint, u, 64, 2);
    TEST_VMOV(q, poly, p, 8, 16);
    TEST_VMOV(q, poly, p, 16, 8);
    TEST_VMOV(q, float, f, 32, 4);

    dump_results_hex (TEST_MSG);
  }
}
