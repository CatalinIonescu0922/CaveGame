/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Assertion.h>

namespace CaveGame
{

void report_assertion_failed(const char* expression, const char* filename, const char* function, u32 line)
{
    // NOTE: Currently, there is nowhere to report the failed assertion information, thus
    // no operation is actually performed in this function.
}

} // namespace CaveGame
