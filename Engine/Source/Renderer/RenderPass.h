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
#include <Core/Math/Color.h>
#include <Renderer/Framebuffer.h>

namespace CaveGame
{

enum class RenderPassAttachmentLoadOperation : u8
{
    Unknown = 0,
    Load,
    Clear,
    DontCare,
};

enum class RenderPassAttachmentStoreOperation : u8
{
    Unknown = 0,
    Store,
    DontCare,
};

struct RenderPassAttachmentDescription
{
    RenderPassAttachmentLoadOperation load_operation { RenderPassAttachmentLoadOperation::Load };
    RenderPassAttachmentStoreOperation store_operation { RenderPassAttachmentStoreOperation::Store };
    
    // Only used when the load operation is set to `Clear`. Otherwise, ignored.
    Color4 clear_color;
};

struct RenderPassDescription
{
    RefPtr<Framebuffer> target_framebuffer;
    Vector<RenderPassAttachmentDescription> target_framebuffer_attachments;
};

class RenderPass : public RefCounted
{
    CAVE_MAKE_NONCOPYABLE(RenderPass);
    CAVE_MAKE_NONMOVABLE(RenderPass);

public:
    NODISCARD static RefPtr<RenderPass> create(const RenderPassDescription& description);

    RenderPass() = default;
    virtual ~RenderPass() override = default;
};

} // namespace CaveGame
