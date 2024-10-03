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

private:
    ID3D11Buffer* m_handle;
    usize m_buffer_size;
    IndexBufferUpdateFrequency m_update_frequency;
    IndexBufferDataType m_data_type;
    u32 m_indices_count;
};

} // namespace CaveGame
