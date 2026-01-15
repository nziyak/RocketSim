#pragma once

#include "Vector3.h"
#include "Quaternion.h"

#define RHO 1.225

class Rocket
{
    public:

        Rocket(float x, float y, float z)
        {
            position.x = x;
            position.y = y;
            position.z = z;
        }

        // state variables
        Vector3 position; //global position (m)
        Vector3 velocity; //global velocity (m/s)
        Quaternion orientation;

        // physical properties
        float mass = 50.0f; //kg
        float area = 0.1f; //m^2 (kesit alanı)
        float Cd = 0.5f; //drag coefficient
        float thrust = 2000.0f; //Newton (engine thrust)

        void Update(float dt);
};
