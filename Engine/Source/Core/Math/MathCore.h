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

class Math
{
public:
    //
    // Constants.
    //

    static constexpr float pi = 3.14159265358979323F;
    static constexpr float inv_pi = 1.0F / pi;
    static constexpr float two_pi = 2.0F * pi;
    static constexpr float half_pi = 0.5F * pi;

    static constexpr float rad_to_deg = 180.0F / pi;
    static constexpr float deg_to_rad = pi / 180.0F;

    static constexpr float small_number = 1e-6F;
    static constexpr float kinda_small_number = 1e-3F;
    static constexpr float large_number = 1e15F;
    static constexpr float kinda_large_number = 1e10F;

public:
    //
    // Generic math structures & types.
    //

    enum class Axis : u8
    {
        X = 0,
        Y = 1,
        Z = 2,
        W = 3,
    };

public:
    //
    // Templated utility functions.
    //

    template<typename T>
    NODISCARD ALWAYS_INLINE static T min(T a, T b)
    {
        return (a < b) ? a : b;
    }

    template<typename T>
    NODISCARD ALWAYS_INLINE static T max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    template<typename T>
    NODISCARD ALWAYS_INLINE static T clamp(T value, T range_min, T range_max)
    {
        return Math::min(Math::max(value, range_min), range_max);
    }

    template<typename T>
    NODISCARD ALWAYS_INLINE static T abs(T value)
    {
        return (value < T(0)) ? -value : value;
    }

public:
    //
    // Real-numbers elementary functions.
    //

    NODISCARD static float sqrt(float value);

    NODISCARD static float sin(float value);
    NODISCARD static float cos(float value);
    NODISCARD static float tan(float value);
    NODISCARD static void sin_and_cos(float value, float& out_sin, float& out_cos);

    NODISCARD static float asin(float value);
    NODISCARD static float acos(float value);
    NODISCARD static float atan(float value);
};

} // namespace CaveGame
