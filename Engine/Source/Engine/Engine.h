/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Platform/Window.h>
#include <Engine/GameLoop.h>

namespace CaveGame
{

class Engine
{
public:
    static bool initialize();
    static void shutdown();

    template<typename GameLoopType>
    ALWAYS_INLINE static void run()
    {
        GameLoopType game_loop_instance = {};
        run(static_cast<GameLoop&>(game_loop_instance));
    }

public:
    //
    // Returns the pointer to the window instance.
    // If no window has been created (or has been destroyed) this function will return nullptr.
    //
    NODISCARD static Window& get_window();

private:
    static void run(GameLoop& game_loop);
};

bool initialize_core_systems();
void shutdown_core_systems();

} // namespace CaveGame
