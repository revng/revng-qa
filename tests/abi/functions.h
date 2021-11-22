#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

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

#define PRINT_FUNCTION_NAME(FUNCTION_NAME) \
  puts("  Function: \"" STRINGIFY(FUNCTION_NAME) "\"");

#define PRINT_VALUE_IMPL(TYPE, MASK, ...) \
  do {                                    \
    printf("    - Type: " #TYPE "\n"      \
           "      Value: " MASK "\n",     \
           __VA_ARGS__);                  \
  } while (0)

#define MASK_8_BITS "0x%02x"
#define MASK_16_BITS "0x%04x"
#define MASK_32_BITS "0x%08x"
#define MASK_64_BITS "0x%016llx"
#define MASK_128_BITS "0x%016llx%016llx"
#define MASK_256_BITS "0x%016llx%016llx0x%016llx%016llx"

/* clang-format off */
#define VALUE_8_BITS(NAME) (uint8_t) NAME
#define VALUE_16_BITS(NAME) (uint16_t) NAME
#define VALUE_32_BITS(NAME) (uint32_t) NAME
#define VALUE_64_BITS(NAME) (uint64_t) NAME
#define VALUE_128_BITS(NAME) (uint64_t) ((NAME) >> 64),  \
                             (uint64_t) NAME
#define VALUE_256_BITS(NAME) (uint64_t) ((NAME) >> 192), \
                             (uint64_t) ((NAME) >> 128), \
                             (uint64_t) ((NAME) >> 64),  \
                             (uint64_t) NAME
/* clang-format on */

#define GET_MASK(SIZE) CONCATENATE_THREE(MASK_, SIZE, _BITS)
#define GET_VALUE(SIZE, NAME) CONCATENATE_THREE(VALUE_, SIZE, _BITS)(NAME)
#define PRINT_VALUE_HELPER(TYPE, SIZE, NAME) \
  PRINT_VALUE_IMPL(TYPE, GET_MASK(SIZE), GET_VALUE(SIZE, NAME))

#define PRINT_8_BIT_VALUE(TYPE, NAME) PRINT_VALUE_HELPER(TYPE, 8, NAME)
#define PRINT_16_BIT_VALUE(TYPE, NAME) PRINT_VALUE_HELPER(TYPE, 16, NAME)
#define PRINT_32_BIT_VALUE(TYPE, NAME) PRINT_VALUE_HELPER(TYPE, 32, NAME)
#define PRINT_64_BIT_VALUE(TYPE, NAME) PRINT_VALUE_HELPER(TYPE, 64, NAME)
#define PRINT_128_BIT_VALUE(TYPE, NAME) PRINT_VALUE_HELPER(TYPE, 128, NAME)
#define PRINT_256_BIT_VALUE(TYPE, NAME) PRINT_VALUE_HELPER(TYPE, 256, NAME)

#define ARGUMENT_TEST_FUNCTION void __attribute__((noinline, weak)) ABIDEF
#define RETURN_VALUE_TEST_FUNCTION void __attribute__((noinline))

/*
 * Argument passing test functions
 */

ARGUMENT_TEST_FUNCTION lots_of_arguments(uint16_t _00,
                                         uint16_t _01,
                                         uint16_t _02,
                                         uint16_t _03,
                                         uint16_t _04,
                                         uint16_t _05,
                                         uint16_t _06,
                                         uint16_t _07,
                                         uint16_t _08,
                                         uint16_t _09,
                                         uint16_t _10,
                                         uint16_t _11,
                                         uint16_t _12,
                                         uint16_t _13,
                                         uint16_t _14,
                                         uint16_t _15,
                                         uint16_t _16) {
  PRINT_FUNCTION_NAME(lots_of_arguments);
  puts("  Arguments:");
  PRINT_16_BIT_VALUE(uint16_t, _00);
  PRINT_16_BIT_VALUE(uint16_t, _01);
  PRINT_16_BIT_VALUE(uint16_t, _02);
  PRINT_16_BIT_VALUE(uint16_t, _03);
  PRINT_16_BIT_VALUE(uint16_t, _04);
  PRINT_16_BIT_VALUE(uint16_t, _05);
  PRINT_16_BIT_VALUE(uint16_t, _06);
  PRINT_16_BIT_VALUE(uint16_t, _07);
  PRINT_16_BIT_VALUE(uint16_t, _08);
  PRINT_16_BIT_VALUE(uint16_t, _09);
  PRINT_16_BIT_VALUE(uint16_t, _10);
  PRINT_16_BIT_VALUE(uint16_t, _11);
  PRINT_16_BIT_VALUE(uint16_t, _12);
  PRINT_16_BIT_VALUE(uint16_t, _13);
  PRINT_16_BIT_VALUE(uint16_t, _14);
  PRINT_16_BIT_VALUE(uint16_t, _15);
  PRINT_16_BIT_VALUE(uint16_t, _16);
  puts("  ReturnValues:");
  puts("");
}

ARGUMENT_TEST_FUNCTION multitype_arguments(int64_t _00,
                                           uint32_t _01,
                                           int16_t _02,
                                           uint8_t _03,
                                           __int128_t _04) {
  PRINT_FUNCTION_NAME(multitype_arguments);
  puts("  Arguments:");
  PRINT_64_BIT_VALUE(int64_t, _00);
  PRINT_32_BIT_VALUE(uint32_t, _01);
  PRINT_16_BIT_VALUE(int16_t, _02);
  PRINT_8_BIT_VALUE(uint8_t, _03);
  PRINT_128_BIT_VALUE(__int128_t, _04);
  puts("  ReturnValues:");
  puts("");
}

ARGUMENT_TEST_FUNCTION randomness_tester(uint8_t _00,
                                         uint8_t _01,
                                         uint8_t _02,
                                         uint8_t _03,
                                         uint8_t _04,
                                         uint8_t _05,
                                         uint8_t _06,
                                         uint8_t _07,
                                         uint8_t _08,
                                         uint8_t _09,
                                         uint8_t _10,
                                         uint8_t _11,
                                         uint8_t _12,
                                         uint8_t _13,
                                         uint8_t _14,
                                         uint8_t _15,
                                         uint8_t _16,
                                         uint8_t _17,
                                         uint8_t _18,
                                         uint8_t _19,
                                         uint8_t _20,
                                         uint8_t _21,
                                         uint8_t _22,
                                         uint8_t _23) {
  PRINT_FUNCTION_NAME(randomness_tester);
  puts("  Arguments:");
  PRINT_8_BIT_VALUE(uint8_t, _00);
  PRINT_8_BIT_VALUE(uint8_t, _01);
  PRINT_8_BIT_VALUE(uint8_t, _02);
  PRINT_8_BIT_VALUE(uint8_t, _03);
  PRINT_8_BIT_VALUE(uint8_t, _04);
  PRINT_8_BIT_VALUE(uint8_t, _05);
  PRINT_8_BIT_VALUE(uint8_t, _06);
  PRINT_8_BIT_VALUE(uint8_t, _07);
  PRINT_8_BIT_VALUE(uint8_t, _08);
  PRINT_8_BIT_VALUE(uint8_t, _09);
  PRINT_8_BIT_VALUE(uint8_t, _10);
  PRINT_8_BIT_VALUE(uint8_t, _11);
  PRINT_8_BIT_VALUE(uint8_t, _12);
  PRINT_8_BIT_VALUE(uint8_t, _13);
  PRINT_8_BIT_VALUE(uint8_t, _14);
  PRINT_8_BIT_VALUE(uint8_t, _15);
  PRINT_8_BIT_VALUE(uint8_t, _16);
  PRINT_8_BIT_VALUE(uint8_t, _17);
  PRINT_8_BIT_VALUE(uint8_t, _18);
  PRINT_8_BIT_VALUE(uint8_t, _19);
  PRINT_8_BIT_VALUE(uint8_t, _20);
  PRINT_8_BIT_VALUE(uint8_t, _21);
  PRINT_8_BIT_VALUE(uint8_t, _22);
  PRINT_8_BIT_VALUE(uint8_t, _23);
  puts("  ReturnValues:");
  puts("");
}

ARGUMENT_TEST_FUNCTION single_16_bit_argument(uint16_t _00) {
  PRINT_FUNCTION_NAME(single_16_bit_argument);
  puts("  Arguments:");
  PRINT_16_BIT_VALUE(int16_t, _00);
  puts("  ReturnValues:");
  puts("");
}

ARGUMENT_TEST_FUNCTION single_32_bit_argument(int32_t _00) {
  PRINT_FUNCTION_NAME(single_32_bit_argument);
  puts("  Arguments:");
  PRINT_32_BIT_VALUE(int32_t, _00);
  puts("  ReturnValues:");
  puts("");
}

ARGUMENT_TEST_FUNCTION single_64_bit_argument(uint64_t _00) {
  PRINT_FUNCTION_NAME(single_64_bit_argument);
  puts("  Arguments:");
  PRINT_64_BIT_VALUE(uint64_t, _00);
  puts("  ReturnValues:");
  puts("");
}

ARGUMENT_TEST_FUNCTION single_128_bit_argument(__uint128_t _00) {
  PRINT_FUNCTION_NAME(single_128_bit_argument);
  puts("  Arguments:");
  PRINT_128_BIT_VALUE(__uint128_t, _00);
  puts("  ReturnValues:");
  puts("");
}

/* clang-format off */
#define ARGUMENT_FUNCTION_LIST lots_of_arguments,      \
                               multitype_arguments,    \
                               randomness_tester,      \
                               single_16_bit_argument, \
                               single_32_bit_argument, \
                               single_64_bit_argument, \
                               single_128_bit_argument
/* clang-format on */

/*
 * Value returning test functions
 */

jmp_buf jump_buffer;

ABIDEF void *set_return_value_up(void);

/* Helps avoiding compiler warnings */
void *typeless_function = (void *) set_return_value_up;
#define GET_SETUP_FUNCTION(TYPE) ((TYPE ABIDEF(*)(void)) typeless_function)

RETURN_VALUE_TEST_FUNCTION single_8_bit_return_value(void) {
  if (setjmp(jump_buffer) == 0) {
    uint8_t return_value = GET_SETUP_FUNCTION(uint8_t)();
    PRINT_FUNCTION_NAME(single_8_bit_return_value);
    puts("  Arguments:");
    puts("  ReturnValues:");
    PRINT_8_BIT_VALUE(uint8_t, return_value);
    puts("");

    longjmp(jump_buffer, 1);
  }
}

RETURN_VALUE_TEST_FUNCTION single_16_bit_return_value(void) {
  if (setjmp(jump_buffer) == 0) {
    int16_t return_value = GET_SETUP_FUNCTION(int16_t)();
    PRINT_FUNCTION_NAME(single_16_bit_return_value);
    puts("  Arguments:");
    puts("  ReturnValues:");
    PRINT_16_BIT_VALUE(int16_t, return_value);
    puts("");

    longjmp(jump_buffer, 1);
  }
}

RETURN_VALUE_TEST_FUNCTION single_32_bit_return_value(void) {
  if (setjmp(jump_buffer) == 0) {
    uint32_t return_value = GET_SETUP_FUNCTION(uint32_t)();
    PRINT_FUNCTION_NAME(single_32_bit_return_value);
    puts("  Arguments:");
    puts("  ReturnValues:");
    PRINT_32_BIT_VALUE(uint32_t, return_value);
    puts("");

    longjmp(jump_buffer, 1);
  }
}

RETURN_VALUE_TEST_FUNCTION single_64_bit_return_value(void) {
  if (setjmp(jump_buffer) == 0) {
    int64_t return_value = GET_SETUP_FUNCTION(int64_t)();
    PRINT_FUNCTION_NAME(single_64_bit_return_value);
    puts("  Arguments:");
    puts("  ReturnValues:");
    PRINT_64_BIT_VALUE(int64_t, return_value);
    puts("");

    longjmp(jump_buffer, 1);
  }
}

RETURN_VALUE_TEST_FUNCTION single_128_bit_return_value(void) {
  if (setjmp(jump_buffer) == 0) {
    __uint128_t return_value = GET_SETUP_FUNCTION(__uint128_t)();
    PRINT_FUNCTION_NAME(single_128_bit_return_value);
    puts("  Arguments:");
    puts("  ReturnValues:");
    PRINT_128_BIT_VALUE(__uint128_t, return_value);
    puts("");

    longjmp(jump_buffer, 1);
  }
}

/* clang-format off */
#define RETURN_VALUE_FUNCTION_LIST single_8_bit_return_value,  \
                                   single_16_bit_return_value, \
                                   single_32_bit_return_value, \
                                   single_64_bit_return_value, \
                                   single_128_bit_return_value
/* clang-format on */
