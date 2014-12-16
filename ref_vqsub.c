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

#define INSN_NAME vqsub
#define TEST_MSG "VQSUB/VQSUBQ"

/* Extra tests for functions requiring types larger than 64 bits to
   compute saturation */
void vqsub_64(void);
#define EXTRA_TESTS vqsub_64

#include "ref_v_binary_sat_op.c"

void vqsub_64(void)
{
  int i;

  DECL_VARIABLE_ALL_VARIANTS(vector1);
  DECL_VARIABLE_ALL_VARIANTS(vector2);
  DECL_VARIABLE_ALL_VARIANTS(vector_res);

  /* Initialize input "vector1" from "buffer"  */
  TEST_MACRO_ALL_VARIANTS_2_5(VLOAD, vector1, buffer);

  VDUP(vector2, , int, s, 64, 1, 0x0);
  VDUP(vector2, , uint, u, 64, 1, 0x0);
  VDUP(vector2, q, int, s, 64, 2, 0x0);
  VDUP(vector2, q, uint, u, 64, 2, 0x0);

  fprintf(ref_file,
	  "\n%s 64 bits saturation cumulative saturation output:\n",
	  TEST_MSG);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 64, 1);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 64, 1);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 64, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 64, 2);

  fprintf(ref_file, "\n64 bits saturation:\n");
  DUMP(TEST_MSG, int, 64, 1, PRIx64);
  DUMP(TEST_MSG, uint, 64, 1, PRIx64);
  DUMP(TEST_MSG, int, 64, 2, PRIx64);
  DUMP(TEST_MSG, uint, 64, 2, PRIx64);

  /* Another set of tests */
  VDUP(vector2, , int, s, 64, 1, 0x44);
  VDUP(vector2, , uint, u, 64, 1, 0x88);
  VDUP(vector2, q, int, s, 64, 2, 0x44);
  VDUP(vector2, q, uint, u, 64, 2, 0x88);

  fprintf(ref_file,
	  "\n%s 64 bits saturation cumulative saturation output:\n",
	  TEST_MSG);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 64, 1);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 64, 1);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 64, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 64, 2);

  DUMP(TEST_MSG, int, 64, 1, PRIx64);
  DUMP(TEST_MSG, uint, 64, 1, PRIx64);
  DUMP(TEST_MSG, int, 64, 2, PRIx64);
  DUMP(TEST_MSG, uint, 64, 2, PRIx64);

  /* Another set of tests */
  VDUP(vector2, , int, s, 64, 1, 0x7fffffffffffffffLL);
  VDUP(vector2, , uint, u, 64, 1, 0xffffffffffffffffULL);

  /* To check positive saturation, we need to write a positive value
     in vector1 */
  VDUP(vector1, q, int, s, 64, 2, 0x3fffffffffffffffLL);
  VDUP(vector2, q, int, s, 64, 2, 0x8000000000000000LL);

  VDUP(vector2, q, uint, u, 64, 2, 0xffffffffffffffffULL);

  fprintf(ref_file,
	  "\n%s 64 bits saturation cumulative saturation output:\n",
	  TEST_MSG);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 64, 1);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 64, 1);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 64, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 64, 2);

  DUMP(TEST_MSG, int, 64, 1, PRIx64);
  DUMP(TEST_MSG, uint, 64, 1, PRIx64);
  DUMP(TEST_MSG, int, 64, 2, PRIx64);
  DUMP(TEST_MSG, uint, 64, 2, PRIx64);

  /* To improve coverage, check saturation with less than 64 bits too */
  fprintf(ref_file, "\nless than 64 bits saturation:\n");
  VDUP(vector2, , int, s, 8, 8, 0x7F);
  VDUP(vector2, , int, s, 16, 4, 0x7FFF);
  VDUP(vector2, , int, s, 32, 2, 0x7FFFFFFF);
  VDUP(vector2, q, int, s, 8, 16, 0x7F);
  VDUP(vector2, q, int, s, 16, 8, 0x7FFF);
  VDUP(vector2, q, int, s, 32, 4, 0x7FFFFFFF);

  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 8, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 16, 4);
  TEST_BINARY_SAT_OP(INSN_NAME, , int, s, 32, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 8, 16);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 16, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, q, int, s, 32, 4);

  DUMP(TEST_MSG, int, 8, 8, PRIx8);
  DUMP(TEST_MSG, int, 16, 4, PRIx16);
  DUMP(TEST_MSG, int, 32, 2, PRIx32);
  DUMP(TEST_MSG, int, 8, 16, PRIx8);
  DUMP(TEST_MSG, int, 16, 8, PRIx16);
  DUMP(TEST_MSG, int, 32, 4, PRIx32);


  VDUP(vector1, , uint, u, 8, 8, 0x10);
  VDUP(vector1, , uint, u, 16, 4, 0x10);
  VDUP(vector1, , uint, u, 32, 2, 0x10);
  VDUP(vector1, q, uint, u, 8, 16, 0x10);
  VDUP(vector1, q, uint, u, 16, 8, 0x10);
  VDUP(vector1, q, uint, u, 32, 4, 0x10);

  VDUP(vector2, , uint, u, 8, 8, 0x20);
  VDUP(vector2, , uint, u, 16, 4, 0x20);
  VDUP(vector2, , uint, u, 32, 2, 0x20);
  VDUP(vector2, q, uint, u, 8, 16, 0x20);
  VDUP(vector2, q, uint, u, 16, 8, 0x20);
  VDUP(vector2, q, uint, u, 32, 4, 0x20);

  fprintf(ref_file,
	  "\n%s less than 64 bits saturation cumulative saturation output:\n",
	  TEST_MSG);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 8, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 16, 4);
  TEST_BINARY_SAT_OP(INSN_NAME, , uint, u, 32, 2);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 8, 16);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 16, 8);
  TEST_BINARY_SAT_OP(INSN_NAME, q, uint, u, 32, 4);

  DUMP(TEST_MSG, uint, 8, 8, PRIx8);
  DUMP(TEST_MSG, uint, 16, 4, PRIx16);
  DUMP(TEST_MSG, uint, 32, 2, PRIx32);
  DUMP(TEST_MSG, uint, 8, 16, PRIx8);
  DUMP(TEST_MSG, uint, 16, 8, PRIx16);
  DUMP(TEST_MSG, uint, 32, 4, PRIx32);
}
