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

#if defined(__cplusplus)
#include <cstdio>
#include <cstdint>
#else
#include <stdio.h>
#if defined(_MSC_VER)
#include "msstdint.h"
#else
#include <stdint.h>
#endif
#endif

#if defined(__arm__) || defined(__aarch64__)
#include <dspfns.h>
#else
#include "stm-dspfns.h"
#endif

extern FILE* ref_file;

#define __xSTR(X) #X
#define __STR(X) __xSTR(X)

#define FN(X) X, __STR(X)

typedef int32_t func32_32_32_ptr(int32_t, int32_t);
typedef int16_t func16_32_ptr(int32_t);
typedef int32_t func32_32_16_16_ptr(int32_t, int16_t, int16_t);

void test_16_fn_32(func16_32_ptr func, const char* func_name,
		   int init_Overflow, int init_Carry)
{
  int32_t svar32_a;
  int16_t svar16_a;

  fprintf(ref_file, "Checking %s with input Overflow=%d and input Carry=%d\n",
	  func_name, init_Overflow, init_Carry);

  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = func(svar32_a);
  fprintf(ref_file, "%s(%#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = func(svar32_a);
  fprintf(ref_file, "%s(%#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = func(svar32_a);
  fprintf(ref_file, "%s(%#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = func(svar32_a);
  fprintf(ref_file, "%s(%#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -32769;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = func(svar32_a);
  fprintf(ref_file, "%s(%#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, Overflow, Carry);
}

void test_32_fn_32_32(func32_32_32_ptr func, const char* func_name,
		      int init_Overflow, int init_Carry)
{
  int32_t svar32_a, svar32_b, svar32_c;

  fprintf(ref_file, "Checking %s with input Overflow=%d and input Carry=%d\n",
	  func_name, init_Overflow, init_Carry);

  svar32_a = 1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = -2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x7000;
  svar32_b = 0x7000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFF;
  svar32_b = 0x8FFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x70000000;
  svar32_b = 0x70000000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFFFFFF;
  svar32_b = 0x8FFFFFFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFFFFFF;
  svar32_b = 0xFFFFFFFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = func(svar32_a, svar32_b);
  fprintf(ref_file, "%s(%#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar32_b, svar32_c, Overflow, Carry);
}

void test_32_fn_32_16_16(func32_32_16_16_ptr func, const char* func_name,
		      int init_Overflow, int init_Carry)
{
  int32_t svar32_a, svar32_b;
  int16_t svar16_a, svar16_b;

  fprintf(ref_file, "Checking %s with input Overflow=%d and input Carry=%d\n",
	  func_name, init_Overflow, init_Carry);

  svar16_a = 2;
  svar16_b = 2;
  svar32_a = 0x1234;
  Overflow = 0;
  Carry = init_Carry;
  svar32_b = func(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "%s(%#x, %#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = 0;
  Carry = init_Carry;
  svar32_b = func(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "%s(%#x, %#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = 0;
  Carry = init_Carry;
  svar32_b = func(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "%s(%#x, %#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = 0;
  Carry = init_Carry;
  svar32_a = -1;
  svar32_b = func(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "%s(%#x, %#x, %#x) = %#x overflow %d carry %d\n",
	  func_name, svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);
}

void exec_dspfns1 (int init_Overflow, int init_Carry)
{
  int32_t svar32_a, svar32_b, svar32_c;
  int16_t svar16_a, svar16_b, svar16_c;


  fprintf(ref_file, "\n\nDSP FNS (non-NEON/ITU) intrinsics with input Overflow=%d and input Carry=%d\n", init_Overflow, init_Carry);

  /* saturate */
  /* int16_t saturate(int32_t x) */
  test_16_fn_32(FN(saturate), init_Overflow, init_Carry);


  /* add */
  /* int16_t add(int16_t x, int16_t y) */
  svar16_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = add(svar16_a, svar16_b);
  fprintf(ref_file, "add(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = add(svar16_a, svar16_b);
  fprintf(ref_file, "add(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 20000;
  svar16_b = 20000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = add(svar16_a, svar16_b);
  fprintf(ref_file, "add(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -20000;
  svar16_b = -20000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = add(svar16_a, svar16_b);
  fprintf(ref_file, "add(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* sub */
  /* int16_t sub(int16_t x, int16_t y) */
  svar16_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = sub(svar16_a, svar16_b);
  fprintf(ref_file, "sub(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = sub(svar16_a, svar16_b);
  fprintf(ref_file, "sub(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 20000;
  svar16_b = 20000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = sub(svar16_a, svar16_b);
  fprintf(ref_file, "sub(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -20000;
  svar16_b = -20000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = sub(svar16_a, svar16_b);
  fprintf(ref_file, "sub(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0;
  svar16_b = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = sub(svar16_a, svar16_b);
  fprintf(ref_file, "sub(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* abs_s */
  /* int16_t abs_s(int16_t x) */
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = abs_s(svar16_b);
  fprintf(ref_file, "abs_s(%#x) = %#x overflow %d carry %d\n", svar16_b, svar16_a, Overflow, Carry);

  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = abs_s(svar16_b);
  fprintf(ref_file, "abs_s(%#x) = %#x overflow %d carry %d\n", svar16_b, svar16_a, Overflow, Carry);

  svar16_b = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = abs_s(svar16_b);
  fprintf(ref_file, "abs_s(%#x) = %#x overflow %d carry %d\n", svar16_b, svar16_a, Overflow, Carry);


  /* shl */
  /* int16_t shl(int16_t x, int16_t y) */
  svar16_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 10;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = 20;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 20;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = -64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shl(svar16_a, svar16_b);
  fprintf(ref_file, "shl(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* shr */
  /* int16_t shr(int16_t x, int16_t y) */
  svar16_a = 1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 10;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = -20;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 20;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = 64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr(svar16_a, svar16_b);
  fprintf(ref_file, "shr(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* mult */
  /* int16_t mult(int16_t x, int16_t y) */
  svar16_a = 2;
  svar16_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mult(svar16_a, svar16_b);
  fprintf(ref_file, "mult(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mult(svar16_a, svar16_b);
  fprintf(ref_file, "mult(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mult(svar16_a, svar16_b);
  fprintf(ref_file, "mult(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* L_mult */
  /* int32_t L_mult(int16_t x, int16_t y) */
  svar16_a = 2;
  svar16_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_mult(svar16_a, svar16_b);
  fprintf(ref_file, "L_mult(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar32_a, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_mult(svar16_a, svar16_b);
  fprintf(ref_file, "L_mult(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar32_a, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_mult(svar16_a, svar16_b);
  fprintf(ref_file, "L_mult(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar32_a, Overflow, Carry);


  /* negate */
  /* int16_t negate(int16_t x) */
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = negate(svar16_b);
  fprintf(ref_file, "negate(%#x) = %#x overflow %d carry %d\n", svar16_b, svar16_a, Overflow, Carry);

  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = negate(svar16_b);
  fprintf(ref_file, "negate(%#x) = %#x overflow %d carry %d\n", svar16_b, svar16_a, Overflow, Carry);

  svar16_b = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = negate(svar16_b);
  fprintf(ref_file, "negate(%#x) = %#x overflow %d carry %d\n", svar16_b, svar16_a, Overflow, Carry);


  /* extract_h */
  /* int16_t extract_h(int32_t x) */
  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_h(svar32_a);
  fprintf(ref_file, "extract_h(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_h(svar32_a);
  fprintf(ref_file, "extract_h(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_h(svar32_a);
  fprintf(ref_file, "extract_h(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 0x12345678;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_h(svar32_a);
  fprintf(ref_file, "extract_h(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);


  /* extract_l */
  /* int16_t extract_l(int32_t x) */
  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_l(svar32_a);
  fprintf(ref_file, "extract_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_l(svar32_a);
  fprintf(ref_file, "extract_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_l(svar32_a);
  fprintf(ref_file, "extract_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 0x43218765;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = extract_l(svar32_a);
  fprintf(ref_file, "extract_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);


  /* round */
  /* int16_t round(int32_t x) */
  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = round(svar32_a);
  fprintf(ref_file, "round(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = round(svar32_a);
  fprintf(ref_file, "round(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = round(svar32_a);
  fprintf(ref_file, "round(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 0x43218765;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = round(svar32_a);
  fprintf(ref_file, "round(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 0x87654321;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = round(svar32_a);
  fprintf(ref_file, "round(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);


  /* L_mac */
  /* int32_t L_mac(int32_t acc, int16_t x, int16_t y) */
  svar16_a = 2;
  svar16_b = 2;
  svar32_a = 0x1234;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_b = L_mac(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_mac(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_b = L_mac(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_mac(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_b = L_mac(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_mac(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = -1;
  svar32_b = L_mac(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_mac(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);


  /* L_msu */
  /* int32_t L_msu(int32_t acc, int16_t x, int16_t y) */
  svar16_a = 2;
  svar16_b = 2;
  svar32_a = 0x1234;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_b = L_msu(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_msu(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_b = L_msu(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_msu(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_b = L_msu(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_msu(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_b = L_msu(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "L_msu(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar32_b, Overflow, Carry);


  /* L_add */
  /* int32_t L_add(int32_t val1, int32_t val2); */
  svar32_a = 1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_add(svar32_a, svar32_b);
  fprintf(ref_file, "L_add(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = -2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_add(svar32_a, svar32_b);
  fprintf(ref_file, "L_add(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_add(svar32_a, svar32_b);
  fprintf(ref_file, "L_add(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x7000;
  svar32_b = 0x7000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_add(svar32_a, svar32_b);
  fprintf(ref_file, "L_add(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFF;
  svar32_b = 0x8FFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_add(svar32_a, svar32_b);
  fprintf(ref_file, "L_add(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x70000000;
  svar32_b = 0x70000000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_add(svar32_a, svar32_b);
  fprintf(ref_file, "L_add(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFFFFFF;
  svar32_b = 0x8FFFFFFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_add(svar32_a, svar32_b);
  fprintf(ref_file, "L_add(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  /* L_sub */
  /* int32_t L_sub(int32_t val1, int32_t val2); */
  svar32_a = 1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = -2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x7000;
  svar32_b = 0xFFFF9000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFF;
  svar32_b = 0xFFFF7001;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x70000000;
  svar32_b = 0x90000000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFFFFFF;
  svar32_b = 0x70000001;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0;
  svar32_b = 0x80000000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_sub(svar32_a, svar32_b);
  fprintf(ref_file, "L_sub(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);


  /* L_add_c */
  /* int32_t L_add_c(int32_t val1, int32_t val2); */
  test_32_fn_32_32(FN(L_add_c), init_Overflow, init_Carry);



  /* L_sub_c */
  /* int32_t L_sub_c(int32_t val1, int32_t val2); */
#undef MYFN
#define MYFN L_sub_c
  svar32_a = 1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = -2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = -1;
  svar32_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x7000;
  svar32_b = 0x7000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFF;
  svar32_b = 0x8FFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x70000000;
  svar32_b = 0x70000000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x8FFFFFFF;
  svar32_b = 0x8FFFFFFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0x1;
  svar32_b = 0x80000000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFFFFFFF;
  svar32_b = 0x7FFFFFFF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = MYFN(svar32_a, svar32_b);
  fprintf(ref_file, __STR(MYFN) "(%#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar32_b, svar32_c, Overflow, Carry);


  /* L_macNs */
  /* int32_t L_macNs(int32_t acc, int16_t x, int16_t y) */
  test_32_fn_32_16_16(FN(L_macNs), init_Overflow, init_Carry);

  /* L_msuNs */
  /* int32_t L_msuNs(int32_t acc, int16_t x, int16_t y) */
  test_32_fn_32_16_16(FN(L_msuNs), init_Overflow, init_Carry);


  /* negate */
  /* int32_t negate(int32_t x) */
  svar32_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = negate(svar32_b);
  fprintf(ref_file, "negate(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);

  svar32_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = negate(svar32_b);
  fprintf(ref_file, "negate(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);

  svar32_b = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = negate(svar32_b);
  fprintf(ref_file, "negate(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);


  /* mult_r */
  /* int16_t mult_r(int16_t x, int16_t y) */
  svar16_a = 2;
  svar16_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mult_r(svar16_a, svar16_b);
  fprintf(ref_file, "mult_r(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mult_r(svar16_a, svar16_b);
  fprintf(ref_file, "mult_r(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mult_r(svar16_a, svar16_b);
  fprintf(ref_file, "mult_r(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* norm_s */
  /* int16_t norm_s(int32_t x) */
  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_s(svar32_a);
  fprintf(ref_file, "norm_s(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_s(svar32_a);
  fprintf(ref_file, "norm_s(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_s(svar32_a);
  fprintf(ref_file, "norm_s(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 12000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_s(svar32_a);
  fprintf(ref_file, "norm_s(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);


  /* norm_l */
  /* int16_t norm_l(int16_t x) */
  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_l(svar32_a);
  fprintf(ref_file, "norm_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_l(svar32_a);
  fprintf(ref_file, "norm_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_l(svar32_a);
  fprintf(ref_file, "norm_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 12000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_l(svar32_a);
  fprintf(ref_file, "norm_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 0x123456;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_l(svar32_a);
  fprintf(ref_file, "norm_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);

  svar32_a = 0xABCDEF;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_a = norm_l(svar32_a);
  fprintf(ref_file, "norm_l(%#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, Overflow, Carry);


  /* L_shl */
  /* int32_t L_shl(int32_t x, int16_t y) */
  svar32_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 10;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = 20;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = 2;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = 40;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 20;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = -64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = -64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shl(svar32_a, svar16_b);
  fprintf(ref_file, "L_shl(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);


  /* L_shr */
  /* int32_t L_shr(int32_t x, int16_t y) */
  svar32_a = 1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 10;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = -20;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = -40;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 20;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = 64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = 64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);


  /* shr_r */
  /* int16_t shr_r(int16_t x, int16_t y) */
  svar16_a = 1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 10;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = -20;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 20;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0xFFF;
  svar16_b = 64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = shr_r(svar16_a, svar16_b);
  fprintf(ref_file, "shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* mac_r */
  /* int16_t mac_r(int32_t acc, int16_t x, int16_t y) */
  svar16_a = 2;
  svar16_b = 2;
  svar32_a = 0x1234;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mac_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "mac_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mac_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "mac_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mac_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "mac_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = -1;
  svar16_c = mac_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "mac_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x244;
  svar16_b = 0x522;
  svar32_a = 0x123456;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = mac_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "mac_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* msu_r */
  /* int32_t msu_r(int32_t acc, int16_t x, int16_t y) */
  svar16_a = 2;
  svar16_b = 2;
  svar32_a = 0x1234;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = msu_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "msu_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = -1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = msu_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "msu_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = msu_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "msu_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x8000;
  svar16_b = 0x8000;
  svar32_a = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = msu_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "msu_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 0x321;
  svar16_b = 0x243;
  svar32_a = 0x123456;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = msu_r(svar32_a, svar16_a, svar16_b);
  fprintf(ref_file, "msu_r(%#x, %#x, %#x) = %#x overflow %d carry %d\n", svar32_a, svar16_a, svar16_b, svar16_c, Overflow, Carry);


  /* L_deposit_h */
  /* int32_t L_deposit_h(int16_t x) */
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_h(svar16_b);
  fprintf(ref_file, "L_deposit_h(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);

  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_h(svar16_b);
  fprintf(ref_file, "L_deposit_h(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);

  svar16_b = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_h(svar16_b);
  fprintf(ref_file, "L_deposit_h(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);

  svar16_b = 0x1234;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_h(svar16_b);
  fprintf(ref_file, "L_deposit_h(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);


  /* L_deposit_l */
  /* int32_t L_deposit_l(int16_t x) */
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_l(svar16_b);
  fprintf(ref_file, "L_deposit_l(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);

  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_l(svar16_b);
  fprintf(ref_file, "L_deposit_l(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);

  svar16_b = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_l(svar16_b);
  fprintf(ref_file, "L_deposit_l(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);

  svar16_b = 0x1234;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_deposit_l(svar16_b);
  fprintf(ref_file, "L_deposit_l(%#x) = %#x overflow %d carry %d\n", svar16_b, svar32_a, Overflow, Carry);


  /* L_shr_r */
  /* int32_t L_shr_r(int32_t x, int16_t y) */
  svar32_a = 1;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 10;
  svar16_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = -20;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = -10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = -40;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 20;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0xFFF;
  svar16_b = 64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = 10;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);

  svar32_a = 0x12345678;
  svar16_b = 64;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_c = L_shr_r(svar32_a, svar16_b);
  fprintf(ref_file, "L_shr_r(%#x, %d) = %#x overflow %d carry %d\n", svar32_a, svar16_b, svar32_c, Overflow, Carry);


  /* L_abs */
  /* int32_t L_abs(int32_t x) */
  svar32_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_abs(svar32_b);
  fprintf(ref_file, "L_abs(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);

  svar32_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_abs(svar32_b);
  fprintf(ref_file, "L_abs(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);

  svar32_b = 0x80000000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_abs(svar32_b);
  fprintf(ref_file, "L_abs(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);


  /* L_sat */
  /* int32_t L_sat(int32_t x) */
  svar32_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_sat(svar32_b);
  fprintf(ref_file, "L_sat(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);

  svar32_b = -1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_sat(svar32_b);
  fprintf(ref_file, "L_sat(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);

  svar32_b = -32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_sat(svar32_b);
  fprintf(ref_file, "L_sat(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);

  svar32_b = 32768;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar32_a = L_sat(svar32_b);
  fprintf(ref_file, "L_sat(%#x) = %#x overflow %d carry %d\n", svar32_b, svar32_a, Overflow, Carry);


  /* div_s */
  /* int16_t div_s(int16_t x, int16_t y) */
  svar16_a = 1;
  svar16_b = 1;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = div_s(svar16_a, svar16_b);
  fprintf(ref_file, "div_s(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

  svar16_a = 10000;
  svar16_b = 20000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = div_s(svar16_a, svar16_b);
  fprintf(ref_file, "div_s(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);


  svar16_a = 10000;
  svar16_b = 20000;
  Overflow = init_Overflow;
  Carry = init_Carry;
  svar16_c = div_s(svar16_a, svar16_b);
  fprintf(ref_file, "div_s(%#x, %#x) = %#x overflow %d carry %d\n", svar16_a, svar16_b, svar16_c, Overflow, Carry);

}

void exec_dspfns(void)
{
  Overflow = 0;

  exec_dspfns1(0, 0);
  exec_dspfns1(0, 1);
  exec_dspfns1(1, 0);
  exec_dspfns1(1, 1);
}
