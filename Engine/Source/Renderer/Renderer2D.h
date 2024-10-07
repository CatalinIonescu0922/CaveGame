/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/OwnPtr.h>
#include <Core/Math/Color.h>
#include <Core/Math/Vector.h>
#include <Renderer/Framebuffer.h>
#include <Renderer/IndexBuffer.h>
#include <Renderer/RenderPass.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexBuffer.h>

namespace CaveGame
{

class Renderer2D
{
    CAVE_MAKE_NONCOPYABLE(Renderer2D);
    CAVE_MAKE_NONMOVABLE(Renderer2D);

public:
    struct QuadVertex
    {
        Vector2 position;
        Color4 color;
    };

public:
    NODISCARD static OwnPtr<Renderer2D> create(RefPtr<Framebuffer> target_framebuffer);
    NODISCARD static OwnPtr<Renderer2D> create(u32 target_framebuffer_width, u32 target_framebuffer_height);

    ~Renderer2D();

public:
    void begin_frame();
    void end_frame();

    void submit_quad(Vector2 translation, Vector2 scale, Color4 color);

public:
    NODISCARD ALWAYS_INLINE u32 get_target_width() const { return m_target_framebuffer->get_width(); }
    NODISCARD ALWAYS_INLINE u32 get_target_height() const { return m_target_framebuffer->get_height(); }

private:
    explicit Renderer2D(RefPtr<Framebuffer> target_framebuffer);

    void initialize_quad_rendering();
    void shutdown_quad_rendering();
    void construct_quad(QuadVertex* destination_vertices, Vector2 translation, Vector2 scale, Color4 color);

    void begin_quad_batch();
    void end_quad_batch();
    void flush_quad_batch();

private:
    RefPtr<Framebuffer> m_target_framebuffer;

    u32 m_max_quads_per_batch { 0 };
    u32 m_current_number_of_quads { 0 };
    u32 m_total_number_of_quads { 0 };
    Vector<QuadVertex> m_quad_vertices;

    RefPtr<Shader> m_quad_shader;
    RefPtr<RenderPass> m_quad_render_pass;
    RefPtr<IndexBuffer> m_quad_index_buffer;
    RefPtr<VertexBuffer> m_quad_vertex_buffer;
};

} // namespace CaveGame
