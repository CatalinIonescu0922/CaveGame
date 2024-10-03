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

class Buffer
{
    CAVE_MAKE_NONCOPYABLE(Buffer);

public:
    NODISCARD static Buffer create(usize initial_byte_count);

    NODISCARD static Buffer copy(const void* initial_data, usize initial_byte_count);

    NODISCARD ALWAYS_INLINE static Buffer copy(const Buffer& source) { return Buffer::copy(source.data(), source.byte_count()); }

public:
    Buffer();
    ~Buffer();

    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

public:
    NODISCARD ALWAYS_INLINE void* data() { return m_data; }
    NODISCARD ALWAYS_INLINE const void* data() const { return m_data; }

    NODISCARD ALWAYS_INLINE usize byte_count() const { return m_byte_count; }
    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_byte_count == 0); }

public:
    void allocate_new(usize new_byte_count);

    void expand(usize new_byte_count);

    void release();

private:
    void* m_data;
    usize m_byte_count;
};

} // namespace CaveGame
