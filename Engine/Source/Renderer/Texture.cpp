/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Renderer.h>
#include <Renderer/Texture.h>

#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    #include <Renderer/Platform/D3D11/D3D11Texture.h>
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11

namespace CaveGame
{

RefPtr<Texture> Texture::create(const TextureDescription& description)
{
    switch (Renderer::get_renderer_api())
    {
#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
        case RendererAPI::D3D11: return create_ref<D3D11Texture>(description).as<Texture>();
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    }

    CAVE_ASSERT(false);
    return {};
}

} // namespace CaveGame
