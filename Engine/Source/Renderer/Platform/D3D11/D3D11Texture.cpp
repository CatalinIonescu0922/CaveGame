/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Renderer/Platform/D3D11/D3D11Image.h>
#include <Renderer/Platform/D3D11/D3D11Renderer.h>
#include <Renderer/Platform/D3D11/D3D11Texture.h>

namespace CaveGame
{

D3D11Texture::D3D11Texture(const TextureDescription& description)
    : m_width(description.width)
    , m_height(description.height)
    , m_format(description.format)
    , m_min_filter(description.min_filter)
    , m_mag_filter(description.mag_filter)
    , m_address_mode_u(description.address_mode_u)
    , m_address_mode_v(description.address_mode_v)
    , m_address_mode_w(description.address_mode_w)
{
    //
    // The specification of the texture.
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_texture2d_desc
    //
    D3D11_TEXTURE2D_DESC texture_description = {};
    texture_description.Width = m_width;
    texture_description.Height = m_height;
    texture_description.MipLevels = 1;
    texture_description.ArraySize = 1;
    texture_description.Format = get_d3d11_image_format(m_format);
    texture_description.SampleDesc.Count = 1;
    texture_description.SampleDesc.Quality = 0;
    texture_description.Usage = D3D11_USAGE_IMMUTABLE;
    texture_description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texture_description.CPUAccessFlags = 0;
    texture_description.MiscFlags = 0;

    CAVE_ASSERT(description.data.has_elements())
    CAVE_ASSERT(description.data.count() % (static_cast<usize>(m_width) * static_cast<usize>(m_height)) == 0);

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data
    D3D11_SUBRESOURCE_DATA texture_initial_data = {};
    texture_initial_data.pSysMem = description.data.elements();
    texture_initial_data.SysMemPitch = static_cast<UINT>(description.data.count() / static_cast<usize>(m_height));

    const HRESULT texture_creation_result = D3D11Renderer::get_device()->CreateTexture2D(&texture_description, &texture_initial_data, &m_handle);
    CAVE_ASSERT(SUCCEEDED(texture_creation_result));

    //
    // The specification of the texture view (shader resource view).
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_shader_resource_view_desc
    //
    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_description = {};
    shader_resource_view_description.Format = get_d3d11_image_format(m_format);
    shader_resource_view_description.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_description.Texture2D.MipLevels = 1;
    shader_resource_view_description.Texture2D.MostDetailedMip = 0;

    const HRESULT texture_view_creation_result =
        D3D11Renderer::get_device()->CreateShaderResourceView(m_handle, &shader_resource_view_description, &m_view_handle);
    CAVE_ASSERT(SUCCEEDED(texture_view_creation_result));

    //
    // The specification of the sampler that is used to sample the texture.
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
    //
    D3D11_SAMPLER_DESC sampler_description = {};
    sampler_description.Filter = get_d3d11_image_filtering_mode(m_min_filter, m_mag_filter);
    sampler_description.AddressU = get_d3d11_image_address_mode(m_address_mode_u);
    sampler_description.AddressV = get_d3d11_image_address_mode(m_address_mode_v);
    sampler_description.AddressW = get_d3d11_image_address_mode(m_address_mode_w);

    const HRESULT texture_sampler_creation_state = D3D11Renderer::get_device()->CreateSamplerState(&sampler_description, &m_sampler_state);
    CAVE_ASSERT(SUCCEEDED(texture_sampler_creation_state));
}

D3D11Texture::~D3D11Texture()
{
    CAVE_D3D11_RELEASE(m_sampler_state);
    CAVE_D3D11_RELEASE(m_view_handle);
    CAVE_D3D11_RELEASE(m_handle);
}

} // namespace CaveGame
