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
#include <Renderer/RenderPass.h>

namespace CaveGame
{

class D3D11RenderPass : public RenderPass
{
public:
    explicit D3D11RenderPass(const RenderPassDescription& description);
    virtual ~D3D11RenderPass() override;

public:
    NODISCARD ALWAYS_INLINE D3D11Framebuffer& get_target_framebuffer() { return *m_target_framebuffer; }
    NODISCARD ALWAYS_INLINE const D3D11Framebuffer& get_target_framebuffer() const { return *m_target_framebuffer; }

    NODISCARD ALWAYS_INLINE const RenderPassAttachmentDescription& get_target_framebuffer_attachment(u32 attachment_index) const
    {
        // Attachment index is out of bounds.
        CAVE_ASSERT(attachment_index < m_target_framebuffer_attachments.count());
        return m_target_framebuffer_attachments[attachment_index];
    }

private:
    RefPtr<D3D11Framebuffer> m_target_framebuffer;
    Vector<RenderPassAttachmentDescription> m_target_framebuffer_attachments;
};

} // namespace CaveGame
