/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/RefPtr.h>
#include <Core/Containers/String.h>
#include <Core/Containers/Vector.h>

namespace CaveGame
{

enum class VertexBufferUpdateFrequency : u8
{
    Unknown = 0,
    Never,
    Normal,
    Often,
};

struct VertexBufferDescription
{
    VertexBufferUpdateFrequency update_frequency { VertexBufferUpdateFrequency::Normal };
    void* data { nullptr };
    usize buffer_size { 0 };
};

class VertexBuffer : public RefCounted
{
public:
    NODISCARD static RefPtr<VertexBuffer> create(const VertexBufferDescription& description);

    VertexBuffer() = default;
    virtual ~VertexBuffer() override = default;

public:
};

} // namespace CaveGame
