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
// Container that stores and manages a contiguos array of heap-allocated elements.
// This is our equivalent implementation of the `std::vector` container.
//
template<typename T>
class Vector
{
public:
    static constexpr usize growth_factor_numerator = 3;
    static constexpr usize growth_factor_denominator = 2;
    // NOTE: The growth factor must always be strictly greater than one, otherwise the vector
    // will always expand by only one element, no matter its size. This can create performance issues.
    static_assert(growth_factor_numerator > growth_factor_denominator);

    using Iterator = T*;
    using ConstIterator = const T*;
    // using ReverseIterator = T*;
    // using ConstReverseIterator = const T*;

public:
    ALWAYS_INLINE Vector()
        : m_elements(nullptr)
        , m_capacity(0)
        , m_count(0)
    {}

    ALWAYS_INLINE Vector(const Vector& other)
        : m_capacity(other.m_count)
        , m_count(other.m_count)
    {
        m_elements = allocate_memory(m_capacity);
        copy_elements(m_elements, other.m_elements, m_count);
    }

    ALWAYS_INLINE Vector(Vector&& other) noexcept
        : m_elements(other.m_elements)
        , m_capacity(other.m_capacity)
        , m_count(other.m_count)
    {
        other.m_elements = nullptr;
        other.m_capacity = 0;
        other.m_count = 0;
    }

    ALWAYS_INLINE Vector& operator=(const Vector& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();

        if (m_capacity < other.m_count)
        {
            release_memory(m_elements, m_capacity);
            m_capacity = other.m_count;
            m_elements = allocate_memory(m_capacity);
        }

        copy_elements(m_elements, other.m_elements, m_count);
        return *this;
    }

    ALWAYS_INLINE Vector& operator=(Vector&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear_and_shrink();

        m_elements = other.m_elements;
        m_capacity = other.m_capacity;
        m_count = other.m_count;

        other.m_elements = nullptr;
        other.m_capacity = 0;
        other.m_count = 0;

        return *this;
    }

    ALWAYS_INLINE ~Vector()
    {
        // Destroy the elements and release the memory.
        clear_and_shrink();
    }

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }

    NODISCARD ALWAYS_INLINE usize capacity() const { return m_capacity; }
    NODISCARD ALWAYS_INLINE usize count() const { return m_count; }

    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_count == 0); }
    NODISCARD ALWAYS_INLINE bool has_elements() const { return (m_count > 0); }

public:
    //
    // Returns the element stored at the given index in the internal array.
    // If the index is out of bounds, an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE T& at(usize index)
    {
        CAVE_ASSERT(index < m_count);
        return m_elements[index];
    }

    //
    // Returns the element stored at the given index in the internal array.
    // If the index is out of bounds, an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE const T& at(usize index) const
    {
        CAVE_ASSERT(index < m_count);
        return m_elements[index];
    }

    // Direct wrappers around the `Vector::at()` API.
    NODISCARD ALWAYS_INLINE T& operator[](usize index) { return at(index); }
    NODISCARD ALWAYS_INLINE const T& operator[](usize index) const { return at(index); }

    //
    // Returns the first element stored in the internal array. Note that this is not undefined behaviour
    // as the elements are stored contiguosly (and thus ordered) in memory.
    // If the container is empty, an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE T& first()
    {
        CAVE_ASSERT(has_elements());
        return m_elements[0];
    }

    //
    // Returns the first element stored in the internal array. Note that this is not undefined behaviour
    // as the elements are stored contiguosly (and thus ordered) in memory.
    // If the container is empty, an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE const T& first() const
    {
        CAVE_ASSERT(has_elements());
        return m_elements[0];
    }

    //
    // Returns the last element stored in the internal array. Note that this is not undefined behaviour
    // as the elements are stored contiguosly (and thus ordered) in memory.
    // If the container is empty, an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE T& last()
    {
        CAVE_ASSERT(has_elements());
        return m_elements[m_count - 1];
    }

    //
    // Returns the last element stored in the internal array. Note that this is not undefined behaviour
    // as the elements are stored contiguosly (and thus ordered) in memory.
    // If the container is empty, an assert will be triggered.
    //
    NODISCARD ALWAYS_INLINE const T& last() const
    {
        CAVE_ASSERT(has_elements());
        return m_elements[m_count - 1];
    }

public:
    //
    // Constructs a new element at the end of the internal array by forwarding the provided
    // parameters to the object constructor.
    // If the memory block is not big enough to store another element the container will expand.
    //
    template<typename... Args>
    ALWAYS_INLINE void emplace(Args&&... args)
    {
        ensure_capacity(m_count + 1);
        new (m_elements + m_count) T(forward<Args>(args)...);
        ++m_count;
    }

    // Wrappers around `Vector::emplace`.
    ALWAYS_INLINE void add(const T& element) { emplace(element); }
    ALWAYS_INLINE void add(T&& element) { emplace(move(element)); }

public:
    //
    // Destroys all elements stored in the container without releasing the internal memory block,
    // thus the capacity of the vector will remain unchanged.
    //
    ALWAYS_INLINE void clear()
    {
        for (usize index = 0; index < m_count; ++index)
            m_elements[index].~T();
        m_count = 0;
    }

    //
    // Destroys the elements stored in the container and releases the internal memory block.
    // The capacity of the vector will be zero.
    //
    ALWAYS_INLINE void clear_and_shrink()
    {
        clear();

        release_memory(m_elements, m_capacity);
        m_elements = nullptr;
        m_capacity = 0;
    }

    //
    // Shrinks the vector internal memory block such that it will have the capacity to store exactly
    // the number of elements currently held by the container, thus wasting no memory.
    // The capacity of the vector will be equal to the number of elements stored in it.
    //
    ALWAYS_INLINE void shrink_to_fit()
    {
        if (m_count == m_capacity)
            return;

        T* new_elements = allocate_memory(m_count);
        move_elements(new_elements, m_elements, m_count);

        release_memory(m_elements, m_capacity);
        m_elements = new_elements;
        m_capacity = m_count;
    }

public:
    // Ensures that at least `in_capacity` elements can be stored without expanding the memory block.
    // The actual capacity *is not* guaranteed to be `in_capacity` after calling this function.
    ALWAYS_INLINE void ensure_capacity(usize in_capacity)
    {
        if (m_capacity >= in_capacity)
            return;

        const usize new_capacity = calculate_next_capacity(in_capacity, m_capacity);
        T* new_elements = allocate_memory(new_capacity);
        move_elements(new_elements, m_elements, m_count);

        release_memory(m_elements, m_capacity);
        m_elements = new_elements;
        m_capacity = new_capacity;
    }

    // Ensures that at least `in_capacity` elements can be stored without expanding the memory block.
    // The actual capacity *is* guaranteed to be `in_capacity` after calling this function.
    ALWAYS_INLINE void set_capacity(usize in_capacity)
    {
        if (m_capacity == in_capacity)
            return;

        CAVE_ASSERT(in_capacity > m_count);
        T* new_elements = allocate_memory(in_capacity);
        move_elements(new_elements, m_elements, m_count);

        release_memory(m_elements, m_capacity);
        m_elements = new_elements;
        m_capacity = in_capacity;
    }

    // Sets the number of elements currently stored in the container. If the new count is greater than
    // the current count, the new elements are not initialized in any way.
    ALWAYS_INLINE void set_count_uninitialized(usize in_count)
    {
        if (in_count == m_count)
            return;

        ensure_capacity(in_count);

        // If the new count is less than the current count the last `m_count - in_count` elements
        // must be destroyed. Note that if this is not the case, this loop does nothing.
        for (usize index = in_count; index < m_count; ++index)
            m_elements[index].~T();

        m_count = in_count;
    }

    // Sets the number of elements currently stored in the container. If the new count is greater than
    // the current count, the new elements are initialized using the default constructor.
    ALWAYS_INLINE void set_count_defaulted(usize in_count)
    {
        const usize current_count = m_count;
        set_count_uninitialized();

        // If the new count is greater than the current count the last `in_count - current_count` elements
        // must be initialized (using their default constructor). Note that if this is not the case, this loop does nothing.
        for (usize index = current_count; index < m_count; ++index)
            new (m_elements + index) T();
    }

    // Sets the number of elements currently stored in the container. If the new count is greater than
    // the current count, the new elements are initialized using the copy constructor and the provided `constructor_element`.
    ALWAYS_INLINE void set_count(usize in_count, const T& constructor_element)
    {
        const usize current_count = m_count;
        set_count_uninitialized();

        // If the new count is greater than the current count the last `in_count - current_count` elements
        // must be initialized (using their copy constructor). Note that if this is not the case, this loop does nothing.
        for (usize index = current_count; index < m_count; ++index)
            new (m_elements + index) T(constructor_element);
    }

public:
    NODISCARD ALWAYS_INLINE Iterator begin() { return Iterator(m_elements); }
    NODISCARD ALWAYS_INLINE Iterator end() { return Iterator(m_elements + m_count); }

    NODISCARD ALWAYS_INLINE ConstIterator begin() const { return ConstIterator(m_elements); }
    NODISCARD ALWAYS_INLINE ConstIterator end() const { return ConstIterator(m_elements + m_count); }

private:
    // Allocates a memory block large enough to store `in_capacity` elements.
    NODISCARD ALWAYS_INLINE static T* allocate_memory(usize in_capacity)
    {
        const usize allocation_size = in_capacity * sizeof(T);
        void* memory_block = ::operator new(allocation_size);
        return static_cast<T*>(memory_block);
    }

    // Releases a memory block large enough to store `in_capacity` elements located at address `in_elements`.
    ALWAYS_INLINE static void release_memory(T* in_elements, usize in_capacity)
    {
        MAYBE_UNUSED const usize allocation_size = in_capacity * sizeof(T);
        ::operator delete(in_elements);
    }

    //
    // Calculates the next capacity of the vector when an expansion is requested.
    // By default, the vector will try to increase its capacity using the provided growth factor. If the
    // capacity calculated given by the geometric series is not sufficient, the next capacity will be the minimum required.
    //
    NODISCARD ALWAYS_INLINE static usize calculate_next_capacity(usize required_capacity, usize current_capacity)
    {
        const usize next_geometric_capacity = (current_capacity * growth_factor_numerator) / growth_factor_denominator;
        if (next_geometric_capacity > required_capacity)
            return next_geometric_capacity;
        return required_capacity;
    }

    // Copies `element_count` elements from the `source_elements` buffer to the `destination_elements` buffer using their
    // copy constructor.
    ALWAYS_INLINE static void copy_elements(T* destination_elements, const T* source_elements, usize element_count)
    {
        for (usize index = 0; index < element_count; ++index)
            new (destination_elements + index) T(source_elements[index]);
    }

    // Moves `element_count` elements from the `source_elements` buffer to the `destination_elements` buffer using their
    // move constructor and deleted the elements stored in the `source_elements` buffer.
    ALWAYS_INLINE static void move_elements(T* destination_elements, T* source_elements, usize element_count)
    {
        for (usize index = 0; index < element_count; ++index)
        {
            new (destination_elements + index) T(move(source_elements[index]));
            source_elements[index].~T();
        }
    }

private:
    T* m_elements;
    usize m_capacity;
    usize m_count;
};

} // namespace CaveGame
