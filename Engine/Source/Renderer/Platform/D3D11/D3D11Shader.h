/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/Optional.h>
#include <Core/Containers/Span.h>
#include <Core/Memory/Buffer.h>
#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>
#include <Renderer/Shader.h>

namespace CaveGame
{

class D3D11Shader : public Shader
{
public:
    struct ShaderModule
    {
        ShaderStage stage;
        IUnknown* handle;
        Buffer bytecode;
    };

public:
    explicit D3D11Shader(const ShaderDescription& description);
    virtual ~D3D11Shader() override;

    NODISCARD ALWAYS_INLINE const Vector<ShaderModule>& get_shader_modules() const { return m_shader_modules; }

    // Returns an empty optional if the shader doesn't contain the given stage.
    NODISCARD Optional<const ShaderModule&> get_shader_module(ShaderStage shader_stage) const;

    //
    // Returns an empty byte span if the shader doesn't contain the given stage.
    // Wrapper around the `get_shader_module` function.
    //
    NODISCARD ALWAYS_INLINE ReadonlyByteSpan get_shader_module_bytecode(ShaderStage shader_stage) const
    {
        Optional<const ShaderModule&> shader_module = get_shader_module(shader_stage);
        return shader_module.has_value() ? shader_module->bytecode.byte_span() : ReadonlyByteSpan();
    }

private:
    struct ShaderCompilationResult
    {
        HRESULT result;
        String error_message;
        Buffer bytecode;
    };

    NODISCARD static ShaderCompilationResult compile_shader_module(ShaderStage stage, StringView source_code);
    NODISCARD static ShaderModule create_shader_module(const ShaderStageDescription& description);

private:
    Vector<ShaderModule> m_shader_modules;
};

} // namespace CaveGame
