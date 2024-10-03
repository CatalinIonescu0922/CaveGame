/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>
#include <Renderer/VertexBuffer.h>

namespace CaveGame
{

class D3D11VertexBuffer : public VertexBuffer
{
public:
    D3D11VertexBuffer(const VertexBufferDescription& description);
    virtual ~D3D11VertexBuffer() override;

private:
    ID3D11Buffer* m_handle;
    usize m_buffer_size;
};

} // namespace CaveGame
