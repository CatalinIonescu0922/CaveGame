/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>
#include <Renderer/RenderingContext.h>

namespace CaveGame
{

class Renderer
{
    CAVE_MAKE_NONCOPYABLE(Renderer);
    CAVE_MAKE_NONMOVABLE(Renderer);

public:
    static bool initialize(Window* window_context);
    static void shutdown();

public:
    static RenderingContext& get_rendering_context();

private:
    Renderer() = default;
    ~Renderer() = default;
};

} // namespace CaveGame
