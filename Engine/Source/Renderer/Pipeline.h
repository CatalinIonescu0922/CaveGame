/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/String.h>
#include <Core/Containers/Vector.h>

namespace CaveGame
{

enum class PipelineVertexAttributeType : u16
{
    Unknown = 0,
    Float1,
    Float2,
    Float3,
    Float4,
};

struct PipelineVertexAttribute
{
    PipelineVertexAttribute() = default;
    ALWAYS_INLINE PipelineVertexAttribute(PipelineVertexAttributeType in_type, String in_name)
        : type(in_type)
        , name(move(in_name))
    {}

    PipelineVertexAttributeType type { PipelineVertexAttributeType::Unknown };
    String name;
};

struct PipelineDescription
{
    Vector<PipelineVertexAttribute> vertex_attributes;
};

} // namespace CaveGame
