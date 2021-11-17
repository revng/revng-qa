#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call draof
    ret

# rax is NoOrDead
.type draof, @function
draof:
    mov rax,42
    ret
