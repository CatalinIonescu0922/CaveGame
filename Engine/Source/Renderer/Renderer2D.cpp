/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Memory/Buffer.h>
#include <Core/Platform/FileStream.h>
#include <Renderer/Renderer.h>
#include <Renderer/Renderer2D.h>

namespace CaveGame
{

OwnPtr<Renderer2D> Renderer2D::create(RefPtr<Framebuffer> target_framebuffer)
{
    Renderer2D* renderer_2d = new Renderer2D(target_framebuffer);
    return adopt_own(renderer_2d);
}

OwnPtr<Renderer2D> Renderer2D::create(u32 target_framebuffer_width, u32 target_framebuffer_height)
{
    FramebufferDescription framebuffer_description = {};
    framebuffer_description.width = target_framebuffer_width;
    framebuffer_description.height = target_framebuffer_height;
    framebuffer_description.attachments.add({ ImageFormat::B8G8R8A8 });
    RefPtr<Framebuffer> target_framebuffer = Framebuffer::create(framebuffer_description);

    return Renderer2D::create(target_framebuffer);
}

Renderer2D::Renderer2D(RefPtr<Framebuffer> target_framebuffer)
    : m_target_framebuffer(target_framebuffer)
{
    initialize_quad_rendering();
}

Renderer2D::~Renderer2D()
{
    shutdown_quad_rendering();
}

void Renderer2D::begin_frame()
{
    Renderer::begin_render_pass(m_quad_render_pass);
    begin_quad_batch();
}

void Renderer2D::end_frame()
{
    end_quad_batch();
    Renderer::end_render_pass();
}

void Renderer2D::submit_quad(Vector2 translation, Vector2 scale, Color4 color)
{
    if (m_current_number_of_quads >= m_max_quads_per_batch)
        flush_quad_batch();

    QuadVertex* vertices = m_quad_vertices.elements() + (4 * m_current_number_of_quads);
    construct_quad(vertices, translation, scale, color);

    m_current_number_of_quads++;
    m_total_number_of_quads++;
}

void Renderer2D::initialize_quad_rendering()
{
    //
    // Generic 2D renderer quad initialization.
    //

    m_max_quads_per_batch = 4096;
    m_quad_vertices.set_count_uninitialized(4 * m_max_quads_per_batch);

    m_current_number_of_quads = 0;
    m_total_number_of_quads = 0;

    //
    // Create the quad shader.
    //

    ShaderDescription shader_description = {};

    FileReadStream shader_vertex_file_stream;
    shader_vertex_file_stream.open("../../Engine/Shaders/Renderer2D_Quad_V.hlsl"sv);
    Buffer shader_vertex_source_code = Buffer::create(shader_vertex_file_stream.get_remaining_byte_count());
    shader_vertex_file_stream.read_entire(shader_vertex_source_code.data());

    ShaderStageDescription shader_vertex_description = {};
    shader_vertex_description.stage = ShaderStage::Vertex;
    shader_vertex_description.source_type = ShaderSourceType::SourceCode;
    shader_vertex_description.source_code = StringView::create_from_utf8(shader_vertex_source_code.readonly_byte_span());
    shader_description.stages.add(shader_vertex_description);
    shader_vertex_file_stream.close();

    FileReadStream shader_fragment_file_stream;
    shader_fragment_file_stream.open("../../Engine/Shaders/Renderer2D_Quad_F.hlsl"sv);
    Buffer shader_fragment_source_code = Buffer::create(shader_fragment_file_stream.get_remaining_byte_count());
    shader_fragment_file_stream.read_entire(shader_fragment_source_code.data());

    ShaderStageDescription shader_fragment_description = {};
    shader_fragment_description.stage = ShaderStage::Fragment;
    shader_fragment_description.source_type = ShaderSourceType::SourceCode;
    shader_fragment_description.source_code = StringView::create_from_utf8(shader_fragment_source_code.readonly_byte_span());
    shader_description.stages.add(shader_fragment_description);
    shader_fragment_file_stream.close();

    m_quad_shader = Shader::create(shader_description);
    shader_vertex_source_code.release();
    shader_fragment_source_code.release();

    //
    // Create the quad render pass.
    //

    RenderPassDescription render_pass_description = {};
    render_pass_description.pipeline_description.shader = m_quad_shader;
    render_pass_description.pipeline_description.vertex_attributes.add({ PipelineVertexAttributeType::Float2, "POSITION"sv });
    render_pass_description.pipeline_description.vertex_attributes.add({ PipelineVertexAttributeType::Float4, "COLOR"sv });
    render_pass_description.target_framebuffer = m_target_framebuffer;

    RenderPassAttachmentDescription render_pass_attachment_description = {};
    render_pass_attachment_description.load_operation = RenderPassAttachmentLoadOperation::Clear;
    render_pass_attachment_description.clear_color = Color4(0, 0, 0);

    CAVE_ASSERT(m_target_framebuffer->get_attachment_count() == 1);
    render_pass_description.target_framebuffer_attachments.add(render_pass_attachment_description);

    m_quad_render_pass = RenderPass::create(render_pass_description);

    //
    // Create the quad index buffer.
    //

    Vector<u32> indices;
    indices.set_count_uninitialized(6 * m_max_quads_per_batch);

    for (u32 quad_index = 0; quad_index < m_max_quads_per_batch; ++quad_index)
    {
        indices[(6 * quad_index) + 0] = (4 * quad_index) + 0;
        indices[(6 * quad_index) + 1] = (4 * quad_index) + 1;
        indices[(6 * quad_index) + 2] = (4 * quad_index) + 2;
        indices[(6 * quad_index) + 3] = (4 * quad_index) + 2;
        indices[(6 * quad_index) + 4] = (4 * quad_index) + 3;
        indices[(6 * quad_index) + 5] = (4 * quad_index) + 0;
    }

    IndexBufferDescription index_buffer_description = {};
    index_buffer_description.update_frequency = IndexBufferUpdateFrequency::Never;
    index_buffer_description.data_type = { IndexBufferDataType::UInt32 };
    index_buffer_description.indices_count = static_cast<u32>(indices.count());
    index_buffer_description.initial_data = indices.elements();

    m_quad_index_buffer = IndexBuffer::create(index_buffer_description);
    indices.clear_and_shrink();

    //
    // Create the quad vertex buffer.
    //

    VertexBufferDescription vertex_buffer_description = {};
    vertex_buffer_description.update_frequency = VertexBufferUpdateFrequency::Often;
    vertex_buffer_description.buffer_size = m_quad_vertices.count() * sizeof(QuadVertex);

    m_quad_vertex_buffer = VertexBuffer::create(vertex_buffer_description);
}

void Renderer2D::shutdown_quad_rendering()
{
    m_quad_vertex_buffer.release();
    m_quad_index_buffer.release();
    m_quad_render_pass.release();
    m_quad_shader.release();

    m_quad_vertices.clear_and_shrink();

    m_max_quads_per_batch = 0;
    m_current_number_of_quads = 0;
    m_total_number_of_quads = 0;
}

void Renderer2D::construct_quad(QuadVertex* destination_vertices, Vector2 translation, Vector2 scale, Color4 color)
{
    // Bottom-left vertex.
    destination_vertices[0].position = translation + Vector2(-0.5F * scale.x, -0.5F * scale.y);
    destination_vertices[0].color = color;

    // Bottom-right vertex.
    destination_vertices[1].position = translation + Vector2(0.5F * scale.x, -0.5F * scale.y);
    destination_vertices[1].color = color;
    
    // Top-right vertex.
    destination_vertices[2].position = translation + Vector2(0.5F * scale.x, 0.5F * scale.y);
    destination_vertices[2].color = color;

    // Top-left vertex.
    destination_vertices[3].position = translation + Vector2(-0.5F * scale.x, 0.5F * scale.y);
    destination_vertices[3].color = color;
}

void Renderer2D::begin_quad_batch()
{
    m_current_number_of_quads = 0;
    m_total_number_of_quads = 0;
}

void Renderer2D::end_quad_batch()
{
    flush_quad_batch();
}

void Renderer2D::flush_quad_batch()
{
    if (m_current_number_of_quads == 0)
        return;

    const ReadonlyByteSpan vertices_data = m_quad_vertices.slice(0, 4 * m_current_number_of_quads).readonly_byte_span();
    m_quad_vertex_buffer->upload_data(vertices_data);

    const u32 indices_count = 6 * m_current_number_of_quads;
    Renderer::draw_indexed(m_quad_vertex_buffer, m_quad_index_buffer, indices_count);

    // Reset the current number of unprocessed quads in the batch.
    m_current_number_of_quads = 0;
}

} // namespace CaveGame
