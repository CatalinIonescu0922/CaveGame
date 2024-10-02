/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Platform/D3D11/D3D11RenderPass.h>

namespace CaveGame
{

D3D11RenderPass::D3D11RenderPass(const RenderPassDescription& description)
{
    m_target_framebuffer = description.target_framebuffer.as<D3D11Framebuffer>();
    m_target_framebuffer_attachments = description.target_framebuffer_attachments;
    
    if (m_target_framebuffer_attachments.count() != m_target_framebuffer->get_attachment_count())
    {
        // The number of render pass attachments specified in the render pass description must match the actual
        // number of attachments of the target framebuffer.
        CAVE_ASSERT(false);
        return;
    }
}

D3D11RenderPass::~D3D11RenderPass()
{
    m_target_framebuffer_attachments.clear_and_shrink();
    m_target_framebuffer.release();
}

} // namespace CaveGame
