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

#define INSN_NAME vmull
#define TEST_MSG "VMULL_N"
void exec_vmull_n (void)
{
  int i;

  /* vector_res = vmull_n(vector,val), then store the result.  */
#define TEST_VMULL_N1(INSN, T1, T2, W, W2, N, L)			\
  VECT_VAR(vector_res, T1, W2, N) =					\
    INSN##_n_##T2##W(VECT_VAR(vector, T1, W, N),			\
		     L);						\
  vst1q_##T2##W2(VECT_VAR(result, T1, W2, N), VECT_VAR(vector_res, T1, W2, N))

#define TEST_VMULL_N(INSN, T1, T2, W, W2, N, L)	\
  TEST_VMULL_N1(INSN, T1, T2, W, W2, N, L)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, uint, 16, 4);
  DECL_VARIABLE(vector, uint, 32, 2);

  DECL_VARIABLE(vector_res, int, 32, 4);
  DECL_VARIABLE(vector_res, int, 64, 2);
  DECL_VARIABLE(vector_res, uint, 32, 4);
  DECL_VARIABLE(vector_res, uint, 64, 2);

  clean_results ();

  /* Initialize vector */
  VDUP(vector, , int, s, 16, 4, 0x1000);
  VDUP(vector, , int, s, 32, 2, 0x1000);
  VDUP(vector, , uint, u, 16, 4, 0x1000);
  VDUP(vector, , uint, u, 32, 2, 0x1000);

  /* Choose multiplier arbitrarily */
  TEST_VMULL_N(INSN_NAME, int, s, 16, 32, 4, 0x11);
  TEST_VMULL_N(INSN_NAME, int, s, 32, 64, 2, 0x22);
  TEST_VMULL_N(INSN_NAME, uint, u, 16, 32, 4, 0x33);
  TEST_VMULL_N(INSN_NAME, uint, u, 32, 64, 2, 0x44);

  fprintf(ref_file, "\n%s output:\n", TEST_MSG);
  fprintf(gcc_tests_file, "\n%s output:\n", TEST_MSG);
  DUMP(TEST_MSG, int, 32, 4, PRIx32);
  DUMP(TEST_MSG, int, 64, 2, PRIx64);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);
  DUMP(TEST_MSG, uint, 64, 2, PRIx64);
}
