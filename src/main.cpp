#include <fstream>
#include <iostream>

#include "Vector3.cpp"
#include "Quaternion.cpp"
#include "Rocket.cpp"

using namespace std;

int main()
{
    Rocket rocket;
    rocket.position = Vector3(0,0,0);

    float dt = 0.01f;
    float flight_time = 0.0f; //to measure the total flight time
    bool running = true;

    ofstream log("flight_data.csv");
    log << "time,altitude,velocity,acceleration\n";

    //to store the last acceleration
    Vector3 acceleration; 
    
    while(running)
    {
        flight_time += dt;

        //burnout
        if(flight_time > 10 && rocket.thrust > 0)
            rocket.thrust = 0;

        //update
        rocket.Update(dt);

        acceleration = rocket.lastAcceleration;

        log << flight_time << ","
            << rocket.position.y << ","
            << rocket.velocity.y << ","
            << acceleration.y << "\n";

        if(rocket.position.y < 0.0f)
        {
            running = false;
            break;
        }
    }
    
    return 0;
}