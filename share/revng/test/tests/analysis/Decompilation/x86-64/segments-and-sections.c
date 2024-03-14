//
// This file is distributed under the MIT License. See LICENSE.md for details.
//

#define WEAK __attribute__((weak))

WEAK struct {
  int x;
  char padding[1000];
  int y;
} TheData = {
  .x = 7,
  .padding = { 0 },
  .y = 12,
};

WEAK char the_char;

WEAK int puts(const char *ptr) {
  the_char = *ptr;
}

WEAK void print_string() {
  puts("hello world!");
}

WEAK int sum_globals() {
  return TheData.x + TheData.y;
}

WEAK
void _start() {
  print_string();
  sum_globals();
}
