@
@ This file is distributed under the MIT License. See LICENSE.md for details.
@

    .syntax unified
    .type _start, "function"
    .global _start
_start:
    push {lr}
    bl function
    pop {lr}
    bx lr

    .type function, "function"
function:
    push {r0}
    push {r1}
    add r1, sp, #4
    pop {r1}
    pop {r0}
    bx lr
