/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/CoreTypes.h>

namespace CaveGame
{

class PlatformCore
{
public:
    // Returns the value of the performance counter at the moment when this function is called.
    static u64 get_current_tick_counter();

    // Returns the frequency of the performance counter, measured in ticks per second.
    static u64 get_tick_counter_frequency();
};

} // namespace CaveGame
