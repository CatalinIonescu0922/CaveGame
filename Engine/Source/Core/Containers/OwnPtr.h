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

//
// Container that exclusively manages the lifetime of a heap-allocated object instance.
//
template<typename T>
class OwnPtr
{
    CAVE_MAKE_NONCOPYABLE(OwnPtr);

    template<typename Q>
    friend class OwnPtr;

    template<typename Q>
    friend OwnPtr<Q> adopt_own(Q*);

public:
    ALWAYS_INLINE OwnPtr()
        : m_instance(nullptr)
    {}

    ALWAYS_INLINE OwnPtr(OwnPtr&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    ALWAYS_INLINE ~OwnPtr()
    {
        // Destroy the instance and release the memory.
        release();
    }

    ALWAYS_INLINE OwnPtr& operator=(OwnPtr&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        other.m_instance = nullptr;

        return *this;
    }

public:
    // Returns whether or not the OwnPtr stores a valid pointer that can be used.
    NODISCARD ALWAYS_INLINE bool is_valid() const { return (m_instance != nullptr); }
    NODISCARD ALWAYS_INLINE operator bool() const { return is_valid(); }

    //
    // Returns the instance stored by the OwnPtr.
    // If the RefPtr is not valid (`is_valid()` returns false) an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE T* get()
    {
        CAVE_ASSERT(is_valid());
        return m_instance;
    }

    //
    // Returns the instance stored by the OwnPtr.
    // If the RefPtr is not valid (`is_valid()` returns false) an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE const T* get() const
    {
        CAVE_ASSERT(is_valid());
        return m_instance;
    }

    // Wrappers around `get()`.
    NODISCARD ALWAYS_INLINE T* operator->() { return get(); }
    NODISCARD ALWAYS_INLINE const T* operator->() const { return get(); }

    // Wrappers around `get()` that dereference the pointer instance.
    NODISCARD ALWAYS_INLINE T& operator*() { return *get(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return *get(); }

public:
    //
    // Invalidates the OwnPtr by releasing the held object instance.
    // If the OwnPtr holds a valid instance this function will delete it and release its memory.
    //
    ALWAYS_INLINE void release()
    {
        if (m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    //
    // Invalidates the OwnPtr, but doesn't destroy the held object instance. Instead, the held
    // instance is returned (or leaked) by this function. It is the responsability of the caller
    // to manage the instance lifetime going forward.
    // If the OwnPtr is not valid, a null pointer will be returned.
    //
    NODISCARD ALWAYS_INLINE T* release_and_leak_value()
    {
        T* instance = m_instance;
        m_instance = nullptr;
        return instance;
    }

    //
    // Casts the current instance to `Q*` using the `reinterpret_cast` cast operator.
    // Casting an invalid OwnPtr will result in another invalid RefPtr instead of crashing.
    //
    template<typename Q>
    NODISCARD ALWAYS_INLINE OwnPtr<Q> as()
    {
        Q* casted_instance = reinterpret_cast<Q*>(m_instance);
        m_instance = nullptr;
        return OwnPtr<Q>(m_instance);
    }

private:
    ALWAYS_INLINE explicit OwnPtr(T* raw_instance)
        : m_instance(raw_instance)
    {}

private:
    T* m_instance;
};

template<typename T>
NODISCARD ALWAYS_INLINE OwnPtr<T> adopt_own(T* raw_instance)
{
    return OwnPtr<T>(raw_instance);
}

template<typename T, typename... Args>
NODISCARD ALWAYS_INLINE OwnPtr<T> create_own(Args&&... args)
{
    T* raw_instance = new T(forward<Args>(args)...);
    return adopt_own(raw_instance);
}

} // namespace CaveGame
