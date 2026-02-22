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

    inline Vector3f operator+(const Vector3f& vector) const;
    inline Vector3f operator-(const Vector3f& vector) const;
    inline Vector3f operator*(const float& value) const;
    inline Vector3f operator/(const float& value) const;

    inline Vector3f operator=(const Vector3f& vector);
    inline Vector3f operator+=(const Vector3f& vector);
    inline Vector3f operator-=(const Vector3f& vector);
    inline Vector3f operator*=(const float& value);
    inline Vector3f operator/=(const float& value);

    inline Vector3f normalized() const;
    inline float magnitude() const;


    inline static float Distance(const Vector3f& vectorA, const Vector3f& vectorB);

    inline static Vector3f One();
    inline static Vector3f Zero();

    inline static Vector3f Front();
    inline static Vector3f Back();
    inline static Vector3f Up();
    inline static Vector3f Down();
    inline static Vector3f Right();
    inline static Vector3f Left();


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

    inline Vector2f operator+(const Vector2f& vector) const;
    inline Vector2f operator-(const Vector2f& vector) const;
    inline Vector2f operator*(const float& value) const;
    inline Vector2f operator/(const float& value) const;

    inline Vector2f operator=(const Vector2f& vector);
    inline Vector2f operator+=(const Vector2f& vector);
    inline Vector2f operator-=(const Vector2f& vector);
    inline Vector2f operator*=(const float& value);
    inline Vector2f operator/=(const float& value);

    inline Vector2f normalized() const;
    inline float magnitude() const;


    inline static float Distance(const Vector2f& vectorA, const Vector2f& vectorB);

    inline static Vector2f One();
    inline static Vector2f Zero();

    inline static Vector2f Up();
    inline static Vector2f Down();
    inline static Vector2f Right();
    inline static Vector2f Left();


    float x;
    float y;
};

#endif //_SV_MATH_HPP