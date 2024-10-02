/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/Platform/D3D11/D3D11Framebuffer.h>
#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>
#include <Renderer/RenderingContext.h>

namespace CaveGame
{

class D3D11RenderingContext : public RenderingContext
{
public:
    D3D11RenderingContext(Window* window_context);
    virtual ~D3D11RenderingContext() override;

public:
    virtual void invalidate_swapchain(u32 new_width, u32 new_height) override;

    ALWAYS_INLINE NODISCARD virtual u32 get_swapchain_width() const override { return m_swapchain.width; }
    ALWAYS_INLINE NODISCARD virtual u32 get_swapchain_height() const override { return m_swapchain.height; }
    NODISCARD virtual ImageFormat get_swapchain_image_format() const override;

    NODISCARD virtual void* get_swapchain_image(u32 image_index = 0) const override;
    NODISCARD virtual void* get_swapchain_image_view(u32 image_index = 0) const override;

    void reference_swapchain_target_framebuffer(D3D11Framebuffer* framebuffer);
    void dereference_swapchain_target_framebuffer(D3D11Framebuffer* framebuffer);

private:
    void destroy_swapchain();

private:
    struct Swapchain
    {
        IDXGISwapChain1* handle { nullptr };
        ID3D11Texture2D* image_handle { nullptr };
        ID3D11RenderTargetView* image_view_handle { nullptr };

        u32 width { 0 };
        u32 height { 0 };
        DXGI_FORMAT format { DXGI_FORMAT_UNKNOWN };
        u32 image_count { 0 };

        Vector<D3D11Framebuffer*> referenced_framebuffers;
    };

    Window* m_window_context;
    Swapchain m_swapchain;
};

} // namespace CaveGame
