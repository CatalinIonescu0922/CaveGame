/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Platform/Timer.h>
#include <Engine/Engine.h>
#include <Renderer/Renderer.h>

namespace CaveGame
{

struct EngineData
{
    Window window;
};

static EngineData* s_engine;

bool Engine::initialize()
{
    if (s_engine)
    {
        // The engine has already been initialized.
        return false;
    }

    // Allocate the memory for the engine structure.
    s_engine = new EngineData();

    if (!s_engine->window.initialize())
    {
        // NOTE: If the window creation fails there is no point in continuing the program execution.
        // Without a window, the game is definetely unplayable.
        return false;
    }

    if (!Renderer::initialize(&s_engine->window))
    {
        // The renderer system initialization has failed.
        return false;
    }

    return true;
}

void Engine::shutdown()
{
    if (!s_engine)
    {
        // The engine has already been shut down.
        return;
    }

    Renderer::shutdown();
    s_engine->window.shutdown();

    delete s_engine;
    s_engine = nullptr;
}

void Engine::run(GameLoop& game_loop)
{
    if (!game_loop.on_game_start())
    {
        // The game start function requested the application to exit.
        return;
    }

    // NOTE: For the first frame assume that the game runs at 60FPS.
    // Running the update function with a delta time of 0 might cause errors.
    float last_frame_delta_time = 1.0F / 60.0F;

    while (game_loop.is_running())
    {
        Timer frame_timer;

        s_engine->window.process_event_queue();
        if (s_engine->window.should_close())
        {
            game_loop.stop_running();
            continue;
        }

        Renderer::begin_frame();
        game_loop.on_game_update(last_frame_delta_time);
        Renderer::end_frame();

        last_frame_delta_time = frame_timer.stop_and_get_elapsed_seconds();
    }

    game_loop.on_game_end();
}

Window& Engine::get_window()
{
    CAVE_ASSERT(s_engine);
    return s_engine->window;
}

bool initialize_core_systems()
{
    return true;
}

void shutdown_core_systems()
{}

} // namespace CaveGame
