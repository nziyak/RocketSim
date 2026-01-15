#include "Vector3.h"
#include "Quaternion.h"
#include "Rocket.h"

int main()
{
    Rocket* rocket = new Rocket(0,0,0);

    rocket->Update(0.01);
    
    return 0;
}