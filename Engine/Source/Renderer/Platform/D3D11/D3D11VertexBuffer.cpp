/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Platform/D3D11/D3D11Renderer.h>
#include <Renderer/Platform/D3D11/D3D11VertexBuffer.h>

namespace CaveGame
{

NODISCARD ALWAYS_INLINE static D3D11_USAGE get_d3d11_usage(VertexBufferUpdateFrequency update_frequency)
{
    switch (update_frequency)
    {
        case VertexBufferUpdateFrequency::Never: return D3D11_USAGE_IMMUTABLE;
        case VertexBufferUpdateFrequency::Normal: return D3D11_USAGE_DEFAULT;
        case VertexBufferUpdateFrequency::Often: return D3D11_USAGE_DYNAMIC;
    }

    CAVE_ASSERT(false);
    return D3D11_USAGE_DEFAULT;
}

D3D11VertexBuffer::D3D11VertexBuffer(const VertexBufferDescription& description)
    : m_handle(nullptr)
    , m_buffer_size(description.buffer_size)
{
    if (description.update_frequency == VertexBufferUpdateFrequency::Never)
    {
        // An immutable buffer must always have an initial data buffer to initialize it with, as there is no
        // method to update its content afterwards.
        // TODO: Inform the user about this error!
        CAVE_ASSERT(description.data != nullptr);
    }

    //
    // The specification of the vertex buffer.
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc
    //
    D3D11_BUFFER_DESC buffer_description = {};
    buffer_description.ByteWidth = static_cast<UINT>(m_buffer_size);
    buffer_description.Usage = get_d3d11_usage(description.update_frequency);
    buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_description.CPUAccessFlags = 0;

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data
    D3D11_SUBRESOURCE_DATA initial_data = {};
    initial_data.pSysMem = description.data;

    const HRESULT buffer_creation_result = D3D11Renderer::get_device()->CreateBuffer(&buffer_description, &initial_data, &m_handle);
    CAVE_ASSERT(SUCCEEDED(buffer_creation_result));
}

D3D11VertexBuffer::~D3D11VertexBuffer()
{
    CAVE_D3D11_RELEASE(m_handle);
}

} // namespace CaveGame
