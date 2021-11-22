#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/* clang-format off */
#ifndef STATE_LOCATION
#define STATE_LOCATION "!!!UNDEFINED_STATE_ERROR!!!"
#endif

typedef uint64_t register_type;
#define REGISTER_SIZE 0x8 /* sizeof(register_type) */
#define REGISTER_BIT_SIZE 64 /* (REGISTER_SIZE * 8) */

#define REGISTER_LIST rax, rbx, rcx, rdx, rsi, rdi,        \
                      r8, r9, r10, r11, r12, r13, r14, r15
#define STACK_POINTER rsp

#define PUSH_REGISTER(REGISTER) "push %" STRINGIFY(REGISTER) "\n"
#define POP_REGISTER(REGISTER) "pop %" STRINGIFY(REGISTER) "\n"

#define CALL_A_FUNCTION(FUNCTION_IDENTIFIER)  \
  "call " STRINGIFY(FUNCTION_IDENTIFIER) "\n"
#define RETURN_FROM_FUNCTION "ret\n"

#define PUSH_CONSTANT(INDEX, OFFSET, REGISTER)               \
  "mov (" STRINGIFY(INDEX) " + " STRINGIFY(OFFSET) ") * "    \
          STRINGIFY(REGISTER_SIZE) " + " STATE_LOCATION ", " \
       "%" STRINGIFY(REGISTER) "\n"                          \
  "push %" STRINGIFY(REGISTER) "\n"

#define RESTORE_STACK(COUNT)                                       \
  "add $((" STRINGIFY(COUNT) ") * " STRINGIFY(REGISTER_SIZE) "), " \
      "%" STRINGIFY(STACK_POINTER) "\n"

#define FILL_REGISTER(REGISTER, INDEX)                         \
  "mov " STRINGIFY(INDEX) " * " STRINGIFY(REGISTER_SIZE) " + " \
         STATE_LOCATION ", %" STRINGIFY(REGISTER) "\n"

#define FIX_RETURN_ADDRESS(STACK_SIZE) \
  "push " STRINGIFY(STACK_SIZE) "(%" STRINGIFY(STACK_POINTER) ")\n"

/* clang-format on */
