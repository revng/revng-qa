#include <stdio.h>

__attribute__((weak)) struct {
  int x;
  char padding[1000];
  int y;
} TheData = {
  .x = 7,
  .padding = { 0 },
  .y = 12,
};

__attribute__((weak))
void print_string() {
  puts("hello world!");
}

int main() {
  print_string();
  return TheData.x + TheData.y;
}
