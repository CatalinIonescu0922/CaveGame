/*
 * Copyright (c) Bengulescu Robert 2024. All rights reserved.
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

namespace CaveGame
{

class Engine
{
public:
    bool initialize();
    void shutdown();

    void run();
};

bool initialize_core_systems();
void shutdown_core_systems();

} // namespace CaveGame
