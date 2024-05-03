#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call function
    ret

.type function, @function
function:
    # rbp is a callee-saved register
    push rbp
    mov rbp, QWORD PTR [0x1000]
    mov QWORD PTR [rbp], rbp
    pop rbp
    ret
