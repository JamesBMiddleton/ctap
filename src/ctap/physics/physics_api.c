#include "physics_api.h"
#include "gravity.h"

static void start_gravity()
{
    phy_load_gravity_model();
    return;
}

const phy_api_t phy_api = {
    .start_gravity = start_gravity
};
