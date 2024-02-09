#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <ucontext.h>

#if defined(TARGET_x86_64)
#define ILLEGAL_INSTRUCTION_WORD 0x0b0f0b0f

#define REGISTER_COUNT NGREG
#define REGISTER(context, i) ((ucontext_t *) (context))->uc_mcontext.gregs[i]
#define SP(context) REGISTER(context, REG_RSP)
#define PC(context) REGISTER(context, REG_RIP)

const char *register_names[] = { "r8",     "r9",      "r10", "r11",    "r12",
                                 "r13",    "r14",     "r15", "rdi",    "rsi",
                                 "rbp",    "rbx",     "rdx", "rax",    "rcx",
                                 "rsp",    "rip",     "efl", "csgsfs", "err",
                                 "trapno", "oldmask", "cr2" };

#elif defined(TARGET_i386)
#define ILLEGAL_INSTRUCTION_WORD 0x0b0f0b0f

#define REGISTER_COUNT NGREG
#define REGISTER(context, i) ((ucontext_t *) (context))->uc_mcontext.gregs[i]
#define SP(context) REGISTER(context, REG_ESP)
#define PC(context) REGISTER(context, REG_EIP)

const char *register_names[] = { "gs",  "fs",  "es",     "ds",  "edi",
                                 "esi", "ebp", "esp",    "ebx", "edx",
                                 "ecx", "eax", "trapno", "err", "eip",
                                 "cs",  "efl", "uesp",   "ss" };

#elif defined(TARGET_aarch64)
#define ILLEGAL_INSTRUCTION_WORD 0xe7f0def0

#define REGISTER_COUNT (NGREG - 2)
#define REGISTER(context, i)                                     \
  (i == NGREG - 3 ? ((ucontext_t *) (context))->uc_mcontext.sp : \
                    ((ucontext_t *) (context))->uc_mcontext.regs[i])
#define SP(context) ((ucontext_t *) (context))->uc_mcontext.sp
#define PC(context) ((ucontext_t *) (context))->uc_mcontext.pc

const char *register_names[] = {
  "x0",  "x1",  "x2",  "x3",  "x4",  "x5",  "x6",  "x7",  "x8",  "x9",  "x10",
  "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21",
  "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "lr",  "sp"
};

#elif defined(TARGET_arm)
#define ILLEGAL_INSTRUCTION_WORD 0xe7f0def0

#define REGISTER_COUNT (NGREG - 2)
#define REGISTER(context, i) \
  *(((unsigned long *) &((ucontext_t *) (context))->uc_mcontext) + 3 + (i))
#define SP(context) ((ucontext_t *) (context))->uc_mcontext.arm_sp
#define PC(context) ((ucontext_t *) (context))->uc_mcontext.arm_pc

const char *register_names[] = { "r0",  "r1",  "r2",  "r3", "r4",  "r5",
                                 "r6",  "r7",  "r8",  "r9", "r10", "r11",
                                 "r12", "r13", "r14", "r15" };

#elif defined(TARGET_mips) || defined(TARGET_mipsel)
#define ILLEGAL_INSTRUCTION_WORD 0xfac0fac0

#include "bits/reg.h"
#define REGISTER_COUNT 32
#define REGISTER(context, i) ((ucontext_t *) (context))->uc_mcontext.gregs[i]
#define SP(context) REGISTER(context, 29)
#define PC(context) ((ucontext_t *) (context))->uc_mcontext.pc

const char *register_names[] = { "zero", "at", "v0", "v1", "a0", "a1", "a2",
                                 "a3",   "t0", "t1", "t2", "t3", "t4", "t5",
                                 "t6",   "t7", "s0", "s1", "s2", "s3", "s4",
                                 "s5",   "s6", "s7", "t8", "t9", "k0", "k1",
                                 "gp",   "sp", "fp", "ra" };

#elif defined(TARGET_s390x)
#define ILLEGAL_INSTRUCTION_WORD 0x00000000

#define REGISTER_COUNT 16
#define REGISTER(context, i) ((ucontext_t *) (context))->uc_mcontext.gregs[i]
#define SP(context) REGISTER(context, 15)
#define PC(context) ((ucontext_t *) (context))->uc_mcontext.psw

const char *register_names[] = { "r0",  "r1",  "r2",  "r3", "r4",  "r5",
                                 "r6",  "r7",  "r8",  "r9", "r10", "r11",
                                 "r12", "r13", "r14", "r15" };

#else
#error "Unsupported target architecture."
#endif
