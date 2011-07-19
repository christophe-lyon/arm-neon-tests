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

#define TEST_MSG "VRECPS/VRECPSQ"
void exec_vrecps(void)
{
  int i;

  /* Basic test: y=vrecps(x), then store the result.  */
#define TEST_VRECPS(Q, T1, T2, W, N)					\
  VECT_VAR(vector_res, T1, W, N) =					\
    vrecps##Q##_##T2##W(VECT_VAR(vector, T1, W, N),			\
			VECT_VAR(vector2, T1, W, N));			\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),				\
		    VECT_VAR(vector_res, T1, W, N))

    /* With ARM RVCT, we need to declare variables before any executable
       statement  */

  /* No need for integer variants */
  DECL_VARIABLE(vector, float, 32, 2);
  DECL_VARIABLE(vector, float, 32, 4);

  DECL_VARIABLE(vector2, float, 32, 2);
  DECL_VARIABLE(vector2, float, 32, 4);

  DECL_VARIABLE(vector_res, float, 32, 2);
  DECL_VARIABLE(vector_res, float, 32, 4);

  clean_results ();

  /* Choose init value arbitrarily */
  TEST_VDUP(vector, , float, f, 32, 2, 12.9f);
  TEST_VDUP(vector, q, float, f, 32, 4, 9.2f);

  TEST_VDUP(vector2, , float, f, 32, 2, 8.9f);
  TEST_VDUP(vector2, q, float, f, 32, 4, 3.2f);

  /* Apply the operator */
  TEST_VRECPS(, float, f, 32, 2);
  TEST_VRECPS(q, float, f, 32, 4);

  fprintf (ref_file, "\n%s output:\n", TEST_MSG);
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 4, PRIx32);
}
