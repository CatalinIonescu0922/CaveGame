/*
 * Copyright (c) Bengulescu Robert 2024. All rights reserved.
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Engine/Engine.h>

namespace CaveGame
{

static int cave_game_main()
{
    if (!initialize_core_systems())
    {
        // Core systems initialization failed. Aborting.
        return 1;
    }

    Engine* engine = new Engine();
    if (!engine->initialize())
    {
        // Engine initialize failed. Aborting.
        return 1;
    }
    
    // Run game.
    engine->run();

    engine->shutdown();
    delete engine;

    shutdown_core_systems();
    return 0;
}

} // namespace CaveGame

int main()
{
    const int return_code = CaveGame::cave_game_main();
    return return_code;
}
