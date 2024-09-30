/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/Assertion.h>
#include <Core/CoreTypes.h>
#include <Core/Math/MathCore.h>

namespace CaveGame
{

#pragma region Vector2

struct Vector2
{
public:
    NODISCARD ALWAYS_INLINE static float length_squared(Vector2 vector)
    {
        const float result = (vector.x * vector.x) + (vector.y * vector.y);
        return result;
    }

    NODISCARD ALWAYS_INLINE static float length(Vector2 vector)
    {
        const float length_squared = Vector2::length_squared(vector);
        const float result = Math::sqrt(length_squared);
        return result;
    }

    NODISCARD ALWAYS_INLINE static float dot(Vector2 a, Vector2 b)
    {
        const float result = (a.x * b.x) + (a.y * b.y);
        return result;
    }

    NODISCARD ALWAYS_INLINE static Vector2 normalize(Vector2 vector)
    {
        const float length = Vector2::length(vector);
        CAVE_ASSERT(length > Math::small_number);
        const float inv_length = 1.0F / length;
        const Vector2 result = Vector2(vector.x * inv_length, vector.y * inv_length);
        return result;
    }

public:
    ALWAYS_INLINE Vector2()
        : x(0.0F)
        , y(0.0F)
    {}

    ALWAYS_INLINE Vector2(const Vector2& other)
        : x(other.x)
        , y(other.y)
    {}

    ALWAYS_INLINE Vector2(float in_x, float in_y)
        : x(in_x)
        , y(in_y)
    {}

    ALWAYS_INLINE Vector2(float scalar)
        : x(scalar)
        , y(scalar)
    {}

public:
    // Wrapper around `Vector2::length_squared`.
    // See the above function declaration for documentation.
    NODISCARD ALWAYS_INLINE float length_squared() const { return Vector2::length_squared(*this); }

    // Wrapper around `Vector2::length`.
    // See the above function declaration for documentation.
    NODISCARD ALWAYS_INLINE float length() const { return Vector2::length(*this); }

    // Wrapper around `Vector2::normalized`.
    // See the above function declaration for documentation.
    NODISCARD ALWAYS_INLINE Vector2 normalized() const { return Vector2::normalize(*this); }

public:
    NODISCARD ALWAYS_INLINE float* value_ptr() { return &x; }
    NODISCARD ALWAYS_INLINE const float* value_ptr() const { return &x; }

    NODISCARD ALWAYS_INLINE float& operator[](Math::Axis axis)
    {
        const u8 value_index = static_cast<u8>(axis);
        CAVE_ASSERT(value_index < 2);
        return value_ptr()[value_index];
    }

    NODISCARD ALWAYS_INLINE const float& operator[](Math::Axis axis) const
    {
        const u8 value_index = static_cast<u8>(axis);
        CAVE_ASSERT(value_index < 2);
        return value_ptr()[value_index];
    }

public:
    float x;
    float y;
};

// Component-wise addition operator.
NODISCARD ALWAYS_INLINE Vector2 operator+(Vector2 a, Vector2 b)
{
    const Vector2 result = Vector2(a.x + b.x, a.y + b.y);
    return result;
}

NODISCARD ALWAYS_INLINE Vector2& operator+=(Vector2& self, Vector2 other)
{
    self.x += other.x;
    self.y += other.y;
    return self;
}

// Component-wise subtraction operator.
NODISCARD ALWAYS_INLINE Vector2 operator-(Vector2 lhs, Vector2 rhs)
{
    const Vector2 result = Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
    return result;
}

NODISCARD ALWAYS_INLINE Vector2& operator-=(Vector2& self, Vector2 other)
{
    self.x -= other.x;
    self.y -= other.y;
    return self;
}

// Component-wise negation operator.
NODISCARD ALWAYS_INLINE Vector2 operator-(Vector2 vector)
{
    const Vector2 result = Vector2(-vector.x, -vector.y);
    return result;
}

// Component-wise scalar multiplication operator.
NODISCARD ALWAYS_INLINE Vector2 operator*(Vector2 vector, float scalar)
{
    const Vector2 result = Vector2(vector.x * scalar, vector.y * scalar);
    return result;
}

// Component-wise scalar multiplication operator.
NODISCARD ALWAYS_INLINE Vector2 operator*(float scalar, Vector2 vector)
{
    const Vector2 result = Vector2(vector.x * scalar, vector.y * scalar);
    return result;
}

NODISCARD ALWAYS_INLINE Vector2& operator*=(Vector2& self, float scalar)
{
    self.x *= scalar;
    self.y *= scalar;
    return self;
}

// Component-wise scalar division operator.
NODISCARD ALWAYS_INLINE Vector2 operator/(Vector2 vector, float scalar)
{
    const float inv_scalar = 1.0F / scalar;
    const Vector2 result = Vector2(vector.x * inv_scalar, vector.y * inv_scalar);
    return result;
}

NODISCARD ALWAYS_INLINE Vector2& operator/=(Vector2& self, float scalar)
{
    const float inv_scalar = 1.0F / scalar;
    self.x *= inv_scalar;
    self.y *= inv_scalar;
    return self;
}

#pragma endregion

#pragma region Vector3

struct Vector3
{
public:
    NODISCARD ALWAYS_INLINE static float length_squared(Vector3 vector)
    {
        const float result = (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);
        return result;
    }

    NODISCARD ALWAYS_INLINE static float length(Vector3 vector)
    {
        const float length_squared = Vector3::length_squared(vector);
        const float result = Math::sqrt(length_squared);
        return result;
    }

    NODISCARD ALWAYS_INLINE static float dot(Vector3 a, Vector3 b)
    {
        const float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
        return result;
    }

    NODISCARD ALWAYS_INLINE static Vector3 cross(Vector3 lhs, Vector3 rhs)
    {
        // clang-format off
        const Vector3 result = Vector3(
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x
        );
        // clang-format on
        return result;
    }

    NODISCARD ALWAYS_INLINE static Vector3 normalize(Vector3 vector)
    {
        const float length = Vector3::length(vector);
        CAVE_ASSERT(length > Math::small_number);
        const float inv_length = 1.0F / length;

        const Vector3 result = Vector3(vector.x * inv_length, vector.y * inv_length, vector.z * inv_length);
        return result;
    }

public:
    ALWAYS_INLINE Vector3()
        : x(0.0F)
        , y(0.0F)
        , z(0.0F)
    {}

    ALWAYS_INLINE Vector3(const Vector3& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
    {}

    ALWAYS_INLINE Vector3(float in_x, float in_y, float in_z)
        : x(in_x)
        , y(in_y)
        , z(in_z)
    {}

    ALWAYS_INLINE Vector3(float scalar)
        : x(scalar)
        , y(scalar)
        , z(scalar)
    {}

public:
    // Wrapper around `Vector3::length_squared`.
    // See the above function declaration for documentation.
    NODISCARD ALWAYS_INLINE float length_squared() const { return Vector3::length_squared(*this); }

    // Wrapper around `Vector3::length`.
    // See the above function declaration for documentation.
    NODISCARD ALWAYS_INLINE float length() const { return Vector3::length(*this); }

    // Wrapper around `Vector3::normalized`.
    // See the above function declaration for documentation.
    NODISCARD ALWAYS_INLINE Vector3 normalized() const { return Vector3::normalize(*this); }

public:
    NODISCARD ALWAYS_INLINE float* value_ptr() { return &x; }
    NODISCARD ALWAYS_INLINE const float* value_ptr() const { return &x; }

    NODISCARD ALWAYS_INLINE float& operator[](Math::Axis axis)
    {
        const u8 value_index = static_cast<u8>(axis);
        CAVE_ASSERT(value_index < 3);
        return value_ptr()[value_index];
    }

    NODISCARD ALWAYS_INLINE const float& operator[](Math::Axis axis) const
    {
        const u8 value_index = static_cast<u8>(axis);
        CAVE_ASSERT(value_index < 3);
        return value_ptr()[value_index];
    }

public:
    float x;
    float y;
    float z;
};

// Component-wise addition operator.
NODISCARD ALWAYS_INLINE Vector3 operator+(Vector3 a, Vector3 b)
{
    const Vector3 result = Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    return result;
}

NODISCARD ALWAYS_INLINE Vector3& operator+=(Vector3& self, Vector3 other)
{
    self.x += other.x;
    self.y += other.y;
    self.z += other.z;
    return self;
}

// Component-wise subtraction operator.
NODISCARD ALWAYS_INLINE Vector3 operator-(Vector3 lhs, Vector3 rhs)
{
    const Vector3 result = Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    return result;
}

NODISCARD ALWAYS_INLINE Vector3& operator-=(Vector3& self, Vector3 other)
{
    self.x -= other.x;
    self.y -= other.y;
    self.z -= other.z;
    return self;
}

// Component-wise negation operator.
NODISCARD ALWAYS_INLINE Vector3 operator-(Vector3 vector)
{
    const Vector3 result = Vector3(-vector.x, -vector.y, -vector.z);
    return result;
}

// Component-wise scalar multiplication operator.
NODISCARD ALWAYS_INLINE Vector3 operator*(Vector3 vector, float scalar)
{
    const Vector3 result = Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    return result;
}

// Component-wise scalar multiplication operator.
NODISCARD ALWAYS_INLINE Vector3 operator*(float scalar, Vector3 vector)
{
    const Vector3 result = Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    return result;
}

NODISCARD ALWAYS_INLINE Vector3& operator*=(Vector3& self, float scalar)
{
    self.x *= scalar;
    self.y *= scalar;
    self.z *= scalar;
    return self;
}

// Component-wise scalar division operator.
NODISCARD ALWAYS_INLINE Vector3 operator/(Vector3 vector, float scalar)
{
    const float inv_scalar = 1.0F / scalar;
    const Vector3 result = Vector3(vector.x * inv_scalar, vector.y * inv_scalar, vector.z * inv_scalar);
    return result;
}

NODISCARD ALWAYS_INLINE Vector3& operator/=(Vector3& self, float scalar)
{
    const float inv_scalar = 1.0F / scalar;
    self.x *= inv_scalar;
    self.y *= inv_scalar;
    self.z *= inv_scalar;
    return self;
}

#pragma endregion

#pragma region Vector4

struct Vector4
{
public:
    ALWAYS_INLINE Vector4()
        : x(0.0F)
        , y(0.0F)
        , z(0.0F)
        , w(0.0F)
    {}

    ALWAYS_INLINE Vector4(const Vector4& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , w(other.w)
    {}

    ALWAYS_INLINE Vector4(float in_x, float in_y, float in_z, float in_w)
        : x(in_x)
        , y(in_y)
        , z(in_z)
        , w(in_w)
    {}

    ALWAYS_INLINE Vector4(float scalar)
        : x(scalar)
        , y(scalar)
        , z(scalar)
        , w(scalar)
    {}

public:
    NODISCARD ALWAYS_INLINE float* value_ptr() { return &x; }
    NODISCARD ALWAYS_INLINE const float* value_ptr() const { return &x; }

    NODISCARD ALWAYS_INLINE float& operator[](Math::Axis axis)
    {
        const u8 value_index = static_cast<u8>(axis);
        CAVE_ASSERT(value_index < 4);
        return value_ptr()[value_index];
    }

    NODISCARD ALWAYS_INLINE const float& operator[](Math::Axis axis) const
    {
        const u8 value_index = static_cast<u8>(axis);
        CAVE_ASSERT(value_index < 4);
        return value_ptr()[value_index];
    }

public:
    float x;
    float y;
    float z;
    float w;
};

// Component-wise addition operator.
NODISCARD ALWAYS_INLINE Vector4 operator+(Vector4 a, Vector4 b)
{
    const Vector4 result = Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    return result;
}

NODISCARD ALWAYS_INLINE Vector4& operator+=(Vector4& self, Vector4 other)
{
    self.x += other.x;
    self.y += other.y;
    self.z += other.z;
    self.w += other.w;
    return self;
}

// Component-wise subtraction operator.
NODISCARD ALWAYS_INLINE Vector4 operator-(Vector4 lhs, Vector4 rhs)
{
    const Vector4 result = Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    return result;
}

NODISCARD ALWAYS_INLINE Vector4& operator-=(Vector4& self, Vector4 other)
{
    self.x -= other.x;
    self.y -= other.y;
    self.z -= other.z;
    self.w -= other.w;
    return self;
}

// Component-wise negation operator.
NODISCARD ALWAYS_INLINE Vector4 operator-(Vector4 vector)
{
    const Vector4 result = Vector4(-vector.x, -vector.y, -vector.z, -vector.w);
    return result;
}

// Component-wise scalar multiplication operator.
NODISCARD ALWAYS_INLINE Vector4 operator*(Vector4 vector, float scalar)
{
    const Vector4 result = Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
    return result;
}

// Component-wise scalar multiplication operator.
NODISCARD ALWAYS_INLINE Vector4 operator*(float scalar, Vector4 vector)
{
    const Vector4 result = Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
    return result;
}

NODISCARD ALWAYS_INLINE Vector4& operator*=(Vector4& self, float scalar)
{
    self.x *= scalar;
    self.y *= scalar;
    self.z *= scalar;
    self.w *= scalar;
    return self;
}

// Component-wise scalar division operator.
NODISCARD ALWAYS_INLINE Vector4 operator/(Vector4 vector, float scalar)
{
    const float inv_scalar = 1.0F / scalar;
    const Vector4 result = Vector4(vector.x * inv_scalar, vector.y * inv_scalar, vector.z * inv_scalar, vector.w * inv_scalar);
    return result;
}

NODISCARD ALWAYS_INLINE Vector4& operator/=(Vector4& self, float scalar)
{
    const float inv_scalar = 1.0F / scalar;
    self.x *= inv_scalar;
    self.y *= inv_scalar;
    self.z *= inv_scalar;
    self.w *= inv_scalar;
    return self;
}

#pragma endregion

} // namespace CaveGame
