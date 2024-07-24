#include "ctap_api.h"
#include "ctap/core/core_api.h"

static ctp_retcode_e load_map(void)
{
    cor_api.start_the_engines();
    return ctp_retcode_MAP_INVALID;
}

const ctp_api_t ctp_api = {
    .load_map = load_map
};
