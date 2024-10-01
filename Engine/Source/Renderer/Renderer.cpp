/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Renderer.h>

namespace CaveGame
{

struct RendererData
{
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

    delete s_renderer;
    s_renderer = nullptr;
}

RenderingContext& Renderer::get_rendering_context()
{
    CAVE_ASSERT(s_renderer && s_renderer->rendering_context.is_valid());
    return *s_renderer->rendering_context.get();
}

} // namespace CaveGame
