/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

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
    };

public:
    explicit D3D11Shader(const ShaderDescription& description);
    virtual ~D3D11Shader() override;

    //
    // Gets the handle of the shader module corresponding to the given stage, casted to the given template parameter type.
    // If the stage doesn't exist, this function will return nullptr.
    //
    template<typename ShaderType>
    NODISCARD ALWAYS_INLINE ShaderType* get_shader_module(ShaderStage stage)
    {
        IUnknown* module_handle = get_shader_module(stage);
        static_assert(is_derived_from<ShaderType, IUnknown>, "ShaderType must be derived from the IUnknown D3D11 interface!");
        return static_cast<ShaderType*>(module_handle);
    }

    //
    // Gets the handle of the shader module corresponding to the given stage.
    // If the stage doesn't exist, this function will return nullptr.
    //
    NODISCARD IUnknown* get_shader_module(ShaderStage stage);

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
