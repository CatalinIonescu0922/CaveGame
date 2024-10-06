/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/IndexBuffer.h>
#include <Renderer/Renderer.h>

#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    #include <Renderer/Platform/D3D11/D3D11IndexBuffer.h>
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11

namespace CaveGame
{

NODISCARD RefPtr<IndexBuffer> IndexBuffer::create(const IndexBufferDescription& description)
{
    switch (Renderer::get_renderer_api())
    {
#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
        case RendererAPI::D3D11: return create_ref<D3D11IndexBuffer>(description).as<IndexBuffer>();
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    }

    CAVE_ASSERT(false);
    return {};
}

} // namespace CaveGame
