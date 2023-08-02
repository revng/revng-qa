#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/**
 * This file contains a dirty trick. Ideally we'd want to automagically import
 * these values from the corresponding abi `yaml` files contained in revng, but
 * for now manually copying the values here will do.
 */

#define UNLIMITED 65536

#if defined(ABI_SystemV_x86_64)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 2
#define MaximumGPRsPerScalarReturnValue 2

#elif defined(ABI_Microsoft_x86_64)
#define MaximumGPRsPerAggregateArgument 1
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 1

#ifndef _MSC_VER
#define ABIDEF __attribute__((ms_abi))
#endif

#elif defined(ABI_SystemV_x86)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2

#elif defined(ABI_SystemV_x86_regparm_3)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2

#define ABIDEF __attribute__((regparm(3)))

#elif defined(ABI_SystemV_x86_regparm_2)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2

#define ABIDEF __attribute__((regparm(2)))

#elif defined(ABI_SystemV_x86_regparm_1)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2

#define ABIDEF __attribute__((regparm(1)))

#elif defined(ABI_Microsoft_x86_cdecl)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2

#ifndef _MSC_VER
#define ABIDEF __attribute__((ms_abi, cdecl))
#else
#define ABIDEF __cdecl
#endif

#elif defined(ABI_Microsoft_x86_fastcall)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2
#define CalleeIsResponsibleForStackCleanup

#ifndef _MSC_VER
#define ABIDEF __attribute__((ms_abi, fastcall))
#else
#define ABIDEF __fastcall
#endif

#elif defined(ABI_Microsoft_x86_stdcall)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2
#define CalleeIsResponsibleForStackCleanup

#ifndef _MSC_VER
#define ABIDEF __attribute__((ms_abi, stdcall))
#else
#define ABIDEF __stdcall
#endif

#elif defined(ABI_Microsoft_x86_thiscall)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 2
#define CalleeIsResponsibleForStackCleanup

#ifndef _MSC_VER
#error "GCC's implementation of `__thiscall` is severely flawed."
#else
#ifndef __cplusplus
#error "`__thiscall` is a c++-only ABI."
#else
#define ABIDEF __thiscall
#endif
#endif

#elif defined(ABI_Microsoft_x86_vectorcall)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 0
#define MaximumGPRsPerScalarReturnValue 1
#define CalleeIsResponsibleForStackCleanup

#ifndef _MSC_VER
#error "GCC doesn't implement `vectorcall`."
#else
#define ABIDEF __vectorcall
#endif

#elif defined(ABI_AAPCS64)
#define MaximumGPRsPerAggregateArgument 2
#define MaximumGPRsPerScalarArgument 2
#define MaximumGPRsPerAggregateReturnValue 2
#define MaximumGPRsPerScalarReturnValue 2

#elif defined(ABI_AAPCS)
#define MaximumGPRsPerAggregateArgument UNLIMITED
#define MaximumGPRsPerScalarArgument UNLIMITED
#define MaximumGPRsPerAggregateReturnValue 1
#define MaximumGPRsPerScalarReturnValue 4

#else
#error "Unknown ABI."
#endif

#ifndef ABIDEF
#define ABIDEF
#endif
