#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

# WIP: drop

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call drvofc
    ret

# In this function call, rax is NoOrDead
.type drvofc, @function
drvofc:
    call rbx
    mov rax,42
    ret
