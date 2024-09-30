/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/Math/MathCore.h>
#include <cmath>

namespace CaveGame
{

float Math::sqrt(float value)
{
    return std::sqrtf(value);
}

float Math::sin(float value)
{
    return std::sinf(value);
}

float Math::cos(float value)
{
    return std::cosf(value);
}

float Math::tan(float value)
{
    return std::tan(value);
}

void Math::sin_and_cos(float value, float& out_sin, float& out_cos)
{
    // NOTE: Computing both the sine and the cosine of the angle in a single call might provide
    // better performance, as the compiler is able to recognize this pattern and *maybe* optimize it.

    out_sin = std::sinf(value);
    out_cos = std::cosf(value);
}

float Math::asin(float value)
{
    return std::asinf(value);
}

float Math::acos(float value)
{
    return std::acosf(value);
}

float Math::atan(float value)
{
    return std::atanf(value);
}

} // namespace CaveGame
