#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <ucontext.h>

#if defined(TARGET_x86_64)
#define ILLEGAL_INSTRUCTION_WORD 0x0b0f0b0f

#define SP(context) (((ucontext_t *) (context))->uc_mcontext.gregs[REG_RSP])

// `uc_mcontext.gregs[]` contains REG_R8 .. REG_CR2.
const void *capture_gpr(void *context, uint32_t index) {
  return &((ucontext_t *) context)->uc_mcontext.gregs[index];
}

// zmm0..zmm15 and st0..st7 live in the FXSAVE area that `fpregs` points at,
// which is NULL on a thread that never touched the FPU.
//
// Only the low 128 bits of a zmm register are in this format: the kernel's
// signal frame saves legacy FXSAVE, not XSAVE, so the upper lanes are not
// available and the float tests stay within that width.
//
// But, luckily, we don't need the upper lanes to check `float` and `double`
// arguments. 128 bits is plenty.
static inline ucontext_t *x86_fp_context(void *context) {
  ucontext_t *ucontext = context;
  assert(ucontext->uc_mcontext.fpregs != NULL
         && "the kernel saved no FXSAVE area");
  return ucontext;
}

const void *capture_zmm(void *context, uint32_t index) {
  return &x86_fp_context(context)->uc_mcontext.fpregs->_xmm[index];
}

const void *capture_st(void *context, uint32_t index) {
  return &x86_fp_context(context)->uc_mcontext.fpregs->_st[index];
}

#elif defined(TARGET_i386)
#define ILLEGAL_INSTRUCTION_WORD 0x0b0f0b0f

#define SP(context) (((ucontext_t *) (context))->uc_mcontext.gregs[REG_ESP])

// `uc_mcontext.gregs[]` is the kernel's REG_GS .. REG_SS array.
//
// But we do not care about everything it exposes. We can start from EDI
// and avoid writing unnecessary stuff.
#define I386_GPR_FIRST_INDEX REG_EDI

const void *capture_gpr(void *context, uint32_t index) {
  return &((ucontext_t *) context)
            ->uc_mcontext.gregs[I386_GPR_FIRST_INDEX + index];
}

// i386 floating point register capture does not work out of the box, so
// let's keep it out for now. With it being out of scope and such.

#elif defined(TARGET_aarch64)
#define ILLEGAL_INSTRUCTION_WORD 0xe7f0def0

#define SP(context) ((ucontext_t *) (context))->uc_mcontext.sp

const void *capture_gpr(void *context, uint32_t index) {
  return &((ucontext_t *) context)->uc_mcontext.regs[index];
}

// v0..v31 are not a field of `mcontext_t`: the kernel appends the FPSIMD
// state to the extended context area that `__reserved` points at, as a chain
// of `{ magic, size }` records, one of which carries the registers.
//
// SVE and ESR records can precede the FPSIMD one, so the chain is walked
// rather than assumed to start with it.

// This is an arbitrary limit, just in case. There is supposed to be
// a zero-record delimiting the end of the chain, but in case something goes
// wrong, it is nice to have a hard-stop so we don't spin forever / try to read
// something where we are not supposed to.
#define AARCH64_EXTENDED_CONTEXT_BYTES (256 * sizeof(long double))

static inline const struct fpsimd_context *
aarch64_fpsimd(const ucontext_t *context) {
  const unsigned char *const area = (const unsigned char *)
                                      context->uc_mcontext.__reserved;
  for (size_t offset = 0; offset + sizeof(struct _aarch64_ctx)
                          <= AARCH64_EXTENDED_CONTEXT_BYTES;) {
    const struct _aarch64_ctx
      *record = (const struct _aarch64_ctx *) (area + offset);
    if (record->size == 0)
      break;
    if (record->magic == FPSIMD_MAGIC)
      return (const struct fpsimd_context *) record;
    offset += record->size;
  }

  assert(0 && "unable to find the FPSIMD record");
}

// NOTE that I'm skipping the SVE registers for now, because they are not
// really used as arguments/return-values.

const void *capture_v(void *context, uint32_t index) {
  return &aarch64_fpsimd((ucontext_t *) context)->vregs[index];
}

#elif defined(TARGET_arm)
#define ILLEGAL_INSTRUCTION_WORD 0xe7f0def0

#define SP(context) ((ucontext_t *) (context))->uc_mcontext.arm_sp

// uClibc's `mcontext_t` is the plain `struct sigcontext`, whose first three
// words (trap_no, error_code, oldmask) sit in front of arm_r0. Skip them.
#define ARM_GPR_FIRST_INDEX 3

const void *capture_gpr(void *context, uint32_t index) {
  return &((unsigned long *) &((ucontext_t *) context)
             ->uc_mcontext)[ARM_GPR_FIRST_INDEX + index];
}

// q0..q15 are the 16 VFP/NEON quad registers. `mcontext_t` has no `vfp`
// pointer to follow the way glibc's has, but `uc_regspace` is where the
// kernel puts the VFP sigframe.
//
// Similarly to arm64, we need to find the corresponding magic header to know
// what to capture.

#define ARM_VFP_MAGIC 0x56465001
struct arm_vfp_sigframe {
  unsigned long magic;
  unsigned long size;
  unsigned long long fpregs[32]; /* d0 .. d31 */
  unsigned long fpscr;
};

static inline const struct arm_vfp_sigframe *
find_arm_vfp_sigframe(const ucontext_t *context) {
  const unsigned char *const area = (const unsigned char *)
                                      context->uc_regspace;
  for (size_t offset = 0;
       offset + sizeof(struct arm_vfp_sigframe) <= sizeof(context->uc_regspace);
       offset += __alignof__(struct arm_vfp_sigframe)) {
    const struct arm_vfp_sigframe
      *frame = (const struct arm_vfp_sigframe *) (area + offset);
    if (frame->magic == ARM_VFP_MAGIC
        && frame->size >= sizeof(struct arm_vfp_sigframe))
      return frame;
  }

  assert(0 && "unable to find the VFP record");
}

const void *capture_q(void *context, uint32_t index) {
  return &find_arm_vfp_sigframe((ucontext_t *) context)->fpregs[2 * index];
}

#elif defined(TARGET_mips) || defined(TARGET_mipsel)
#define ILLEGAL_INSTRUCTION_WORD 0xfac0fac0

// `uc_mcontext.gregs[]` is the kernel's $zero .. $ra array, in 64-bit slots,
// while a register is 32 bits: a register is the low half of its slot, which
// is the half at the higher address on a big-endian target. That is why the
// offset depends on the byte order rather than being zero everywhere.

// $zero and $at are not reported, so the first reported register -- $v0 -- is
// gregs[2].
#define MIPS_GPR_FIRST_INDEX 2

static inline const unsigned char *mips_gpr_slot(const ucontext_t *context,
                                                 uint32_t index) {
  const unsigned char *const slot = (const unsigned char *) &context
                                      ->uc_mcontext
                                      .gregs[MIPS_GPR_FIRST_INDEX + index];
  return slot + (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ ? 4 : 0);
}

// `$sp` is `gregs[29]`, so 27 with the offset above.
#define SP(context) \
  (*((const unsigned long *) mips_gpr_slot((ucontext_t *) (context), 27)))

const void *capture_gpr(void *context, uint32_t index) {
  return mips_gpr_slot((ucontext_t *) context, index);
}

// `f0-f31` are presented in a dedicated array, so no complexity here, just
// read them as they are.
const void *capture_f(void *context, uint32_t index) {
  return &((ucontext_t *) context)->uc_mcontext.fpregs[index];
}

#elif defined(TARGET_s390x)
#define ILLEGAL_INSTRUCTION_WORD 0x00000000

#define SP(context) (((ucontext_t *) (context))->uc_mcontext.gregs[15])

// Dedicated arrays here too. Just read them directly.

const void *capture_gpr(void *context, uint32_t index) {
  return &((ucontext_t *) context)->uc_mcontext.gregs[index];
}

const void *capture_f(void *context, uint32_t index) {
  return &((ucontext_t *) context)->uc_mcontext.fpregs.fprs[index];
}

#else
#error "Unsupported target architecture."
#endif

// The generated register list table refers to the functions above by name,
// as such, this `include` must be the last thing in this file.
#include "registers.h"
