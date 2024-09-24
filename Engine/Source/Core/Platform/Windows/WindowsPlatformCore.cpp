/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Assertion.h>
#include <Core/Platform/PlatformCore.h>
#include <Core/Platform/Windows/WindowsGuardedInclude.h>

namespace CaveGame
{

u64 PlatformCore::get_current_tick_counter()
{
    LARGE_INTEGER tick_counter;
    if (!QueryPerformanceCounter(&tick_counter))
    {
        // For some reason, the `QueryPerformanceCounter` call failed.
        CAVE_ASSERT(false);
        return 0;
    }

    return tick_counter.QuadPart;
}

//
// The performance counter frequency is fixed at boot time and thus its value can be cached.
// https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
//
static u64 s_tick_counter_frequency = 0;

u64 PlatformCore::get_tick_counter_frequency()
{
    if (s_tick_counter_frequency == 0)
    {
        LARGE_INTEGER tick_counter_frequency;
        if (!QueryPerformanceFrequency(&tick_counter_frequency))
        {
            // For some reason, the `QueryPerformanceFrequency` call failed.
            CAVE_ASSERT(false);
            return 0;
        }

        s_tick_counter_frequency = tick_counter_frequency.QuadPart;
    }

    CAVE_ASSERT(s_tick_counter_frequency != 0);
    return s_tick_counter_frequency;
}

} // namespace CaveGame
