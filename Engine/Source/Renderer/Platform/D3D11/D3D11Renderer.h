/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>
#include <Renderer/RendererInterface.h>

namespace CaveGame
{

// Forward declarations.
class D3D11RenderPass;

class D3D11Renderer : public RendererInterface
{
public:
    D3D11Renderer() = default;
    virtual ~D3D11Renderer() override = default;

    virtual bool initialize() override;
    virtual void shutdown() override;

    virtual void begin_frame() override;
    virtual void end_frame() override;

    virtual void begin_render_pass(RefPtr<RenderPass> render_pass) override;
    virtual void end_render_pass() override;

public:
    virtual void draw_indexed(RefPtr<VertexBuffer> vertex_buffer, RefPtr<IndexBuffer> index_buffer, u32 indices_count) override;

public:
    virtual void bind_input_texture(RefPtr<Texture> texture, u32 bind_slot_index) override;

public:
    static ID3D11Device* get_device();
    static ID3D11DeviceContext* get_device_context();
    static IDXGIFactory2* get_dxgi_factory();

private:
    static void bind_pipeline(D3D11RenderPass& render_pass);
    static void bind_target_framebuffer(D3D11RenderPass& render_pass);
};

} // namespace CaveGame
