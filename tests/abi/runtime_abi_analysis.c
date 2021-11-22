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
#elif TARGET_x86
#include "platform_specific/x86.h"
#else
#include "platform_specific/x86_64.h"
_Static_assert(0, "unsupported platform architecture");
#endif

/* LFSR generator */

static uint64_t lfsr = 0xdc984ef2647af904ull;
uint64_t get_next_lfsr(void) {
  for (uint32_t iteration = 0; iteration < 64; ++iteration)
    lfsr = (lfsr >> 1) | ((((lfsr >> 1) ^ (lfsr >> 2)) & 1) << 63);
  return lfsr;
}

/* Helpers */

#define GET_1ST_HELPER(THIS_ONE, ...) THIS_ONE
#define GET_1ST(THIS_ONE, ...) GET_1ST_HELPER(THIS_ONE, __VA_ARGS__)
#define FIRST_REGISTER GET_1ST(REGISTER_LIST)

#define STACK_VALUE_COUNT 32
#define REGISTER_COUNT COUNT_ARGUMENTS(REGISTER_LIST)
#define GENERATED_COUNT REGISTER_COUNT + STACK_VALUE_COUNT

#ifdef STATE_LOCATION
#undef STATE_LOCATION
#endif
#define STATE_LOCATION "random_state"

char *register_list[REGISTER_COUNT] = {
  COMMA_SEPARATED_FOR_EACH(STRINGIFY, REGISTER_LIST)
};

static __attribute((used)) register_type random_state[GENERATED_COUNT];
void regenerate_state(void) {
  for (uint32_t counter = 0; counter < GENERATED_COUNT; ++counter)
    random_state[counter] = get_next_lfsr();
}
void print_current_state(void) {
  /* Dump registers */
  puts("- Registers:");
  for (uint32_t index = 0; index < REGISTER_COUNT; ++index) {
    printf("    - Name: \"%s\"\n"
           "      Value: " GET_MASK(REGISTER_BIT_SIZE) "\n",
           register_list[index],
           GET_VALUE(REGISTER_BIT_SIZE,
                     random_state[REGISTER_COUNT - index - 1]));
  }

  /* Dump Stack */
  puts("  Stack:");
  for (uint32_t index = REGISTER_COUNT; index < GENERATED_COUNT; ++index)
    printf("    - Offset: -0x%04x\n      Value: 0x%016lx\n",
           (GENERATED_COUNT - index) * REGISTER_SIZE,
           random_state[index]);
}

#define CALL_ITERATION_WITH(FUNCTION_NAME) \
  CONCATENATE(iteration_impl_on_, FUNCTION_NAME)

#define GENERATED_BYTES ((GENERATED_COUNT) * (REGISTER_SIZE))

#ifdef __APPLE__
#define FUNCTION_NAME_PREFIX _
#else
#define FUNCTION_NAME_PREFIX
#endif

/* clang-format off */
#define DEFINE_ITERATION_WITH(FUNCTION_NAME)                                   \
void CALL_ITERATION_WITH(FUNCTION_NAME)(void) {                                \
  if (setjmp(jump_buffer) == 0) {                                              \
    regenerate_state();                                                        \
    print_current_state();                                                     \
                                                                               \
    __asm__ (                                                                  \
      /* Popularize the stack with random data */                              \
      REPEAT(PUSH_CONSTANT, STACK_VALUE_COUNT, REGISTER_COUNT, FIRST_REGISTER) \
      /* Popularize all the registers with random data */                      \
      INDEXED_FOR_EACH(FILL_REGISTER, REGISTER_LIST)                           \
                                                                               \
      /* Call the function */                                                  \
      CALL_A_FUNCTION(FUNCTION_NAME)                                           \
                                                                               \
      /* Restore the stack */                                                  \
      RESTORE_STACK(STACK_VALUE_COUNT)                                         \
    );                                                                         \
                                                                               \
    longjmp(jump_buffer, 1);                                                   \
  }                                                                            \
}

__asm__ ( /* Define the return value setup function */
STRINGIFY(FUNCTION_NAME_PREFIX) "set_return_value_up:\n"

  /* Generate random values */
  CALL_A_FUNCTION(regenerate_state)
  /* Output those values */
  CALL_A_FUNCTION(print_current_state)

  /* Fill stack with pre-generated random values */
  REPEAT(PUSH_CONSTANT, STACK_VALUE_COUNT, REGISTER_COUNT, FIRST_REGISTER)
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
  SEMICOLON_SEPARATED_FOR_EACH(ITERATE_ON_ARGUMENTS, ARGUMENT_FUNCTION_LIST);
  SEMICOLON_SEPARATED_FOR_EACH(ITERATE_ON_RVALUES, RETURN_VALUE_FUNCTION_LIST);
  puts("...");

  return 0;
}
