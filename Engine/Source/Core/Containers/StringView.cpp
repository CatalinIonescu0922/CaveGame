/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Assertion.h>
#include <Core/Containers/StringView.h>

namespace CaveGame
{

StringView StringView::create_from_utf8(const char* characters, usize byte_count)
{
    StringView view;
    view.m_characters = characters;
    view.m_byte_count = byte_count;
    return view;
}

StringView StringView::create_from_utf8(ReadonlyByteSpan character_byte_span)
{
    return create_from_utf8(reinterpret_cast<const char*>(character_byte_span.elements()), character_byte_span.count());
}

StringView StringView::create_from_utf8(const char* null_terminated_characters)
{
    CAVE_ASSERT(null_terminated_characters != nullptr);
    const char* characters = null_terminated_characters;
    while (*characters)
        ++characters;

    StringView view;
    view.m_characters = null_terminated_characters;
    view.m_byte_count = (characters - null_terminated_characters);
    return view;
}

} // namespace CaveGame
