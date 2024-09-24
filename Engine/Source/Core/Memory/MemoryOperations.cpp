/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Memory/MemoryOperations.h>

namespace CaveGame
{

void copy_memory(void* destination, const void* source, usize byte_count)
{
    u8* dst_buffer = static_cast<u8*>(destination);
    const u8* src_buffer = static_cast<const u8*>(source);

    // NOTE: Maybe using compiler intrinsics would generate faster assembly code for the following loop.
    // However, modern compilers should be smart enough to recognize this pattern and optimize it away anyway,
    // thus writting the code as simple as possible might be better.

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset)
        dst_buffer[byte_offset] = src_buffer[byte_offset];
}

void copy_memory_reversed(void* destination, const void* source, usize byte_count)
{
    u8* dst_buffer = static_cast<u8*>(destination);
    const u8* src_buffer = static_cast<const u8*>(source);

    for (usize byte_offset = 1; byte_offset <= byte_count; ++byte_offset)
        dst_buffer[byte_count - byte_offset] = src_buffer[byte_count - byte_offset];
}

void set_memory(void* destination, u8 byte_value, usize byte_count)
{
    u8* dst_buffer = static_cast<u8*>(destination);

    // NOTE: Maybe using compiler intrinsics would generate faster assembly code for the following loop.
    // However, modern compilers should be smart enough to recognize this pattern and optimize it away anyway,
    // thus writting the code as simple as possible might be better.

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset)
        dst_buffer[byte_offset] = byte_value;
}

void zero_memory(void* destination, usize byte_count)
{
    u8* dst_buffer = static_cast<u8*>(destination);

    // NOTE: Maybe using compiler intrinsics would generate faster assembly code for the following loop.
    // However, modern compilers should be smart enough to recognize this pattern and optimize it away anyway,
    // thus writting the code as simple as possible might be better.

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset)
        dst_buffer[byte_offset] = 0;
}

} // namespace CaveGame
