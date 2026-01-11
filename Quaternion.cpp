#include "Quaternion.h"
#include <cmath>

#define DEG_TO_RAD (3.14159265359f / 180.0f)

Quaternion::Quaternion() : x(0), y(0), z(0), w(1) {}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
    : x(_x), y(_y), z(_z), w(_w) {}

// To avoid drift caused by the quaternion magnitude deviating from 1,
// we must normalize the quaternion.
//
// This drift can be caused by several factors:
// - Floating point precision errors
// - Repeated quaternion multiplications (numerical integration)
// - Trigonometric approximations
void Quaternion::Normalize()
{
    float mag = sqrt(x * x + y * y + z * z + w * w);

    // Safety check to avoid division by zero
    if (mag < 0.00001f)
        return;

    x /= mag;
    y /= mag;
    z /= mag;
    w /= mag;
}

// If we do not normalize, the aircraft (or object) may slowly bend,
// rotate incorrectly, or behave unpredictably over time.
// Quaternions should be normalized especially after quaternion
// multiplications, not necessarily every frame, but very frequently.


// rotate around an axis by angleDegrees
Quaternion Quaternion::AngleAxis(Vector3 axis, float angleDegrees)
{
    // 1-normalize the axis
    axis.Normalize();

    // 2-convert the degree to radiant and divide by two
    float halfRad = angleDegrees * DEG_TO_RAD * 0.5f;

    // 3-calculations
    float s = sin(halfRad);
    float c = cos(halfRad);

    return Quaternion(
        axis.x * s,
        axis.y * s,
        axis.z * s,
        c);
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
    return Quaternion(
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z);
}

// rotate this vector using the rotation represented by this quaternion
Vector3 Quaternion::RotateVector(const Vector3 &v) const
{
    Vector3 qv(x, y, z);

    Vector3 uv = qv.Cross(v);
    Vector3 uuv = qv.Cross(uv);

    uv = uv * (2.0f * w);
    uuv = uuv * 2.0f;

    return v + uv + uuv;
}

// let object look at that direction
// first creates a coordinate system
// then packages that into a quaternion
// because it's a more stabilized storage method
Quaternion Quaternion::LookRotation(const Vector3 &forward)
{
    Vector3 f = forward.Normalized();

    // world up vector -simple version-
    Vector3 up(0, 1, 0);

    // extract the right axis
    Vector3 right = up.Cross(f).Normalized();

    // re-orthogonalize the up vector
    up = f.Cross(right);

    // now we have 3 perpendicular axices
    // right(x), up(y), forward(z)
    // that means a rotation matrix
    float m00 = right.x, m01 = right.y, m02 = right.z;
    float m10 = up.x, m11 = up.y, m12 = up.z;
    float m20 = f.x, m21 = f.y, m22 = f.z;

    float trace = m00 + m11 + m22; // trace represents the rotation of the matrix with minimal numerical error
    // larger trace -> smaller rotation angle
    // trace = 1 + 2 * cos(angle)
    // quaternion w = cos(angle / 2)
    // larger w means smaller rotation angle
    // we use the largest quaternion component as reference for stability
    Quaternion q;

    if (trace > 0.0f)
    {
        float s = sqrt(trace + 1.0f) * 2.0f;
        // comes from s = 4*w
        // and w = 0.5*sqrt(trace+1)
        q.w = 0.25f * s;
        // these come from the antisymmetric parts of the rotation matrix
        // which encode the rotation axis
        // cross differences of the matrix carry the direction of the rotation axis
        // dividing by s keeps the scale correct
        // quaternion x,y,z components are proportional to sin(angle/2)
        // dividing by s converts full-angle information into half-angle form
        q.x = (m21 - m12) / s;
        q.y = (m02 - m20) / s;
        q.z = (m10 - m01) / s;
    }
    else
    {
        q = Quaternion();
    }

    q.Normalize();
    return q;
}

// smoothly interpolate between two orientations
Quaternion Quaternion::Slerp(const Quaternion &a, const Quaternion &b, float t)
{
    // measure the angle between them
    float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

    Quaternion bb = b;

    // choose the shorter path
    if (dot < 0.0f)
    {
        dot = -dot;
        bb.x = -bb.x;
        bb.y = -bb.y;
        bb.z = -bb.z;
        bb.w = -bb.w;
    }

    // if quaternions are very close, linear interpolation is sufficient
    if (dot > 0.9995f)
    {
        Quaternion result(
            a.x + t * (bb.x - a.x),
            a.y + t * (bb.y - a.y),
            a.z + t * (bb.z - a.z),
            a.w + t * (bb.w - a.w));
        result.Normalize();
        return result;
    }

    // true slerp (interpolation along the arc)
    float theta0 = acos(dot);
    float theta = theta0 * t;

    float sinTheta = sin(theta);
    float sinTheta0 = sin(theta0);

    float s0 = cos(theta) - dot * sinTheta / sinTheta0;
    float s1 = sinTheta / sinTheta0;

    return Quaternion(
        a.x * s0 + bb.x * s1,
        a.y * s0 + bb.y * s1,
        a.z * s0 + bb.z * s1,
        a.w * s0 + bb.w * s1);
}