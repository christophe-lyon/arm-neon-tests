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

#ifndef _STM_ARM_NEON_REF_H_
#define _STM_ARM_NEON_REF_H_

#if defined(__cplusplus)
#include <cstdio>
#include <cinttypes>
#include <cstring>
#else
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#endif

#define xSTR(X) #X
#define STR(X) xSTR(X)

#define xNAME1(V,T) V ## _ ##  T
#define xNAME(V,T) xNAME1(V,T)

#define VAR(V,T,W) xNAME(V,T##W)
#define VAR_DECL(V, T, W) T##W##_t VAR(V,T,W)

#define VECT_NAME(T, W, N) T##W##x##N
#define VECT_ARRAY_NAME(T, W, N, L) T##W##x##N##x##L
#define VECT_TYPE(T, W, N) xNAME(VECT_NAME(T,W,N),t)
#define VECT_ARRAY_TYPE(T, W, N, L) xNAME(VECT_ARRAY_NAME(T,W,N,L),t)

#define VECT_VAR(V,T,W,N) xNAME(V,VECT_NAME(T,W,N))
#define VECT_VAR_DECL(V, T, W, N) T##W##_t VECT_VAR(V,T,W,N)
#define VECT_VAR_DECL_INIT(V, T, W, N) T##W##_t VECT_VAR(V,T,W,N) INIT_TAB(T##W##_t)
#define ARRAY(V, T, W, N) VECT_VAR_DECL(V,T,W,N)[N]

#define VECT_ARRAY_VAR(V,T,W,N,L) xNAME(V,VECT_ARRAY_NAME(T,W,N,L))

static int result_idx = 0;
#define DUMP(MSG,T,W,N,FMT)						\
  fprintf(ref_file, "%s:%d:%s [] = { ", MSG, result_idx++,		\
	  STR(VECT_VAR(result, T, W, N)));				\
  for(i=0; i<N ; i++)							\
    {									\
      fprintf(ref_file, "%" FMT ", ", VECT_VAR(result, T, W, N)[i]);	\
    }									\
  fprintf(ref_file, " }\n");

#define DUMP_FP(MSG,T,W,N,FMT)						\
  fprintf(ref_file, "%s:%d:%s [] = { ", MSG, result_idx++,		\
	  STR(VECT_VAR(result, T, W, N)));				\
  for(i=0; i<N ; i++)							\
    {									\
      union fp_operand {						\
	uint##W##_t i;							\
	float##W##_t f;							\
      } tmp;								\
      tmp.f = VECT_VAR(result, T, W, N)[i];				\
      fprintf(ref_file, "%" FMT " %a %g, ", tmp.i, tmp.f, tmp.f);	\
    }									\
  fprintf(ref_file, " }\n");

#define CLEAN_PATTERN_8  0x33
#define CLEAN_PATTERN_16 0x3333
#define CLEAN_PATTERN_32 0x33333333
#define CLEAN_PATTERN_64 0x3333333333333333

#define CLEAN(VAR,T,W,N)						\
  memset(VECT_VAR(VAR, T, W, N),					\
	 CLEAN_PATTERN_8,						\
	 sizeof(VECT_VAR(VAR, T, W, N)));

#define CHECK_INIT(VAR,Q,T1,T2,W,N)					\
  {									\
    ARRAY(check_result, T1, W, N);					\
    int i;								\
									\
    vst1##Q##_##T2##W(VECT_VAR(check_result, T1, W, N),			\
		      VECT_VAR(VAR, T1, W, N));				\
    for(i=0; i<N ; i++)							\
      {									\
	/*if (VECT_VAR(check_result, T1, W, N)[i] == CLEAN_PATTERN_##W)*/ { \
	  fprintf(stdout, "%s:%d: %s[%d] unintialized! %#x\n",		\
		  __FUNCTION__,	__LINE__,				\
		  STR(VECT_VAR(VAR, T1, W, N)), i,			\
		  VECT_VAR(check_result, T1, W, N)[i]);			\
	}								\
      }									\
  }

/* Generic declarations: */
extern FILE* log_file;
extern FILE* ref_file;

/* Sample initialization vectors */
#define INIT_TAB(T) [] = { (T)-16, (T)-15, (T)-14, (T)-13, (T)-12, (T)-11, \
			   (T)-10, (T)-9, (T)-8, (T)-7, (T)-6, (T)-5, (T)-4, \
			   (T)-3, (T)-2, (T)-1, (T)0, (T)1, (T)2, (T)3, (T)4, \
			   (T)5, (T)6, (T)7, (T)8, (T)9, (T)10, (T)11, (T)12, \
			   (T)13, (T)14, (T)15 }

/* Input buffers, 1 of each size */
static VECT_VAR_DECL_INIT(buffer, int, 8, 8);
static VECT_VAR_DECL_INIT(buffer, int, 16, 4);
static VECT_VAR_DECL_INIT(buffer, int, 32, 2);
static VECT_VAR_DECL_INIT(buffer, int, 64, 1);
static VECT_VAR_DECL_INIT(buffer, uint, 8, 8);
static VECT_VAR_DECL_INIT(buffer, uint, 16, 4);
static VECT_VAR_DECL_INIT(buffer, uint, 32, 2);
static VECT_VAR_DECL_INIT(buffer, uint, 64, 1);
static VECT_VAR_DECL_INIT(buffer, float, 32, 2);
static VECT_VAR_DECL_INIT(buffer, int, 8, 16);
static VECT_VAR_DECL_INIT(buffer, int, 16, 8);
static VECT_VAR_DECL_INIT(buffer, int, 32, 4);
static VECT_VAR_DECL_INIT(buffer, int, 64, 2);
static VECT_VAR_DECL_INIT(buffer, uint, 8, 16);
static VECT_VAR_DECL_INIT(buffer, uint, 16, 8);
static VECT_VAR_DECL_INIT(buffer, uint, 32, 4);
static VECT_VAR_DECL_INIT(buffer, uint, 64, 2);
static VECT_VAR_DECL_INIT(buffer, float, 32, 4);

/* Output buffers, 1 of each size */
static ARRAY(result, int, 8, 8);
static ARRAY(result, int, 16, 4);
static ARRAY(result, int, 32, 2);
static ARRAY(result, int, 64, 1);
static ARRAY(result, uint, 8, 8);
static ARRAY(result, uint, 16, 4);
static ARRAY(result, uint, 32, 2);
static ARRAY(result, uint, 64, 1);
static ARRAY(result, float, 32, 2);
static ARRAY(result, int, 8, 16);
static ARRAY(result, int, 16, 8);
static ARRAY(result, int, 32, 4);
static ARRAY(result, int, 64, 2);
static ARRAY(result, uint, 8, 16);
static ARRAY(result, uint, 16, 8);
static ARRAY(result, uint, 32, 4);
static ARRAY(result, uint, 64, 2);
static ARRAY(result, float, 32, 4);

/* Dump results (generic function) */
static void dump_results (char *test_name)
{
  int i;

  fprintf(ref_file, "\n%s output:\n", test_name);

  DUMP(test_name, int, 8, 8, PRId8);
  DUMP(test_name, int, 16, 4, PRId16);
  DUMP(test_name, int, 32, 2, PRId32);
  DUMP(test_name, int, 64, 1, PRId64);
  DUMP(test_name, uint, 8, 8, PRIu8);
  DUMP(test_name, uint, 16, 4, PRIu16);
  DUMP(test_name, uint, 32, 2, PRIu32);
  DUMP(test_name, uint, 64, 1, PRIu64);
  DUMP_FP(test_name, float, 32, 2, PRIx32);

  DUMP(test_name, int, 8, 16, PRId8);
  DUMP(test_name, int, 16, 8, PRId16);
  DUMP(test_name, int, 32, 4, PRId32);
  DUMP(test_name, int, 64, 2, PRId64);
  DUMP(test_name, uint, 8, 16, PRIu8);
  DUMP(test_name, uint, 16, 8, PRIu16);
  DUMP(test_name, uint, 32, 4, PRIu32);
  DUMP(test_name, uint, 64, 2, PRIu64);
  DUMP_FP(test_name, float, 32, 4, PRIx32);
}

/* Dump results in hex (generic function) */
static void dump_results_hex2 (const char *test_name, const char* comment)
{
  int i;

  fprintf(ref_file, "\n%s%s output:\n", test_name, comment);

  DUMP(test_name, int, 8, 8, PRIx8);
  DUMP(test_name, int, 16, 4, PRIx16);
  DUMP(test_name, int, 32, 2, PRIx32);
  DUMP(test_name, int, 64, 1, PRIx64);
  DUMP(test_name, uint, 8, 8, PRIx8);
  DUMP(test_name, uint, 16, 4, PRIx16);
  DUMP(test_name, uint, 32, 2, PRIx32);
  DUMP(test_name, uint, 64, 1, PRIx64);
  DUMP_FP(test_name, float, 32, 2, PRIx32);

  DUMP(test_name, int, 8, 16, PRIx8);
  DUMP(test_name, int, 16, 8, PRIx16);
  DUMP(test_name, int, 32, 4, PRIx32);
  DUMP(test_name, int, 64, 2, PRIx64);
  DUMP(test_name, uint, 8, 16, PRIx8);
  DUMP(test_name, uint, 16, 8, PRIx16);
  DUMP(test_name, uint, 32, 4, PRIx32);
  DUMP(test_name, uint, 64, 2, PRIx64);
  DUMP_FP(test_name, float, 32, 4, PRIx32);
}

static void dump_results_hex (const char *test_name)
{
  dump_results_hex2(test_name, "");
}

#ifndef STM_ARM_NEON_MODELS

#ifndef __BIG_ENDIAN

typedef union {
  struct {
    int _xxx:27;
    int QC:1;
    int V:1;
    int C:1;
    int Z:1;
    int N:1;
  } b;
  unsigned int word;
} _ARM_FPSCR;

#else /* __BIG_ENDIAN */

typedef union {
  struct {
    int N:1;
    int Z:1;
    int C:1;
    int V:1;
    int QC:1;
    int _dnm:27;
  } b;
  unsigned int word;
} _ARM_FPSCR;

#endif /* __BIG_ENDIAN */

#ifdef __ARMCC_VERSION
register _ARM_FPSCR _afpscr_for_qc __asm("fpscr");
#define Neon_Overflow _afpscr_for_qc.b.QC
#else
/* Fake declaration because GCC/ARM does not know this register */
extern int errno;
#define Neon_Overflow errno
#endif

#endif /* STM_ARM_NEON_MODELS */

static void dump_neon_overflow(const char* msg, const char *name)
{
  fprintf(ref_file, "%s:%d:%s Neon overflow %d\n", msg, result_idx++,
	  name, Neon_Overflow);
}

/* Clean output buffers before execution */
static void clean_results (void)
{
  result_idx = 0;
  CLEAN(result, int, 8, 8);
  CLEAN(result, int, 16, 4);
  CLEAN(result, int, 32, 2);
  CLEAN(result, int, 64, 1);
  CLEAN(result, uint, 8, 8);
  CLEAN(result, uint, 16, 4);
  CLEAN(result, uint, 32, 2);
  CLEAN(result, uint, 64, 1);
  CLEAN(result, float, 32, 2);

  CLEAN(result, int, 8, 16);
  CLEAN(result, int, 16, 8);
  CLEAN(result, int, 32, 4);
  CLEAN(result, int, 64, 2);
  CLEAN(result, uint, 8, 16);
  CLEAN(result, uint, 16, 8);
  CLEAN(result, uint, 32, 4);
  CLEAN(result, uint, 64, 2);
  CLEAN(result, float, 32, 4);
}


/* Helpers to declare variables of various types  */
#define DECL_VARIABLE(VAR, T1, W, N)		\
  VECT_TYPE(T1, W, N) VECT_VAR(VAR, T1, W, N)

#define DECL_VARIABLE_64BITS_SIGNED_VARIANTS(VAR)	\
  DECL_VARIABLE(VAR, int, 8, 8);			\
  DECL_VARIABLE(VAR, int, 16, 4);			\
  DECL_VARIABLE(VAR, int, 32, 2);			\
  DECL_VARIABLE(VAR, int, 64, 1)

#define DECL_VARIABLE_64BITS_UNSIGNED_VARIANTS(VAR)	\
  DECL_VARIABLE(VAR, uint, 8, 8);			\
  DECL_VARIABLE(VAR, uint, 16, 4);			\
  DECL_VARIABLE(VAR, uint, 32, 2);			\
  DECL_VARIABLE(VAR, uint, 64, 1)

#define DECL_VARIABLE_128BITS_SIGNED_VARIANTS(VAR)	\
  DECL_VARIABLE(VAR, int, 8, 16);			\
  DECL_VARIABLE(VAR, int, 16, 8);			\
  DECL_VARIABLE(VAR, int, 32, 4);			\
  DECL_VARIABLE(VAR, int, 64, 2)

#define DECL_VARIABLE_128BITS_UNSIGNED_VARIANTS(VAR)	\
  DECL_VARIABLE(VAR, uint, 8, 16);			\
  DECL_VARIABLE(VAR, uint, 16, 8);			\
  DECL_VARIABLE(VAR, uint, 32, 4);			\
  DECL_VARIABLE(VAR, uint, 64, 2)

#define DECL_VARIABLE_64BITS_VARIANTS(VAR)	\
  DECL_VARIABLE_64BITS_SIGNED_VARIANTS(VAR);	\
  DECL_VARIABLE_64BITS_UNSIGNED_VARIANTS(VAR);	\
  DECL_VARIABLE(VAR, float, 32, 2)

#define DECL_VARIABLE_128BITS_VARIANTS(VAR)	\
  DECL_VARIABLE_128BITS_SIGNED_VARIANTS(VAR);	\
  DECL_VARIABLE_128BITS_UNSIGNED_VARIANTS(VAR);	\
  DECL_VARIABLE(VAR, float, 32, 4)

#define DECL_VARIABLE_ALL_VARIANTS(VAR)		\
  DECL_VARIABLE_64BITS_VARIANTS(VAR);		\
  DECL_VARIABLE_128BITS_VARIANTS(VAR)

#define DECL_VARIABLE_SIGNED_VARIANTS(VAR)	\
  DECL_VARIABLE_64BITS_SIGNED_VARIANTS(VAR);	\
  DECL_VARIABLE_128BITS_SIGNED_VARIANTS(VAR)

#define DECL_VARIABLE_UNSIGNED_VARIANTS(VAR)	\
  DECL_VARIABLE_64BITS_UNSIGNED_VARIANTS(VAR);	\
  DECL_VARIABLE_128BITS_UNSIGNED_VARIANTS(VAR)

/* Helpers to initialize vectors */
#define TEST_VDUP(VAR, Q, T1, T2, W, N, V)		\
  VECT_VAR(VAR, T1, W, N) = vdup##Q##_n_##T2##W(V)

#define TEST_VSET_LANE(VAR, Q, T1, T2, W, N, L, V)			\
  VECT_VAR(VAR, T1, W, N) = vset##Q##_lane_##T2##W(V,			\
						   VECT_VAR(VAR, T1, W, N), \
						   L)

/* We need to load initial values first, so rely on VLD1 */
#define TEST_VLOAD(VAR, BUF, Q, T1, T2, W, N)				\
  VECT_VAR(VAR, T1, W, N) = vld1##Q##_##T2##W(VECT_VAR(BUF, T1, W, N))

/* Helpers for macros with 1 constant and 5 variable arguments */
#define TEST_MACRO_64BITS_SIGNED_VARIANTS_1_5(MACRO, VAR)	\
  MACRO(VAR, , int, s, 8, 8);					\
  MACRO(VAR, , int, s, 16, 4);					\
  MACRO(VAR, , int, s, 32, 2);					\
  MACRO(VAR, , int, s, 64, 1)

#define TEST_MACRO_64BITS_UNSIGNED_VARIANTS_1_5(MACRO, VAR)	\
  MACRO(VAR, , uint, u, 8, 8);					\
  MACRO(VAR, , uint, u, 16, 4);					\
  MACRO(VAR, , uint, u, 32, 2);					\
  MACRO(VAR, , uint, u, 64, 1)

#define TEST_MACRO_128BITS_SIGNED_VARIANTS_1_5(MACRO, VAR)	\
  MACRO(VAR, q, int, s, 8, 16);					\
  MACRO(VAR, q, int, s, 16, 8);					\
  MACRO(VAR, q, int, s, 32, 4);					\
  MACRO(VAR, q, int, s, 64, 2)

#define TEST_MACRO_128BITS_UNSIGNED_VARIANTS_1_5(MACRO,VAR)	\
  MACRO(VAR, q, uint, u, 8, 16);				\
  MACRO(VAR, q, uint, u, 16, 8);				\
  MACRO(VAR, q, uint, u, 32, 4);				\
  MACRO(VAR, q, uint, u, 64, 2)

#define TEST_MACRO_64BITS_VARIANTS_1_5(MACRO, VAR)	\
  TEST_MACRO_64BITS_SIGNED_VARIANTS_1_5(MACRO, VAR);	\
  TEST_MACRO_64BITS_UNSIGNED_VARIANTS_1_5(MACRO, VAR)

#define TEST_MACRO_128BITS_VARIANTS_1_5(MACRO, VAR)	\
  TEST_MACRO_128BITS_SIGNED_VARIANTS_1_5(MACRO, VAR);	\
  TEST_MACRO_128BITS_UNSIGNED_VARIANTS_1_5(MACRO, VAR)

#define TEST_MACRO_ALL_VARIANTS_1_5(MACRO, VAR)	\
  TEST_MACRO_64BITS_VARIANTS_1_5(MACRO, VAR);	\
  TEST_MACRO_128BITS_VARIANTS_1_5(MACRO, VAR)

#define TEST_MACRO_SIGNED_VARIANTS_1_5(MACRO, VAR)	\
  TEST_MACRO_64BITS_SIGNED_VARIANTS_1_5(MACRO, VAR);	\
  TEST_MACRO_128BITS_SIGNED_VARIANTS_1_5(MACRO, VAR)

/* Helpers for macros with 2 constant and 5 variable arguments */
#define TEST_MACRO_64BITS_SIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  MACRO(VAR1, VAR2, , int, s, 8, 8);					\
  MACRO(VAR1, VAR2, , int, s, 16, 4);					\
  MACRO(VAR1, VAR2, , int, s, 32, 2);					\
  MACRO(VAR1, VAR2 , , int, s, 64, 1)

#define TEST_MACRO_64BITS_UNSIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  MACRO(VAR1, VAR2, , uint, u, 8, 8);					\
  MACRO(VAR1, VAR2, , uint, u, 16, 4);					\
  MACRO(VAR1, VAR2, , uint, u, 32, 2);					\
  MACRO(VAR1, VAR2, , uint, u, 64, 1)

#define TEST_MACRO_128BITS_SIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  MACRO(VAR1, VAR2, q, int, s, 8, 16);					\
  MACRO(VAR1, VAR2, q, int, s, 16, 8);					\
  MACRO(VAR1, VAR2, q, int, s, 32, 4);					\
  MACRO(VAR1, VAR2, q, int, s, 64, 2)

#define TEST_MACRO_128BITS_UNSIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  MACRO(VAR1, VAR2, q, uint, u, 8, 16);					\
  MACRO(VAR1, VAR2, q, uint, u, 16, 8);					\
  MACRO(VAR1, VAR2, q, uint, u, 32, 4);					\
  MACRO(VAR1, VAR2, q, uint, u, 64, 2)

#define TEST_MACRO_64BITS_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  TEST_MACRO_64BITS_SIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2);	\
  TEST_MACRO_64BITS_UNSIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)

#define TEST_MACRO_128BITS_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  TEST_MACRO_128BITS_SIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2);	\
  TEST_MACRO_128BITS_UNSIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)

#define TEST_MACRO_ALL_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  TEST_MACRO_64BITS_VARIANTS_2_5(MACRO, VAR1, VAR2);	\
  TEST_MACRO_128BITS_VARIANTS_2_5(MACRO, VAR1, VAR2)

#define TEST_MACRO_SIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)	\
  TEST_MACRO_64BITS_SIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2);	\
  TEST_MACRO_128BITS_SIGNED_VARIANTS_2_5(MACRO, VAR1, VAR2)

#endif /* _STM_ARM_NEON_REF_H_ */
