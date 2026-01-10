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

    Vector3 Cross(const Vector3 &v2) const
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

    //let object look at that direction
    //first creates a coordinate system
    //then packages that into a quaternion 
    //because it's a more stabilized storage method
    static Quaternion LookRotation(const Vector3& forward)
    {
        Vector3 f = forward.Normalized();

        //dünya yukarısı -simple version-
        Vector3 up(0,1,0);

        //sağ ekseni çıkar
        Vector3 right = up.Cross(f).Normalized();

        //up'ı tekrar ortogonal yap
        up = f.Cross(right);

        //now we have 3 perpendicular axices
        //right(x), up(y), forward(z)
        //that means a rotation matrix
        float m00 = right.x, m01 = right.y, m02 = right.z;
        float m10 = up.x, m11 = up.y, m12 = up.z;
        float m20 = f.x, m21 = f.y, m22 = f.z;

        float trace = m00 + m11 + m22;//representation of the rotation of that matrix with least error 
        //trace greater -> rotation angle is less
        //trace = 1 + 2cos(angle)
        //also w of the quaternion is w = (angle/2)
        //w is greater if angle is less
        //we take the component of the quaternion that is big as reference
        Quaternion q;

        if(trace > 0.0f)
        {
            float s = sqrt(trace + 1.0f) * 2.0f;
            //comes from s = 4*w
            //and w = 0.5*sqrt(trace+1)
            q.w = 0.25f * s;
            //and these are coming from
            //antisymmetric parts of the matrix
            //means "which axis it rotated around?"
            //cross differences in the matrix carries the direction of the rotation axis
            //and we normalize with s to not break the scale
            //we divide to sin because x,y,z axices of the quaternion are proportional with s
            //we divide by s to convert the full angle info to half angle info that quaternion needs
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

    //iki duruş arasında düzgün geç
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t)
    {
        //measure the angle between them
        float dot = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;

        Quaternion bb = b;

        //choose the shorter path
        if (dot < 0.0f)
        {
            dot = -dot;
            bb.x = -bb.x;
            bb.y = -bb.y;
            bb.z = -bb.z;
            bb.w = -bb.w;
        }

        //if distance is so close Lerp is enough
         if (dot > 0.9995f)
        {
            Quaternion result(
                a.x + t*(bb.x - a.x),
                a.y + t*(bb.y - a.y),
                a.z + t*(bb.z - a.z),
                a.w + t*(bb.w - a.w)
            );
            result.Normalize();
            return result;
        }

        //real slerp(yay üzerinde)
        float theta0 = acos(dot);
        float theta  = theta0 * t;

        float sinTheta  = sin(theta);
        float sinTheta0 = sin(theta0);

        float s0 = cos(theta) - dot * sinTheta / sinTheta0;
        float s1 = sinTheta / sinTheta0;

        return Quaternion(
            a.x*s0 + bb.x*s1,
            a.y*s0 + bb.y*s1,
            a.z*s0 + bb.z*s1,
            a.w*s0 + bb.w*s1
        );
    }
};