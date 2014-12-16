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

#define TEST_MSG "VMOVL"
void exec_vmovl (void)
{
  /* Basic test: vec128=vmovl(vec64), then store the result.  */
#define TEST_VMOVL(T1, T2, W, W2, N)					\
  VECT_VAR(vector128, T1, W2, N) =					\
    vmovl_##T2##W(VECT_VAR(vector64, T1, W, N));			\
  vst1q_##T2##W2(VECT_VAR(result, T1, W2, N), VECT_VAR(vector128, T1, W2, N))

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_64BITS_VARIANTS(vector64);
  DECL_VARIABLE_128BITS_VARIANTS(vector128);

  TEST_MACRO_64BITS_VARIANTS_2_5(VLOAD, vector64, buffer);

  clean_results ();

  TEST_VMOVL(int, s, 8, 16, 8);
  TEST_VMOVL(int, s, 16, 32, 4);
  TEST_VMOVL(int, s, 32, 64, 2);
  TEST_VMOVL(uint, u, 8, 16, 8);
  TEST_VMOVL(uint, u, 16, 32, 4);
  TEST_VMOVL(uint, u, 32, 64, 2);

  dump_results_hex (TEST_MSG);
}
