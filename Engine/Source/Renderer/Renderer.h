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
#include <Renderer/RenderingContext.h>
#include <Renderer/VertexBuffer.h>

namespace CaveGame
{

class Renderer
{
    CAVE_MAKE_NONCOPYABLE(Renderer);
    CAVE_MAKE_NONMOVABLE(Renderer);

public:
    static bool initialize(Window* window_context);
    static void shutdown();

    static void begin_frame();
    static void end_frame();

    static void begin_render_pass(RefPtr<RenderPass> render_pass);
    static void end_render_pass();

    static void draw_indexed(RefPtr<VertexBuffer> vertex_buffer, RefPtr<IndexBuffer> index_buffer, u32 indices_count);

public:
    static RendererAPI get_renderer_api();

    static RenderingContext& get_rendering_context();

private:
    Renderer() = default;
    ~Renderer() = default;
};

} // namespace CaveGame
