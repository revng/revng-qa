/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stddef.h>
#include <stdint.h>

// clang-format off
#define INTEL_ASM(name, ...) asm(".intel_syntax noprefix\n"    \
                                 ".globl " #name "\n"          \
                                 ".type " #name ",@function\n" \
                                 #name ":\n"                   \
                                 ".intel_syntax noprefix\n "   \
                                 #__VA_ARGS__                  \
                                 "\n.att_syntax\n")

#define WEAK __attribute__((weak))

WEAK void side_effects(void) {
}

uint32_t *escape_local32 = NULL;
uint64_t *escape_local64 = NULL;

WEAK void stack_frame(void);

// This function has 8-bytes wide stack frame
INTEL_ASM(stack_frame,
  sub rsp, 0x8\n
  mov DWORD PTR [rsp],0xDEADBEEF\n
  mov DWORD PTR [rsp+0x4],0x13371337\n
  add rsp, 0x8\n
  ret\n
);

// This function has a stack frame of 80-bytes but all the accesses are indirect
//
// Note: if we don't use `jb` we won't able to detect this since LVI won't be
//       able to prove the induction variable is within a certain range.
INTEL_ASM(stack_frame_array,
  sub rsp, 80\n
  xor eax, eax\n
loop:\n
  mov QWORD PTR [rsp+rax*8], 42\n
  add rax, 1\n
  cmp rax, 10\n
  jb loop\n
  add rsp, 80\n
  ret\n
);

// This function has 24 bytes of stack arguments
WEAK void stack_arguments_64(uint64_t rdi,
                             uint64_t rsi,
                             uint64_t rdx,
                             uint64_t rcx,
                             uint64_t r8,
                             uint64_t r9,
                             uint64_t stack1,
                             uint64_t stack2,
                             uint64_t stack3) {
  *escape_local64 = stack1 * stack2 * stack3;

  side_effects();
}

// This function has a zero-sized stack frame and a call site with three 64-bits
// arguments
WEAK void call_stack_arguments_64(void);

INTEL_ASM(call_stack_arguments_64,
  mov    edi,0x1\n
  mov    esi,0x2\n
  mov    edx,0x3\n
  mov    ecx,0x4\n
  mov    r8d,0x5\n
  mov    r9d,0x6\n
  push   0x1337\n
  push   0xbeef\n
  push   0xdead\n
  call   stack_arguments_64\n
  add    rsp,0x18\n
  ret\n
);

WEAK void indirect_call_stack_arguments_64(void);

// Same as before but the call site is indirect.
// We won't be able to detect this at this stage.
//
// One way to detect this could be the following: try to identify a trailing
// range of the stack frame respecting the following properties:
//
// * No loads
// * All the stores targeting them are before an indirect function call
INTEL_ASM(indirect_call_stack_arguments_64,
  mov    edi,0x1\n
  mov    esi,0x2\n
  mov    edx,0x3\n
  mov    ecx,0x4\n
  mov    r8d,0x5\n
  mov    r9d,0x6\n
  push   0x1337\n
  push   0xbeef\n
  push   0xdead\n
  call   rax\n
  add    rsp,0x18\n
  ret\n
);
// clang-format on

// This function has 24 bytes of stack arguments, but until we integrate
// ABI-specific information we'll get 20 bytes and detect only the stack1 and
// stack2
WEAK void stack_arguments_32(uint32_t rdi,
                             uint32_t rsi,
                             uint32_t rdx,
                             uint32_t rcx,
                             uint32_t r8,
                             uint32_t r9,
                             uint32_t stack1,
                             uint32_t stack2,
                             uint32_t stack3) {
  *escape_local32 = stack1 * stack2 * stack3;

  side_effects();
}

// This function accepts a 80-bytes argument on the stack. We should be able to
// detect it.
typedef struct {
  uint64_t array[10];
} array_struct;

array_struct *escape_array_struct = NULL;

// TODO: we're not currently able to detect this for the same reason as for
//       `stack_frame_array`. We need to normalize induction variables so LVI
//       can prove I is always < 10. Alternatively we could use SCEV to get that
//       information.
WEAK void array_stack_argument(uint64_t rdi,
                               uint64_t rsi,
                               uint64_t rdx,
                               uint64_t rcx,
                               uint64_t r8,
                               uint64_t r9,
                               array_struct stack_array) {
  for (size_t I = 0; I < 10; ++I) {
    stack_array.array[I] = stack_array.array[I] * 2;
  }

  escape_array_struct = &stack_array;

  side_effects();
}

// Function passing stack arguments with direct and indirect stack accesses
WEAK void call_array_stack_argument(void) {
  array_struct my_struct;

  for (size_t I = 0; I < 10; ++I) {
    // Indirect store
    my_struct.array[I] = I * 3;
  }

  // Direct store
  my_struct.array[4] = 42;

  array_stack_argument(1, 2, 3, 4, 5, 6, my_struct);

  escape_array_struct = &my_struct;

  side_effects();
}

int main() {
  return 0;
}
