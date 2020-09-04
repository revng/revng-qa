#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int foo(int x) {
  int a = 0;
  switch(x) {
    case 1:
      a = 1;
    case 2:
      a = 2;
      break;
    case 3:
      a = 3;
      break;
    case 4:
      a = 4;
      break;
    case 5:
      a = 5;
      break;
    default:
      a = 0;
      break;
  }
  return a;
}

int main(int a) {
  int b = 0;
  b = foo(a);
  return b + a;
}
