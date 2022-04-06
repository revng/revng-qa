/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <memory.h>
#include <stdint.h>

typedef struct __attribute__((packed)) {
  uint16_t first_uint16;
  uint64_t second_uint64;
} TestStruct;

static TestStruct global_struct[10] = { 0 };

static TestStruct *getGlobalStructArray(void) {
  return global_struct;
}

int main() {
  TestStruct struct_array[10];
  memcpy(struct_array, getGlobalStructArray(), sizeof(TestStruct) * 10);
  uint16_t max_first = 0;
  uint64_t max_second = 0;

  for (int i = 0; i < 10; i++) {
    uint16_t cur_first = struct_array[0].first_uint16;
    uint64_t cur_second = struct_array[0].second_uint64;

    if (cur_first > max_first)
      max_first = cur_first;

    if (cur_second > max_second)
      max_second = cur_second;
  }

  return max_first + max_second;
}
