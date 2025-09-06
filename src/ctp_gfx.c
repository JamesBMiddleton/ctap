#include "ctp_gfx.h"
#include "ctp_gfx_reflect.h"

#include "util/ctp_guard.h"
#include "util/ctp_log.h"

struct gfx_engine_starter {
    unsigned int startiness;
    unsigned int num_horses;
};

struct {
    unsigned placeholder;
} static gfx_state = {0};

// placeholder comment
static ctp_result gfx_start_the_engines(struct gfx_engine_starter starter)
{
    CTP_GUARD_IF(starter.num_horses > 1, CTP_ERRNO_NULL);
    return (ctp_result){CTP_RESULT_OK};
}

// another placeholder comment
static ctp_result gfx_spaghettify_value(unsigned* value)
{
    *value = 1010101;
    return (ctp_result){CTP_RESULT_OK};
}

// Initialise the core module.
ctp_result ctp_gfx_init(struct ctp_gfx_init_args args)
{
    gfx_state.placeholder = args.placeholder;
    CTP_GUARD(ctp_reflect_init());
    CTP_GUARD(gfx_spaghettify_value(&gfx_state.placeholder));
    CTP_GUARD(gfx_start_the_engines((struct gfx_engine_starter){.num_horses = args.placeholder}));
    CTP_LOG("Map loaded.");
    return (ctp_result){CTP_RESULT_OK};
}
