/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Assertion.h>
#include <Core/Containers/StringView.h>

namespace CaveGame
{

//
// Container that stores a UTF-8 encoded, null-terminated, copy-on-write (COW) string.
//
// If the string is small enough, the threshold being determined by the platform
// architecture, no memory will be allocated from the heap and the characters will
// be instead stored inline. This allows small strings to be very efficient in terms
// of performance, as creating them would be very cheap.
//
class String
{
public:
#if CAVE_COMPILER_MSVC
    // Disables the following compiler warning:
    // 'nonstandard extension used: zero-sized array in struct/union'.
    #pragma warning(push)
    #pragma warning(disable : 4200)
#endif // CAVE_COMPILER_MSVC

    struct HeapBuffer
    {
        CAVE_MAKE_NONCOPYABLE(HeapBuffer);
        CAVE_MAKE_NONMOVABLE(HeapBuffer);

        u32 reference_count;
        char characters[];
    };

#if CAVE_COMPILER_MSVC
    #pragma warning(pop)
#endif // CAVE_COMPILER_MSVC

    static constexpr usize inline_capacity = sizeof(char*);
    static_assert(inline_capacity > 0);

public:
    String();

    String(const String& other);
    String(String&& other) noexcept;
    String(StringView view);

    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String& operator=(StringView view);

    ~String();

public:
    NODISCARD ALWAYS_INLINE bool is_stored_inline() const { return (m_byte_count <= inline_capacity); }

    NODISCARD ALWAYS_INLINE usize byte_count() const { return m_byte_count; }
    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_byte_count == 1); }

    NODISCARD ALWAYS_INLINE const char* characters() const { return is_stored_inline() ? m_inline_buffer : m_heap_buffer->characters; }

    NODISCARD ALWAYS_INLINE StringView view() const
    {
        CAVE_ASSERT(m_byte_count >= 1);
        return StringView::create_from_utf8(characters(), m_byte_count - 1);
    }

public:
    void clear();

private:
    NODISCARD ALWAYS_INLINE HeapBuffer* allocate_heap_buffer(usize byte_count)
    {
        const usize allocation_size = sizeof(HeapBuffer) + byte_count;
        void* memory_block = ::operator new(allocation_size);
        return static_cast<HeapBuffer*>(memory_block);
    }

    ALWAYS_INLINE void release_heap_buffer(HeapBuffer* heap_buffer, usize byte_count)
    {
        MAYBE_UNUSED const usize allocation_size = sizeof(HeapBuffer) + byte_count;
        ::operator delete(heap_buffer);
    }

private:
    usize m_byte_count;
    union
    {
        char m_inline_buffer[inline_capacity];
        HeapBuffer* m_heap_buffer;
    };
};

} // namespace CaveGame
