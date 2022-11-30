/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

uint64_t Unknown;

typedef struct {
  uint64_t A;
  uint64_t B;
} TwoIntegers;

// RawFunctionType with register arguments, all primitives
uint64_t raw_primitives_on_registers(uint64_t A, uint64_t B) {
  return Unknown + A + B;
}

// RawFunctionType with register arguments, with pointers
uint64_t raw_pointers_on_registers(uint64_t *A, uint64_t *B) {
  return Unknown + *A + *B;
}

// RawFunctionType with stack arguments, all primitives
uint64_t raw_primitives_on_stack(uint64_t A,
                                 uint64_t B,
                                 uint64_t C,
                                 uint64_t D,
                                 uint64_t E,
                                 uint64_t F,
                                 uint64_t G,
                                 uint64_t H) {
  return Unknown + G + H;
}

// CABIFunctionType with scalar arguments, only in registers
uint64_t cabi_primitives_on_registers(uint64_t A, uint64_t B) {
  return Unknown + A + B;
}

// CABIFunctionType with scalar arguments, both in registers and on the stack
uint64_t cabi_primitives_on_stack(uint64_t A,
                                  uint64_t B,
                                  uint64_t C,
                                  uint64_t D,
                                  uint64_t E,
                                  uint64_t F,
                                  uint64_t G,
                                  uint64_t H) {
  return Unknown + G + H;
}

// CABIFunctionType with aggregate on registers
uint64_t cabi_aggregate_on_registers(TwoIntegers Arg) {
  return Unknown + Arg.A + Arg.B;
}

// CABIFunctionType with aggregate on the stack
uint64_t cabi_aggregate_on_stack(uint64_t A,
                                 uint64_t B,
                                 uint64_t C,
                                 uint64_t D,
                                 uint64_t E,
                                 uint64_t F,
                                 TwoIntegers Arg) {
  return Unknown + Arg.A + Arg.B;
}

// CABIFunctionType with aggregate partly in the registers and partly on the
// stack
uint64_t cabi_aggregate_on_stack_and_registers(uint64_t A,
                                               uint64_t B,
                                               uint64_t C,
                                               uint64_t D,
                                               uint64_t E,
                                               TwoIntegers Arg) {
  return Unknown + Arg.A + Arg.B;
}

typedef struct {
  uint64_t A;
  uint64_t B;
  uint64_t D;
  uint64_t E;
  uint64_t F;
  uint64_t G;
  uint64_t H;
} ManyIntegers;

ManyIntegers cabi_return_big_aggregate(void) {
  ManyIntegers result;
  result.D = 123;
  return result;
}

uint64_t caller() {
  uint64_t Result = 0;
  Result += raw_primitives_on_registers(1, 2);
  Result += raw_pointers_on_registers(&Result, &Result);
  Result += raw_primitives_on_stack(1, 2, 3, 4, 5, 6, 7, 8);
  Result += cabi_primitives_on_registers(1, 2);
  Result += cabi_primitives_on_stack(1, 2, 3, 4, 5, 6, 7, 8);
  Result += cabi_aggregate_on_registers((TwoIntegers){ 1, 2 });
  Result += cabi_aggregate_on_stack(1, 2, 3, 4, 5, 6, (TwoIntegers){ 1, 2 });
  Result += cabi_aggregate_on_stack_and_registers(1,
                                                  2,
                                                  3,
                                                  4,
                                                  5,
                                                  (TwoIntegers){ 1, 2 });
  Result += cabi_return_big_aggregate().D;
  return Unknown + Result;
}

int main() {
  return caller();
}
