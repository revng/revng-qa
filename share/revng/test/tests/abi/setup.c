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
#elif TARGET_mips
#include "mips/setup.inc"
#elif TARGET_mipsel
#include "mipsel/setup.inc"
#elif TARGET_s390x
#include "s390x/setup.inc"
#else
#error "Impossible to find sources because the target architecture is unknown."
#endif


#ifdef _MSC_VER
/*
 * Lack of `_fltused` is an MSVC-specific problem: it is impossible to link any
 * Microsoft ABI binaries that use floating point types without either manually
 * defining it OR linking against ANY C runtime library (which we have to avoid,
 * as we are dynamically calling functions built with a different toolchain).
 *
 * For some context, this is an old x86-x87 compatibility hack and is only
 * really relevant on machines without x87 FPU support.
 *
 * As such, in the context of our test suite, there's not a reason to do
 * anything more involved than just to define this marker to make the linker
 * happy.
 */
void _fltused(void) { /* Intentionally empty. */ }
#endif
