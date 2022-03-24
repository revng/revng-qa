/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdint.h>

typedef struct __attribute__((packed)) {
  uint16_t uint16_array[10];
  uint64_t uint64;
} TestStruct;

static TestStruct global_struct = {0};

static TestStruct *getGlobalStruct(void) {
  return &global_struct;
}

static uint16_t getArrayMax(TestStruct *struct_ptr) {
  uint16_t max = 0;
  for (int i = 0; i < 10; i++) {
    uint16_t cur = struct_ptr->uint16_array[i];
    if (cur > max)
      max = cur;
  }

  return max;
}

static uint64_t getSecondFieldTimes2(TestStruct *struct_ptr) {
  return struct_ptr->uint64 * 2;
}

int main() {
  TestStruct *struct_ptr = getGlobalStruct();
  uint16_t array_max = getArrayMax(struct_ptr);
  uint64_t second_field_mul2 = getSecondFieldTimes2(struct_ptr);

  return second_field_mul2 + array_max;
}
