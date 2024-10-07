/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Assertion.h>
#include <Core/CoreTypes.h>

namespace CaveGame
{

template<typename T>
class Span
{
    template<typename Q>
    friend class Span;

public:
    ALWAYS_INLINE Span()
        : m_elements(nullptr)
        , m_count(0)
    {}

    ALWAYS_INLINE Span(const Span& other)
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {}

    template<typename Q>
    requires (is_const<T> && is_same<RemoveConst<T>, Q>)
    ALWAYS_INLINE Span(const Span<Q>& other)
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {}

    ALWAYS_INLINE Span(Span&& other) noexcept
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {
        other.m_elements = nullptr;
        other.m_count = 0;
    }

    ALWAYS_INLINE Span(T* in_elements, usize in_count)
        : m_elements(in_elements)
        , m_count(in_count)
    {}

    ALWAYS_INLINE Span& operator=(const Span& other)
    {
        m_elements = other.m_elements;
        m_count = other.m_count;
        return *this;
    }

    template<typename Q>
    requires (is_const<T> && is_same<RemoveConst<T>, Q>)
    ALWAYS_INLINE Span& operator=(const Span<Q>& other)
    {
        m_elements = other.m_elements;
        m_count = other.m_count;
        return *this;
    }

    ALWAYS_INLINE Span& operator=(Span&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        m_elements = other.m_elements;
        m_count = other.m_count;
        other.m_elements = nullptr;
        other.m_count = 0;

        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }

    NODISCARD ALWAYS_INLINE usize count() const { return m_count; }
    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_count == 0); }
    NODISCARD ALWAYS_INLINE bool has_elements() const { return (m_count > 0); }

    NODISCARD ALWAYS_INLINE Span<const u8> readonly_byte_span() const
    {
        const usize byte_count = m_count * sizeof(T);
        return Span<const u8>(reinterpret_cast<const u8*>(m_elements), byte_count);
    }
private:
    T* m_elements;
    usize m_count;
};

using ReadonlyByteSpan = Span<const u8>;
using ReadWriteByteSpan = Span<u8>;

} // namespace CaveGame
