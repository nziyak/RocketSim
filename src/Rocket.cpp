#include "../include/Vector3.h"
#include "../include/Quaternion.h"
#include "../include/Rocket.h"

void Rocket::Update(float dt)
{
    // thrust (local -> global)
    Vector3 ThrustVector = orientation.RotateVector(Vector3(0,1,0)) * thrust;

    // gravity
    Vector3 GravityVector = Vector3(0,-1,0) * (mass * 9.81f);

    // aerodynamic drag
    Vector3 AeroDrag(0,0,0);

    float speedSq = velocity.LengthSquared();

    if(speedSq > 0.0001f)
    {
        Vector3 dragDir = velocity.Normalized();
        float dragMag = 0.5f * RHO * Cd * area * speedSq;

        AeroDrag = dragDir * (-dragMag);
    }

    // total force (Newton's second law)
    Vector3 TotalForce = ThrustVector + GravityVector + AeroDrag;

    // acceleration
    Vector3 acceleration = TotalForce * (1.0f / mass);
    lastAcceleration = acceleration; // save the last acceleration

    // update velocity and position with Euler integration
    velocity += acceleration * dt;
    position += velocity * dt;

    //spin stabilization
    float turnSpeed = 360.0f; 
    //incremental rotation for this frame
    Quaternion spin = Quaternion::AngleAxis(Vector3(0,1,0), turnSpeed * dt);
    
    orientation = orientation * spin;
    orientation.Normalize();
}