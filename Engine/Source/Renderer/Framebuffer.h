/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/Optional.h>
#include <Core/Containers/RefPtr.h>
#include <Core/Containers/Vector.h>
#include <Renderer/Image.h>
#include <Renderer/Platform/D3D11/D3D11GuardedInclude.h>

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

    friend class RenderingContext;

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

    ~Framebuffer();

public:
    //
    // Invalidates the current framebuffer by resizing its attachments.
    // If the framebuffer is a swapchain target, the provided width and height parameters are ignored (and
    // thus should be set to zero).
    //
    void invalidate(u32 new_width, u32 new_height);

    NODISCARD ALWAYS_INLINE bool is_swapchain_target() const { return m_is_swapchain_target; }

    NODISCARD ALWAYS_INLINE u32 get_width() const { return m_width; }
    NODISCARD ALWAYS_INLINE u32 get_height() const { return m_height; }
    NODISCARD ALWAYS_INLINE u32 get_attachment_count() const { return static_cast<u32>(m_attachments.count()); }

    NODISCARD void* get_attachment_image(u32 attachment_index) const;
    NODISCARD void* get_attachment_image_view(u32 attachment_index) const;
    NODISCARD const FramebufferAttachmentDescription& get_attachment_description(u32 attachment_index) const;

private:
    explicit Framebuffer(const FramebufferDescription& description);
    explicit Framebuffer(RenderingContext& rendering_context);

    void invalidate_swapchain_target();

    void destroy();

private:
    struct Attachment
    {
        ID3D11Texture2D* image_handle;
        ID3D11RenderTargetView* image_rtv_handle;
        FramebufferAttachmentDescription description;
    };

    bool m_is_swapchain_target;
    // Has value only when the framebuffer is a swapchain target.
    Optional<RenderingContext&> m_rendering_context;

    u32 m_width;
    u32 m_height;
    Vector<Attachment> m_attachments;
};

} // namespace CaveGame
