#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Determine the endianness
 */
#if defined(__BYTE_ORDER__)
#define BO __BYTE_ORDER__
#elif defined(__BYTE_ORDER)
#define BO __BYTE_ORDER
#endif

#if defined(__ORDER_LITTLE_ENDIAN__)
#define LEO __ORDER_LITTLE_ENDIAN__
#elif defined(__ORDER_LITTLE_ENDIAN)
#define LEO __ORDER_LITTLE_ENDIAN
#endif

#if defined(__ORDER_BIG_ENDIAN__)
#define BEO __ORDER_BIG_ENDIAN__
#elif defined(__ORDER_BIG_ENDIAN)
#define BEO __ORDER_BIG_ENDIAN
#endif

#if !defined(BO) || !defined(LEO) || !defined(BEO)
#error "No known way to detect endianness!" \
       "(What kind of crazy compiler are you using?)"
#endif

#if BO == LEO
#define ENDIANNESS_LITTLE
#elif BO == BEO
#define ENDIANNESS_BIG
#else
#error "Unsupported endianness!" \
       "(pdp? something crazy? Memory corruption?)"
#endif

#undef BO
#undef LEO
#undef BEO

_Static_assert(sizeof(uint8_t) == 1, "A type with size == 1 is required.");
#define MAKE_PRINT_HELPER(TYPE, POINTER, RESULT) \
  typedef union {                                \
    TYPE v;                                      \
    uint8_t a[sizeof(TYPE)];                     \
  } printing_##RESULT;                           \
  printing_##RESULT *RESULT = (printing_##RESULT *) POINTER

#define PRINT_BYTES(TYPE, HELPER)                          \
  do {                                                     \
    printf("[ ");                                          \
    for (unsigned i = 0; i < sizeof(TYPE) - 1; ++i)        \
      printf("0x%.2hhx, ", (HELPER)->a[i]);                \
    printf("0x%.2hhx ]\n", (HELPER)->a[sizeof(TYPE) - 1]); \
  } while (0)

#define PRINT(TYPE, POINTER)                         \
  do {                                               \
    printf("      - Type: " #TYPE " # size = 0x%x\n" \
           "        Bytes: ",                        \
           sizeof(TYPE));                            \
    MAKE_PRINT_HELPER(TYPE, POINTER, local_helper);  \
    PRINT_BYTES(TYPE, local_helper);                 \
  } while (0)

#define POINTER(TYPE, POINTER)                        \
  do {                                                \
    printf("      - Type: " #TYPE " # size = 0x%x\n"  \
           "        Pointer: 0x%x\n        Bytes: ",  \
           sizeof(TYPE),                              \
           POINTER);                                  \
    MAKE_PRINT_HELPER(TYPE *, POINTER, local_helper); \
    PRINT_BYTES(TYPE *, local_helper);                \
  } while (0)
