#include "Vector3.h"

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float a, float b, float c) : x(a), y(b), z(c) {}

float Vector3::LengthSquared() const
{
    return x*x + y*y + z*z;
}

float Vector3::Length() const
{
    return sqrt(LengthSquared());
}

void Vector3::Normalize()
{
    float len = Length();

    // if magnitude is too close to 1, dont take sqrt
    if (len < 0.0001f) return;

    x /= len;
    y /= len;
    z /= len;
}

Vector3 Vector3::Normalized() const
{
    Vector3 result = *this;
    result.Normalize();
    return result;
}

float Vector3::Dot(const Vector3& v2) const
{
    return x*v2.x + y*v2.y + z*v2.z;
}

Vector3 Vector3::Cross(const Vector3& v2) const
{
    return Vector3(
        y*v2.z - z*v2.y,
        z*v2.x - x*v2.z,
        x*v2.y - y*v2.x
    );
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator+=(const Vector3& other)
{
    *this = Vector3(x + other.x, y + other.y, z + other.z);
    return *this;
}
