/*

Copyright (c) 2009, 2010, 2011, 2012, 2013 STMicroelectronics
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

#define _AEABI_PORTABILITY_LEVEL 1
#undef _AEABI_PORTABLE
#if defined(__cplusplus)
#include <cstdio>
#include <cstdlib>
#else
#include <stdio.h>
#include <stdlib.h>
#endif

FILE* log_file = NULL;
FILE* ref_file = NULL;
FILE* gcc_tests_file = NULL;

#define LOGFILE "stm-arm-neon-ref.log"

void cleanup (void)
{
  if (log_file) fclose (log_file);
  if (ref_file) fclose (ref_file);
  if (gcc_tests_file) fclose (gcc_tests_file);
  exit (1);
}

extern void exec_vld1(void);
extern void exec_vadd(void);
extern void exec_vld1_lane(void);
extern void exec_vld1_dup(void);
extern void exec_vdup(void);
extern void exec_vget_high(void);
extern void exec_vget_low(void);
extern void exec_vqdmlal_lane(void);
extern void exec_vqdmlsl_lane(void);
extern void exec_vqdmlal_n(void);
extern void exec_vqdmlsl_n(void);
extern void exec_vext(void);
extern void exec_vshr_n(void);
extern void exec_vshrn_n(void);
extern void exec_vrshrn_n(void);
extern void exec_vqrshrn_n(void);
extern void exec_vset_lane(void);
extern void exec_vget_lane(void);
extern void exec_vqsub(void);
extern void exec_vqdmulh(void);
extern void exec_vqdmulh_lane(void);
extern void exec_vqdmulh_n(void);
extern void exec_vqdmull(void);
extern void exec_vqdmlal(void);
extern void exec_vqdmlsl(void);
extern void exec_vceq(void);
extern void exec_vcge(void);
extern void exec_vcle(void);
extern void exec_vcgt(void);
extern void exec_vclt(void);
extern void exec_vbsl(void);
extern void exec_vshl(void);
extern void exec_vqshl(void);
extern void exec_vqshl_n(void);
extern void exec_vrshl(void);
extern void exec_vshl_n(void);
extern void exec_vldX(void);
extern void exec_vdup_lane(void);
extern void exec_vqdmull_lane(void);
extern void exec_vqdmull_n(void);
extern void exec_vst1_lane(void);
extern void exec_vsub(void);
extern void exec_vqadd(void);
extern void exec_vabs(void);
extern void exec_vqabs(void);
extern void exec_vcombine(void);
extern void exec_vmax(void);
extern void exec_vmin(void);
extern void exec_vneg(void);
extern void exec_vqneg(void);
extern void exec_vmlal(void);
extern void exec_vmlal_lane(void);
extern void exec_vmlal_n(void);
extern void exec_vmlsl(void);
extern void exec_vmlsl_lane(void);
extern void exec_vmlsl_n(void);
extern void exec_vmovl(void);
extern void exec_vmovn(void);
extern void exec_vmull(void);
extern void exec_vmull_lane(void);
extern void exec_vrev(void);
extern void exec_vsra_n(void);
extern void exec_vtrn(void);
extern void exec_vuzp(void);
extern void exec_vzip(void);
extern void exec_vreinterpret(void);
extern void exec_vqrdmulh(void);
extern void exec_vqrdmulh_lane(void);
extern void exec_vqrdmulh_n(void);
extern void exec_vqrshl(void);
extern void exec_vaba(void);
extern void exec_vabal(void);
extern void exec_vabd(void);
extern void exec_vabdl(void);
extern void exec_vand(void);
extern void exec_vorr(void);
extern void exec_vorn(void);
extern void exec_veor(void);
extern void exec_vbic(void);
extern void exec_vcreate(void);
extern void exec_vldX_lane(void);
extern void exec_vldX_dup(void);
extern void exec_vmla(void);
extern void exec_vmls(void);
extern void exec_vmul(void);
extern void exec_vmul_lane(void);
extern void exec_vmul_n(void);
extern void exec_vmull_n(void);
extern void exec_vmla_lane(void);
extern void exec_vmls_lane(void);
extern void exec_vmla_n(void);
extern void exec_vmls_n(void);
extern void exec_vsli_n(void);
extern void exec_vsri_n(void);
extern void exec_vtst(void);
extern void exec_vaddhn(void);
extern void exec_vraddhn(void);
extern void exec_vaddl(void);
extern void exec_vaddw(void);
extern void exec_vhadd(void);
extern void exec_vrhadd(void);
extern void exec_vhsub(void);
extern void exec_vsubl(void);
extern void exec_vsubw(void);
extern void exec_vsubhn(void);
extern void exec_vrsubhn(void);
extern void exec_vmvn(void);
extern void exec_vqmovn(void);
extern void exec_vqmovun(void);
extern void exec_vrshr_n(void);
extern void exec_vrsra_n(void);
extern void exec_vshll_n(void);
extern void exec_vpaddl(void);
extern void exec_vpadd(void);
extern void exec_vpadal(void);
extern void exec_vqshlu_n(void);
extern void exec_vclz(void);
extern void exec_vcls(void);
extern void exec_vcnt(void);
extern void exec_vqshrn_n(void);
extern void exec_vpmax(void);
extern void exec_vpmin(void);
extern void exec_vqshrun_n(void);
extern void exec_vqrshrun_n(void);
extern void exec_vstX_lane(void);
extern void exec_vtbX(void);
extern void exec_vrecpe(void);
extern void exec_vrsqrte(void);

extern void exec_vcage(void);
extern void exec_vcagt(void);
extern void exec_vcale(void);
extern void exec_vcalt(void);
extern void exec_vcvt(void);
extern void exec_vrecps(void);
extern void exec_vrsqrts(void);

#if defined(__ARMCC_VERSION) || !defined(__arm__)
extern void exec_integer(void); /* Integer (non-NEON) intrinsics */
extern void exec_dsp(void); /* DSP (non-NEON) intrinsics */
extern void exec_dspfns(void); /* DSP FNS (non-NEON/ITU) intrinsics */
#endif

#include "compute_ref_data.c"

int main (void)
{
#if defined(_MSC_VER)
    /* When compiled with MSVC, force output of FP numbers with only 2
     * digits for the exponent, for easier comparison of the results
     * with GCC.  */
    _set_output_format(_TWO_DIGIT_EXPONENT);
#endif

  log_file = fopen (LOGFILE, "w");
  if (log_file == NULL) {
    fprintf (stderr, "Error opening log file " LOGFILE "\n");
    cleanup ();
  }

  ref_file = fopen (REFFILE, "w");
  if (ref_file == NULL) {
    fprintf (log_file, "Error opening ref file %s\n", REFFILE);
    cleanup ();
  }

  gcc_tests_file = fopen (GCCTESTS_FILE, "w");
  if (gcc_tests_file == NULL) {
    fprintf (log_file, "Error opening GCC ref file %s\n", GCCTESTS_FILE);
    cleanup ();
  }

  fprintf (log_file, "Computing refs....\n");

  exec_vld1 ();
  exec_vadd ();
  exec_vld1_lane ();
  exec_vld1_dup ();
  exec_vdup ();
  exec_vget_high ();
  exec_vget_low ();
  exec_vqdmlal_lane ();
  exec_vqdmlsl_lane ();
  exec_vqdmlal_n ();
  exec_vqdmlsl_n ();
  exec_vext ();
  exec_vshr_n ();
  exec_vshrn_n ();
  exec_vrshrn_n ();
  exec_vqrshrn_n ();
  exec_vset_lane ();
  exec_vget_lane ();
  exec_vqsub ();
  exec_vqdmulh ();
  exec_vqdmulh_lane ();
  exec_vqdmulh_n ();
  exec_vqdmull ();
  exec_vqdmlal ();
  exec_vqdmlsl ();
  exec_vceq ();
  exec_vcge ();
  exec_vcle ();
  exec_vcgt ();
  exec_vclt ();
  exec_vbsl ();
  exec_vshl ();
  exec_vshl_n ();
  exec_vqshl ();
  exec_vqshl_n ();
  exec_vrshl ();
  exec_vldX ();
  exec_vdup_lane ();
  exec_vqdmull_lane ();
  exec_vqdmull_n ();
  exec_vst1_lane ();
  exec_vsub ();
  exec_vqadd ();
  exec_vabs ();
  exec_vqabs ();
  exec_vcombine ();
  exec_vmax ();
  exec_vmin ();
  exec_vneg ();
  exec_vqneg ();
  exec_vmlal ();
  exec_vmlsl ();
  exec_vmlal_lane ();
  exec_vmlsl_lane ();
  exec_vmlal_n ();
  exec_vmlsl_n ();
  exec_vmovl ();
  exec_vmovn ();
  exec_vmull ();
  exec_vmull_lane ();
  exec_vrev ();
  exec_vsra_n ();
  exec_vtrn ();
  exec_vuzp ();
  exec_vzip ();
  exec_vreinterpret ();
  exec_vqrdmulh ();
  exec_vqrdmulh_lane ();
  exec_vqrdmulh_n ();
  exec_vqrshl ();
  exec_vaba ();
  exec_vabal ();
  exec_vabd ();
  exec_vabdl ();
  exec_vand ();
  exec_vorr ();
  exec_vorn ();
  exec_veor ();
  exec_vbic ();
  exec_vcreate ();
  exec_vldX_lane ();
  exec_vldX_dup ();
  exec_vmla ();
  exec_vmls ();
  exec_vmul ();
  exec_vmul_lane ();
  exec_vmul_n ();
  exec_vmull_n ();
  exec_vmla_lane ();
  exec_vmls_lane ();
  exec_vmla_n ();
  exec_vmls_n ();
  exec_vsli_n ();
  exec_vsri_n ();
  exec_vtst ();
  exec_vaddhn ();
  exec_vraddhn ();
  exec_vaddl ();
  exec_vaddw ();
  exec_vhadd ();
  exec_vrhadd ();
  exec_vhsub ();
  exec_vsubl ();
  exec_vsubw ();
  exec_vsubhn ();
  exec_vrsubhn ();
  exec_vmvn ();
  exec_vqmovn ();
  exec_vqmovun ();
  exec_vrshr_n ();
  exec_vrsra_n ();
  exec_vshll_n ();
  exec_vpaddl ();
  exec_vpadd ();
  exec_vpadal ();
  exec_vqshlu_n ();
  exec_vclz ();
  exec_vcls ();
  exec_vcnt ();
  exec_vqshrn_n ();
  exec_vpmax ();
  exec_vpmin ();
  exec_vqshrun_n ();
  exec_vqrshrun_n ();
  exec_vstX_lane ();
  exec_vtbX ();
  exec_vrecpe ();
  exec_vrsqrte ();

  exec_vcage ();
  exec_vcale ();
  exec_vcagt ();
  exec_vcalt ();
  exec_vcvt ();
  exec_vrecps ();
  exec_vrsqrts ();

#if defined(__ARMCC_VERSION) || (!defined(__arm__) && !defined(__aarch64__))
  exec_integer ();
  exec_dsp ();
  exec_dspfns ();
#endif

  fprintf (log_file, "Finished\n");

  return 0;
}
