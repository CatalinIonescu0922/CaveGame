/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/RenderPass.h>
#include <Renderer/Renderer.h>

#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    #include <Renderer/Platform/D3D11/D3D11RenderPass.h>
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11

namespace CaveGame
{

RefPtr<RenderPass> RenderPass::create(const RenderPassDescription& description)
{
    switch (Renderer::get_renderer_api())
    {
        case RendererAPI::D3D11: return create_ref<D3D11RenderPass>(description).as<RenderPass>();
    }

    CAVE_ASSERT(false);
    return {};
}

} // namespace CaveGame
