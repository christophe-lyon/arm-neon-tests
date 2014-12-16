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

#define INSN_NAME vceq
#define TEST_MSG "VCEQ/VCEQQ"

/* Extra tests for _p8 variants, which exist only for vceq */
void exec_vceq_p8(void);
#define EXTRA_TESTS exec_vceq_p8

#include "ref_v_comp_op.c"

void exec_vceq_p8(void)
{
  int i;  /* Used by DUMP() */

  DECL_VARIABLE(vector, poly, 8, 8);
  DECL_VARIABLE(vector, poly, 8, 16);

  DECL_VARIABLE(vector2, poly, 8, 8);
  DECL_VARIABLE(vector2, poly, 8, 16);

  DECL_VARIABLE(vector_res, uint, 8, 8);
  DECL_VARIABLE(vector_res, uint, 8, 16);

  clean_results ();

  VLOAD(vector, buffer, , poly, p, 8, 8);
  VLOAD(vector, buffer, q, poly, p, 8, 16);

  VDUP(vector2, , poly, p, 8, 8, 0xF3);
  VDUP(vector2, q, poly, p, 8, 16, 0xF4);

  fprintf(ref_file, "\n%s output:\n", TEST_MSG " p8");
  TEST_VCOMP(INSN_NAME, , poly, p, uint, 8, 8);
  TEST_VCOMP(INSN_NAME, q, poly, p, uint, 8, 16);

  DUMP(TEST_MSG, uint, 8, 8, PRIx8);
  DUMP(TEST_MSG, uint, 8, 16, PRIx8);

}
