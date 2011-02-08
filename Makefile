# Copyright (c) 2009, 2010, 2011 STMicroelectronics
# Written by Christophe Lyon

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# ARM RVCT
CC.rvct := armcc
CFLAGS.rvct = -g --cpu=cortex-a9 -Ono_special_regs_postregalloc -I.
LD.rvct := armlink
LDFLAGS.rvct := --cpu=cortex-a9 --entry 0x2000

# GCC/ARM cross compiler
CC.gccarm := arm-none-eabi-gcc
CFLAGS.gccarm := -g -Wall -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=neon -fshort-wchar -Wno-unused-variable -Wno-unused-function
LD.gccarm := armlink
LDFLAGS.gccarm := --cpu=cortex-a9 --entry 0x2000

# List of validated intrinsics
REFNAMES = vld1 vadd vld1_lane vld1_dup vdup vget_high vget_low		\
	   vqdmlal_lane vqdmlsl_lane vext vshrn_n vset_lane vget_lane	\
	   vqsub vqdmulh_lane vqdmull vqdmlal vqdmlsl vceq vcge vcle	\
	   vcgt vclt vbsl vshl vldX vdup_lane vrshrn_n vqdmull_lane	\
	   vst1_lane vqshl vqshl_n vqrshrn_n vsub vqadd vabs vqabs	\
	   vcombine vmax vmin vneg vqneg vmlal vmlal_lane vmlsl		\
	   vmlsl_lane vmovl vmovn vmull vmull_lane vrev vrshl vshl_n	\
	   vshr_n vsra_n vtrn vuzp vzip vreinterpret vqdmulh vqrdmulh	\
	   vqrdmulh_lane vqrshl vaba vabal vabd vabdl vand vorr vorn	\
	   veor vbic vcreate vldX_lane vldX_dup vmla vmls vmul		\
	   vmul_lane vmul_n vmull_n vqdmulh_n vqdmull_n vqrdmulh_n	\
	   vmla_lane vmls_lane vmla_n vmls_n vmlal_n vmlsl_n vqdmlal_n	\
	   vqdmlsl_n vsri_n vsli_n vtst vaddhn vraddhn vaddl vaddw	\
	   vhadd vrhadd vhsub vsubl vsubw vsubhn vrsubhn vmvn vqmovn	\
	   vqmovun vrshr_n vrsra_n vshll_n vpaddl vpadd vpadal		\
	   vqshlu_n vclz vcls vcnt vqshrn_n vpmax vpmin vqshrun_n	\
	   vqrshrun_n vstX_lane vtbX vrecpe vrsqrte integer vcage	\
	   vcagt vcale vcalt vrecps vrsqrts vcvt dsp dspfns
REFLIST = $(addprefix ref_, $(REFNAMES))

all: ref-rvct.qemu

check:
	diff stm-arm-neon.refrvct ref-rvct.txt

# Building reference files with RVCT
REFOBJS.rvct = $(addsuffix .rvct.o, $(REFLIST))
REFRVCT=stm-arm-neon.refrvct
ref-rvct: $(REFRVCT)
ref-rvct.qemu: $(REFRVCT).qemu

$(REFRVCT): compute_ref.axf
	rvdebug -stdiolog=stdio.log -jou=journal.log -log=log.log -nologo -cmd -init @coretile.core.cpu0@RTSM -inc armscript.inc -exec $^

$(REFRVCT).qemu: compute_ref.axf
	qemu-system-arm -cpu cortex-a9 -semihosting -nographic -kernel $^
# Avoid rebuilding compute_ref.axf if already present, for users who
# don't have rvct
.INTERMEDIATE: compute_ref.rvct.o retarget.rvct.o InitCache.o Init.o	\
	$(REFOBJS.rvct)

compute_ref.axf: scatter.scat compute_ref.rvct.o retarget.rvct.o	\
	InitCache.o Init.o $(REFOBJS.rvct)
	$(LD.rvct) $(LDFLAGS.rvct) --scatter $^ -o $@

compute_ref.rvct.o retarget.rvct.o: %.rvct.o: %.c
	$(CC.rvct) $(CFLAGS.rvct) -c $^ -o $@ -DREFFILE=\"$(REFRVCT)\"

ref_%.rvct.o: ref_%.c stm-arm-neon-ref.h
	$(CC.rvct) $(CFLAGS.rvct) -c $< -o $@

InitCache.o Init.o: %.o: %.s
	$(CC.rvct) $(CFLAGS.rvct) -c $^ -o $@


# Building reference files with GCC/ARM
REFOBJS.gccarm = $(addsuffix .gccarm.o, $(REFLIST))
REFGCCARM=stm-arm-neon.gccarm
ref-gccarm: $(REFGCCARM)

$(REFGCCARM): compute_ref.gccarm
	rvdebug -stdiolog=stdio.log -jou=journal.log -log=log.log -nologo -cmd -init @coretile.core.cpu0@RTSM -inc armscript.inc -exec $^

compute_ref.gccarm: scatter.scat compute_ref.gccarm.o retarget.rvct.o	\
	InitCache.o Init.o $(REFOBJS.gccarm)
	$(LD.rvct) $(LDFLAG.rvct) --scatter $^ -o $@

compute_ref.gccarm.o: %.gccarm.o: %.c
	$(CC.gccarm) $(CFLAGS.gccarm) -c $^ -o $@ -DREFFILE=\"$(REFGCCARM)\"

ref_%.gccarm.o: ref_%.c stm-arm-neon-ref.h
	$(CC.gccarm) $(CFLAGS.gccarm) -c $< -o $@

# Use '*' rather than '%' in these rules:
# - using '%' does not make them add to the implicit rules above (they
#   are different rules, only the 1st one matches)
# - they are needed only when the target already exists, so the
#   wildcard matches when needed.
# - if the target does not already exist, the implicit rules apply.
ref_vadd.*.o ref_vsub.*.o ref_vand.*.o ref_vbic.*.o ref_veor.*.o ref_vorn.*.o ref_vorr.*.o: ref_v_binary_op.c
ref_vqadd.*.o ref_vqsub.*.o: ref_v_binary_sat_op.c
ref_vabs.*.o ref_vneg.*.o  ref_vmvn.*.o: ref_v_unary_op.c
ref_vqabs.*.o ref_vqneg.*.o: ref_v_unary_sat_op.c
ref_vceq.*.o ref_vcge.*.o ref_vcle.*.o ref_vcgt.*.o ref_vclt.*.o: ref_v_comp_op.c
ref_vhadd.*.o ref_vrhadd.*.o ref_vhsub.*.o ref_vmin.*.o: ref_vmax.c
ref_vmls.*.o: ref_vmla.c
ref_vmls_lane.*.o: ref_vmla_lane.c
ref_vmls_n.*.o: ref_vmla_n.c
ref_vmlsl.*.o: ref_vmlal.c
ref_vmlsl_lane.*.o: ref_vmlal_lane.c
ref_vmlsl_n.*.o: ref_vmlal_n.c
ref_vqdmlsl.*.o: ref_vqdmlal.c
ref_vqdmlsl_lane.*.o: ref_vqdmlal_lane.c
ref_vqdmlsl_n.*.o: ref_vqdmlal_n.c
ref_vtrn.*.o ref_vzip.*.o: ref_vuzp.c
ref_vsli_n.*.o ref_vsri_n.*.o: ref_vsXi_n.c
ref_vraddhn.*.o ref_vsubhn.*.o ref_vrsubhn.*.o: ref_vaddhn.c
ref_vsubl.*.o: ref_vaddl.c
ref_vsubw.*.o: ref_vaddw.c
ref_vcage.*.o ref_vcale.*.o ref_vcagt.*.o ref_vcalt.*.o: ref_v_comp_f_op.c

clean:
	rm -f *.o *.log stm-arm-neon.refrvct
