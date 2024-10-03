/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/RefPtr.h>

namespace CaveGame
{

enum class IndexBufferUpdateFrequency : u8
{
    Unknown = 0,
    Never,
    Normal,
    Often,
};

enum class IndexBufferDataType : u8
{
    Unknown = 0,
    UInt16,
    UInt32,
};

struct IndexBufferDescription
{
    IndexBufferUpdateFrequency update_frequency { IndexBufferUpdateFrequency::Normal };
    IndexBufferDataType data_type { IndexBufferDataType::Unknown };
    u32 indices_count { 0 };
    void* initial_data { nullptr };
};

class IndexBuffer : public RefCounted
{
public:
    NODISCARD static RefPtr<IndexBuffer> create(const IndexBufferDescription& description);

    IndexBuffer() = default;
    virtual ~IndexBuffer() override = default;

public:
};

} // namespace CaveGame
