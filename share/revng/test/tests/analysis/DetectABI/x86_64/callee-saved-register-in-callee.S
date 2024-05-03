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
    push rbp
    mov rbp, QWORD PTR [0x1000]
    mov QWORD PTR [rbp], rbp
    pop rbp
    ret

.type caller, @function
caller:
    # rbp is callee-saved register in the callee, therefore it must not be an
    # argument nor a return value of caller
    call callee
    ret
