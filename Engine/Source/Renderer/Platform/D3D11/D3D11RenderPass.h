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
    struct Pipeline
    {
        PipelineDescription description;
        u32 vertex_stride { 0 };
        ID3D11InputLayout* input_layout { nullptr };
    };

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

    NODISCARD ALWAYS_INLINE const PipelineDescription& get_pipeline_description() const { return m_pipeline.description; }
    NODISCARD ALWAYS_INLINE u32 get_pipeline_vertex_stride() const { return m_pipeline.vertex_stride; }
    NODISCARD ALWAYS_INLINE ID3D11InputLayout* get_pipeline_input_layout() const { return m_pipeline.input_layout; }

private:
    RefPtr<D3D11Framebuffer> m_target_framebuffer;
    Vector<RenderPassAttachmentDescription> m_target_framebuffer_attachments;

    Pipeline m_pipeline;
};

} // namespace CaveGame
