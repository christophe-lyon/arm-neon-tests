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

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#if defined(__cplusplus)
#include <cstdint>
#else
#if defined(_MSC_VER)
#include "msstdint.h"
#else
#include <stdint.h>
#endif
#endif

#ifndef INSN_NAME
#define INSN_NAME vaddhn
#define TEST_MSG "VADDHN"
#endif

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* Basic test: vec64=vaddhn(vec128_a, vec128_b), then store the result.  */
#define TEST_VADDHN1(INSN, T1, T2, W, W2, N)				\
  VECT_VAR(vector64, T1, W2, N) = INSN##_##T2##W(VECT_VAR(vector1, T1, W, N), \
						 VECT_VAR(vector2, T1, W, N)); \
  vst1_##T2##W2(VECT_VAR(result, T1, W2, N), VECT_VAR(vector64, T1, W2, N))

#define TEST_VADDHN(INSN, T1, T2, W, W2, N)				\
  TEST_VADDHN1(INSN, T1, T2, W, W2, N)

  /* With ARM RVCT, we need to declare variables before any executable
     statement */
  DECL_VARIABLE_64BITS_VARIANTS(vector64);
  DECL_VARIABLE_128BITS_VARIANTS(vector1);
  DECL_VARIABLE_128BITS_VARIANTS(vector2);

  clean_results ();

  /* Fill input vector1 and vector2 with arbitrary values */
  VDUP(vector1, q, int, s, 16, 8, 50*(UINT8_MAX+1));
  VDUP(vector1, q, int, s, 32, 4, 50*(UINT16_MAX+1));
  VDUP(vector1, q, int, s, 64, 2, 24*((uint64_t)UINT32_MAX+1));
  VDUP(vector1, q, uint, u, 16, 8, 3*(UINT8_MAX+1));
  VDUP(vector1, q, uint, u, 32, 4, 55*(UINT16_MAX+1));
  VDUP(vector1, q, uint, u, 64, 2, 3*((uint64_t)UINT32_MAX+1));

  VDUP(vector2, q, int, s, 16, 8, (uint16_t)UINT8_MAX);
  VDUP(vector2, q, int, s, 32, 4, (uint32_t)UINT16_MAX);
  VDUP(vector2, q, int, s, 64, 2, (uint64_t)UINT32_MAX);
  VDUP(vector2, q, uint, u, 16, 8, (uint16_t)UINT8_MAX);
  VDUP(vector2, q, uint, u, 32, 4, (uint32_t)UINT16_MAX);
  VDUP(vector2, q, uint, u, 64, 2, (uint64_t)UINT32_MAX);

  TEST_VADDHN(INSN_NAME, int, s, 16, 8, 8);
  TEST_VADDHN(INSN_NAME, int, s, 32, 16, 4);
  TEST_VADDHN(INSN_NAME, int, s, 64, 32, 2);
  TEST_VADDHN(INSN_NAME, uint, u, 16, 8, 8);
  TEST_VADDHN(INSN_NAME, uint, u, 32, 16, 4);
  TEST_VADDHN(INSN_NAME, uint, u, 64, 32, 2);

  dump_results_hex (TEST_MSG);
}
