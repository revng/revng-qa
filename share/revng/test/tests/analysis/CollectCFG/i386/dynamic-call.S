#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call use_plt
    ret

.type use_plt, @function
use_plt:
    push ebx
    call __x86.get_pc_thunk.bx
    add ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
    call strlen@PLT
    pop ebx
    ret

.type __x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
    mov ebx, DWORD PTR [esp]
    ret
