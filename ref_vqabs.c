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

#define INSN_NAME vqabs
#define TEST_MSG "VQABS/VQABSQ"

/* Extra tests for functions requiring corner cases tests */
void vqabs_extra(void);
#define EXTRA_TESTS vqabs_extra

#include "ref_v_unary_sat_op.c"

void vqabs_extra()
{
  /* No need for 64 bits variants */
  DECL_VARIABLE(vector, int, 8, 8);
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, int, 8, 16);
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);

  DECL_VARIABLE(vector_res, int, 8, 8);
  DECL_VARIABLE(vector_res, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 8, 16);
  DECL_VARIABLE(vector_res, int, 16, 8);
  DECL_VARIABLE(vector_res, int, 32, 4);

  clean_results ();

  /* Initialize input "vector" with max negative values to check
     saturation  */
  VDUP(vector, , int, s, 8, 8, 0x80);
  VDUP(vector, , int, s, 16, 4, 0x8000);
  VDUP(vector, , int, s, 32, 2, 0x80000000);
  VDUP(vector, q, int, s, 8, 16, 0x80);
  VDUP(vector, q, int, s, 16, 8, 0x8000);
  VDUP(vector, q, int, s, 32, 4, 0x80000000);

  /* Apply a unary operator named INSN_NAME  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_UNARY_SAT_OP(INSN_NAME, , int, s, 8, 8);
  TEST_UNARY_SAT_OP(INSN_NAME, , int, s, 16, 4);
  TEST_UNARY_SAT_OP(INSN_NAME, , int, s, 32, 2);
  TEST_UNARY_SAT_OP(INSN_NAME, q, int, s, 8, 16);
  TEST_UNARY_SAT_OP(INSN_NAME, q, int, s, 16, 8);
  TEST_UNARY_SAT_OP(INSN_NAME, q, int, s, 32, 4);

  dump_results_hex (TEST_MSG);
}
