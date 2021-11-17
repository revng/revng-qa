#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call wrapper
    ret

.type wrapper, @function
wrapper:
    call recursive
    ret

.type recursive, @function
recursive:
    je skip
    call intermediate
skip:
    ret

.type intermediate, @function
intermediate:
    call recursive
    ret
