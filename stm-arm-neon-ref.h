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
#if defined(_MSC_VER)
#include "msinttypes.h"
#include <float.h> /* for isnan() ... */
static int32_t _ptrNan[]={0x7fc00000L};
#define NAN (*(float*)_ptrNan)
static int32_t _ptrInf[]={0x7f800000L};
#define INFINITY (*(float*)_ptrInf)
#define HUGE_VALF INFINITY
#else
#include <inttypes.h>
#endif
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

/* This one is used for padding between input buffers.  */
#define PAD(V, T, W, N) char VECT_VAR(V,T,W,N)=42;

/* Array declarations.  */
#define ARRAY(V, T, W, N) VECT_VAR_DECL(V,T,W,N)[N]
#define ARRAY4(V, T, W, N) VECT_VAR_DECL(V,T,W,N)[4]

/* Arrays of vectors.  */
#define VECT_ARRAY_VAR(V,T,W,N,L) xNAME(V,VECT_ARRAY_NAME(T,W,N,L))
#define VECT_ARRAY(V, T, W, N, L) T##W##_t VECT_ARRAY_VAR(V,T,W,N,L)[N*L]

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
      fprintf(ref_file, "%" FMT " %.7a %.7g, ", tmp.i, tmp.f, tmp.f);	\
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

/* Input buffers, one of each size */
extern ARRAY(buffer, int, 8, 8);
extern ARRAY(buffer, int, 16, 4);
extern ARRAY(buffer, int, 32, 2);
extern ARRAY(buffer, int, 64, 1);
extern ARRAY(buffer, uint, 8, 8);
extern ARRAY(buffer, uint, 16, 4);
extern ARRAY(buffer, uint, 32, 2);
extern ARRAY(buffer, uint, 64, 1);
extern ARRAY(buffer, float, 32, 2);
extern ARRAY(buffer, int, 8, 16);
extern ARRAY(buffer, int, 16, 8);
extern ARRAY(buffer, int, 32, 4);
extern ARRAY(buffer, int, 64, 2);
extern ARRAY(buffer, uint, 8, 16);
extern ARRAY(buffer, uint, 16, 8);
extern ARRAY(buffer, uint, 32, 4);
extern ARRAY(buffer, uint, 64, 2);
extern ARRAY(buffer, float, 32, 4);

/* The tests for vld1_dup and vdup expect at least 4 entries in the
   input buffer, so force 1- and 2-elements initializers to have 4
   entries.  */
extern ARRAY(buffer_dup, int, 8, 8);
extern ARRAY(buffer_dup, int, 16, 4);
extern ARRAY4(buffer_dup, int, 32, 2);
extern ARRAY4(buffer_dup, int, 64, 1);
extern ARRAY(buffer_dup, uint, 8, 8);
extern ARRAY(buffer_dup, uint, 16, 4);
extern ARRAY4(buffer_dup, uint, 32, 2);
extern ARRAY4(buffer_dup, uint, 64, 1);
extern ARRAY4(buffer_dup, float, 32, 2);
extern ARRAY(buffer_dup, int, 8, 16);
extern ARRAY(buffer_dup, int, 16, 8);
extern ARRAY(buffer_dup, int, 32, 4);
extern ARRAY4(buffer_dup, int, 64, 2);
extern ARRAY(buffer_dup, uint, 8, 16);
extern ARRAY(buffer_dup, uint, 16, 8);
extern ARRAY(buffer_dup, uint, 32, 4);
extern ARRAY4(buffer_dup, uint, 64, 2);
extern ARRAY(buffer_dup, float, 32, 4);

/* Input buffers for vld2, one of each size */
extern VECT_ARRAY(buffer_vld2, int, 8, 8, 2);
extern VECT_ARRAY(buffer_vld2, int, 16, 4, 2);
extern VECT_ARRAY(buffer_vld2, int, 32, 2, 2);
extern VECT_ARRAY(buffer_vld2, int, 64, 1, 2);
extern VECT_ARRAY(buffer_vld2, uint, 8, 8, 2);
extern VECT_ARRAY(buffer_vld2, uint, 16, 4, 2);
extern VECT_ARRAY(buffer_vld2, uint, 32, 2, 2);
extern VECT_ARRAY(buffer_vld2, uint, 64, 1, 2);
extern VECT_ARRAY(buffer_vld2, float, 32, 2, 2);
extern VECT_ARRAY(buffer_vld2, int, 8, 16, 2);
extern VECT_ARRAY(buffer_vld2, int, 16, 8, 2);
extern VECT_ARRAY(buffer_vld2, int, 32, 4, 2);
extern VECT_ARRAY(buffer_vld2, int, 64, 2, 2);
extern VECT_ARRAY(buffer_vld2, uint, 8, 16, 2);
extern VECT_ARRAY(buffer_vld2, uint, 16, 8, 2);
extern VECT_ARRAY(buffer_vld2, uint, 32, 4, 2);
extern VECT_ARRAY(buffer_vld2, uint, 64, 2, 2);
extern VECT_ARRAY(buffer_vld2, float, 32, 4, 2);

/* Input buffers for vld3, one of each size */
extern VECT_ARRAY(buffer_vld3, int, 8, 8, 3);
extern VECT_ARRAY(buffer_vld3, int, 16, 4, 3);
extern VECT_ARRAY(buffer_vld3, int, 32, 2, 3);
extern VECT_ARRAY(buffer_vld3, int, 64, 1, 3);
extern VECT_ARRAY(buffer_vld3, uint, 8, 8, 3);
extern VECT_ARRAY(buffer_vld3, uint, 16, 4, 3);
extern VECT_ARRAY(buffer_vld3, uint, 32, 2, 3);
extern VECT_ARRAY(buffer_vld3, uint, 64, 1, 3);
extern VECT_ARRAY(buffer_vld3, float, 32, 2, 3);
extern VECT_ARRAY(buffer_vld3, int, 8, 16, 3);
extern VECT_ARRAY(buffer_vld3, int, 16, 8, 3);
extern VECT_ARRAY(buffer_vld3, int, 32, 4, 3);
extern VECT_ARRAY(buffer_vld3, int, 64, 2, 3);
extern VECT_ARRAY(buffer_vld3, uint, 8, 16, 3);
extern VECT_ARRAY(buffer_vld3, uint, 16, 8, 3);
extern VECT_ARRAY(buffer_vld3, uint, 32, 4, 3);
extern VECT_ARRAY(buffer_vld3, uint, 64, 2, 3);
extern VECT_ARRAY(buffer_vld3, float, 32, 4, 3);

/* Input buffers for vld4, one of each size */
extern VECT_ARRAY(buffer_vld4, int, 8, 8, 4);
extern VECT_ARRAY(buffer_vld4, int, 16, 4, 4);
extern VECT_ARRAY(buffer_vld4, int, 32, 2, 4);
extern VECT_ARRAY(buffer_vld4, int, 64, 1, 4);
extern VECT_ARRAY(buffer_vld4, uint, 8, 8, 4);
extern VECT_ARRAY(buffer_vld4, uint, 16, 4, 4);
extern VECT_ARRAY(buffer_vld4, uint, 32, 2, 4);
extern VECT_ARRAY(buffer_vld4, uint, 64, 1, 4);
extern VECT_ARRAY(buffer_vld4, float, 32, 2, 4);
extern VECT_ARRAY(buffer_vld4, int, 8, 16, 4);
extern VECT_ARRAY(buffer_vld4, int, 16, 8, 4);
extern VECT_ARRAY(buffer_vld4, int, 32, 4, 4);
extern VECT_ARRAY(buffer_vld4, int, 64, 2, 4);
extern VECT_ARRAY(buffer_vld4, uint, 8, 16, 4);
extern VECT_ARRAY(buffer_vld4, uint, 16, 8, 4);
extern VECT_ARRAY(buffer_vld4, uint, 32, 4, 4);
extern VECT_ARRAY(buffer_vld4, uint, 64, 2, 4);
extern VECT_ARRAY(buffer_vld4, float, 32, 4, 4);

/* Input buffers for vld2_lane */
extern VECT_VAR_DECL(buffer_vld2_lane, int, 8, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, int, 16, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, int, 32, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, int, 64, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, uint, 8, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, uint, 16, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, uint, 32, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, uint, 64, 2)[2];
extern VECT_VAR_DECL(buffer_vld2_lane, float, 32, 2)[2];

/* Input buffers for vld3_lane */
extern VECT_VAR_DECL(buffer_vld3_lane, int, 8, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, int, 16, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, int, 32, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, int, 64, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, uint, 8, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, uint, 16, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, uint, 32, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, uint, 64, 3)[3];
extern VECT_VAR_DECL(buffer_vld3_lane, float, 32, 3)[3];

/* Input buffers for vld4_lane */
extern VECT_VAR_DECL(buffer_vld4_lane, int, 8, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, int, 16, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, int, 32, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, int, 64, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, uint, 8, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, uint, 16, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, uint, 32, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, uint, 64, 4)[4];
extern VECT_VAR_DECL(buffer_vld4_lane, float, 32, 4)[4];

/* Output buffers, one of each size */
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

/* This hack is to cope with various compilers/libc which may not
   provide endian.h or cross-compilers such as llvm which includes the
   host's endian.h.  */
#ifndef __arm__
#include <endian.h>
#define THIS_ENDIAN __BYTE_ORDER
#else /* __arm__ */
#ifdef __ARMEL__
#define THIS_ENDIAN __LITTLE_ENDIAN
#else /* __ARMEL__ */
#define THIS_ENDIAN __BIG_ENDIAN
#endif
#endif /* __arm__ */

#if THIS_ENDIAN == __LITTLE_ENDIAN

typedef union {
  struct {
    int _xxx:27;
    unsigned int QC:1;
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
    unsigned int QC:1;
    int _dnm:27;
  } b;
  unsigned int word;
} _ARM_FPSCR;

#endif /* __BIG_ENDIAN */

#ifdef __ARMCC_VERSION
register _ARM_FPSCR _afpscr_for_qc __asm("fpscr");
#define Neon_Overflow _afpscr_for_qc.b.QC
#define Set_Neon_Overflow(x)  {Neon_Overflow = (x);}
#else
/* GCC/ARM does not know this register */
#define Neon_Overflow  __read_neon_overflow()
static int __read_neon_overflow() {
    _ARM_FPSCR _afpscr_for_qc;
    asm("vmrs %0,fpscr" : "=r" (_afpscr_for_qc));
    return _afpscr_for_qc.b.QC;
}
#define Set_Neon_Overflow(x)  __set_neon_overflow((x))
static void __set_neon_overflow(int x) {
    _ARM_FPSCR _afpscr_for_qc;
    asm("vmrs %0,fpscr" : "=r" (_afpscr_for_qc));
    _afpscr_for_qc.b.QC = x;
    asm("vmsr fpscr,%0" : : "r" (_afpscr_for_qc));
    return;
}

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
