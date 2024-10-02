/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Framebuffer.h>
#include <Renderer/Renderer.h>

#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    #include <Renderer/Platform/D3D11/D3D11Framebuffer.h>
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11

namespace CaveGame
{

RefPtr<Framebuffer> Framebuffer::create(const FramebufferDescription& description)
{
    switch (Renderer::get_renderer_api())
    {
        case RendererAPI::D3D11: return create_ref<D3D11Framebuffer>(description).as<Framebuffer>();
    }

    CAVE_ASSERT(false);
    return {};
}

RefPtr<Framebuffer> Framebuffer::create(RenderingContext& rendering_context)
{
    switch (Renderer::get_renderer_api())
    {
        case RendererAPI::D3D11: return create_ref<D3D11Framebuffer>(rendering_context).as<Framebuffer>();
    }

    CAVE_ASSERT(false);
    return {};
}

} // namespace CaveGame
