/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Containers/Optional.h>
#include <Core/Platform/Window.h>
#include <Renderer/Framebuffer.h>
#include <Renderer/RenderingContext.h>

namespace CaveGame
{

OwnPtr<RenderingContext> RenderingContext::create(Window* window_context)
{
    RenderingContext* rendering_context = new RenderingContext(window_context);
    return adopt_own(rendering_context);
}

RenderingContext::RenderingContext(Window* window_context)
    : m_window_context(window_context)
    , m_device(nullptr)
    , m_device_context(nullptr)
{
    UINT device_creation_flags = 0;
#if CAVE_CONFIGURATION_DEBUG
    // Enable the debug flag in the device configuration when compiling to a debug configuration.
    device_creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // CAVE_CONFIGURATION_DEBUG

    const D3D_FEATURE_LEVEL feature_levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    D3D_FEATURE_LEVEL selected_feature_level = {};

    const HRESULT device_creation_result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        device_creation_flags,
        feature_levels,
        ARRAY_COUNT(feature_levels),
        D3D11_SDK_VERSION,
        &m_device,
        &selected_feature_level,
        &m_device_context
    );

    // NOTE: There is no point in trying to continue running the engine if the D3D11 device can't be created.
    // Exiting the application (by forcing to crash in this case) will be the end result anyway.
    CAVE_VERIFY(SUCCEEDED(device_creation_result));

    const HRESULT dxgi_factory_creation_result = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgi_factory));
    CAVE_VERIFY(SUCCEEDED(dxgi_factory_creation_result));

    // Set the swapchain immutable configuration parameters.
    m_swapchain.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    m_swapchain.image_count = 2;

    // Create the swapchain.
    invalidate_swapchain(window_context->get_client_area_width(), window_context->get_client_area_height());
}

RenderingContext::~RenderingContext()
{
    // NOTE: If there are still framebuffers alive it means the rendering context shouldn't be deleted, thus
    // is represents an invalid state. Continuing the execution will cause crashes in unexpected places.
    CAVE_VERIFY(m_swapchain.referenced_framebuffers.is_empty());
    destroy_swapchain();

    CAVE_D3D11_RELEASE(m_dxgi_factory);
    CAVE_D3D11_RELEASE(m_device_context);
    CAVE_D3D11_RELEASE(m_device);
}

void RenderingContext::invalidate_swapchain(u32 new_width, u32 new_height)
{
    destroy_swapchain();

    m_swapchain.width = new_width;
    m_swapchain.height = new_height;

    //
    // The specification of the window associated swapchain.
    // https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_2/ns-dxgi1_2-dxgi_swap_chain_desc1
    //
    DXGI_SWAP_CHAIN_DESC1 swapchain_description = {};
    swapchain_description.Width = static_cast<UINT>(m_swapchain.width);
    swapchain_description.Height = static_cast<UINT>(m_swapchain.height);
    swapchain_description.Format = m_swapchain.format;
    swapchain_description.SampleDesc.Count = 1;
    swapchain_description.SampleDesc.Quality = 0;
    swapchain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_description.BufferCount = m_swapchain.image_count;
    swapchain_description.Scaling = DXGI_SCALING_STRETCH;
    swapchain_description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    // https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_2/ns-dxgi1_2-dxgi_swap_chain_fullscreen_desc
    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchain_fullscreen_description = {};
    swapchain_fullscreen_description.Scaling = DXGI_MODE_SCALING_CENTERED;
    swapchain_fullscreen_description.Windowed = true;

    const HRESULT swapchain_creation_result = m_dxgi_factory->CreateSwapChainForHwnd(
        m_device,
        static_cast<HWND>(m_window_context->get_native_handle()),
        &swapchain_description,
        &swapchain_fullscreen_description,
        nullptr,
        &m_swapchain.handle
    );

    // NOTE: There is no point in trying to continue running the engine if the swapchain creation failed, as there will
    // be no context where the graphics will be rendered to. In the end, the program will exit (or crash) anyway.
    CAVE_VERIFY(SUCCEEDED(swapchain_creation_result));

    const HRESULT swapchain_acquire_image_result = m_swapchain.handle->GetBuffer(0, IID_PPV_ARGS(&m_swapchain.image_handle));
    CAVE_VERIFY(SUCCEEDED(swapchain_acquire_image_result));

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_render_target_view_desc
    D3D11_RENDER_TARGET_VIEW_DESC render_target_view_description = {};
    render_target_view_description.Format = m_swapchain.format;
    render_target_view_description.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    const HRESULT swapchain_image_create_rtv_result =
        m_device->CreateRenderTargetView(m_swapchain.image_handle, &render_target_view_description, &m_swapchain.image_view_handle);
    CAVE_VERIFY(SUCCEEDED(swapchain_image_create_rtv_result));

    for (Framebuffer* referenced_framebuffer : m_swapchain.referenced_framebuffers)
    {
        // Invalidate all referenced framebuffers.
        referenced_framebuffer->invalidate(0, 0);
    }
}

void RenderingContext::destroy_swapchain()
{
    // Destroy all framebuffers that are referenced by the swapchain.
    for (Framebuffer* referenced_framebuffer : m_swapchain.referenced_framebuffers)
        referenced_framebuffer->destroy();

    CAVE_D3D11_RELEASE(m_swapchain.image_view_handle)
    CAVE_D3D11_RELEASE(m_swapchain.image_handle)
    CAVE_D3D11_RELEASE(m_swapchain.handle);

    m_swapchain.width = 0;
    m_swapchain.height = 0;
}

ImageFormat RenderingContext::get_swapchain_image_format() const
{
    switch (m_swapchain.format)
    {
        case DXGI_FORMAT_UNKNOWN: return ImageFormat ::Unknown;
        case DXGI_FORMAT_B8G8R8A8_UNORM: return ImageFormat::B8G8R8A8;
    }

    CAVE_ASSERT(false);
    return ImageFormat::Unknown;
}

ID3D11Texture2D* RenderingContext::get_swapchain_image(u32 image_index /*= 0*/) const
{
    CAVE_ASSERT(image_index == 0);
    return m_swapchain.image_handle;
}

ID3D11RenderTargetView* RenderingContext::get_swapchain_image_view(u32 image_index /*= 0*/) const
{
    CAVE_ASSERT(image_index == 0);
    return m_swapchain.image_view_handle;
}

void RenderingContext::reference_swapchain_target_framebuffer(Framebuffer& framebuffer)
{
    for (const Framebuffer* referenced_framebuffer : m_swapchain.referenced_framebuffers)
    {
        // The provided framebuffer is already referenced.
        if (referenced_framebuffer == &framebuffer)
            return;
    }

    // Add the framebuffer to the referenced list.
    m_swapchain.referenced_framebuffers.add(&framebuffer);
}

void RenderingContext::dereference_swapchain_target_framebuffer(Framebuffer& framebuffer)
{
    Optional<usize> framebuffer_index;

    for (usize index = 0; index < m_swapchain.referenced_framebuffers.count(); ++index)
    {
        // The provided framebuffer is already referenced.
        if (m_swapchain.referenced_framebuffers[index] == &framebuffer)
        {
            framebuffer_index = index;
            break;
        }
    }

    if (!framebuffer_index.has_value())
    {
        // The provided framebuffer is not referenced. Maybe this should raise an error?
        return;
    }

    // Remove the framebuffer from the referenced list.
    m_swapchain.referenced_framebuffers.remove_unordered(framebuffer_index.value());
}

} // namespace CaveGame
