/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/OwnPtr.h>
#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>

namespace CaveGame
{

// Forward declarations.
class Window;

class RenderingContext
{
    CAVE_MAKE_NONCOPYABLE(RenderingContext);
    CAVE_MAKE_NONMOVABLE(RenderingContext);

public:
    NODISCARD static OwnPtr<RenderingContext> create(Window* window_context);
    ~RenderingContext();

    void invalidate_swapchain(u32 new_width, u32 new_height);

    NODISCARD ALWAYS_INLINE ID3D11Device* get_device() const { return m_device; }
    NODISCARD ALWAYS_INLINE ID3D11DeviceContext* get_device_context() const { return m_device_context; }

private:
    RenderingContext(Window* window_context);

    void destroy_swapchain();

private:
    Window* m_window_context;
    
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_device_context;
    IDXGIFactory2* m_dxgi_factory;

    struct Swapchain
    {
        IDXGISwapChain1* handle { nullptr };
        ID3D11Texture2D* image_handle { nullptr };
        ID3D11RenderTargetView* image_view_handle { nullptr };

        u32 width { 0 };
        u32 height { 0 };
        DXGI_FORMAT format { DXGI_FORMAT_UNKNOWN };
        u32 image_count { 0 };
    };

    Swapchain m_swapchain;
};

} // namespace CaveGame
