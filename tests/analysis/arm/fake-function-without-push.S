@
@ This file is distributed under the MIT License. See LICENSE.md for details.
@

    .syntax unified
    .type _start, "function"
    .global _start
_start:
    push {lr}
    add sp,sp,-8
    bl prologue
    ldr r0, [r0]
    b epilogue

    .type prologue, "function"
prologue:
    str r1,[sp,0]
    str r0,[sp,4]
    bx lr

    .type epilogue, "function"
epilogue:
    pop {r1}
    pop {r0}
    pop {lr}
    bx lr
