#include <cmath>

#define PI 3.14159265359f

const float DEG_TO_RAD = 3.14159265359f / 180.0f;

class Vector3
{
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(float a, float b, float c)
    {
        x = a;
        y = b;
        z = c;
    }

    float LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    float Length() const
    {
        return sqrt(LengthSquared());
    }

    void Normalize()
    {
        float len = Length();

        // if magnitude is too close to 1, dont take sqrt
        if (len < 0.0001f)
            return;

        x /= len;
        y /= len;
        z /= len;
    }

    Vector3 Normalized() const
    {
        Vector3 result = *this;
        result.Normalize();
        return result;
    }

    float Dot(const Vector3 &v2) const
    {
        return (x * v2.x) + (y * v2.y) + (z * v2.z);
    }

    Vector3 CrossProduct(const Vector3 &v2) const
    {
        return Vector3(
            ((y * v2.z) - (z * v2.y)),
            ((z * v2.x) - (x * v2.z)),
            ((x * v2.y) - (y * v2.x)));
    }

    Vector3 operator+(const Vector3 &other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3 &other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
};

class Quaternion
{
public:
    float x, y, z, w;

    Quaternion() : x(0), y(0), z(0), w(1) {}

    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    void Normalize()
    {
        float mag = sqrt(x * x + y * y + z * z + w * w);

        if (mag < 0.00001f)
            return;

        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;
    }

    //bir eksen etrafında angleDegrees kadar dön
    static Quaternion AngleAxis(Vector3 axis, float angleDegrees)
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

    Quaternion operator*(const Quaternion &q) const
    {
        return Quaternion(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }
    
    //bu quaternionun temsil ettiği dönüşle bu vektörü döndür
    Vector3 RotateVector(const Vector3& v) const
    {
        Vector3 qv(x,y,z);

        Vector3 uv = qv.Cross(v);
        Vector3 uuv = qv.Cross(uv);

        uv = uv * (2.0f * w);
        uuv = uuv * 2.0f;

        return v + uv + uuv;
    }
};