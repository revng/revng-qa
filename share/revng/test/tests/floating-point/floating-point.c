/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

typedef float float32_t;
typedef double float64_t;

_Static_assert(sizeof(float32_t) == 4, "float32_t must be 4 bytes wide");
_Static_assert(sizeof(float64_t) == 8, "float64_t must be 8 bytes wide");

#define WEAK __attribute__((weak))

/*
 * The operands and results are globals: the compiler cannot constant-fold the
 * floating-point work away, so the softfloat calls reliably emerge in the
 * lifted IR. The matching `CHECK` directives live in the `.filecheck` file in
 * the `revng` repository.
 */
float64_t float64_a, float64_b, float64_result;
float32_t float32_a, float32_b, float32_result;
int32_t int32_a, int32_result;
int64_t int64_a, int64_result;

WEAK void add_float64(void) {
  float64_result = float64_a + float64_b;
}

WEAK void add_float32(void) {
  float32_result = float32_a + float32_b;
}

WEAK void mul_float64(void) {
  float64_result = float64_a * float64_b;
}

WEAK void mul_float32(void) {
  float32_result = float32_a * float32_b;
}

WEAK void div_float64(void) {
  float64_result = float64_a / float64_b;
}

WEAK void div_float32(void) {
  float32_result = float32_a / float32_b;
}

WEAK void compare_float64(void) {
  int32_result = float64_a > float64_b;
}

WEAK void compare_float32(void) {
  int32_result = float32_a > float32_b;
}

WEAK void convert_int32_t_to_float64(void) {
  float64_result = int32_a;
}

WEAK void convert_float64_to_int32_t(void) {
  int32_result = float64_a;
}

WEAK void convert_int32_t_to_float32(void) {
  float32_result = int32_a;
}

WEAK void convert_float32_to_int32_t(void) {
  int32_result = float32_a;
}

/*
 * On 32-bit architectures, the toolchain lowers 64-bit-integer <-> floating
 * point conversions through libgcc helpers, which then use the underlying
 * 32-bit primitives, which are already tested.
 * Their `CHECK-64` directives in the `.filecheck` file are gated on the same
 * architectures.
 */
#if defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8

WEAK void convert_int64_t_to_float64(void) {
  float64_result = int64_a;
}

WEAK void convert_float64_to_int64_t(void) {
  int64_result = float64_a;
}

WEAK void convert_int64_t_to_float32(void) {
  float32_result = int64_a;
}

WEAK void convert_float32_to_int64_t(void) {
  int64_result = float32_a;
}

#endif

void _start(void) {
  add_float64();
  add_float32();
  mul_float64();
  mul_float32();
  div_float64();
  div_float32();
  compare_float64();
  compare_float32();
  convert_int32_t_to_float64();
  convert_float64_to_int32_t();
  convert_int32_t_to_float32();
  convert_float32_to_int32_t();
#if defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8
  convert_int64_t_to_float64();
  convert_float64_to_int64_t();
  convert_int64_t_to_float32();
  convert_float32_to_int64_t();
#endif
}
