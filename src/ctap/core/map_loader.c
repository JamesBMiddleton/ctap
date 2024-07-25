#include "src/ctap/core/map_loader.h"

#include "src/ctap/physics_api.h"
#include "src/ctap/input_api.h"
#include "src/ctap/audio_api.h"
#include "src/ctap/graphics_api.h"

#include "utils/types.h"

void cor_load_the_map(void)
{
    inp_api.start_the_input();
    gfx_api.start_the_graphics();
    aud_api.start_the_audio();
    phy_api.start_gravity();
    return;
}
