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
    mov rbp, QWORD PTR [0x1000]
    call rbp

    # rax is read from the call site
    mov QWORD PTR [0x1000], rax
    ret
