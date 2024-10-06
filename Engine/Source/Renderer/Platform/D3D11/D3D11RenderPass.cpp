/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Containers/Vector.h>
#include <Renderer/Platform/D3D11/D3D11RenderPass.h>
#include <Renderer/Platform/D3D11/D3D11Renderer.h>
#include <Renderer/Platform/D3D11/D3D11Shader.h>

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

NODISCARD ALWAYS_INLINE static DXGI_FORMAT get_pipeline_vertex_attribute_type_format(PipelineVertexAttributeType attribute_type)
{
    switch (attribute_type)
    {
        case PipelineVertexAttributeType::Float1: return DXGI_FORMAT_R32_FLOAT;
        case PipelineVertexAttributeType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
        case PipelineVertexAttributeType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
        case PipelineVertexAttributeType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }

    CAVE_ASSERT(false);
    return DXGI_FORMAT_UNKNOWN;
}

D3D11RenderPass::D3D11RenderPass(const RenderPassDescription& description)
{
    m_target_framebuffer = description.target_framebuffer.as<D3D11Framebuffer>();
    m_target_framebuffer_attachments = description.target_framebuffer_attachments;

    m_pipeline.description = description.pipeline_description;
    m_pipeline.vertex_stride = 0;
    m_pipeline.input_layout = nullptr;

    if (m_target_framebuffer_attachments.count() != m_target_framebuffer->get_attachment_count())
    {
        // The number of render pass attachments specified in the render pass description must match the actual
        // number of attachments of the target framebuffer.
        CAVE_ASSERT(false);
        return;
    }

    Vector<D3D11_INPUT_ELEMENT_DESC> input_element_descriptions;
    input_element_descriptions.set_capacity(m_pipeline.description.vertex_attributes.count());

    u32 attribute_offset = 0;
    for (const PipelineVertexAttribute& vertex_attribute : m_pipeline.description.vertex_attributes)
    {
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
        D3D11_INPUT_ELEMENT_DESC input_element_description = {};
        input_element_description.SemanticName = vertex_attribute.name.characters();
        // TODO: Actually correctly implement semantic indices!
        input_element_description.SemanticIndex = 0;
        input_element_description.Format = get_pipeline_vertex_attribute_type_format(vertex_attribute.type);
        input_element_description.InputSlot = 0;
        input_element_description.AlignedByteOffset = attribute_offset;
        input_element_description.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        input_element_description.InstanceDataStepRate = 0;

        input_element_descriptions.add(input_element_description);
        attribute_offset += get_pipeline_vertex_attribute_type_size(vertex_attribute.type);
    }

    const ReadonlyByteSpan shader_bytecode = m_pipeline.description.shader.as<D3D11Shader>()->get_shader_module_bytecode(ShaderStage::Vertex);
    // NOTE: The provided shader doesn't have a vertex stage.
    CAVE_ASSERT(shader_bytecode.has_elements());

    const HRESULT input_layout_creation_result = D3D11Renderer::get_device()->CreateInputLayout(
        input_element_descriptions.elements(),
        static_cast<UINT>(input_element_descriptions.count()),
        shader_bytecode.elements(),
        shader_bytecode.count(),
        &m_pipeline.input_layout
    );
    CAVE_ASSERT(SUCCEEDED(input_layout_creation_result));
}

D3D11RenderPass::~D3D11RenderPass()
{
    CAVE_D3D11_RELEASE(m_pipeline.input_layout);

    m_target_framebuffer_attachments.clear_and_shrink();
    m_target_framebuffer.release();
}

} // namespace CaveGame
