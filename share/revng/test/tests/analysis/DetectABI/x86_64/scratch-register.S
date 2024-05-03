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
    # rbp here is not callee-saved register, not an argument, not a return value
    mov rbp, QWORD PTR [0x1000]
    mov QWORD PTR [0x2000], rbp
    ret
