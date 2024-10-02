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
#include <Core/Memory/New.h>

namespace CaveGame
{

template<typename T>
class Optional
{
public:
    ALWAYS_INLINE Optional()
        : m_value_storage {}
        , m_has_value(false)
    {}

    ALWAYS_INLINE Optional(const Optional& other)
        : m_has_value(other.m_has_value)
    {
        if (m_has_value)
            new (m_value_storage) T(*other.raw_value_ptr());
    }

    ALWAYS_INLINE Optional(Optional&& other) noexcept
        : m_has_value(other.m_has_value)
    {
        if (m_has_value)
        {
            new (m_value_storage) T(move(*other.raw_value_ptr()));
            other.clear();
        }
    }

    ALWAYS_INLINE Optional(const T& value)
        : m_has_value(true)
    {
        new (m_value_storage) T(value);
    }

    ALWAYS_INLINE Optional(T&& value)
        : m_has_value(true)
    {
        new (m_value_storage) T(move(value));
    }

    ALWAYS_INLINE ~Optional() { clear(); }

    ALWAYS_INLINE Optional& operator=(const Optional& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();

        if (other.m_has_value)
        {
            new (m_value_storage) T(*other.raw_value_ptr());
            m_has_value = true;
        }

        return *this;
    }

    ALWAYS_INLINE Optional& operator=(Optional&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();

        if (other.m_has_value)
        {
            new (m_value_storage) T(move(*other.raw_value_ptr()));
            m_has_value = true;
            other.clear();
        }

        return *this;
    }

    ALWAYS_INLINE Optional& operator=(const T& value)
    {
        clear();
        new (m_value_storage) T(value);
        m_has_value = true;
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(T&& value)
    {
        clear();
        new (m_value_storage) T(move(value));
        m_has_value = true;
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE bool has_value() const { return m_has_value; }
    NODISCARD ALWAYS_INLINE operator bool() const { return has_value(); }

    NODISCARD ALWAYS_INLINE T& value()
    {
        CAVE_ASSERT(has_value());
        return *raw_value_ptr();
    }

    NODISCARD ALWAYS_INLINE const T& value() const
    {
        CAVE_ASSERT(has_value());
        return *raw_value_ptr();
    }

    NODISCARD ALWAYS_INLINE T* operator->() { return &value(); }
    NODISCARD ALWAYS_INLINE const T* operator->() const { return &value(); }

public:
    ALWAYS_INLINE void clear()
    {
        if (m_has_value)
        {
            raw_value_ptr()->~T();
            m_has_value = false;
        }
    }

private:
    NODISCARD ALWAYS_INLINE T* raw_value_ptr() { return reinterpret_cast<T*>(m_value_storage); }
    NODISCARD ALWAYS_INLINE const T* raw_value_ptr() const { return reinterpret_cast<const T*>(m_value_storage); }

private:
    alignas(T) u8 m_value_storage[sizeof(T)];
    bool m_has_value;
};

template<typename T>
class Optional<T&>
{
public:
    ALWAYS_INLINE Optional()
        : m_value(nullptr)
    {}

    ALWAYS_INLINE Optional(const Optional& other)
        : m_value(other.m_value)
    {}

    ALWAYS_INLINE Optional(Optional&& other) noexcept
        : m_value(other.m_value)
    {
        other.m_value = nullptr;
    }

    ALWAYS_INLINE Optional(T& value)
        : m_value(&value)
    {}

    ALWAYS_INLINE Optional& operator=(const Optional& other)
    {
        m_value = other.m_value;
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(Optional&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        m_value = other.m_value;
        other.m_value = nullptr;

        return *this;
    }

    ALWAYS_INLINE Optional& operator=(T& value)
    {
        m_value = &value;
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE bool has_value() const { return (m_value != nullptr); }
    NODISCARD ALWAYS_INLINE operator bool() const { return has_value(); }

    NODISCARD ALWAYS_INLINE T& value()
    {
        CAVE_ASSERT(has_value());
        return *m_value;
    }

    NODISCARD ALWAYS_INLINE const T& value() const
    {
        CAVE_ASSERT(has_value());
        return *m_value;
    }

    NODISCARD ALWAYS_INLINE T* operator->() { return &value(); }
    NODISCARD ALWAYS_INLINE const T* operator->() const { return &value(); }

public:
    ALWAYS_INLINE void clear()
    {
        // Simply set the value poitner to null to mark an empty reference optional.
        m_value = nullptr;
    }

private:
    T* m_value;
};

} // namespace CaveGame
