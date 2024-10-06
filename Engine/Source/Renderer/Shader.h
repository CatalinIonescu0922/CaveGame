/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/RefPtr.h>
#include <Core/Containers/Span.h>
#include <Core/Containers/StringView.h>
#include <Core/Containers/Vector.h>

namespace CaveGame
{

enum class ShaderStage : u8
{
    Unknown = 0,
    Vertex,
    Fragment,
};

enum class ShaderSourceType
{
    Unknown = 0,
    SourceCode,
    Bytecode,
};

struct ShaderStageDescription
{
    ShaderStage stage { ShaderStage::Unknown };
    ShaderSourceType source_type { ShaderSourceType::Unknown };

    // Only used when the shader stage source type is set to `SourceCode`.
    StringView source_code;

    // Only used when the shader stage source type is set to `Bytecode`.
    ReadonlyByteSpan source_bytecode;
};

struct ShaderDescription
{
    Vector<ShaderStageDescription> stages;
};

class Shader : public RefCounted
{
public:
    NODISCARD static RefPtr<Shader> create(const ShaderDescription& description);

    Shader() = default;
    virtual ~Shader() override = default;
};

} // namespace CaveGame
