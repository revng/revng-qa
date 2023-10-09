#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "abi.h"

#ifdef TARGET_x86_64
#include "x86_64/constants.h"
#elif TARGET_i386
#include "i386/constants.h"
#elif TARGET_aarch64
#include "aarch64/constants.h"
#elif TARGET_arm
#include "arm/constants.h"
#else
#error "Impossible to find sources because the target architecture is unknown."
#endif

/*
 * Determine the endianness
 */

#ifdef _MSC_VER
#define ENDIANNESS_LITTLE
#else

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

#endif
