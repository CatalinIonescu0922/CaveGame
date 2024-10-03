/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Assertion.h>
#include <Renderer/Platform/D3D11/D3D11RenderPass.h>
#include <Renderer/Platform/D3D11/D3D11Renderer.h>
#include <Renderer/Platform/D3D11/D3D11RenderingContext.h>
#include <Renderer/Renderer.h>

namespace CaveGame
{

struct D3D11RendererData
{
    ID3D11Device* device { nullptr };
    ID3D11DeviceContext* device_context { nullptr };
    IDXGIFactory2* dxgi_factory { nullptr };
};

static D3D11RendererData* s_d3d11_renderer;

bool D3D11Renderer::initialize()
{
    if (s_d3d11_renderer)
    {
        // The D3D11 renderer has already been initialized.
        return false;
    }

    // Allocate the D3D11 renderer system structure.
    s_d3d11_renderer = new D3D11RendererData();

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
        &s_d3d11_renderer->device,
        &selected_feature_level,
        &s_d3d11_renderer->device_context
    );

    // NOTE: There is no point in trying to continue running the engine if the D3D11 device can't be created.
    // Exiting the application (by forcing to crash in this case) will be the end result anyway.
    CAVE_VERIFY(SUCCEEDED(device_creation_result));

    const HRESULT dxgi_factory_creation_result = CreateDXGIFactory1(IID_PPV_ARGS(&s_d3d11_renderer->dxgi_factory));
    CAVE_VERIFY(SUCCEEDED(dxgi_factory_creation_result));

    return true;
}

void D3D11Renderer::shutdown()
{
    if (!s_d3d11_renderer)
    {
        // The D3D11 renderer has already been shut down.
        return;
    }

    CAVE_D3D11_RELEASE(s_d3d11_renderer->dxgi_factory);
    CAVE_D3D11_RELEASE(s_d3d11_renderer->device_context);
    CAVE_D3D11_RELEASE(s_d3d11_renderer->device);

    delete s_d3d11_renderer;
    s_d3d11_renderer = nullptr;
}

void D3D11Renderer::begin_frame()
{
    // Get the active rendering context. The generic renderer system is reponsible for ensuring that
    // the active context is always set correctly.
    D3D11RenderingContext& context = static_cast<D3D11RenderingContext&>(Renderer::get_rendering_context());
}

void D3D11Renderer::end_frame()
{
    // Get the active rendering context. The generic renderer system is reponsible for ensuring that
    // the active context is always set correctly.
    D3D11RenderingContext& context = static_cast<D3D11RenderingContext&>(Renderer::get_rendering_context());

    const HRESULT swapchain_present_result = context.get_swapchain()->Present(0, 0);
    if (FAILED(swapchain_present_result))
    {
        // The swapchain failed to present the image to the screen.
        // TODO: Inform the user about this error!
        return;
    }
}

void D3D11Renderer::begin_render_pass(RefPtr<RenderPass> generic_render_pass)
{
    RefPtr<D3D11RenderPass> render_pass = generic_render_pass.as<D3D11RenderPass>();
    D3D11Framebuffer& target_framebuffer = render_pass->get_target_framebuffer();
    const PipelineDescription& pipeline = render_pass->get_pipeline_description();

    // Bind the pipeline.
    // ...

    // Get a list with views towards all target framebuffer attachments.
    Vector<ID3D11RenderTargetView*> attachment_rtvs;
    attachment_rtvs.set_capacity(target_framebuffer.get_attachment_count());
    for (u32 attachment_index = 0; attachment_index < target_framebuffer.get_attachment_count(); ++attachment_index)
        attachment_rtvs.add(static_cast<ID3D11RenderTargetView*>(target_framebuffer.get_attachment_image_view(attachment_index)));

    // Set the render targets.
    get_device_context()->OMSetRenderTargets(target_framebuffer.get_attachment_count(), attachment_rtvs.elements(), nullptr);

    // Clear the framebuffer attachment if the load operation is set to `Clear`.
    for (u32 attachment_index = 0; attachment_index < target_framebuffer.get_attachment_count(); ++attachment_index)
    {
        const RenderPassAttachmentDescription& render_pass_attachment = render_pass->get_target_framebuffer_attachment(attachment_index);
        if (render_pass_attachment.load_operation == RenderPassAttachmentLoadOperation::Clear)
        {
            ID3D11RenderTargetView* attachment_view = static_cast<ID3D11RenderTargetView*>(target_framebuffer.get_attachment_image_view(attachment_index));
            const FLOAT clear_color[] = {
                render_pass_attachment.clear_color.r,
                render_pass_attachment.clear_color.g,
                render_pass_attachment.clear_color.b,
                render_pass_attachment.clear_color.a,
            };

            get_device_context()->ClearRenderTargetView(attachment_view, clear_color);
        }
    }
}

void D3D11Renderer::end_render_pass()
{}

ID3D11Device* D3D11Renderer::get_device()
{
    return s_d3d11_renderer->device;
}

ID3D11DeviceContext* D3D11Renderer::get_device_context()
{
    return s_d3d11_renderer->device_context;
}

IDXGIFactory2* D3D11Renderer::get_dxgi_factory()
{
    return s_d3d11_renderer->dxgi_factory;
}

} // namespace CaveGame
