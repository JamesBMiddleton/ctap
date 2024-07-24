#include "core_api.h"
#include "map_loader.h"

static void start_the_engines(void)
{
    cor_load_the_map();
    cor_load_the_map();
    return;
}

const cor_api_t cor_api = {
    .start_the_engines = start_the_engines
};
