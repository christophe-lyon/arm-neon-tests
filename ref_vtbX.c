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

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

void exec_vtbX (void)
{
  int i;

  /* In this case, input variables are arrays of vectors */
#define DECL_VTBX(T1, W, N, X)						\
  VECT_ARRAY_TYPE(T1, W, N, X) VECT_ARRAY_VAR(table_vector, T1, W, N, X)

  /* The vtbl1 variant is different from vtbl{2,3,4} because it takes a
     vector as 1st param, instead of an array of vectors */
#define TEST_VTBL1(T1, T2, T3, W, N)			\
  VECT_VAR(table_vector, T1, W, N) =			\
    vld1##_##T2##W((T1##W##_t *)lookup_table);		\
							\
  VECT_VAR(vector_res, T1, W, N) =			\
    vtbl1_##T2##W(VECT_VAR(table_vector, T1, W, N),	\
		  VECT_VAR(vector, T3, W, N));		\
  vst1_##T2##W(VECT_VAR(result, T1, W, N),		\
	       VECT_VAR(vector_res, T1, W, N));

#define TEST_VTBLX(T1, T2, T3, W, N, X)					\
  VECT_ARRAY_VAR(table_vector, T1, W, N, X) =				\
    vld##X##_##T2##W((T1##W##_t *)lookup_table);			\
									\
  VECT_VAR(vector_res, T1, W, N) =					\
    vtbl##X##_##T2##W(VECT_ARRAY_VAR(table_vector, T1, W, N, X),	\
		      VECT_VAR(vector, T3, W, N));			\
  vst1_##T2##W(VECT_VAR(result, T1, W, N),				\
	       VECT_VAR(vector_res, T1, W, N));

  /* With ARM RVCT, we need to declare variables before any executable
     statement */

  /* We need to define a lookup table */
  uint8_t lookup_table[32];

  DECL_VARIABLE(vector, int, 8, 8);
  DECL_VARIABLE(vector, uint, 8, 8);
  DECL_VARIABLE(vector, poly, 8, 8);
  DECL_VARIABLE(vector_res, int, 8, 8);
  DECL_VARIABLE(vector_res, uint, 8, 8);
  DECL_VARIABLE(vector_res, poly, 8, 8);

  /* For vtbl1 */
  DECL_VARIABLE(table_vector, int, 8, 8);
  DECL_VARIABLE(table_vector, uint, 8, 8);
  DECL_VARIABLE(table_vector, poly, 8, 8);

  /* For vtbx* */
  DECL_VARIABLE(default_vector, int, 8, 8);
  DECL_VARIABLE(default_vector, uint, 8, 8);
  DECL_VARIABLE(default_vector, poly, 8, 8);

  /* We need only 8 bits variants */
#define DECL_ALL_VTBLX(X)			\
  DECL_VTBX(int, 8, 8, X);			\
  DECL_VTBX(uint, 8, 8, X);			\
  DECL_VTBX(poly, 8, 8, X)

#define TEST_ALL_VTBL1()			\
  TEST_VTBL1(int, s, int, 8, 8);		\
  TEST_VTBL1(uint, u, uint, 8, 8);		\
  TEST_VTBL1(poly, p, uint, 8, 8)

#define TEST_ALL_VTBLX(X)			\
  TEST_VTBLX(int, s, int, 8, 8, X);		\
  TEST_VTBLX(uint, u, uint, 8, 8, X);		\
  TEST_VTBLX(poly, p, uint, 8, 8, X)

  /* Declare the temporary buffers / variables */
  DECL_ALL_VTBLX(2);
  DECL_ALL_VTBLX(3);
  DECL_ALL_VTBLX(4);

  /* Fill the lookup table */
  for (i=0; i<32; i++) {
    lookup_table[i] = i-15;
  }

  /* Choose init value arbitrarily, will be used as table index */
  VDUP(vector, , int, s, 8, 8, 1);
  VDUP(vector, , uint, u, 8, 8, 2);
  VDUP(vector, , poly, p, 8, 8, 2);

  /* To ensure code coverage of lib, add some indexes larger than 8,16 and 32 */
  /* except: lane 0 (by 6), lane 1 (by 8) and lane 2 (by 9) */
  TEST_VSET_LANE(vector, , int, s, 8, 8, 0, 10);
  TEST_VSET_LANE(vector, , int, s, 8, 8, 4, 20);
  TEST_VSET_LANE(vector, , int, s, 8, 8, 5, 40);
  TEST_VSET_LANE(vector, , uint, u, 8, 8, 0, 10);
  TEST_VSET_LANE(vector, , uint, u, 8, 8, 4, 20);
  TEST_VSET_LANE(vector, , uint, u, 8, 8, 5, 40);
  TEST_VSET_LANE(vector, , poly, p, 8, 8, 0, 10);
  TEST_VSET_LANE(vector, , poly, p, 8, 8, 4, 20);
  TEST_VSET_LANE(vector, , poly, p, 8, 8, 5, 40);


  /* Check vtbl1 */
  clean_results ();
#define TEST_MSG "VTBL1"
  TEST_ALL_VTBL1();
  dump_results_hex (TEST_MSG);

  /* Check vtbl2 */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VTBL2"
  TEST_ALL_VTBLX(2);
  dump_results_hex (TEST_MSG);

  /* Check vtbl3 */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VTBL3"
  TEST_ALL_VTBLX(3);
  dump_results_hex (TEST_MSG);

  /* Check vtbl4 */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VTBL4"
  TEST_ALL_VTBLX(4);
  dump_results_hex (TEST_MSG);


  /* Now test VTBX */

  /* The vtbx1 variant is different from vtbx{2,3,4} because it takes a
     vector as 1st param, instead of an array of vectors */
#define TEST_VTBX1(T1, T2, T3, W, N)			\
  VECT_VAR(table_vector, T1, W, N) =			\
    vld1##_##T2##W((T1##W##_t *)lookup_table);		\
							\
  VECT_VAR(vector_res, T1, W, N) =			\
    vtbx1_##T2##W(VECT_VAR(default_vector, T1, W, N),	\
		  VECT_VAR(table_vector, T1, W, N),	\
		  VECT_VAR(vector, T3, W, N));		\
  vst1_##T2##W(VECT_VAR(result, T1, W, N),		\
	       VECT_VAR(vector_res, T1, W, N));

#define TEST_VTBXX(T1, T2, T3, W, N, X)					\
  VECT_ARRAY_VAR(table_vector, T1, W, N, X) =				\
    vld##X##_##T2##W((T1##W##_t *)lookup_table);			\
									\
  VECT_VAR(vector_res, T1, W, N) =					\
    vtbx##X##_##T2##W(VECT_VAR(default_vector, T1, W, N),		\
		      VECT_ARRAY_VAR(table_vector, T1, W, N, X),	\
		      VECT_VAR(vector, T3, W, N));			\
  vst1_##T2##W(VECT_VAR(result, T1, W, N),				\
	       VECT_VAR(vector_res, T1, W, N));

#define TEST_ALL_VTBX1()			\
  TEST_VTBX1(int, s, int, 8, 8);		\
  TEST_VTBX1(uint, u, uint, 8, 8);		\
  TEST_VTBX1(poly, p, uint, 8, 8)

#define TEST_ALL_VTBXX(X)			\
  TEST_VTBXX(int, s, int, 8, 8, X);		\
  TEST_VTBXX(uint, u, uint, 8, 8, X);		\
  TEST_VTBXX(poly, p, uint, 8, 8, X)

  /* Choose init value arbitrarily, will be used as default value */
  VDUP(default_vector, , int, s, 8, 8, 0x33);
  VDUP(default_vector, , uint, u, 8, 8, 0xCC);
  VDUP(default_vector, , poly, p, 8, 8, 0xCC);

  /* Check vtbx1 */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VTBX1"
  TEST_ALL_VTBX1();
  dump_results_hex (TEST_MSG);

  /* Check vtbx2 */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VTBX2"
  TEST_ALL_VTBXX(2);
  dump_results_hex (TEST_MSG);

  /* Check vtbx3 */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VTBX3"
  TEST_ALL_VTBXX(3);
  dump_results_hex (TEST_MSG);

  /* Check vtbx4 */
  clean_results ();
#undef TEST_MSG
#define TEST_MSG "VTBX4"
  TEST_ALL_VTBXX(4);
  dump_results_hex (TEST_MSG);
}
