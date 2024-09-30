/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Assertion.h>
#include <Core/Containers/String.h>
#include <Core/Memory/MemoryOperations.h>

namespace CaveGame
{

String::String()
    : m_byte_count(1)
{
    m_inline_buffer[0] = 0;
}

String::String(const String& other)
    : m_byte_count(other.m_byte_count)
{
    if (is_stored_inline())
    {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else
    {
        m_heap_buffer = other.m_heap_buffer;
        CAVE_ASSERT(m_heap_buffer);
        m_heap_buffer->reference_count++;
    }
}

String::String(String&& other) noexcept
    : m_byte_count(other.m_byte_count)
{
    if (is_stored_inline())
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    else
        m_heap_buffer = other.m_heap_buffer;

    other.m_heap_buffer = nullptr;
    zero_memory(other.m_inline_buffer, inline_capacity);
    other.m_byte_count = 1;
}

String::String(StringView view)
    : m_byte_count(view.byte_count() + 1)
{
    if (is_stored_inline())
    {
        copy_memory(m_inline_buffer, view.characters(), view.byte_count());
        m_inline_buffer[m_byte_count - 1] = 0;
    }
    else
    {
        m_heap_buffer = allocate_heap_buffer(m_byte_count);
        m_heap_buffer->reference_count = 1;
        copy_memory(m_heap_buffer->characters, view.characters(), view.byte_count());
        m_heap_buffer->characters[m_byte_count - 1] = 0;
    }
}

String& String::operator=(const String& other)
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    clear();

    m_byte_count = other.m_byte_count;
    if (is_stored_inline())
    {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else
    {
        m_heap_buffer = other.m_heap_buffer;
        CAVE_ASSERT(m_heap_buffer);
        m_heap_buffer->reference_count++;
    }

    return *this;
}

String& String::operator=(String&& other) noexcept
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    clear();

    m_byte_count = other.m_byte_count;
    if (is_stored_inline())
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    else
        m_heap_buffer = other.m_heap_buffer;

    other.m_heap_buffer = nullptr;
    zero_memory(other.m_inline_buffer, inline_capacity);
    other.m_byte_count = 1;

    return *this;
}

String& String::operator=(StringView view)
{
    clear();

    m_byte_count = view.byte_count() + 1;
    if (is_stored_inline())
    {
        copy_memory(m_inline_buffer, view.characters(), view.byte_count());
        m_inline_buffer[m_byte_count - 1] = 0;
    }
    else
    {
        m_heap_buffer = allocate_heap_buffer(m_byte_count);
        m_heap_buffer->reference_count = 1;
        copy_memory(m_heap_buffer->characters, view.characters(), view.byte_count());
        m_heap_buffer->characters[m_byte_count - 1] = 0;
    }

    return *this;
}

String::~String()
{
    clear();
}

void String::clear()
{
    if (is_stored_inline())
    {
        zero_memory(m_inline_buffer, m_byte_count);
    }
    else
    {
        CAVE_ASSERT(m_heap_buffer);
        m_heap_buffer->reference_count--;
        if (m_heap_buffer->reference_count == 0)
            release_heap_buffer(m_heap_buffer, m_byte_count);

        m_heap_buffer = nullptr;
    }

    m_byte_count = 1;
    m_inline_buffer[0] = 0;
}

} // namespace CaveGame
