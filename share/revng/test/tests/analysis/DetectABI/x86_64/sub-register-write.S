#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call function
    ret

# `sete dl` writes the low byte of rdx and leaves the rest alone, so the lifter
# reads rdx back in order to merge, and rdx looks read-before-written. It is
# not: `movzx edx, dl` throws the bits that were kept away, so rdi is the only
# argument.
.type function, @function
function:
    cmp rdi, 0
    sete dl
    movzx edx, dl
    mov QWORD PTR [0x2000], rdx
    ret
