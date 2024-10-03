/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Containers/String.h>

namespace CaveGame
{

enum class FileResult : u32
{
    Success = 0,
    Unknown = 1,

    FileNotFound,
    AccessDenied,
    StreamNotOpened,
    BufferTooSmall,
};

class FileReadStream
{
    CAVE_MAKE_NONCOPYABLE(FileReadStream);
    CAVE_MAKE_NONMOVABLE(FileReadStream);

public:
    FileReadStream();
    explicit FileReadStream(const String& filepath);

    ~FileReadStream();

public:
    // Returns whether or not the stream has been opened (the native file handle has been created).
    NODISCARD bool is_opened() const;

    // Returns false when the file stream is not opened or the stream has no more data.
    NODISCARD bool is_ready_to_read_from() const;

    // These return zero (0) if the stream is not opened.
    NODISCARD ALWAYS_INLINE usize get_file_size() const { return m_file_size; }
    NODISCARD ALWAYS_INLINE usize get_stream_offset() const { return m_stream_offset; }
    NODISCARD ALWAYS_INLINE bool is_empty_file() const { return (m_file_size == 0); }

    // Returns zero when the stream is not opened.
    NODISCARD ALWAYS_INLINE usize get_remaining_byte_count() const
    {
        CAVE_ASSERT(m_file_size >= m_stream_offset);
        return m_file_size - m_stream_offset;
    }

public:
    FileResult open(const String& filepath);

    void close();
    void reset_stream_pointer();

    //
    // Reads `byte_count` from the stream and advances the pointer by the same number of bytes.
    //
    FileResult read(void* destination_buffer, usize byte_count);

    //
    // Reads `byte_count` from the stream but doesn't advance the pointer. The next `read` or `peek` call
    // will thus read the same memory again.
    //
    FileResult peek(void* destination_buffer, usize byte_count);

    //
    // Writes the entire remaining stream to the provided destination buffer.
    // Get the number of bytes written by calling `get_remaining_byte_count()`.
    //
    FileResult read_entire(void* destination_buffer);

private:
    void* m_native_file_handle;
    usize m_file_size;
    usize m_stream_offset;
};

} // namespace CaveGame
