/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Platform/D3D11/D3D11IndexBuffer.h>
#include <Renderer/Platform/D3D11/D3D11Renderer.h>

namespace CaveGame
{

NODISCARD ALWAYS_INLINE static D3D11_USAGE get_d3d11_usage(IndexBufferUpdateFrequency update_frequency)
{
    switch (update_frequency)
    {
        case IndexBufferUpdateFrequency::Never: return D3D11_USAGE_IMMUTABLE;
        case IndexBufferUpdateFrequency::Normal: return D3D11_USAGE_DEFAULT;
        case IndexBufferUpdateFrequency::Often: return D3D11_USAGE_DYNAMIC;
    }

    CAVE_ASSERT(false);
    return D3D11_USAGE_DEFAULT;
}

NODISCARD ALWAYS_INLINE static usize get_data_type_size(IndexBufferDataType data_type)
{
    switch (data_type)
    {
        case IndexBufferDataType::UInt16: return 2;
        case IndexBufferDataType::UInt32: return 4;
    }

    CAVE_ASSERT(false);
    return 0;
}

D3D11IndexBuffer::D3D11IndexBuffer(const IndexBufferDescription& description)
    : m_handle(nullptr)
    , m_buffer_size(0)
    , m_update_frequency(description.update_frequency)
    , m_data_type(description.data_type)
    , m_indices_count(description.indices_count)
{
    if (description.update_frequency == IndexBufferUpdateFrequency::Never)
    {
        // An immutable buffer must always have an initial data buffer to initialize it with, as there is no
        // method to update its content afterwards.
        // TODO: Inform the user about this error!
        CAVE_ASSERT(description.initial_data != nullptr);
        return;
    }

    m_buffer_size = get_data_type_size(m_data_type) * m_indices_count;

    //
    // The specification of the index buffer.
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc
    //
    D3D11_BUFFER_DESC buffer_description = {};
    buffer_description.ByteWidth = static_cast<UINT>(m_buffer_size);
    buffer_description.Usage = get_d3d11_usage(description.update_frequency);
    buffer_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data
    D3D11_SUBRESOURCE_DATA initial_data = {};
    initial_data.pSysMem = description.initial_data;

    const HRESULT buffer_creation_result = D3D11Renderer::get_device()->CreateBuffer(&buffer_description, &initial_data, &m_handle);
    CAVE_ASSERT(SUCCEEDED(buffer_creation_result));
}

D3D11IndexBuffer::~D3D11IndexBuffer()
{
    CAVE_D3D11_RELEASE(m_handle);
}

} // namespace CaveGame
