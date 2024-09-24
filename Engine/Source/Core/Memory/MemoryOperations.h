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

//
// Copies the provided number of bytes from the `destination` buffer to the `source` buffer.
// Both the destination and source buffer must be at least large enough to contain `byte_count` bytes, otherwise
// a buffer overrun may occur. This function performs no such checks, so it is up to the caller to ensure it.
//
void copy_memory(void* destination, const void* source, usize byte_count);

//
// Copies the provided number of bytes from the `destination` buffer to the `source` buffer, in reverse order.
// Both the destination and source buffer must be at least large enough to contain `byte_count` bytes, otherwise
// a buffer overrun may occur. This function performs no such checks, so it is up to the caller to ensure it.
//
void copy_memory_reversed(void* destination, const void* source, usize byte_count);

//
// Sets the first `byte_count` bytes from the `destination` buffer to the provided `byte_value`.
// The destination buffer must be at least large enough to contain `byte_count` bytes, otherwise
// a buffer overrun may occur. This function performs no such checks, so it is up to the caller to ensure it.
//
void set_memory(void* destination, u8 byte_value, usize byte_count);

//
// Sets the first `byte_count` bytes from the `destination` buffer to zero (0).
// The destination buffer must be at least large enough to contain `byte_count` bytes, otherwise
// a buffer overrun may occur. This function performs no such checks, so it is up to the caller to ensure it.
//
void zero_memory(void* destination, usize byte_count);

} // namespace CaveGame
