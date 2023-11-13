/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/* RUN-default: nope */

#include "functions.inc"

#ifdef TARGET_x86_64
#include "x86_64/setup.inc"
#elif TARGET_i386
#include "i386/setup.inc"
#elif TARGET_aarch64
#include "aarch64/setup.inc"
#elif TARGET_arm
#include "arm/setup.inc"
#else
#error "Impossible to find sources because the target architecture is unknown."
#endif
