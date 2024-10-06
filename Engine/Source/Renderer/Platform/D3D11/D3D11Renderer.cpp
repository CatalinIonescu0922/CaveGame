/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Assertion.h>
#include <Renderer/Platform/D3D11/D3D11IndexBuffer.h>
#include <Renderer/Platform/D3D11/D3D11RenderPass.h>
#include <Renderer/Platform/D3D11/D3D11Renderer.h>
#include <Renderer/Platform/D3D11/D3D11RenderingContext.h>
#include <Renderer/Platform/D3D11/D3D11Shader.h>
#include <Renderer/Platform/D3D11/D3D11VertexBuffer.h>
#include <Renderer/Renderer.h>

namespace CaveGame
{

struct D3D11RendererData
{
    ID3D11Device* device { nullptr };
    ID3D11DeviceContext* device_context { nullptr };
    IDXGIFactory2* dxgi_factory { nullptr };

    Optional<RefPtr<D3D11RenderPass>> active_render_pass;
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

void D3D11Renderer::bind_pipeline(D3D11RenderPass& render_pass)
{
    // Get the pipeline description corresponding to the given render pass.
    const PipelineDescription& pipeline = render_pass.get_pipeline_description();

    //====================================================
    // INPUT-ASSEMBLER (IA) STAGE.
    //====================================================

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetinputlayout
    get_device_context()->IASetInputLayout(render_pass.get_pipeline_input_layout());

    D3D11_PRIMITIVE_TOPOLOGY primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    switch (pipeline.topology)
    {
        case PipelineTopology::TriangleList: primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
    }

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetprimitivetopology
    get_device_context()->IASetPrimitiveTopology(primitive_topology);

    //====================================================
    // SHADER STAGES.
    //====================================================

    RefPtr<D3D11Shader> shader = pipeline.shader.as<D3D11Shader>();
    const auto& shader_modules = shader->get_shader_modules();
    for (const D3D11Shader::ShaderModule& shader_module : shader_modules)
    {
        switch (shader_module.stage)
        {
            case ShaderStage::Vertex: get_device_context()->VSSetShader(static_cast<ID3D11VertexShader*>(shader_module.handle), nullptr, 0); break;
            case ShaderStage::Fragment: get_device_context()->PSSetShader(static_cast<ID3D11PixelShader*>(shader_module.handle), nullptr, 0); break;

            default: CAVE_ASSERT(false); break;
        }
    }

    //====================================================
    // RASTERIZER (RS) STAGE.
    //====================================================

    get_device_context()->RSSetState(render_pass.get_pipeline_rasterizer_state());

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(render_pass.get_target_framebuffer().get_width());
    viewport.Height = static_cast<FLOAT>(render_pass.get_target_framebuffer().get_height());
    viewport.MinDepth = 0.0F;
    viewport.MaxDepth = 1.0F;

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-rssetviewports
    get_device_context()->RSSetViewports(1, &viewport);
}

void D3D11Renderer::bind_target_framebuffer(D3D11RenderPass& render_pass)
{
    D3D11Framebuffer& target_framebuffer = render_pass.get_target_framebuffer();

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
        const RenderPassAttachmentDescription& render_pass_attachment = render_pass.get_target_framebuffer_attachment(attachment_index);
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

void D3D11Renderer::begin_render_pass(RefPtr<RenderPass> generic_render_pass)
{
    if (s_d3d11_renderer->active_render_pass.has_value())
    {
        // A render pass is already active!
        CAVE_ASSERT(false);
        return;
    }

    RefPtr<D3D11RenderPass> render_pass = generic_render_pass.as<D3D11RenderPass>();
    s_d3d11_renderer->active_render_pass = render_pass;

    bind_pipeline(*render_pass);
    bind_target_framebuffer(*render_pass);
}

void D3D11Renderer::end_render_pass()
{
    if (!s_d3d11_renderer->active_render_pass.has_value())
    {
        // No render pass is currently active!
        CAVE_ASSERT(false);
        return;
    }

    s_d3d11_renderer->active_render_pass.clear();
}

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
