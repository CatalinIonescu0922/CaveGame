/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/RefPtr.h>
#include <Renderer/Image.h>

namespace CaveGame
{

struct TextureDescription
{
    u32 width { 0 };
    u32 height { 0 };
    ImageFormat format { ImageFormat::Unknown };
    
    ImageFilteringMode min_filter { ImageFilteringMode::Linear };
    ImageFilteringMode mag_filter { ImageFilteringMode::Linear };

    ImageAddressMode address_mode_u { ImageAddressMode::Wrap };
    ImageAddressMode address_mode_v { ImageAddressMode::Wrap };
    ImageAddressMode address_mode_w { ImageAddressMode::Wrap };

    ReadonlyByteSpan data;
};

class Texture : public RefCounted
{
public:
    NODISCARD static RefPtr<Texture> create(const TextureDescription& description);

    Texture() = default;
    virtual ~Texture() override = default;

public:
    NODISCARD virtual u32 get_width() const = 0;
    NODISCARD virtual u32 get_height() const = 0;
    NODISCARD virtual ImageFormat get_format() const = 0;
};

} // namespace CaveGame
