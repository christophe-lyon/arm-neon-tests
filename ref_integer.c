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

#ifndef __arm__
#include "stm-arm.h"
#include "stm-dspfns.h" /* For Overflow */
#else
#include <dspfns.h> /* For Overflow */
#endif

extern FILE* ref_file;

void exec_integer (void)
{
  int i;
  uint32_t uvar, ures;
  int32_t svar1, svar2, sres;

  uint8_t clz;

  fprintf(ref_file, "\n\nInteger (non-NEON) intrinsics\n");

  /* __clz */
  /* uint8_t __clz(uint32_t val); */
  uvar = 0xFFFFFFFF;
  for(i=0; i<=32; i++) {
    clz = __clz(uvar);
    fprintf(ref_file, "__clz(%#x) = %d\n", (unsigned int)uvar, clz);
    uvar >>= 1;
  }

  /* __qadd */
  /* int32_t __qadd(int32_t val1, int32_t val2); */
  svar1 = 1;
  svar2 = 2;
  Overflow = 0;
  sres = __qadd(svar1, svar2);
  fprintf(ref_file, "__qadd(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = -1;
  svar2 = -2;
  Overflow = 0;
  sres = __qadd(svar1, svar2);
  fprintf(ref_file, "__qadd(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = -1;
  svar2 = 2;
  Overflow = 0;
  sres = __qadd(svar1, svar2);
  fprintf(ref_file, "__qadd(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x7000;
  svar2 = 0x7000;
  Overflow = 0;
  sres = __qadd(svar1, svar2);
  fprintf(ref_file, "__qadd(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x8FFF;
  svar2 = 0x8FFF;
  Overflow = 0;
  sres = __qadd(svar1, svar2);
  fprintf(ref_file, "__qadd(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x70000000;
  svar2 = 0x70000000;
  Overflow = 0;
  sres = __qadd(svar1, svar2);
  fprintf(ref_file, "__qadd(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x8FFFFFFF;
  svar2 = 0x8FFFFFFF;
  Overflow = 0;
  sres = __qadd(svar1, svar2);
  fprintf(ref_file, "__qadd(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  /* __qdbl */
  /* int32_t __qdbl(int32_t val); */
  svar1 = 1;
  Overflow = 0;
  sres = __qdbl(svar1);
  fprintf(ref_file, "__qdbl(%#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x70000000;
  Overflow = 0;
  sres = __qdbl(svar1);
  fprintf(ref_file, "__qdbl(%#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x8FFFFFFF;
  Overflow = 0;
  sres = __qdbl(svar1);
  fprintf(ref_file, "__qdbl(%#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0xEFFFFFFF;
  Overflow = 0;
  sres = __qdbl(svar1);
  fprintf(ref_file, "__qdbl(%#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  /* __qsub */
  /* int32_t __qsub(int32_t val1, int32_t val2); */
  svar1 = 1;
  svar2 = 2;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = -1;
  svar2 = -2;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = -1;
  svar2 = 2;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x7000;
  svar2 = 0xFFFF9000;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x8FFF;
  svar2 = 0xFFFF7001;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x70000000;
  svar2 = 0x90000000;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0x8FFFFFFF;
  svar2 = 0x70000001;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  svar1 = 0;
  svar2 = 0x80000000;
  Overflow = 0;
  sres = __qsub(svar1, svar2);
  fprintf(ref_file, "__qsub(%#x, %#x) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)svar2, (unsigned int)sres, Overflow);

  /* __rbit */
  /* uint32_t __rbit(uint32_t val); */
  uvar = 0x12345678;
  ures = __rbit(uvar);
  fprintf(ref_file, "__rbit(%#x) = %#x\n",
          (unsigned int)uvar, (unsigned int)ures);

  /* __rev */
  /* uint32_t __rev(uint32_t val); */
  uvar = 0x12345678;
  ures = __rev(uvar);
  fprintf(ref_file, "__rev(%#x) = %#x\n",
          (unsigned int)uvar, (unsigned int)ures);

  /* __ssat */
  /* int32_t __ssat(int32_t val, uint32_t sat); */
  svar1 = 0x12345678;
  Overflow = 0;
  sres = __ssat(svar1, 30);
  fprintf(ref_file, "__ssat(%#x, 30) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x12345678;
  Overflow = 0;
  sres = __ssat(svar1, 19);
  fprintf(ref_file, "__ssat(%#x, 19) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __ssat(svar1, 29);
  fprintf(ref_file, "__ssat(%#x, 29) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __ssat(svar1, 12);
  fprintf(ref_file, "__ssat(%#x, 12) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __ssat(svar1, 32);
  fprintf(ref_file, "__ssat(%#x, 32) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __ssat(svar1, 1);
  fprintf(ref_file, "__ssat(%#x, 1) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  /* __usat */
  /* int32_t __usat(uint32_t val, uint32_t sat); */
  svar1 = 0x12345678;
  Overflow = 0;
  sres = __usat(svar1, 30);
  fprintf(ref_file, "__usat(%#x, 30) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x12345678;
  Overflow = 0;
  sres = __usat(svar1, 19);
  fprintf(ref_file, "__usat(%#x, 19) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __usat(svar1, 29);
  fprintf(ref_file, "__usat(%#x, 29) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __usat(svar1, 12);
  fprintf(ref_file, "__usat(%#x, 12) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __usat(svar1, 31);
  fprintf(ref_file, "__usat(%#x, 31) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);

  svar1 = 0x87654321;
  Overflow = 0;
  sres = __usat(svar1, 0);
  fprintf(ref_file, "__usat(%#x, 0) = %#x sat %d\n",
          (unsigned int)svar1, (unsigned int)sres, Overflow);
}
