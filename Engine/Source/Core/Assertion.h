/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/CoreTypes.h>

//
// The default assertion configurations states that:
//   - CAVE_ASSERT       is enabled in Debug and Development builds.
//   - CAVE_DEBUG_ASSERT is only enabled in Debug builds.
//   - CAVE_VERIFY       is enabled in any build.
//

#if CAVE_CONFIGURATION_DEBUG
    #define CAVE_ENABLE_ASSERTS       1
    #define CAVE_ENABLE_DEBUG_ASSERTS 1
    #define CAVE_ENABLE_VERIFIES      1
#endif // CAVE_CONFIGURATION_DEBUG

#if CAVE_CONFIGURATION_DEVELOPMENT
    #define CAVE_ENABLE_ASSERTS       0
    #define CAVE_ENABLE_DEBUG_ASSERTS 1
    #define CAVE_ENABLE_VERIFIES      1
#endif // CAVE_CONFIGURATION_DEBUG

#if CAVE_CONFIGURATION_SHIPPING
    #define CAVE_ENABLE_ASSERTS       0
    #define CAVE_ENABLE_DEBUG_ASSERTS 0
    #define CAVE_ENABLE_VERIFIES      1
#endif // CAVE_CONFIGURATION_DEBUG

namespace CaveGame
{

//
// Reports that an assertion has been triggerd, by writting (if possible) the relevant information
// to the console and the output log file.
//
void report_assertion_failed(const char* expression, const char* filename, const char* function, u32 line);

} // namespace CaveGame

#if CAVE_ENABLE_ASSERTS
    //
    // If the provided expression evaluates to false, `report_assertion_failed` is invoked and a debug break is triggered.
    // The expression is only evaluated if `CAVE_ENABLE_ASSERTS` is set to 1.
    //
    #define CAVE_ASSERT(expression)                                                              \
        if (!(expression))                                                                       \
        {                                                                                        \
            ::CaveGame::report_assertion_failed(#expression, __FILE__, CAVE_FUNCTION, __LINE__); \
            CAVE_DEBUGBREAK;                                                                     \
        }
#else
    #define CAVE_ASSERT(...)
#endif // CAVE_ENABLE_ASSERTS

#if CAVE_ENABLE_DEBUG_ASSERTS
    //
    // If the provided expression evaluates to false, `report_assertion_failed` is invoked and a debug break is triggered.
    // The expression is only evaluated if `CAVE_ENABLE_DEBUG_ASSERTS` is set to 1.
    //
    #define CAVE_DEBUG_ASSERT(expression)                                                        \
        if (!(expression))                                                                       \
        {                                                                                        \
            ::CaveGame::report_assertion_failed(#expression, __FILE__, CAVE_FUNCTION, __LINE__); \
            CAVE_DEBUGBREAK;                                                                     \
        }
#else
    #define CAVE_ENABLE_DEBUG_ASSERTS(...)
#endif // CAVE_ENABLE_DEBUG_ASSERTS

#if CAVE_ENABLE_VERIFIES
    //
    // If the provided expression evaluates to false, `report_assertion_failed` is invoked and a debug break is triggered.
    // The expression is only evaluated if `CAVE_ENABLE_VERIFIES` is set to 1.
    //
    #define CAVE_VERIFY(expression)                                                              \
        if (!(expression))                                                                       \
        {                                                                                        \
            ::CaveGame::report_assertion_failed(#expression, __FILE__, CAVE_FUNCTION, __LINE__); \
            CAVE_DEBUGBREAK;                                                                     \
        }
#else
    #define CAVE_VERIFY(...)
#endif // CAVE_ENABLE_VERIFIES
