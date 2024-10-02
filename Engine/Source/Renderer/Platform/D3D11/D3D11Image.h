/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Assertion.h>
#include <Renderer/Image.h>
#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>

namespace CaveGame
{

//
// Converts from our representation of the image format to the D3D11 (DXGI) representation.
// Returns `DXGI_FORMAT_UNKNOWN` if invalid input is given.
//
NODISCARD ALWAYS_INLINE DXGI_FORMAT get_d3d11_image_format(ImageFormat image_format)
{
    switch (image_format)
    {
        case ImageFormat::Unknown: return DXGI_FORMAT_UNKNOWN;
        case ImageFormat::B8G8R8A8: return DXGI_FORMAT_B8G8R8A8_UNORM;
    }

    CAVE_ASSERT(false);
    return DXGI_FORMAT_UNKNOWN;
}

//
// Converts from our representation of the image filtering mode to the D3D11 representation.
// Returns `D3D11_FILTER_MIN_MAG_MIP_LINEAR` if invalid input is given.
//
NODISCARD ALWAYS_INLINE D3D11_FILTER get_d3d11_image_filtering_mode(ImageFilteringMode min_filtering_mode, ImageFilteringMode mag_filtering_mode)
{
    // NOTE: Who decided that this D3D11 filter enum is good???
    // Why on earth not have a structure with three distinct flags for each min, mag, mip filter.

    if (min_filtering_mode == ImageFilteringMode::Nearest)
    {
        if (mag_filtering_mode == ImageFilteringMode::Nearest)
            return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

        if (mag_filtering_mode == ImageFilteringMode::Linear)
            return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
    }
    else if (min_filtering_mode == ImageFilteringMode::Linear)
    {
        if (mag_filtering_mode == ImageFilteringMode::Nearest)
            return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

        if (mag_filtering_mode == ImageFilteringMode::Linear)
            return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    }

    CAVE_ASSERT(false);
    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
}

//
// Converts from our representation of the image address mode to the D3D11 representation.
// Returns `D3D11_TEXTURE_ADDRESS_WRAP` if invalid input is given.
//
NODISCARD ALWAYS_INLINE D3D11_TEXTURE_ADDRESS_MODE get_d3d11_image_address_mode(ImageAddressMode image_address_mode)
{
    switch (image_address_mode)
    {
        case ImageAddressMode::Unknown: return D3D11_TEXTURE_ADDRESS_WRAP;
        case ImageAddressMode::Wrap: return D3D11_TEXTURE_ADDRESS_WRAP;
        case ImageAddressMode::Mirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
        case ImageAddressMode::Clamp: return D3D11_TEXTURE_ADDRESS_CLAMP;
    }

    CAVE_ASSERT(false);
    return D3D11_TEXTURE_ADDRESS_WRAP;
}

} // namespace CaveGame
