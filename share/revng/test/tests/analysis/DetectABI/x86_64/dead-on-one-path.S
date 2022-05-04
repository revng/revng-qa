#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call func
    ret

# In this function on one path rax is an argument, on the other one rax is
# dead. Therefore, rax is an argument
.type func, @function
func:
    jns use
    mov rbx,rax
use:
    mov rax,42
    ret
