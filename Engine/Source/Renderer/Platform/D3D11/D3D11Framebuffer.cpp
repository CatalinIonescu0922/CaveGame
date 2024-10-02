/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Framebuffer.h>
#include <Renderer/Platform/D3D11/D3D11Image.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderingContext.h>

namespace CaveGame
{

RefPtr<Framebuffer> Framebuffer::create(const FramebufferDescription& description)
{
    Framebuffer* framebuffer = new Framebuffer(description);
    return adopt_ref(framebuffer);
}

RefPtr<Framebuffer> Framebuffer::create(RenderingContext& rendering_context)
{
    Framebuffer* framebuffer = new Framebuffer(rendering_context);
    return adopt_ref(framebuffer);
}

Framebuffer::Framebuffer(const FramebufferDescription& description)
    : m_is_swapchain_target(false)
    , m_width(0)
    , m_height(0)
{
    m_attachments.set_capacity(description.attachments.count());
    for (const auto& attachment_description : description.attachments)
    {
        Attachment attachment = {};
        attachment.description = attachment_description;
        m_attachments.add(attachment);
    }

    invalidate(description.width, description.heigth);
}

Framebuffer::Framebuffer(RenderingContext& rendering_context)
    : m_is_swapchain_target(true)
    , m_rendering_context(rendering_context)
    , m_width(0)
    , m_height(0)
{
    m_rendering_context->reference_swapchain_target_framebuffer(*this);

    Attachment attachment = {};
    attachment.description.format = rendering_context.get_swapchain_image_format();

    invalidate(0, 0);
}

Framebuffer::~Framebuffer()
{
    if (m_is_swapchain_target)
    {
        CAVE_ASSERT(m_rendering_context.has_value());
        m_rendering_context->dereference_swapchain_target_framebuffer(*this);
    }

    destroy();
}

void Framebuffer::invalidate(u32 new_width, u32 new_height)
{
    if (m_is_swapchain_target)
    {
        // Forward the invalidation implementation to the swapchain target version.
        invalidate_swapchain_target();
        return;
    }

    destroy();

    for (Attachment& attachment : m_attachments)
    {
        //
        // The specification of the framebuffer attachment image (texture).
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_texture2d_desc
        //
        D3D11_TEXTURE2D_DESC texture_description = {};
        texture_description.Width = (UINT)(m_width);
        texture_description.Height = (UINT)(m_height);
        texture_description.MipLevels = 1;
        texture_description.ArraySize = 1;
        texture_description.Format = get_d3d11_image_format(attachment.description.format);
        texture_description.SampleDesc.Count = 1;
        texture_description.SampleDesc.Quality = 0;
        texture_description.Usage = D3D11_USAGE_DEFAULT;
        texture_description.BindFlags = D3D11_BIND_RENDER_TARGET;
        texture_description.CPUAccessFlags = 0;

        //
        // The specification of the framebuffer attachment image view (RTV).
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_render_target_view_desc
        //
        D3D11_RENDER_TARGET_VIEW_DESC render_target_view_description = {};
        render_target_view_description.Format = get_d3d11_image_format(attachment.description.format);
        render_target_view_description.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        render_target_view_description.Texture2D.MipSlice = 0;

        // Obtain the D3D11 device.
        auto* device = Renderer::get_rendering_context().get_device();

        const HRESULT image_creation_result = device->CreateTexture2D(&texture_description, nullptr, &attachment.image_handle);
        CAVE_ASSERT(SUCCEEDED(image_creation_result));

        const HRESULT image_rtv_creation_result =
            device->CreateRenderTargetView(attachment.image_handle, &render_target_view_description, &attachment.image_rtv_handle);
        CAVE_ASSERT(SUCCEEDED(image_rtv_creation_result));
    }
}

void Framebuffer::invalidate_swapchain_target()
{
    destroy();

    CAVE_ASSERT(m_is_swapchain_target && m_rendering_context.has_value());
    CAVE_ASSERT(m_attachments.count() == 1);

    Attachment& attachment = m_attachments.first();
    attachment.image_handle = m_rendering_context->get_swapchain_image();
    attachment.image_rtv_handle = m_rendering_context->get_swapchain_image_view();

    m_width = m_rendering_context->get_swapchain_width();
    m_height = m_rendering_context->get_swapchain_height();
}

void Framebuffer::destroy()
{
    for (Attachment& attachment : m_attachments)
    {
        if (m_is_swapchain_target)
        {
            attachment.image_handle = nullptr;
            attachment.image_rtv_handle = nullptr;
        }
        else
        {
            CAVE_D3D11_RELEASE(attachment.image_handle);
            CAVE_D3D11_RELEASE(attachment.image_rtv_handle);
        }
    }

    m_width = 0;
    m_height = 0;
}

void* Framebuffer::get_attachment_image(u32 attachment_index) const
{
    CAVE_ASSERT(attachment_index < m_attachments.count());
    return m_attachments[attachment_index].image_handle;
}

void* Framebuffer::get_attachment_image_view(u32 attachment_index) const
{
    CAVE_ASSERT(attachment_index < m_attachments.count());
    return m_attachments[attachment_index].image_rtv_handle;
}

const FramebufferAttachmentDescription& Framebuffer::get_attachment_description(u32 attachment_index) const
{
    CAVE_ASSERT(attachment_index < m_attachments.count());
    return m_attachments[attachment_index].description;
}

} // namespace CaveGame
