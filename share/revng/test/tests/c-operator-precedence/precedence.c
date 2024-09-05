/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <stdbool.h>
#include <stdint.h>

#define WEAK __attribute__((weak))
#define TEST(name, value) \
  WEAK uint64_t name(uint64_t a, uint64_t b, uint64_t c) { return (value); }

TEST(from_class_12_to_class_11, a || b && c)
TEST(from_class_12_to_class_10, a | b && c)
TEST(from_class_12_to_class_9, a ^ b && c)
TEST(from_class_12_to_class_8, a & b && c)
TEST(from_class_12_to_class_7, a == b && c)
TEST(from_class_12_to_class_6, a < b && c)
TEST(from_class_12_to_class_5, a << b && c)
TEST(from_class_12_to_class_4, a + b && c)
TEST(from_class_12_to_class_3, a * b && c)
TEST(from_class_12_to_class_2, -a && c)
TEST(from_class_12_to_class_1, a-- && c)

TEST(from_class_11_to_class_10, a | b || c)
TEST(from_class_11_to_class_9, a ^ b || c)
TEST(from_class_11_to_class_8, a & b || c)
TEST(from_class_11_to_class_7, a == b || c)
TEST(from_class_11_to_class_6, a < b || c)
TEST(from_class_11_to_class_5, a << b || c)
TEST(from_class_11_to_class_4, a + b || c)
TEST(from_class_11_to_class_3, a * b || c)
TEST(from_class_11_to_class_2, -a || c)
TEST(from_class_11_to_class_1, a-- || c)

TEST(from_class_10_to_class_9, a ^ b | c)
TEST(from_class_10_to_class_8, a & b | c)
TEST(from_class_10_to_class_7, a == b | c)
TEST(from_class_10_to_class_6, a < b | c)
TEST(from_class_10_to_class_5, a << b | c)
TEST(from_class_10_to_class_4, a + b | c)
TEST(from_class_10_to_class_3, a * b | c)
TEST(from_class_10_to_class_2, -a | c)
TEST(from_class_10_to_class_1, a-- | c)

TEST(from_class_9_to_class_8, a & b ^ c)
TEST(from_class_9_to_class_7, a == b ^ c)
TEST(from_class_9_to_class_6, a < b ^ c)
TEST(from_class_9_to_class_5, a << b ^ c)
TEST(from_class_9_to_class_4, a + b ^ c)
TEST(from_class_9_to_class_3, a * b ^ c)
TEST(from_class_9_to_class_2, -a ^ c)
TEST(from_class_9_to_class_1, a-- ^ c)

TEST(from_class_8_to_class_7, a == b & c)
TEST(from_class_8_to_class_6, a < b & c)
TEST(from_class_8_to_class_5, a << b & c)
TEST(from_class_8_to_class_4, a + b & c)
TEST(from_class_8_to_class_3, a * b & c)
TEST(from_class_8_to_class_2, -a & c)
TEST(from_class_8_to_class_1, a-- & c)

TEST(from_class_7_to_class_6, a < b == c)
TEST(from_class_7_to_class_5, a << b == c)
TEST(from_class_7_to_class_4, a + b == c)
TEST(from_class_7_to_class_3, a * b == c)
TEST(from_class_7_to_class_2, -a == c)
TEST(from_class_7_to_class_1, a-- == c)

TEST(from_class_6_to_class_5, a << b < c)
TEST(from_class_6_to_class_4, a + b < c)
TEST(from_class_6_to_class_3, a * b < c)
TEST(from_class_6_to_class_2, -a < c)
TEST(from_class_6_to_class_1, a-- < c)

TEST(from_class_5_to_class_4, a + b << c)
TEST(from_class_5_to_class_3, a * b << c)
TEST(from_class_5_to_class_2, -a << c)
TEST(from_class_5_to_class_1, a-- << c)

TEST(from_class_4_to_class_3, a * b + c)
TEST(from_class_4_to_class_2, -a + c)
TEST(from_class_4_to_class_1, a-- + c)

TEST(from_class_3_to_class_2, -a * c)
TEST(from_class_3_to_class_1, a-- * c)

TEST(from_class_2_to_class_1, -a--)

WEAK uint64_t a;
WEAK uint64_t b;
WEAK uint64_t c;

int main() {
  from_class_12_to_class_11(a, b, c);
  from_class_12_to_class_10(a, b, c);
  from_class_12_to_class_9(a, b, c);
  from_class_12_to_class_8(a, b, c);
  from_class_12_to_class_7(a, b, c);
  from_class_12_to_class_6(a, b, c);
  from_class_12_to_class_5(a, b, c);
  from_class_12_to_class_4(a, b, c);
  from_class_12_to_class_3(a, b, c);
  from_class_12_to_class_2(a, b, c);
  from_class_12_to_class_1(a, b, c);
  from_class_11_to_class_10(a, b, c);
  from_class_11_to_class_9(a, b, c);
  from_class_11_to_class_8(a, b, c);
  from_class_11_to_class_7(a, b, c);
  from_class_11_to_class_6(a, b, c);
  from_class_11_to_class_5(a, b, c);
  from_class_11_to_class_4(a, b, c);
  from_class_11_to_class_3(a, b, c);
  from_class_11_to_class_2(a, b, c);
  from_class_11_to_class_1(a, b, c);
  from_class_10_to_class_9(a, b, c);
  from_class_10_to_class_8(a, b, c);
  from_class_10_to_class_7(a, b, c);
  from_class_10_to_class_6(a, b, c);
  from_class_10_to_class_5(a, b, c);
  from_class_10_to_class_4(a, b, c);
  from_class_10_to_class_3(a, b, c);
  from_class_10_to_class_2(a, b, c);
  from_class_10_to_class_1(a, b, c);
  from_class_9_to_class_8(a, b, c);
  from_class_9_to_class_7(a, b, c);
  from_class_9_to_class_6(a, b, c);
  from_class_9_to_class_5(a, b, c);
  from_class_9_to_class_4(a, b, c);
  from_class_9_to_class_3(a, b, c);
  from_class_9_to_class_2(a, b, c);
  from_class_9_to_class_1(a, b, c);
  from_class_8_to_class_7(a, b, c);
  from_class_8_to_class_6(a, b, c);
  from_class_8_to_class_5(a, b, c);
  from_class_8_to_class_4(a, b, c);
  from_class_8_to_class_3(a, b, c);
  from_class_8_to_class_2(a, b, c);
  from_class_8_to_class_1(a, b, c);
  from_class_7_to_class_6(a, b, c);
  from_class_7_to_class_5(a, b, c);
  from_class_7_to_class_4(a, b, c);
  from_class_7_to_class_3(a, b, c);
  from_class_7_to_class_2(a, b, c);
  from_class_7_to_class_1(a, b, c);
  from_class_6_to_class_5(a, b, c);
  from_class_6_to_class_4(a, b, c);
  from_class_6_to_class_3(a, b, c);
  from_class_6_to_class_2(a, b, c);
  from_class_6_to_class_1(a, b, c);
  from_class_5_to_class_4(a, b, c);
  from_class_5_to_class_3(a, b, c);
  from_class_5_to_class_2(a, b, c);
  from_class_5_to_class_1(a, b, c);
  from_class_4_to_class_3(a, b, c);
  from_class_4_to_class_2(a, b, c);
  from_class_4_to_class_1(a, b, c);
  from_class_3_to_class_2(a, b, c);
  from_class_3_to_class_1(a, b, c);
  from_class_2_to_class_1(a, b, c);
}
