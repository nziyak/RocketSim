#pragma once
#include <cmath>

class Vector3
{
public:
    float x, y, z;

    Vector3();
    Vector3(float a, float b, float c);

    float LengthSquared() const;
    float Length() const;

    void Normalize();
    Vector3 Normalized() const;

    float Dot(const Vector3& v2) const;
    Vector3 Cross(const Vector3& v2) const;

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator+=(const Vector3& other);
};
