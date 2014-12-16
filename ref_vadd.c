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

#define INSN_NAME vadd
#define TEST_MSG "VADD/VADDQ"

/* Extra tests for functions requiring floating-point types */
void exec_vadd_f32(void);
#define EXTRA_TESTS exec_vadd_f32

#include "ref_v_binary_op.c"

void exec_vadd_f32(void)
{
  int i;

  DECL_VARIABLE(vector, float, 32, 2);
  DECL_VARIABLE(vector, float, 32, 4);

  DECL_VARIABLE(vector2, float, 32, 2);
  DECL_VARIABLE(vector2, float, 32, 4);

  DECL_VARIABLE(vector_res, float, 32, 2);
  DECL_VARIABLE(vector_res, float, 32, 4);

  VDUP(vector, , float, f, 32, 2, 2.3f);
  VDUP(vector, q, float, f, 32, 4, 3.4f);

  VDUP(vector2, , float, f, 32, 2, 4.5f);
  VDUP(vector2, q, float, f, 32, 4, 5.6f);

  TEST_BINARY_OP(INSN_NAME, , float, f, 32, 2);
  TEST_BINARY_OP(INSN_NAME, q, float, f, 32, 4);

  fprintf(ref_file, "\nfloat32:\n");
  DUMP_FP(TEST_MSG, float, 32, 2, PRIx32);
  DUMP_FP(TEST_MSG, float, 32, 4, PRIx32);
}
