#include "api_ctap.h"
#include "ctap/core/api_core.h"

static void _load_map()
{
    cor.start_the_engines();
    return;
}

const ctap_t ctap = {
    .load_map = _load_map
};
