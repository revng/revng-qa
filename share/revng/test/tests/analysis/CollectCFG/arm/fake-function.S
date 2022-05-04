@
@ This file is distributed under the MIT License. See LICENSE.md for details.
@

    .syntax unified
    .type _start, "function"
    .global _start
_start:
    push {lr}
    bl prologue
    ldr r0, [r0]
    b epilogue

    .type prologue, "function"
prologue:
    push {r0}
    push {r1}
    bx lr

    .type epilogue, "function"
epilogue:
    pop {r1}
    pop {r0}
    pop {lr}
    bx lr
