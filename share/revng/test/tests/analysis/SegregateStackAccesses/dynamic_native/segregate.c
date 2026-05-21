/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

uint64_t Unknown;

typedef struct {
  uint64_t A;
  uint64_t B;
} TwoIntegers;

typedef struct {
  uint64_t A;
  uint64_t B;
  uint64_t D;
  uint64_t E;
  uint64_t F;
  uint64_t G;
  uint64_t H;
} ManyIntegers;

// RawFunctionType with register arguments, all primitives
uint64_t raw_primitives_on_registers(uint64_t A, uint64_t B) {
  return Unknown + A + B;
}

uint64_t call_raw_primitives_on_registers() {
  return Unknown + raw_primitives_on_registers(1, 2);
}

// RawFunctionType with register arguments, with pointers
uint64_t raw_pointers_on_registers(uint64_t *A, uint64_t *B) {
  return Unknown + *A + *B;
}

uint64_t call_raw_pointers_on_registers() {
  return Unknown + raw_pointers_on_registers(&Unknown, &Unknown);
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

uint64_t call_raw_primitives_on_stack() {
  return Unknown + raw_primitives_on_stack(1, 2, 3, 4, 5, 6, 7, 8);
}

// CABIFunctionType with scalar arguments, only in registers
uint64_t cabi_primitives_on_registers(uint64_t A, uint64_t B) {
  return Unknown + A + B;
}

uint64_t call_cabi_primitives_on_registers() {
  return Unknown + cabi_primitives_on_registers(1, 2);
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

uint64_t call_cabi_primitives_on_stack() {
  return Unknown + cabi_primitives_on_stack(1, 2, 3, 4, 5, 6, 7, 8);
}

// CABIFunctionType with aggregate on registers
uint64_t cabi_aggregate_on_registers(TwoIntegers Arg) {
  return Unknown + Arg.A + Arg.B;
}

uint64_t call_cabi_aggregate_on_registers() {
  return Unknown + cabi_aggregate_on_registers((TwoIntegers){ 1, 2 });
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

uint64_t call_cabi_aggregate_on_stack() {
  return Unknown + cabi_aggregate_on_stack(1, 2, 3, 4, 5, 6, (TwoIntegers){ 1, 2 });
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

uint64_t call_cabi_aggregate_on_stack_and_registers() {
  return Unknown + cabi_aggregate_on_stack_and_registers(1,
                                                         2,
                                                         3,
                                                         4,
                                                         5,
                                                         (TwoIntegers){ 1, 2 });
}

TwoIntegers raw_return_small_aggregate(void) {
  TwoIntegers result;
  result.A = 124;
  result.B = 123;
  return result;
}

uint64_t call_raw_return_small_aggregate() {
  return Unknown + raw_return_small_aggregate().B;
}

TwoIntegers cabi_return_small_aggregate(void) {
  TwoIntegers result;
  result.A = 124;
  result.B = 123;
  return result;
}

uint64_t call_cabi_return_small_aggregate() {
  return Unknown + cabi_return_small_aggregate().B;
}

ManyIntegers cabi_return_big_aggregate(void) {
  ManyIntegers result;
  result.D = 123;
  return result;
}

uint64_t call_cabi_return_big_aggregate() {
  return Unknown + cabi_return_big_aggregate().D;
}

int main(int argc, char *argv[]) {
  return (call_raw_primitives_on_registers()
          + call_raw_pointers_on_registers()
          + call_raw_primitives_on_stack()
          + call_cabi_primitives_on_registers()
          + call_cabi_primitives_on_stack()
          + call_cabi_aggregate_on_registers()
          + call_cabi_aggregate_on_stack()
          + call_cabi_aggregate_on_stack_and_registers()
          + call_raw_return_small_aggregate()
          + call_cabi_return_small_aggregate()
          + call_cabi_return_big_aggregate());
}
