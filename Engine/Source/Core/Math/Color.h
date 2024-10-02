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

struct Color4
{
public:
    ALWAYS_INLINE Color4()
        : r(0.0F)
        , g(0.0F)
        , b(0.0F)
        , a(0.0F)
    {}

    ALWAYS_INLINE Color4(const Color4& other)
        : r(other.r)
        , g(other.g)
        , b(other.b)
        , a(other.a)
    {}

    ALWAYS_INLINE Color4(float in_r, float in_g, float in_b, float in_a)
        : r(in_r)
        , g(in_g)
        , b(in_b)
        , a(in_a)
    {}

    ALWAYS_INLINE Color4(float in_r, float in_g, float in_b)
        : r(in_r)
        , g(in_g)
        , b(in_b)
        , a(1.0F)
    {}

    ALWAYS_INLINE Color4(float scalar)
        : r(scalar)
        , g(scalar)
        , b(scalar)
        , a(scalar)
    {}

    ALWAYS_INLINE Color4& operator=(const Color4& other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    }

public:
    float r;
    float g;
    float b;
    float a;
};

} // namespace CaveGame
