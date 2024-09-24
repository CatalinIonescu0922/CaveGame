/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

//======================================================================================
// PLATFORM CONFIGURATION MACROS.
//======================================================================================
//
// NOTE: The platform and configurations macros are set by the build system.
// It is the responsability of the build system to ensure that they are set correctly.
//

#ifndef CAVE_PLATFORM_WINDOWS
    #define CAVE_PLATFORM_WINDOWS 0
#endif // CAVE_PLATFORM_WINDOWS

//
// Ensure that at least one platform macro is set to 1.
// Otherwise, the project configuration is wrong and a compiler error should be raised.
//
#if !CAVE_PLATFORM_WINDOWS
    #error Unknown or unsupported platform!
#endif // Any supported platform.

//======================================================================================
// BUILD CONFIGURATION MACROS.
//======================================================================================

#ifndef CAVE_CONFIGURATION_DEBUG
    #define CAVE_CONFIGURATION_DEBUG 0
#endif // CAVE_CONFIGURATION_DEBUG

#ifndef CAVE_CONFIGURATION_DEVELOPMENT
    #define CAVE_CONFIGURATION_DEVELOPMENT 0
#endif // CAVE_CONFIGURATION_DEVELOPMENT

#ifndef CAVE_CONFIGURATION_SHIPPING
    #define CAVE_CONFIGURATION_SHIPPING 0
#endif // CAVE_CONFIGURATION_SHIPPING

//
// Ensure that at least one build configuration macro is set to 1.
// Otherwise, the project configuration is wrong and a compiler error should be raised.
//
#if !CAVE_CONFIGURATION_DEBUG && !CAVE_CONFIGURATION_DEVELOPMENT && !CAVE_CONFIGURATION_SHIPPING
    #error Unknown or unsupported build configuration!
#endif // Any supported build configuration.

//======================================================================================
// COMPILER CONFIGURATION MACROS.
//======================================================================================

// The `__clang__` is only set when compiling using clang.
#ifdef __clang__
    #define CAVE_COMPILER_CLANG 1
#else
    // The `_MSC_BUILD` is defined when compiling with MSVC, or sometimes when using clang. As we
    // already checked for the clang compiler, we can be confident that this flag is only set when
    // using MSVC.
    #ifdef _MSC_BUILD
        #define CAVE_COMPILER_MSVC 1
    #endif // _MSC_BUILD

    // Similarly to the `_MSC_BUILD` flag, `__gnuc__` is only set when compiling using GCC.
    #ifdef __GNUC__
        #define CAVE_COMPILER_GCC 1
    #endif // __GNUC__
#endif // __clang__

#ifndef CAVE_COMPILER_MSVC
    #define CAVE_COMPILER_MSVC 0
#endif // CAVE_COMPILER_MSVC

#ifndef CAVE_COMPILER_CLANG
    #define CAVE_COMPILER_CLANG 0
#endif // CAVE_COMPILER_CLANG

#ifndef CAVE_COMPILER_GCC
    #define CAVE_COMPILER_GCC 0
#endif // CAVE_COMPILER_GCC

//
// Ensure that at least one compiler macro is set to 1.
// Otherwise, an unknown or unsupported compiler is used, which should raise a compiler error.
//
#if !CAVE_COMPILER_MSVC && !CAVE_COMPILER_CLANG && !CAVE_COMPILER_GCC
    #error Unknown or unsupported compiler!
#endif // Any supported compiler.

//======================================================================================
// UTILITY (GENERAL PURPOSE) MACROS.
//======================================================================================

#if CAVE_COMPILER_MSVC
    // Hint for the compiler that the function should always be inlined.
    #define ALWAYS_INLINE __forceinline

    // Traps the debugger. Triggers a breakpoint if a debugger is attached or crashes the program otherwise.
    #define CAVE_DEBUGBREAK __debugbreak()

    // Expands to the signature of the function in which the macro is located.
    #define CAVE_FUNCTION __FUNCSIG__
#endif // CAVE_COMPILER_MSVC

// The compiler is encouraged to issue a warning if the function return value is not stored/used.
#define NODISCARD [[nodiscard]]

// Suppreses warnings on unused entities.
#define MAYBE_UNUSED [[maybe_unused]]

// Represent hints to the compiler that the path of execution is more or less likely than the alternative.
#define LIKELY   [[likely]]
#define UNLIKELY [[unlikely]]
