#include <cmath>

#define PI 3.14159265359f

const float DEG_TO_RAD = 3.14159265359f / 180.0f;

class Vector3
{
    public: 

        float x,y,z;

        Vector3() : x(0), y(0), z(0) {}

        Vector3(float a, float b, float c)
        {
            x = a;
            y = b;
            z = c;
        }

        void Normalize()
        {
            float sqrMag = x*x + y*y + z*z;

            //if magnitude is too close to 1, dont take sqrt
            if(abs(sqrMag - 1.0f) < 0.0001f) return;

            //if magnitude is too small, do not divide
            if(sqrMag < 0.00001f) return;
            
            //if not take sqrt and divide every part
            float length = sqrt(sqrMag);
            x /= length;
            y /= length;
            z /= length;
        }

        Vector3 Normalized() const 
        {
            Vector3 result = *this;
            result.Normalize();
            return result;
        }
        
        float DotProduct(const Vector3& v2)
        {
            float product;
            
            product = (x * v2.x) + (y * v2.y) + (z * v2.z);
            
            return product;
        }

        Vector3 CrossProduct(const Vector3& v2)
        {
            Vector3 product;

            product = Vector3(
                ((y * v2.z) - (z * v2.y)), 
                ((z * v2.x) - (x * v2.z)), 
                ((x * v2.y) - (y * v2.x))
            );

            return product;
        }

        Vector3 operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(double scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
};

class Quaternion
{
    public:

        float x,y,z,w;

        static Quaternion AngleAxis(Vector3 axis, float angleDegrees)
        {
            //1-normalize of the axis
            axis.Normalize();

            //2-convert the degree to radiant and divide by two
            float halfAngleRad = (angleDegrees * DEG_TO_RAD) * 0.5f;

            //3-calculations
            float sinVal = sin(halfAngleRad);
            float cosVal = cos(halfAngleRad);
            
            Quaternion result;

            result.x = axis.x * sinVal;
            result.y = axis.y * sinVal;
            result.z = axis.z * sinVal;
            result.w = cosVal;

            return result;
        }

        static Vector3 RotateVector(Vector3 vector, float angleDegrees)
        {
            Vector3 result; 
            Vector3 u(x,y,z);

            result = vector + q.axis.CrossProduct(q.axis.CrossProduct(vector) + vector * q.w) * 2.0;

            return result;
        }
};