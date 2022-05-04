@
@ This file is distributed under the MIT License. See LICENSE.md for details.
@

    .syntax unified
    .type _start, "function"
    .global _start
_start:
    push {lr}
    bl indirectcall
    pop {lr}
    bx lr

    .type indirectcall, "function"
indirectcall:
    push {lr}
    push {r0}
    blx r0
    pop {r0}
    pop {lr}
    bx lr
