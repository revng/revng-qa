#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call indirectcall
    ret

.type indirectcall, @function
indirectcall:
    push rbx
    call rbx
    pop rbx
    ret
