/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Renderer.h>
#include <Renderer/RenderingContext.h>

#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    #include <Renderer/Platform/D3D11/D3D11RenderingContext.h>
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11

namespace CaveGame
{

OwnPtr<RenderingContext> RenderingContext::create(Window* window_context)
{
    switch (Renderer::get_renderer_api())
    {
        case RendererAPI::D3D11: return create_own<D3D11RenderingContext>(window_context).as<RenderingContext>();
    }

    CAVE_ASSERT(false);
    return {};
}

} // namespace CaveGame
