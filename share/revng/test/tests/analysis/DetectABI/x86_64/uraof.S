#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

# WIP: drop

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call uraof
    ret

# rax is an argument.
.type uraof, @function
uraof:
    mov rbx,rax
    ret
