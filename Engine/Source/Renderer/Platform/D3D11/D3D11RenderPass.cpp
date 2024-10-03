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

NODISCARD ALWAYS_INLINE static u32 get_pipeline_vertex_attribute_type_size(PipelineVertexAttributeType attribute_type)
{
    switch (attribute_type)
    {
        case PipelineVertexAttributeType::Float1: return 4;
        case PipelineVertexAttributeType::Float2: return 8;
        case PipelineVertexAttributeType::Float3: return 12;
        case PipelineVertexAttributeType::Float4: return 16;
    }

    CAVE_ASSERT(false);
    return 0;
}

D3D11RenderPass::D3D11RenderPass(const RenderPassDescription& description)
    : m_pipeline_description(description.pipeline_description)
    , m_pipeline_vertex_stride(0)
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

    for (const auto& attribute : m_pipeline_description.vertex_attributes)
        m_pipeline_vertex_stride += get_pipeline_vertex_attribute_type_size(attribute.type);
}

D3D11RenderPass::~D3D11RenderPass()
{
    m_target_framebuffer_attachments.clear_and_shrink();
    m_target_framebuffer.release();
}

} // namespace CaveGame
