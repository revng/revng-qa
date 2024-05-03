#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call caller
    ret

.type callee, @function
callee:
    mov QWORD PTR [0x1000], rdi
    ret

.type caller, @function
caller:
    # callee has an argument, we should inherit it
    call callee
    ret
