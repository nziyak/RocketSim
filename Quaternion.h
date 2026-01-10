#pragma once
#include "Vector3.h"

class Quaternion
{
public:
    float x, y, z, w;

    Quaternion();
    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    static Quaternion AngleAxis(Vector3 axis, float angleDegrees);
    static Quaternion LookRotation(const Vector3& forward);
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

    Quaternion operator*(const Quaternion& q) const;
    Vector3 RotateVector(const Vector3& v) const;
};
