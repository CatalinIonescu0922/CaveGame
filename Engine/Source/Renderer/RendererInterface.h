/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/RendererAPI.h>

namespace CaveGame
{

class RendererInterface
{
    CAVE_MAKE_NONCOPYABLE(RendererInterface);
    CAVE_MAKE_NONMOVABLE(RendererInterface);

public:
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

    RendererInterface() = default;
    virtual ~RendererInterface() = default;
};

} // namespace CaveGame
