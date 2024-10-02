/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/CoreTypes.h>

#if CAVE_PLATFORM_WINDOWS
    #define CAVE_RENDERER_API_IS_AVAILABLE_D3D11  1
    #define CAVE_RENDERER_API_IS_AVAILABLE_D3D12  1
    #define CAVE_RENDERER_API_IS_AVAILABLE_VULKAN 1
    #define CAVE_RENDERER_API_IS_AVAILABLE_OPENGL 1
    #define CAVE_RENDERER_API_IS_AVAILABLE_METAL  0
#endif // CAVE_PLATFORM_WINDOWS

namespace CaveGame
{

//
// Enumeration of all renderer APIs implemented by the engine.
// Always check that the renderer API is available on the current platform before using it.
//
enum class RendererAPI : u8
{
    Unknown = 0,
    D3D11,
};

NODISCARD ALWAYS_INLINE bool is_renderer_api_available(RendererAPI renderer_api)
{
    switch (renderer_api)
    {
        case RendererAPI::Unknown: return true;

        case RendererAPI::D3D11:
#if CAVE_RENDERER_API_IS_AVAILABLE_D3D11
            return true;
#else
            return false;
#endif // CAVE_RENDERER_API_IS_AVAILABLE_D3D11
    }

    return false;
}

NODISCARD ALWAYS_INLINE RendererAPI get_recommended_renderer_api_for_current_platform()
{
#if CAVE_PLATFORM_WINDOWS
    return RendererAPI::D3D11;
#else
    return RendererAPI::Unknown;
#endif // CAVE_PLATFORM_WINDOWS
}

} // namespace CaveGame
