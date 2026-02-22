#ifndef _SV_MATH_HPP
#define _SV_MATH_HPP 1

struct Vector3f
{
    Vector3f();
    Vector3f(const float value);
    Vector3f(const float x, const float y, const float z);
    Vector3f(const Vector3f& vector);
    Vector3f(Vector3f&&) = default;
    ~Vector3f() = default;

    Vector3f operator+(const Vector3f& vector) const;
    Vector3f operator-(const Vector3f& vector) const;
    Vector3f operator*(const float& value) const;
    Vector3f operator/(const float& value) const;

    Vector3f operator=(const Vector3f& vector);
    Vector3f operator+=(const Vector3f& vector);
    Vector3f operator-=(const Vector3f& vector);
    Vector3f operator*=(const float& value);
    Vector3f operator/=(const float& value);

    Vector3f normalized() const;
    float magnitude() const;


    static float Distance(const Vector3f& vectorA, const Vector3f& vectorB);

    static Vector3f One();
    static Vector3f Zero();

    static Vector3f Front();
    static Vector3f Back();
    static Vector3f Up();
    static Vector3f Down();
    static Vector3f Right();
    static Vector3f Left();


    float x;
    float y;
    float z;
};

struct Vector2f
{
    Vector2f();
    Vector2f(const float value);
    Vector2f(const float x, const float y);
    Vector2f(const Vector2f& vector);
    Vector2f(Vector2f&&) = default;
    ~Vector2f() = default;

    Vector2f operator+(const Vector2f& vector) const;
    Vector2f operator-(const Vector2f& vector) const;
    Vector2f operator*(const float& value) const;
    Vector2f operator/(const float& value) const;

    Vector2f operator=(const Vector2f& vector);
    Vector2f operator+=(const Vector2f& vector);
    Vector2f operator-=(const Vector2f& vector);
    Vector2f operator*=(const float& value);
    Vector2f operator/=(const float& value);

    Vector2f normalized() const;
    float magnitude() const;


    static float Distance(const Vector2f& vectorA, const Vector2f& vectorB);

    static Vector2f One();
    static Vector2f Zero();

    static Vector2f Up();
    static Vector2f Down();
    static Vector2f Right();
    static Vector2f Left();


    float x;
    float y;
};

#endif //_SV_MATH_HPP