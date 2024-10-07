/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/IndexBuffer.h>
#include <Renderer/RenderPass.h>
#include <Renderer/RendererAPI.h>
#include <Renderer/VertexBuffer.h>

namespace CaveGame
{

class RendererInterface
{
    CAVE_MAKE_NONCOPYABLE(RendererInterface);
    CAVE_MAKE_NONMOVABLE(RendererInterface);

public:
    RendererInterface() = default;
    virtual ~RendererInterface() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;

    virtual void begin_render_pass(RefPtr<RenderPass> render_pass) = 0;
    virtual void end_render_pass() = 0;

public:
    virtual void draw_indexed(RefPtr<VertexBuffer> vertex_buffer, RefPtr<IndexBuffer> index_buffer, u32 indices_count) = 0;
};

} // namespace CaveGame
