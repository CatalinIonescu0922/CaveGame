/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/CoreDefines.h>
#if CAVE_PLATFORM_WINDOWS

    #include <Core/Platform/FileStream.h>
    #include <Core/Platform/Windows/WindowsGuardedInclude.h>

namespace CaveGame
{

FileReadStream::FileReadStream()
    : m_native_file_handle(INVALID_HANDLE_VALUE)
    , m_file_size(0)
    , m_stream_offset(0)
{}

FileReadStream::FileReadStream(const String& filepath)
    : m_native_file_handle(INVALID_HANDLE_VALUE)
    , m_file_size(0)
    , m_stream_offset(0)
{
    const FileResult open_file_result = open(filepath);
    CAVE_ASSERT(open_file_result == FileResult::Success);
}

FileReadStream::~FileReadStream()
{
    close();
}

bool FileReadStream::is_opened() const
{
    return (m_native_file_handle != INVALID_HANDLE_VALUE);
}

bool FileReadStream::is_ready_to_read_from() const
{
    return is_opened() && (get_remaining_byte_count() > 0);
}

FileResult FileReadStream::open(const String& filepath)
{
    close();

    m_native_file_handle = CreateFileA(filepath.characters(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (m_native_file_handle == INVALID_HANDLE_VALUE)
    {
        MAYBE_UNUSED const DWORD file_creation_error_code = GetLastError();
        // TODO: Actually verify that this is the error that occurred.
        return FileResult::FileNotFound;
    }

    LARGE_INTEGER file_size = {};
    if (!GetFileSizeEx(m_native_file_handle, &file_size))
    {
        CloseHandle(m_native_file_handle);
        m_native_file_handle = INVALID_HANDLE_VALUE;
        return FileResult::Unknown;
    }

    m_file_size = file_size.QuadPart;
    m_stream_offset = 0;

    return FileResult::Success;
}

void FileReadStream::close()
{
    if (!is_opened())
        return;

    // Close the native file handle.
    CloseHandle(m_native_file_handle);

    m_native_file_handle = INVALID_HANDLE_VALUE;
    m_file_size = 0;
    m_stream_offset = 0;
}

FileResult FileReadStream::read(void* destination_buffer, usize byte_count)
{
    const FileResult peek_result = peek(destination_buffer, byte_count);
    if (peek_result != FileResult::Success)
        return peek_result;

    m_stream_offset += byte_count;
    return FileResult::Success;
}

FileResult FileReadStream::peek(void* destination_buffer, usize byte_count)
{
    if (!is_opened())
        return FileResult::StreamNotOpened;

    // The stream doesn't have enough data to fill the required number of bytes.
    CAVE_VERIFY(byte_count <= get_remaining_byte_count());

    // TODO: The Win32 API is limited to only read a maximum of 0xFFFFFFFF bytes at once, as the function parameter
    //       in the read file API is a 32-bit unsigned integer. We truncate the number of bytes to read, which can be a
    //       64-bit unsigned integer, to match the Win32 API limitation, however a proper solution must be implemented!
    //       Fix this problem by issuing more calls to the read file API.
    const DWORD number_of_bytes_to_read = static_cast<DWORD>(byte_count);

    OVERLAPPED overlapped = {};
    overlapped.Offset = m_stream_offset & 0xFFFFFFFF;
    overlapped.OffsetHigh = m_stream_offset >> 32;

    DWORD number_of_bytes_read;
    if (!ReadFile(m_native_file_handle, destination_buffer, number_of_bytes_to_read, &number_of_bytes_read, &overlapped))
    {
        // TODO: Investigate why reading from a file can fail (in the context of a game engine)!
        return FileResult::Unknown;
    }

    CAVE_ASSERT(number_of_bytes_read == number_of_bytes_to_read);
    return FileResult::Success;
}

FileResult FileReadStream::read_entire(void* destination_buffer)
{
    const usize remaining_byte_count = get_remaining_byte_count();
    if (remaining_byte_count == 0)
        return FileResult::Success;

    return read(destination_buffer, remaining_byte_count);
}

} // namespace CaveGame

#endif // CAVE_PLATFORM_WINDOWS
