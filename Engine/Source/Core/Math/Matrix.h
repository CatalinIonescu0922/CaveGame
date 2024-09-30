/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Math/MathCore.h>
#include <Core/Math/Vector.h>

namespace CaveGame
{

#pragma region Matrix3

struct Matrix3
{
public:
    NODISCARD ALWAYS_INLINE static Matrix3 identity()
    {
        // clang-format off
        const Matrix3 result = Matrix3(
            Vector3(1, 0, 0),
            Vector3(0, 1, 0),
            Vector3(0, 0, 1)
        );
        // clang-format on
        return result;
    }

public:
    ALWAYS_INLINE Matrix3()
        : rows { Vector3(0), Vector3(0), Vector3(0) }
    {}

    ALWAYS_INLINE Matrix3(const Matrix3& other)
        : rows { other.rows[0], other.rows[1], other.rows[2] }
    {}

    ALWAYS_INLINE Matrix3(Vector3 row0, Vector3 row1, Vector3 row2)
        : rows { row0, row1, row2 }
    {}

public:
    union
    {
        Vector3 rows[3];
        float m[3][3];
        float v[9];
    };
};

#pragma endregion

#pragma region Matrix4

struct Matrix4
{
public:
    NODISCARD ALWAYS_INLINE static Matrix4 identity()
    {
        // clang-format off
        const Matrix4 result = Matrix4(
            Vector4(1, 0, 0, 0),
            Vector4(0, 1, 0, 0),
            Vector4(0, 0, 1, 0),
            Vector4(0, 0, 0, 1)
        );
        // clang-format on
        return result;
    }

public:
    ALWAYS_INLINE Matrix4()
        : rows { Vector4(0), Vector4(0), Vector4(0), Vector4(0) }
    {}

    ALWAYS_INLINE Matrix4(const Matrix4& other)
        : rows { other.rows[0], other.rows[1], other.rows[2], other.rows[3] }
    {}

    ALWAYS_INLINE Matrix4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3)
        : rows { row0, row1, row2, row3 }
    {}

public:
    union
    {
        Vector4 rows[4];
        float m[4][4];
        float v[16];
    };
};

#pragma endregion

} // namespace CaveGame
