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
    mov rbp, QWORD PTR [0x1000]
    mov QWORD PTR [rbp], rbp
    ret

.type caller, @function
caller:
    # rbp is a not return value of the caller (clobbered in callee)
    mov rbp, 42
    call callee
    ret
