#include "api_physics.h"
#include "gravity.h"

void _start_gravity()
{
    phy_load_gravity_model();
    return;
}

const phy_t phy = {
    .start_gravity = _start_gravity
};
