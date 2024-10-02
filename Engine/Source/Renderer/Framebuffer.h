/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/RefPtr.h>
#include <Core/Containers/Vector.h>
#include <Renderer/Image.h>

namespace CaveGame
{

// Forward declarations.
class RenderingContext;

struct FramebufferAttachmentDescription
{
    ALWAYS_INLINE FramebufferAttachmentDescription()
        : format(ImageFormat::B8G8R8A8)
    {}

    ALWAYS_INLINE FramebufferAttachmentDescription(ImageFormat in_format)
        : format(in_format)
    {}

    ImageFormat format;

    ImageFilteringMode min_filtering_mode { ImageFilteringMode::Linear };
    ImageFilteringMode mag_filtering_mode { ImageFilteringMode::Linear };

    ImageAddressMode address_mode_u { ImageAddressMode::Wrap };
    ImageAddressMode address_mode_v { ImageAddressMode::Wrap };
    ImageAddressMode address_mode_w { ImageAddressMode::Wrap };
};

struct FramebufferDescription
{
    u32 width;
    u32 heigth;
    Vector<FramebufferAttachmentDescription> attachments;
};

class Framebuffer : public RefCounted
{
    CAVE_MAKE_NONCOPYABLE(Framebuffer);
    CAVE_MAKE_NONMOVABLE(Framebuffer);

public:
    //
    // Creates a new framebuffer by allocating a new image for each attachment.
    //
    NODISCARD static RefPtr<Framebuffer> create(const FramebufferDescription& description);

    //
    // Creates a new framebuffer that represents a swapchain target, meaning that the attachment images are not allocated
    // from scratch and instead they reference an image of the swapchain.
    //
    NODISCARD static RefPtr<Framebuffer> create(RenderingContext& rendering_context);

    Framebuffer() = default;
    virtual ~Framebuffer() override = default;

public:
    //
    // Invalidates the current framebuffer by resizing its attachments.
    // If the framebuffer is a swapchain target, the provided width and height parameters are ignored (and
    // thus should be set to zero).
    //
    virtual void invalidate(u32 new_width, u32 new_height) = 0;

    NODISCARD virtual bool is_swapchain_target() const = 0;

    NODISCARD virtual u32 get_width() const = 0;
    NODISCARD virtual u32 get_height() const = 0;
    NODISCARD virtual u32 get_attachment_count() const = 0;

    NODISCARD virtual void* get_attachment_image(u32 attachment_index) const = 0;
    NODISCARD virtual void* get_attachment_image_view(u32 attachment_index) const = 0;
    NODISCARD virtual const FramebufferAttachmentDescription& get_attachment_description(u32 attachment_index) const = 0;
};

} // namespace CaveGame
