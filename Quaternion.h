class Quaternion
{
public:
    float x, y, z, w;

    // ----------------------------
    // 1) Constructor'lar
    // ----------------------------

    // Identity rotation (hiç dönmemiş)
    Quaternion() : x(0), y(0), z(0), w(1) {}

    Quaternion(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {}

    // ----------------------------
    // 2) Normalize (çok kritik)
    // ----------------------------

    void Normalize()
    {
        float mag = sqrt(x*x + y*y + z*z + w*w);
        if (mag < 0.00001f) return;

        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;
    }

    // =========================================================
    // SENARYO C — AngleAxis
    // "Bir eksen etrafında şu kadar dön"
    // =========================================================
    //
    // Bu bir DURUŞ değil
    // Bu bir DÖNÜŞ KOMUTU
    //
    // O yüzden genelde:
    // orientation = orientation * deltaRotation;
    //
    static Quaternion AngleAxis(float angleDegrees, Vector3 axis)
    {
        // 1) Eksen yön olduğu için normalize
        axis.Normalize();

        // 2) Quaternion yarım açı kullanır
        float halfRad = angleDegrees * DEG_TO_RAD * 0.5f;

        float s = sin(halfRad);
        float c = cos(halfRad);

        // 3) Dönüşü paketle
        return Quaternion(
            axis.x * s,
            axis.y * s,
            axis.z * s,
            c
        );
    }

    // =========================================================
    // Quaternion Çarpımı
    // "Dönüş üzerine dönüş"
    // =========================================================
    Quaternion operator*(const Quaternion& q) const
    {
        return Quaternion(
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w,
            w*q.w - x*q.x - y*q.y - z*q.z
        );
    }

    // =========================================================
    // Vektör Döndürme
    // "Bu duruşla bu vektörü global'e taşı"
    // =========================================================
    Vector3 RotateVector(const Vector3& v) const
    {
        Vector3 qv(x, y, z);

        Vector3 uv  = qv.Cross(v);
        Vector3 uuv = qv.Cross(uv);

        uv  = uv  * (2.0f * w);
        uuv = uuv * 2.0f;

        return v + uv + uuv;
    }

    // =========================================================
    // SENARYO A — LookRotation
    // "Şu yöne bakan bir DURUŞ üret"
    // =========================================================
    //
    // Burada:
    // forward = bakılan yön
    // up = referans (roll'ü sabitler)
    //
    static Quaternion LookRotation(const Vector3& forward)
    {
        // 1) Bakış yönü
        Vector3 f = forward.Normalized();

        // 2) Dünya yukarısı (basit versiyon)
        Vector3 up(0, 1, 0);

        // 3) Sağ ekseni çıkar
        Vector3 right = up.Cross(f).Normalized();

        // 4) Up'ı tekrar ortogonal yap
        up = f.Cross(right);

        // Artık elimizde 3 dik eksen var:
        // right, up, forward
        // Bu bir ROTATION MATRIX demek

        float m00 = right.x, m01 = right.y, m02 = right.z;
        float m10 = up.x,    m11 = up.y,    m12 = up.z;
        float m20 = f.x,     m21 = f.y,     m22 = f.z;

        float trace = m00 + m11 + m22;
        Quaternion q;

        if (trace > 0.0f)
        {
            float s = sqrt(trace + 1.0f) * 2.0f;
            q.w = 0.25f * s;
            q.x = (m21 - m12) / s;
            q.y = (m02 - m20) / s;
            q.z = (m10 - m01) / s;
        }
        else
        {
            // Basit fallback (nadir durum)
            q = Quaternion();
        }

        q.Normalize();
        return q;
    }

    // =========================================================
    // SENARYO B — Slerp
    // "İki duruş arasında düzgün geç"
    // =========================================================
    static Quaternion Slerp(const Quaternion& a,
                            const Quaternion& b,
                            float t)
    {
        // 1) Aralarındaki açıyı ölç
        float dot =
            a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;

        Quaternion bb = b;

        // 2) Uzun yolu değil kısa yolu seç
        if (dot < 0.0f)
        {
            dot = -dot;
            bb.x = -bb.x;
            bb.y = -bb.y;
            bb.z = -bb.z;
            bb.w = -bb.w;
        }

        // 3) Çok yakınsa Lerp yeter
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

        // 4) Gerçek slerp (yay üzerinde)
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
