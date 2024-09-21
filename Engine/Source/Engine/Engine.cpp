/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Engine/Engine.h>

namespace CaveGame
{

bool Engine::initialize()
{
    return true;
}

void Engine::shutdown()
{}

void Engine::run()
{}

bool initialize_core_systems()
{
    return true;
}

void shutdown_core_systems()
{}

} // namespace CaveGame
