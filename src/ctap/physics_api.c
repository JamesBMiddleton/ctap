#include "src/ctap/physics_api.h"
#include "src/ctap/physics/gravity.h"

static void start_gravity(void)
{
    phy_load_gravity_model();
    return;
}

const phy_api_t phy_api = {.start_gravity = start_gravity};
