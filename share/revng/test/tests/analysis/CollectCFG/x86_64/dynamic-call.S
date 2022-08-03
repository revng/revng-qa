#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call use_plt
    call do_not_use_plt
    ret

.type add2, @function
add2:
    add rdi, 2
    mov rax, rdi
    ret

.type use_plt, @function
use_plt:
    call strlen@PLT
    mov rdi, rax
    call add2
    ret

.type do_not_use_plt, @function
do_not_use_plt:
    mov rax, QWORD PTR strlen@GOTPCREL[rip]
    call rax
    mov rdi, rax
    call add2
    ret
