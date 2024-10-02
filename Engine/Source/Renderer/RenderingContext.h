/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/OwnPtr.h>
#include <Renderer/Image.h>

namespace CaveGame
{

// Forward declarations.
class Window;

class RenderingContext
{
    CAVE_MAKE_NONCOPYABLE(RenderingContext);
    CAVE_MAKE_NONMOVABLE(RenderingContext);

public:
    NODISCARD static OwnPtr<RenderingContext> create(Window* window_context);
    
    virtual ~RenderingContext() = default;
    RenderingContext() = default;

public:
    virtual void invalidate_swapchain(u32 new_width, u32 new_height) = 0;

    NODISCARD virtual u32 get_swapchain_width() const = 0;
    NODISCARD virtual u32 get_swapchain_height() const = 0;
    NODISCARD virtual ImageFormat get_swapchain_image_format() const = 0;

    NODISCARD virtual void* get_swapchain_image(u32 image_index = 0) const = 0;
    NODISCARD virtual void* get_swapchain_image_view(u32 image_index = 0) const = 0;
};

} // namespace CaveGame
