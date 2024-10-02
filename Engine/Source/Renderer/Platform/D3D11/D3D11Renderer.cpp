/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Assertion.h>
#include <Renderer/Platform/D3D11/D3D11Renderer.h>

namespace CaveGame
{

struct D3D11RendererData
{
    ID3D11Device* device { nullptr };
    ID3D11DeviceContext* device_context { nullptr };
    IDXGIFactory2* dxgi_factory { nullptr };
};

static D3D11RendererData* s_d3d11_renderer;

bool D3D11Renderer::initialize()
{
    if (s_d3d11_renderer)
    {
        // The D3D11 renderer has already been initialized.
        return false;
    }

    // Allocate the D3D11 renderer system structure.
    s_d3d11_renderer = new D3D11RendererData();

    UINT device_creation_flags = 0;
#if CAVE_CONFIGURATION_DEBUG
    // Enable the debug flag in the device configuration when compiling to a debug configuration.
    device_creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // CAVE_CONFIGURATION_DEBUG

    const D3D_FEATURE_LEVEL feature_levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    D3D_FEATURE_LEVEL selected_feature_level = {};

    const HRESULT device_creation_result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        device_creation_flags,
        feature_levels,
        ARRAY_COUNT(feature_levels),
        D3D11_SDK_VERSION,
        &s_d3d11_renderer->device,
        &selected_feature_level,
        &s_d3d11_renderer->device_context
    );

    // NOTE: There is no point in trying to continue running the engine if the D3D11 device can't be created.
    // Exiting the application (by forcing to crash in this case) will be the end result anyway.
    CAVE_VERIFY(SUCCEEDED(device_creation_result));

    const HRESULT dxgi_factory_creation_result = CreateDXGIFactory1(IID_PPV_ARGS(&s_d3d11_renderer->dxgi_factory));
    CAVE_VERIFY(SUCCEEDED(dxgi_factory_creation_result));

    return true;
}

void D3D11Renderer::shutdown()
{
    if (!s_d3d11_renderer)
    {
        // The D3D11 renderer has already been shut down.
        return;
    }

    CAVE_D3D11_RELEASE(s_d3d11_renderer->dxgi_factory);
    CAVE_D3D11_RELEASE(s_d3d11_renderer->device_context);
    CAVE_D3D11_RELEASE(s_d3d11_renderer->device);

    delete s_d3d11_renderer;
    s_d3d11_renderer = nullptr;
}

ID3D11Device* D3D11Renderer::get_device()
{
    return s_d3d11_renderer->device;
}

ID3D11DeviceContext* D3D11Renderer::get_device_context()
{
    return s_d3d11_renderer->device_context;
}

IDXGIFactory2* D3D11Renderer::get_dxgi_factory()
{
    return s_d3d11_renderer->dxgi_factory;
}

} // namespace CaveGame
