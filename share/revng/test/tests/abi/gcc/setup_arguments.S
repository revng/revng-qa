/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#ifdef TARGET_x86_64
# include "../x86_64/gcc/setup_arguments.S"
#elif TARGET_i386
# include "../i386/gcc/setup_arguments.S"
#elif TARGET_aarch64
# include "../aarch64/gcc/setup_arguments.S"
#elif TARGET_arm
# include "../arm/gcc/setup_arguments.S"
#else
# error "Impossible to find sources because the target architecture is unknown."
#endif
