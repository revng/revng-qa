/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/* RUN-default: nope */

#ifdef TARGET_x86_64
#include "x86_64/decoders.inc"
#elif TARGET_i386
#include "i386/decoders.inc"
#elif TARGET_aarch64
#include "aarch64/decoders.inc"
#elif TARGET_arm
#include "arm/decoders.inc"
#else
#error "Impossible to find sources because the target architecture is unknown."
#endif
