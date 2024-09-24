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

class GameLoop
{
public:
    virtual bool on_game_start() { return true; }
    
    virtual void on_game_end() {}

    virtual void on_game_update(float delta_time) = 0;

public:
    NODISCARD ALWAYS_INLINE bool is_running() const { return m_is_running; }
    ALWAYS_INLINE void stop_running() { m_is_running = false; }

private:
    bool m_is_running { true };
};

} // namespace CaveGame
