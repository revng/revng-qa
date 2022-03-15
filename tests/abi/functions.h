#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "for_each.h"

/*
 * Helper macros
 */

#ifndef ABIDEF
#define ABIDEF
#endif

#define STRINGIFY_HELPER(X) #X
#define STRINGIFY(X) STRINGIFY_HELPER(X)

#define CONCATENATE_HELPER(LHS, RHS) LHS##RHS
#define CONCATENATE(LHS, RHS) CONCATENATE_HELPER(LHS, RHS)

#define CONCATENATE_THREE_HELPER(F, S, T) F##S##T
#define CONCATENATE_THREE(F, S, T) CONCATENATE_THREE_HELPER(F, S, T)

#define DO_NOTHING_HELPER(LHS, RHS) LHS RHS
#define DO_NOTHING(LHS, RHS) DO_NOTHING_HELPER(LHS, RHS)

static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                || __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__,
              "Unsupported endianness (pdp? something crazy? Memory "
              "corruption?)");

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define PRINT_FUNCTION_HEADER(FUNCTION_NAME)            \
  puts("  Function: \"" STRINGIFY(FUNCTION_NAME) "\"\n" \
       "  IsLittleEndian: yes");
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define PRINT_FUNCTION_HEADER(FUNCTION_NAME)            \
  puts("  Function: \"" STRINGIFY(FUNCTION_NAME) "\"\n" \
       "  IsLittleEndian: no");
#endif

#define UNROLL_ARGUMENT(LHS, RHS) DO_NOTHING(LHS, argument_##RHS)

size_t runtime_endianness_check() {
  size_t i = 1;
  return !*((char *) &i);
}
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define PRINT_BYTES(VARIABLE_NAME, BYTE_COUNT)           \
  for (size_t i = BYTE_COUNT - 1; i != (size_t) -1; --i) \
    printf("%.2x", VARIABLE_NAME[i]);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define PRINT_BYTES(VARIABLE_NAME, BYTE_COUNT) \
  for (size_t i = 0; i < BYTE_COUNT; ++i)      \
    printf("%.2x", VARIABLE_NAME[i]);
#endif

static_assert(sizeof(uint8_t) == 1, "A type with size == 1 is required.");
#define PRINT_VALUE(TYPE, VALUE)         \
  do {                                   \
    typedef union {                      \
      TYPE v;                            \
      uint8_t a[sizeof(TYPE)];           \
    } printing_helper;                   \
                                         \
    printing_helper helper;              \
    helper.v = VALUE;                    \
                                         \
    PRINT_BYTES(helper.a, sizeof(TYPE)); \
  } while (0);

#define PRINT_VARIABLE(TYPE, VALUE)                                        \
  do {                                                                     \
    printf("    - Type: " #TYPE "\n      Address: 0x%0x\n      Value: 0x", \
           &VALUE);                                                        \
    PRINT_VALUE(TYPE, VALUE)                                               \
    printf("\n");                                                          \
  } while (0);

#define UNROLL_ARGUMENTS(...) \
  COMMA_SEPARATED_INDEXED_FOR_EACH(UNROLL_ARGUMENT, __VA_ARGS__)

#define PRINT_ARGUMENT(TYPE, INDEX) PRINT_VARIABLE(TYPE, argument_##INDEX)
#define PRINT_ARGUMENTS(...) INDEXED_FOR_EACH(PRINT_ARGUMENT, __VA_ARGS__)

#define NOINLINE_WEAK __attribute__((noinline, weak))
#define ARGUMENT_TEST_FUNCTION(NAME, ...)                         \
  void NOINLINE_WEAK ABIDEF NAME(UNROLL_ARGUMENTS(__VA_ARGS__)) { \
    PRINT_FUNCTION_HEADER(NAME)                                   \
    puts("  Arguments:");                                         \
    PRINT_ARGUMENTS(__VA_ARGS__)                                  \
    puts("  ReturnValue:");                                       \
    puts("");                                                     \
  }

ABIDEF void *set_return_value_up(void);

/* Helps avoiding compiler warnings */
void *typeless_function = (void *) set_return_value_up;
#define GET_SETUP_FUNCTION(TYPE) ((TYPE ABIDEF(*)(void)) typeless_function)

jmp_buf jump_buffer;
#define RETURN_VALUE_TEST_FUNCTION(NAME, TYPE)        \
  TYPE __attribute__((noinline)) NAME(void) {         \
    if (setjmp(jump_buffer) == 0) {                   \
      TYPE return_value = GET_SETUP_FUNCTION(TYPE)(); \
      puts("  ReturnValue:");                         \
      PRINT_VARIABLE(TYPE, return_value);             \
      PRINT_FUNCTION_HEADER(NAME);                    \
      puts("  Arguments:");                           \
      puts("");                                       \
                                                      \
      longjmp(jump_buffer, 1);                        \
    }                                                 \
  }

/*
 * Helper structs
 */
typedef struct {
  uint16_t a;
  uint16_t b;
} small_struct;

typedef struct {
  uint8_t a;
  uint16_t c;
} padded_struct;

typedef struct {
  uint32_t a[3];
} array_struct;

typedef struct {
  uint32_t a[3];
  uint64_t b[3];
} two_arrays_struct;

typedef struct {
  uint64_t b[8];
} big_struct;

typedef struct {
  uint64_t b[32];
} huge_struct;

/*
 * Argument passing test functions
 */

ARGUMENT_TEST_FUNCTION(single_8_bit_argument, uint8_t);
ARGUMENT_TEST_FUNCTION(single_16_bit_argument, int16_t);
ARGUMENT_TEST_FUNCTION(single_32_bit_argument, uint32_t);
ARGUMENT_TEST_FUNCTION(single_64_bit_argument, int64_t);
ARGUMENT_TEST_FUNCTION(single_128_bit_argument, __uint128_t);
ARGUMENT_TEST_FUNCTION(single_small_struct_argument, small_struct);
ARGUMENT_TEST_FUNCTION(single_padded_struct_argument, padded_struct);
ARGUMENT_TEST_FUNCTION(single_array_struct_argument, array_struct);
ARGUMENT_TEST_FUNCTION(single_two_arrays_struct_argument, two_arrays_struct);
ARGUMENT_TEST_FUNCTION(single_big_struct_argument, big_struct);
ARGUMENT_TEST_FUNCTION(single_huge_struct_argument, huge_struct);
ARGUMENT_TEST_FUNCTION(single_void_pointer_argument, void *);
ARGUMENT_TEST_FUNCTION(single_int_pointer_argument, uint32_t *);
ARGUMENT_TEST_FUNCTION(single_struct_pointer_argument, huge_struct *);

// TODO: Floating point!

ARGUMENT_TEST_FUNCTION(mixed_integer_arguments,
                       int64_t,
                       uint32_t,
                       int16_t,
                       uint8_t,
                       __int128_t);
ARGUMENT_TEST_FUNCTION(mixed_int_struct_arguments,
                       int64_t,
                       padded_struct,
                       int16_t,
                       big_struct,
                       __int128_t);
ARGUMENT_TEST_FUNCTION(mixed_int_struct_pointer_arguments,
                       padded_struct *,
                       padded_struct,
                       __int128_t *,
                       huge_struct,
                       void *);

ARGUMENT_TEST_FUNCTION(six_register_test,
                       array_struct,
                       __int128_t,
                       array_struct);

ARGUMENT_TEST_FUNCTION(multiple_stack_arguments,
                       big_struct,
                       big_struct,
                       big_struct);

ARGUMENT_TEST_FUNCTION(lots_of_arguments,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t,
                       uint8_t);

ARGUMENT_TEST_FUNCTION(equivalence_test_0,
                       small_struct *,
                       small_struct,
                       uint64_t);

ARGUMENT_TEST_FUNCTION(equivalence_test_64_bit_1, uint64_t, uint32_t, uint64_t);
ARGUMENT_TEST_FUNCTION(equivalence_test_64_bit_2,
                       uint64_t,
                       uint16_t,
                       uint16_t,
                       uint64_t);

ARGUMENT_TEST_FUNCTION(equivalence_test_32_bit_1, uint32_t, uint32_t, uint64_t);
ARGUMENT_TEST_FUNCTION(equivalence_test_32_bit_2,
                       uint32_t,
                       uint16_t,
                       uint16_t,
                       uint64_t);

/*
 * Value returning test functions
 */

RETURN_VALUE_TEST_FUNCTION(single_8_bit_return_value, uint8_t);
RETURN_VALUE_TEST_FUNCTION(single_16_bit_return_value, int16_t);
RETURN_VALUE_TEST_FUNCTION(single_32_bit_return_value, uint32_t);
RETURN_VALUE_TEST_FUNCTION(single_64_bit_return_value, int64_t);
RETURN_VALUE_TEST_FUNCTION(single_128_bit_return_value, __uint128_t);

RETURN_VALUE_TEST_FUNCTION(single_small_struct_return_value, small_struct);
RETURN_VALUE_TEST_FUNCTION(single_padded_struct_return_value, padded_struct);

RETURN_VALUE_TEST_FUNCTION(single_void_pointer_return_value, void *);
RETURN_VALUE_TEST_FUNCTION(single_int_pointer_return_value, uint32_t *);
RETURN_VALUE_TEST_FUNCTION(single_struct_pointer_return_value, huge_struct *);

// TODO: Big structs by value!
// TODO: Floating point!

/*
 * ENABLED TEST LISTS
 */

/* clang-format off */
#define ARGUMENT_FUNCTION_LIST single_8_bit_argument,              \
                               single_16_bit_argument,             \
                               single_32_bit_argument,             \
                               single_64_bit_argument,             \
                               single_128_bit_argument,            \
                               single_small_struct_argument,       \
                               single_padded_struct_argument,      \
                               single_array_struct_argument,       \
                               single_two_arrays_struct_argument,  \
                               single_big_struct_argument,         \
                               single_huge_struct_argument,        \
                               single_void_pointer_argument,       \
                               single_int_pointer_argument,        \
                               single_struct_pointer_argument,     \
                               mixed_integer_arguments,            \
                               mixed_int_struct_arguments,         \
                               mixed_int_struct_pointer_arguments, \
                               six_register_test,                  \
                               multiple_stack_arguments,           \
                               lots_of_arguments,                  \
                               equivalence_test_0,                 \
                               equivalence_test_64_bit_1,          \
                               equivalence_test_64_bit_2,          \
                               equivalence_test_32_bit_1,          \
                               equivalence_test_32_bit_2

#define RETURN_VALUE_FUNCTION_LIST single_8_bit_return_value,             \
                                   single_16_bit_return_value,            \
                                   single_32_bit_return_value,            \
                                   single_64_bit_return_value,            \
                                   single_128_bit_return_value,           \
                                   single_void_pointer_return_value,      \
                                   single_small_struct_return_value,      \
                                   single_padded_struct_return_value,     \
                                   single_int_pointer_return_value,       \
                                   single_struct_pointer_return_value

/* clang-format on */
