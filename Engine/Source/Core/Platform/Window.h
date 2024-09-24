/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/CoreTypes.h>

namespace CaveGame
{

class Window
{
public:
    //
    // Initializes the window by creating the native window object.
    // Returns false if the window initialization has failed.
    //
    bool initialize();

    //
    // Shuts down the window by destroying the native window object.
    //
    void shutdown();

    NODISCARD ALWAYS_INLINE bool should_close() const { return m_should_close; }
    ALWAYS_INLINE void mark_as_should_close() { m_should_close = true; }

    void process_event_queue();

public:
    NODISCARD ALWAYS_INLINE void* get_native_handle() const { return m_native_handle; }

    NODISCARD u32 get_client_area_width() const;
    NODISCARD u32 get_client_area_height() const;

private:
    void* m_native_handle { nullptr };
    bool m_should_close { false };
};

} // namespace CaveGame
