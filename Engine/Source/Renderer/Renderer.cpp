/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Renderer.h>
#include <Renderer/RendererInterface.h>

#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    #include <Renderer/Platform/D3D11/D3D11Renderer.h>
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11

namespace CaveGame
{

struct RendererData
{
    OwnPtr<RendererInterface> renderer_interface;
    OwnPtr<RenderingContext> rendering_context;
};

static RendererData* s_renderer;

bool Renderer::initialize(Window* window_context)
{
    if (s_renderer)
    {
        // The renderer system has already been initialized.
        return false;
    }

    s_renderer = new RendererData();
    CAVE_VERIFY(s_renderer != nullptr);

    // Create the renderer interface (D3D11).
    CAVE_ASSERT(get_renderer_api() == RendererAPI::D3D11);
    s_renderer->renderer_interface = create_own<D3D11Renderer>().as<RendererInterface>();
    if (!s_renderer->renderer_interface->initialize())
    {
        // The renderer interface initialization has failed.
        return false;
    }

    // Create the rendering (D3D11) context.
    s_renderer->rendering_context = RenderingContext::create(window_context);

    return true;
}

void Renderer::shutdown()
{
    if (!s_renderer)
    {
        // The renderer system has already been shut down or was never initialized.
        return;
    }

    s_renderer->rendering_context.release();

    s_renderer->renderer_interface->shutdown();
    s_renderer->renderer_interface.release();

    delete s_renderer;
    s_renderer = nullptr;
}

void Renderer::begin_frame()
{
    s_renderer->renderer_interface->begin_frame();
}

void Renderer::end_frame()
{
    s_renderer->renderer_interface->end_frame();
}

void Renderer::begin_render_pass(RefPtr<RenderPass> render_pass)
{
    s_renderer->renderer_interface->begin_render_pass(render_pass);
}

void Renderer::end_render_pass()
{
    s_renderer->renderer_interface->end_render_pass();
}

void Renderer::draw_indexed(RefPtr<VertexBuffer> vertex_buffer, RefPtr<IndexBuffer> index_buffer, u32 indices_count)
{
    CAVE_ASSERT(s_renderer->renderer_interface.is_valid());
    s_renderer->renderer_interface->draw_indexed(vertex_buffer, index_buffer, indices_count);
}

void Renderer::bind_input_texture(RefPtr<Texture> texture, u32 bind_slot_index)
{
    CAVE_ASSERT(s_renderer->renderer_interface.is_valid());
    s_renderer->renderer_interface->bind_input_texture(texture, bind_slot_index);
}

RendererAPI Renderer::get_renderer_api()
{
    const RendererAPI recommended_renderer_api = get_recommended_renderer_api_for_current_platform();
    CAVE_ASSERT(is_renderer_api_available(recommended_renderer_api));
    return recommended_renderer_api;
}

RenderingContext& Renderer::get_rendering_context()
{
    CAVE_ASSERT(s_renderer && s_renderer->rendering_context.is_valid());
    return *s_renderer->rendering_context.get();
}

} // namespace CaveGame
