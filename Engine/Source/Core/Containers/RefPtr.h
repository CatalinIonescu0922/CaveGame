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
// Base class for all types that are intended to be managed by a RefPtr.
// Holds the object's reference count intrusively and manages the increment/decrement operations.
//
class RefCounted
{
    CAVE_MAKE_NONCOPYABLE(RefCounted);
    CAVE_MAKE_NONMOVABLE(RefCounted);

    template<typename T>
    friend class RefPtr;

public:
    ALWAYS_INLINE RefCounted()
        : m_reference_count(0)
    {}

    virtual ~RefCounted() = default;

private:
    ALWAYS_INLINE void increment_reference_count() { ++m_reference_count; }

    //
    // Returns true if the reference count hits zero after the decrement operation, signaling
    // that the instance should be deleted as it is not referenced by anyone.
    //
    NODISCARD ALWAYS_INLINE bool decrement_reference_count()
    {
        // Decrementing the reference count of an instance that should have been deleted is not valid.
        CAVE_DEBUG_ASSERT(m_reference_count > 0);

        --m_reference_count;
        return (m_reference_count == 0);
    }

private:
    u32 m_reference_count;
};

//
// Container that manages the lifetime of an intrusive reference counted object instance.
// The provided template parameter type must be derived from `RefCounted`, otherwise a static assert will be issued.
//
template<typename T>
class RefPtr
{
    template<typename Q>
    friend class RefPtr;

    template<typename Q>
    friend RefPtr<Q> adopt_ref(Q* raw_instance);

public:
    ALWAYS_INLINE RefPtr()
        : m_instance(nullptr)
    {}

    ALWAYS_INLINE RefPtr(const RefPtr& other)
        : m_instance(other.m_instance)
    {
        if (m_instance)
            increment_reference_count();
    }

    ALWAYS_INLINE RefPtr(RefPtr&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    ALWAYS_INLINE ~RefPtr() { release(); }

    ALWAYS_INLINE RefPtr& operator=(const RefPtr& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        if (m_instance)
            increment_reference_count();

        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(RefPtr&& other) noexcept
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
    // Returns whether or not the RefPtr stores a valid pointer that can be used.
    NODISCARD ALWAYS_INLINE bool is_valid() const { return (m_instance != nullptr); }
    NODISCARD ALWAYS_INLINE operator bool() const { return is_valid(); }

    //
    // Returns the instance stored by the RefPtr.
    // If the RefPtr is not valid (`is_valid()` returns false) an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE T* get()
    {
        CAVE_ASSERT(is_valid());
        return m_instance;
    }

    //
    // Returns the instance stored by the RefPtr.
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
    // Invalidates the RefPtr by releasing the held reference to the object instance.
    // If the instance's reference count hits zero after the decrement operation (the object is not
    // reference by any other RefPtr's) this function will delete the object.
    //
    ALWAYS_INLINE void release()
    {
        if (m_instance)
        {
            if (decrement_reference_count())
                delete m_instance;
            m_instance = nullptr;
        }
    }

    //
    // Casts the current instance to `Q*` using the `reinterpret_cast` cast operator.
    // Casting an invalid RefPtr will result in another invalid RefPtr instead of crashing.
    //
    template<typename Q>
    NODISCARD ALWAYS_INLINE RefPtr<Q> as() const
    {
        Q* casted_raw_instance = reinterpret_cast<Q*>(m_instance);
        return RefPtr<Q>(casted_raw_instance);
    }

    //
    // Returns whether or not the provided RefPtr's store a pointer to the same object instance.
    // If both `this` and `other` are invalid RefPtr's, this function will return true, as they are
    // still considered to be equal.
    //
    NODISCARD ALWAYS_INLINE bool operator==(const RefPtr& other) const
    {
        const bool hold_the_same_pointer = (m_instance == other.m_instance);
        return hold_the_same_pointer;
    }

    //
    // Returns whether or not the provided RefPtr's atore a pointer to different object instances (are inequal).
    // If both `this` and `other` are invalid RefPtr's, this function will return false, as it negates the
    // result given by the equality check operator.
    //
    NODISCARD ALWAYS_INLINE bool operator!=(const RefPtr& other) const
    {
        // NOTE: The inequal operator is derived from the equal operator by negating its result.
        const bool are_equal = (*this == other);
        return !are_equal;
    }

private:
    ALWAYS_INLINE explicit RefPtr(T* raw_instance)
        : m_instance(raw_instance)
    {
        if (m_instance)
            increment_reference_count();
    }

    ALWAYS_INLINE void increment_reference_count()
    {
        static_assert(is_derived_from<T, RefCounted>, "T must be derived from RefCounted!");
        RefCounted* ref_counted = static_cast<RefCounted*>(m_instance);
        ref_counted->increment_reference_count();
    }

    NODISCARD ALWAYS_INLINE bool decrement_reference_count()
    {
        static_assert(is_derived_from<T, RefCounted>, "T must be derived from RefCounted!");
        RefCounted* ref_counted = static_cast<RefCounted*>(m_instance);
        return ref_counted->decrement_reference_count();
    }

private:
    T* m_instance;
};

template<typename T>
NODISCARD ALWAYS_INLINE RefPtr<T> adopt_ref(T* raw_instance)
{
    return RefPtr<T>(raw_instance);
}

template<typename T, typename... Args>
NODISCARD ALWAYS_INLINE RefPtr<T> create_ref(Args&&... args)
{
    T* raw_instance = new T(forward<Args>(args)...);
    return adopt_ref(raw_instance);
}

} // namespace CaveGame
