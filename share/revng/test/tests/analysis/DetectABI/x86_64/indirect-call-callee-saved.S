#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call indirect_caller
    ret

.type indirect_caller, @function
indirect_caller:
    push r12
    call rax
    pop r12
    ret
