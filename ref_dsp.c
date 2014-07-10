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
#include <armdsp.h>
#include <dspfns.h> /* For Overflow */
#else
#include "stm-armdsp.h"
#include "stm-dspfns.h" /* For Overflow */
#endif

extern FILE* ref_file;

void exec_dsp (void)
{
  int32_t svar1, svar2, sacc, sres;
  int32_t lo, hi;


  fprintf(ref_file, "\n\nDSP (non-NEON) intrinsics\n");

  /* qadd */
  /* int32_t qadd(int32_t val1, int32_t val2); */
  svar1 = 1;
  svar2 = 2;
  Overflow = 0;
  sres = qadd(svar1, svar2);
  fprintf(ref_file, "qadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = -2;
  Overflow = 0;
  sres = qadd(svar1, svar2);
  fprintf(ref_file, "qadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = 2;
  Overflow = 0;
  sres = qadd(svar1, svar2);
  fprintf(ref_file, "qadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x7000;
  svar2 = 0x7000;
  Overflow = 0;
  sres = qadd(svar1, svar2);
  fprintf(ref_file, "qadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFF;
  svar2 = 0x8FFF;
  Overflow = 0;
  sres = qadd(svar1, svar2);
  fprintf(ref_file, "qadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x70000000;
  svar2 = 0x70000000;
  Overflow = 0;
  sres = qadd(svar1, svar2);
  fprintf(ref_file, "qadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFFFFFF;
  svar2 = 0x8FFFFFFF;
  Overflow = 0;
  sres = qadd(svar1, svar2);
  fprintf(ref_file, "qadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  /* qsub */
  /* int32_t qsub(int32_t val1, int32_t val2); */
  svar1 = 1;
  svar2 = 2;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = -2;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = 2;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x7000;
  svar2 = 0xFFFF9000;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFF;
  svar2 = 0xFFFF7001;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x70000000;
  svar2 = 0x90000000;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFFFFFF;
  svar2 = 0x70000001;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0;
  svar2 = 0x80000000;
  Overflow = 0;
  sres = qsub(svar1, svar2);
  fprintf(ref_file, "qsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);


  /* qdadd */
  /* int32_t qdadd(int32_t val1, int32_t val2); */
  svar1 = 1;
  svar2 = 2;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = -2;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = 2;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x7000;
  svar2 = 0x7000;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFF;
  svar2 = 0x8FFF;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x70000000;
  svar2 = 0x70000000;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0;
  svar2 = 0x70000000;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFFFFFF;
  svar2 = 0x8FFFFFFF;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0;
  svar2 = 0x8FFFFFFF;
  Overflow = 0;
  sres = qdadd(svar1, svar2);
  fprintf(ref_file, "qdadd(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  /* qdsub */
  /* int32_t qdsub(int32_t val1, int32_t val2); */
  svar1 = 1;
  svar2 = 2;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = -2;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = -1;
  svar2 = 2;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x7000;
  svar2 = 0xFFFF9000;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFF;
  svar2 = 0xFFFF7001;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x70000000;
  svar2 = 0x90000000;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0;
  svar2 = 0x90000000;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0x8FFFFFFF;
  svar2 = 0x70000001;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);

  svar1 = 0;
  svar2 = 0x70000001;
  Overflow = 0;
  sres = qdsub(svar1, svar2);
  fprintf(ref_file, "qdsub(%#x, %#x) = %#x sat %d\n", svar1, svar2, sres, Overflow);


  /* smulbb, smulbt, smultb, smultt */
  /* int32_t smulbb(int32_t val1, int32_t val2); */
  svar1 = 0x12345678;
  svar2 = 0x12345678;
  sres = smulbb(svar1, svar2);
  fprintf(ref_file, "smulbb(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smulbt(svar1, svar2);
  fprintf(ref_file, "smulbt(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smultb(svar1, svar2);
  fprintf(ref_file, "smultb(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smultt(svar1, svar2);
  fprintf(ref_file, "smultt(%#x, %#x) = %#x\n", svar1, svar2, sres);

  svar1 = 0xF123F456;
  svar2 = 0xF123F456;
  sres = smulbb(svar1, svar2);
  fprintf(ref_file, "smulbb(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smulbt(svar1, svar2);
  fprintf(ref_file, "smulbt(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smultb(svar1, svar2);
  fprintf(ref_file, "smultb(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smultt(svar1, svar2);
  fprintf(ref_file, "smultt(%#x, %#x) = %#x\n", svar1, svar2, sres);


  /* smlabb, smlabt, smlatb, smlatt */
  /* int32_t smlabb(int32_t val1, int32_t val2, int32_t acc); */
  sacc = 0x01020304;
  svar1 = 0x12345678;
  svar2 = 0x12345678;
  sres = smlabb(svar1, svar2, sacc);
  fprintf(ref_file, "smlabb(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlabt(svar1, svar2, sacc);
  fprintf(ref_file, "smlabt(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlatb(svar1, svar2, sacc);
  fprintf(ref_file, "smlatb(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlatt(svar1, svar2, sacc);
  fprintf(ref_file, "smlatt(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);

  svar1 = 0xF123F456;
  svar2 = 0xF123F456;
  sres = smlabb(svar1, svar2, sacc);
  fprintf(ref_file, "smlabb(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlabt(svar1, svar2, sacc);
  fprintf(ref_file, "smlabt(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlatb(svar1, svar2, sacc);
  fprintf(ref_file, "smlatb(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlatt(svar1, svar2, sacc);
  fprintf(ref_file, "smlatt(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);


  /* smlalbb, smlalbt, smlaltb, smlaltt */
  /* int32_t smlalbb(int32_t *lo, int32_t *hi, int32_t val1, int32_t val2); */
  svar1 = 0x12345678;
  svar2 = 0x12345678;
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlalbb(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlalbb(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlalbt(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlalbt(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlaltb(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlaltb(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlaltt(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlaltt(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);

  svar1 = 0xF123F456;
  svar2 = 0xF123F456;
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlalbb(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlalbb(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlalbt(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlalbt(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlaltb(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlaltb(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0x9ABCDEF0;
  fprintf(ref_file, "smlaltt(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlaltt(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);

  svar1 = 0x7FFF7FFF;
  svar2 = 0x7FFF7FFF;
  hi = 0x12345678;
  lo = 0xFFFFFFFF;
  fprintf(ref_file, "smlalbb(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlalbb(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0xFFFFFFFF;
  fprintf(ref_file, "smlalbt(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlalbt(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0xFFFFFFFF;
  fprintf(ref_file, "smlaltb(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlaltb(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);
  hi = 0x12345678;
  lo = 0xFFFFFFFF;
  fprintf(ref_file, "smlaltt(&%#x, &%#x, %#x, %#x) = ", lo, hi, svar1, svar2);
  smlaltt(&lo, &hi, svar1, svar2);
  fprintf(ref_file, "%#x%#x\n", hi, lo);


  /* smulwb, smulwt */
  /* int32_t smulwb(int32_t val1, int32_t val2); */
  svar1 = 0x12345678;
  svar2 = 0x12345678;
  sres = smulwb(svar1, svar2);
  fprintf(ref_file, "smulwb(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smulwt(svar1, svar2);
  fprintf(ref_file, "smulwt(%#x, %#x) = %#x\n", svar1, svar2, sres);

  svar1 = 0xF123F456;
  svar2 = 0xF123F456;
  sres = smulwb(svar1, svar2);
  fprintf(ref_file, "smulwb(%#x, %#x) = %#x\n", svar1, svar2, sres);
  sres = smulwt(svar1, svar2);
  fprintf(ref_file, "smulwt(%#x, %#x) = %#x\n", svar1, svar2, sres);


  /* smlawb, smlawt */
  /* int32_t smlawb(int32_t val1, int32_t val2, int32_t acc); */
  sacc = 0x01020304;
  svar1 = 0x12345678;
  svar2 = 0x12345678;
  sres = smlawb(svar1, svar2, sacc);
  fprintf(ref_file, "smlawb(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlawt(svar1, svar2, sacc);
  fprintf(ref_file, "smlawt(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);

  svar1 = 0xF123F456;
  svar2 = 0xF123F456;
  sres = smlawb(svar1, svar2, sacc);
  fprintf(ref_file, "smlawb(%#x, %#x, %#x) = %#x\n", svar1, svar2, sacc, sres);
  sres = smlawt(svar1, svar2, sacc);
  fprintf(ref_file, "smlawt(%#x, %#x, %#X) = %#x\n", svar1, svar2, sacc, sres);

}
