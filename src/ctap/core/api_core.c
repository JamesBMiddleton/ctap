#include "api_core.h"
#include "map_loader.h"

static void _start_the_engines()
{
    cor_load_the_map();
    return;
}

const cor_t cor = {
    .start_the_engines = _start_the_engines
};
