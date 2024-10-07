/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/Span.h>
#include <Core/CoreTypes.h>

namespace CaveGame
{

#if CAVE_COMPILER_MSVC
    // Disables the following compiler warning:
    // 'operator ""sv': literal suffix identifiers that do not start with an underscore are reserved.
    #pragma warning(push)
    #pragma warning(disable : 4455)
#endif // CAVE_COMPILER_MSVC

//
// A view towards a UTF-8 encoded string.
// The held string is not null-terminated and can't be mutated by the string view.
//
class StringView
{
    friend class StringView constexpr operator""sv(const char*, usize);

public:
    NODISCARD static StringView create_from_utf8(const char* characters, usize byte_count);
    NODISCARD static StringView create_from_utf8(ReadonlyByteSpan character_byte_span);
    NODISCARD static StringView create_from_utf8(const char* null_terminated_characters);

public:
    ALWAYS_INLINE constexpr StringView()
        : m_characters(nullptr)
        , m_byte_count(0)
    {}

    ALWAYS_INLINE constexpr StringView(const StringView& other)
        : m_characters(other.m_characters)
        , m_byte_count(other.m_byte_count)
    {}

    ALWAYS_INLINE constexpr StringView(StringView&& other) noexcept
        : m_characters(other.m_characters)
        , m_byte_count(other.m_byte_count)
    {
        other.m_characters = nullptr;
        other.m_byte_count = 0;
    }

    ALWAYS_INLINE constexpr StringView& operator=(const StringView& other)
    {
        m_characters = other.m_characters;
        m_byte_count = other.m_byte_count;
        return *this;
    }

    ALWAYS_INLINE constexpr StringView& operator=(StringView&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        m_characters = other.m_characters;
        m_byte_count = other.m_byte_count;
        other.m_characters = nullptr;
        other.m_byte_count = 0;
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE const char* characters() const { return m_characters; }
    NODISCARD ALWAYS_INLINE usize byte_count() const { return m_byte_count; }

    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_byte_count == 0); }

private:
    const char* m_characters;
    usize m_byte_count;
};

NODISCARD ALWAYS_INLINE constexpr StringView operator""sv(const char* ascii_literal, usize ascii_literal_character_count)
{
    // Because ASCII is a subset of UTF-8, no conversion is needed.
    StringView view;
    view.m_characters = ascii_literal;
    view.m_byte_count = ascii_literal_character_count;
    return view;
}

#if CAVE_COMPILER_MSVC
    #pragma warning(pop)
#endif // CAVE_COMPILER_MSVC

} // namespace CaveGame
