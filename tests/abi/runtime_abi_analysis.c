/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "for_each.h"
#include "functions.h"

#ifdef TARGET_x86_64
#include "platform_specific/x86_64.h"
#define TARGET "x86_64"
#elif TARGET_x86
#include "platform_specific/x86.h"
#define TARGET "x86"
#elif TARGET_arm
#include "platform_specific/arm.h"
#define TARGET "arm"
#else
#define TARGET "UNSUPPORTED"
_Static_assert(0, "unsupported platform architecture");
#endif

/* LFSR generator */

static uint64_t lfsr = 0xdc984ef2647af904ull;
uint64_t get_next_lfsr(uint32_t iteration_count) {
  for (uint32_t iteration = 0; iteration < iteration_count; ++iteration)
    lfsr = (lfsr >> 1) | ((((lfsr >> 1) ^ (lfsr >> 2)) & 1) << 63);
  return lfsr;
}

/* Helpers */

#define GET_1ST_HELPER(THIS_ONE, ...) THIS_ONE
#define GET_1ST(THIS_ONE, ...) GET_1ST_HELPER(THIS_ONE, __VA_ARGS__)
#define TMP_REGISTER GET_1ST(REGISTER_LIST)

#define STACK_VALUE_COUNT 32
#define REGISTER_COUNT COUNT_ARGUMENTS(REGISTER_LIST)
#define GENERATED_COUNT REGISTER_COUNT + STACK_VALUE_COUNT
#define GENERATED_BYTE_COUNT REGISTER_SIZE * (GENERATED_COUNT)

#ifdef STATE_LOCATION
#undef STATE_LOCATION
#endif
#define STATE_LOCATION "randomized_state"

static char *register_list[REGISTER_COUNT] = {
  COMMA_SEPARATED_FOR_EACH(STRINGIFY, REGISTER_LIST)
};

_Static_assert(sizeof(uint8_t) == 1, "A type with size == 1 is required.");
static __attribute((used)) uint8_t randomized_state[GENERATED_BYTE_COUNT];
void regenerate_state(void) {
  for (uint32_t counter = 0; counter < GENERATED_BYTE_COUNT; ++counter)
    randomized_state[counter] = (uint8_t) get_next_lfsr(8);
}

void print_current_state(void) {
  /* Dump registers */
  puts("  - Registers:");
  for (uint32_t index = 0; index < REGISTER_COUNT; ++index) {
    typedef uint8_t single_word[REGISTER_SIZE];
    uint32_t offset = (REGISTER_COUNT - index - 1) * REGISTER_SIZE;
    single_word *word_ptr = (single_word *) (randomized_state + offset);
    MAKE_PRINT_HELPER(register_type, word_ptr, word_helper);

    printf("      - Name: \"%s\"\n        Value: 0x%.*zx\n",
           register_list[index],
           REGISTER_SIZE * 2,
           word_helper->v);
    printf("        Bytes: ");
    PRINT_BYTES(register_type, word_helper);
  }
  
  /* Dump Stack */
  printf("    Stack: ");
  typedef uint8_t stack_state[STACK_VALUE_COUNT * REGISTER_SIZE];
  uint32_t offset = (REGISTER_COUNT) * REGISTER_SIZE;
  stack_state *state_ptr = (stack_state *) (randomized_state + offset);
  MAKE_PRINT_HELPER(stack_state, state_ptr, stack_helper);
  PRINT_BYTES(stack_state, stack_helper);
}

#define CALL_ITERATION_WITH(FUNCTION_NAME) \
  CONCATENATE(iteration_impl_on_, FUNCTION_NAME)

#ifdef __APPLE__
#define FUNCTION_NAME_PREFIX _
#else
#define FUNCTION_NAME_PREFIX
#endif

/* clang-format off */
#define DEFINE_ITERATION_WITH(FUNCTION_NAME)                                  \
void CALL_ITERATION_WITH(FUNCTION_NAME)(void) {                               \
  if (setjmp(jump_buffer) == 0) {                                             \
    regenerate_state();                                                       \
    print_current_state();                                                    \
                                                                              \
    __asm__ (                                                                 \
      /* Popularize the stack with random data */                             \
      REPEAT(PUSH_CONSTANT, STACK_VALUE_COUNT, GENERATED_COUNT, TMP_REGISTER) \
      /* Popularize all the registers with random data */                     \
      INDEXED_FOR_EACH(FILL_REGISTER, REGISTER_LIST)                          \
                                                                              \
      /* Call the function */                                                 \
      CALL_A_FUNCTION(FUNCTION_NAME)                                          \
                                                                              \
      /* Restore the stack */                                                 \
      RESTORE_STACK(STACK_VALUE_COUNT)                                        \
    );                                                                        \
                                                                              \
    longjmp(jump_buffer, 1);                                                  \
  }                                                                           \
}

__asm__ ( /* Define the return value setup function */
STRINGIFY(FUNCTION_NAME_PREFIX) "set_return_value_up:\n"

  /* Generate random values */
  CALL_A_FUNCTION(regenerate_state)
  /* Output those values */
  CALL_A_FUNCTION(print_current_state)

  /* Fill stack with pre-generated random values */
  REPEAT(PUSH_CONSTANT, STACK_VALUE_COUNT, REGISTER_COUNT, TMP_REGISTER)
  /* Fill registers with pre-generated random values */
  INDEXED_FOR_EACH(FILL_REGISTER, REGISTER_LIST)

  /* Extract the return address from the stack and push it anew */
  FIX_RETURN_ADDRESS((STACK_VALUE_COUNT) * (REGISTER_SIZE))
  /* Function return instruction */
  RETURN_FROM_FUNCTION
);
/* clang-format on */

#define ITERATION_COUNT 5

#define ITERATE_ON_ARGUMENTS(FUNCTION_NAME)                  \
  for (uint32_t index = 0; index < ITERATION_COUNT; ++index) \
    CALL_ITERATION_WITH(FUNCTION_NAME)();

#define ITERATE_ON_RVALUES(FUNCTION_NAME)                    \
  for (uint32_t index = 0; index < ITERATION_COUNT; ++index) \
    FUNCTION_NAME();

/* Main */

SEMICOLON_SEPARATED_FOR_EACH(DEFINE_ITERATION_WITH, ARGUMENT_FUNCTION_LIST);
int main(void) {
  puts("---");
  puts("TargetArchitecture: " TARGET);

#ifdef ENDIANNESS_LITTLE
  assert(!runtime_endianness_check());
  puts("IsLittleEndian: yes");
#elif defined(ENDIANNESS_BIG)
  assert(runtime_endianness_check());
  puts("IsLittleEndian: no");
#endif

  puts("Iterations:");
  SEMICOLON_SEPARATED_FOR_EACH(ITERATE_ON_ARGUMENTS, ARGUMENT_FUNCTION_LIST);
  SEMICOLON_SEPARATED_FOR_EACH(ITERATE_ON_RVALUES, RETURN_VALUE_FUNCTION_LIST);

  puts("...");
  return 0;
}
