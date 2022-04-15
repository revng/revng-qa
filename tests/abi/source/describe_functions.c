/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include "common.h"
#include "revng-qa/generated/functions.h"
#include "revng-qa/generated/functions.inc"

#ifdef TARGET_x86_64
#include "revng-qa/generated/x86_64/describe_functions.inc"
#elif TARGET_x86
#include "revng-qa/generated/x86/describe_functions.inc"
#elif TARGET_arm
#include "revng-qa/generated/arm/describe_functions.inc"
#else
#error "Impossible to find sources because the target architecture is unknown."
#endif
