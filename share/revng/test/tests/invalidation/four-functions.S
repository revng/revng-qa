#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix

.type _start, @function
.global _start
_start:
    call one
    ret

.type one, @function
one:
    ret

.type two, @function
two:
    ret

.type three, @function
three:
    ret
