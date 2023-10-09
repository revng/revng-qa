/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/* RUN-default: nope */

#include "functions.h"

#ifdef TARGET_x86_64
#include "x86_64/printers.inc"
#elif TARGET_i386
#include "i386/printers.inc"
#elif TARGET_aarch64
#include "aarch64/printers.inc"
#elif TARGET_arm
#include "arm/printers.inc"
#else
#error "Impossible to find sources because the target architecture is unknown."
#endif
