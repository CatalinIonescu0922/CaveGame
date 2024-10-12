/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>
#include <Renderer/Texture.h>

namespace CaveGame
{

class D3D11Texture : public Texture
{
public:
    explicit D3D11Texture(const TextureDescription& description);
    virtual ~D3D11Texture() override;

public:
    NODISCARD ALWAYS_INLINE virtual u32 get_width() const override { return m_width; }
    NODISCARD ALWAYS_INLINE virtual u32 get_height() const override { return m_height; }
    NODISCARD ALWAYS_INLINE virtual ImageFormat get_format() const override { return m_format; }

    NODISCARD ALWAYS_INLINE ID3D11Texture2D* get_handle() { return m_handle; }
    NODISCARD ALWAYS_INLINE const ID3D11Texture2D* get_handle() const { return m_handle; }

    NODISCARD ALWAYS_INLINE ID3D11ShaderResourceView* get_view_handle() { return m_view_handle; }
    NODISCARD ALWAYS_INLINE const ID3D11ShaderResourceView* get_view_handle() const { return m_view_handle; }

    NODISCARD ALWAYS_INLINE ID3D11SamplerState* get_sampler_state() { return m_sampler_state; }
    NODISCARD ALWAYS_INLINE const ID3D11SamplerState* get_sampler_state() const { return m_sampler_state; }

private:
    ID3D11Texture2D* m_handle;
    ID3D11ShaderResourceView* m_view_handle;
    ID3D11SamplerState* m_sampler_state;

    u32 m_width;
    u32 m_height;
    ImageFormat m_format;

    ImageFilteringMode m_min_filter;
    ImageFilteringMode m_mag_filter;

    ImageAddressMode m_address_mode_u;
    ImageAddressMode m_address_mode_v;
    ImageAddressMode m_address_mode_w;
};

} // namespace CaveGame
