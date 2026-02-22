#include "sv_math.hpp"
#include <math.h>

#pragma region Vector3

Vector3f::Vector3f()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3f::Vector3f(const float value)
{
    x = value;
    y = value;
    z = value;
}

Vector3f::Vector3f(const float X, const float Y, const float Z)
{
    x = X;
    y = Y;
    z = Z;
}

Vector3f::Vector3f(const Vector3f &vector)
{
    x = vector.x;
    y = vector.y;
    z = vector.z;
}

Vector3f Vector3f::operator+(const Vector3f &vector) const
{
    return Vector3f(x + vector.x, y + vector.y, z + vector.z);
}

Vector3f Vector3f::operator-(const Vector3f &vector) const
{
    return Vector3f(x - vector.x, y - vector.y, z - vector.z);
}

Vector3f Vector3f::operator*(const float &value) const
{
    return Vector3f(x * value, y * value, z * value);
}

Vector3f Vector3f::operator/(const float &value) const
{
    return Vector3f(x / value, y / value, z / value);
}

inline Vector3f Vector3f::operator=(const Vector3f &vector)
{
    return Vector3f(vector);
}

Vector3f Vector3f::operator+=(const Vector3f &vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;

    return *this;
}

Vector3f Vector3f::operator-=(const Vector3f &vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;

    return *this;
}

Vector3f Vector3f::operator*=(const float &value)
{
    x *= value;
    y *= value;
    z *= value;

    return *this;
}

Vector3f Vector3f::operator/=(const float &value)
{
    x /= value;
    y /= value;
    z /= value;

    return *this;
}

Vector3f Vector3f::normalized() const
{
    float ratio = 1 / magnitude();

    return Vector3f(x * ratio, y * ratio, z * ratio);
}

float Vector3f::magnitude() const
{
    return std::abs(std::sqrtf((x * x) + (y * y) + (z * z)));
}

float Vector3f::Distance(const Vector3f &vectorA, const Vector3f &vectorB)
{
    Vector3f diff = Vector3f(vectorB - vectorA);
    return diff.magnitude();
}

Vector3f Vector3f::One()
{
    return Vector3f(1.f, 1.f, 1.f);
}
Vector3f Vector3f::Zero()
{
    return Vector3f(0.f, 0.f, 0.f);
}

Vector3f Vector3f::Front()
{
    return Vector3f(0.f, 0.f, 1.f);
}
Vector3f Vector3f::Back()
{
    return Vector3f(0.f, 0.f, -1.f);
}
Vector3f Vector3f::Up()
{
    return Vector3f(0.f, 1.f, 0.f);
}
Vector3f Vector3f::Down()
{
    return Vector3f(0.f, -1.f, 0.f);
}
Vector3f Vector3f::Right()
{
    return Vector3f(1.f, 0.f, 0.f);
}
Vector3f Vector3f::Left()
{
    return Vector3f(-1.f, 0.f, 0.f);
}

#pragma endregion

#pragma region Vector2

Vector2f::Vector2f()
{
    x = 0;
    y = 0;
}

Vector2f::Vector2f(const float value)
{
    x = value;
    y = value;
}

Vector2f::Vector2f(const float X, const float Y)
{
    x = X;
    y = Y;
}

Vector2f::Vector2f(const Vector2f &vector)
{
    x = vector.x;
    y = vector.y;
}

Vector2f Vector2f::operator+(const Vector2f &vector) const
{
    return Vector2f(x + vector.x, y + vector.y);
}

Vector2f Vector2f::operator-(const Vector2f &vector) const
{
    return Vector2f(x - vector.x, y - vector.y);
}

Vector2f Vector2f::operator*(const float &value) const
{
    return Vector2f(x * value, y * value);
}

Vector2f Vector2f::operator/(const float &value) const
{
    return Vector2f(x / value, y / value);
}

inline Vector2f Vector2f::operator=(const Vector2f &vector)
{
    return Vector2f(vector);
}

Vector2f Vector2f::operator+=(const Vector2f &vector)
{
    x += vector.x;
    y += vector.y;

    return *this;
}

Vector2f Vector2f::operator-=(const Vector2f &vector)
{
    x -= vector.x;
    y -= vector.y;

    return *this;
}

Vector2f Vector2f::operator*=(const float &value)
{
    x *= value;
    y *= value;

    return *this;
}

Vector2f Vector2f::operator/=(const float &value)
{
    x /= value;
    y /= value;

    return *this;
}

Vector2f Vector2f::normalized() const
{
    float ratio = 1 / magnitude();

    return Vector2f(x * ratio, y * ratio);
}

float Vector2f::magnitude() const
{
    return std::abs(std::sqrtf((x * x) + (y * y)));
}

float Vector2f::Distance(const Vector2f &vectorA, const Vector2f &vectorB)
{
    Vector2f diff = Vector2f(vectorB - vectorA);
    return diff.magnitude();
}

Vector2f Vector2f::One()
{
    return Vector2f(1.f, 1.f);
}
Vector2f Vector2f::Zero()
{
    return Vector2f(0.f, 0.f);
}

Vector2f Vector2f::Up()
{
    return Vector2f(0.f, 1.f);
}
Vector2f Vector2f::Down()
{
    return Vector2f(0.f, -1.f);
}
Vector2f Vector2f::Right()
{
    return Vector2f(1.f, 0.f);
}
Vector2f Vector2f::Left()
{
    return Vector2f(-1.f, 0.f);
}

#pragma endregion
