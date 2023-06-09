#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.info:
    .string "test"

.intel_syntax noprefix
.type _start, @function
.global main
main:
    mov rdi, offset flat:.info
    call puts
    xor rax, rax
    ret
