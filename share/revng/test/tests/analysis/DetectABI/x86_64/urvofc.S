#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call urvofc
    ret

# rax is a return value
.type urvofc, @function
urvofc:
    call callee
    mov rbx,rax
    ret

.type callee, @function
callee:
    mov rax, 42
    mov rbx, rax
    ret
