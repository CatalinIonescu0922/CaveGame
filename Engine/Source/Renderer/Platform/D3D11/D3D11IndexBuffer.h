/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/IndexBuffer.h>
#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>

namespace CaveGame
{

class D3D11IndexBuffer : public IndexBuffer
{
public:
    D3D11IndexBuffer(const IndexBufferDescription& description);
    virtual ~D3D11IndexBuffer() override;

    NODISCARD ALWAYS_INLINE ID3D11Buffer* get_handle() { return m_handle; }
    NODISCARD ALWAYS_INLINE const ID3D11Buffer* get_handle() const { return m_handle; }

    NODISCARD ALWAYS_INLINE usize get_size() const { return m_buffer_size; }
    NODISCARD ALWAYS_INLINE u32 get_indices_count() const { return m_indices_count; }

    NODISCARD ALWAYS_INLINE IndexBufferDataType get_data_type() const { return m_data_type; }
    NODISCARD DXGI_FORMAT get_data_type_format() const;

private:
    ID3D11Buffer* m_handle;
    usize m_buffer_size;
    IndexBufferUpdateFrequency m_update_frequency;
    IndexBufferDataType m_data_type;
    u32 m_indices_count;
};

} // namespace CaveGame
