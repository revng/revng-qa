#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

.intel_syntax noprefix
.type _start, @function
.global _start
_start:
    call helper
    ret

.type helper, @function
helper:
    # This instruction calls an helper that writes rax. After that, rax it will
    # be read, so we want to make sure that rax is not accidentally considered
    # an argument
    bsr rax, rbx
    ret
