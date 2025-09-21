#include "tap_render.h"
#include "tap_render_reflect.h"

#include "util/tap_guard.h"
#include "util/tap_log.h"

typedef struct {
    unsigned int startiness;
    unsigned int num_horses;
} RenderEngineStarter;

struct {
    unsigned int placeholder;
} static render_state = {0};

/* placeholder comment */
static TapResult render_start_the_engines(RenderEngineStarter starter)
{
    TapResult result = {TAP_RESULT_OK};
    if (starter.num_horses < 1)
        TAP_GUARD_BAIL(TAP_ERRNO_NULL);
    return result; 
}

/* another placeholder comment */
static TapResult render_spaghettify_value(unsigned int *value)
{
    TapResult result = {TAP_RESULT_OK};
    *value = 1010101;
    return result;
}

/* Initialise the core module. */
TapResult tap_render_init(TapRenderInitOpt opt)
{
    TapResult result = {TAP_RESULT_OK};
    RenderEngineStarter starter;

    render_state.placeholder = opt.placeholder;
    starter.num_horses = opt.placeholder;

    TAP_GUARD(tap_reflect_init());
    TAP_GUARD(render_spaghettify_value(&render_state.placeholder));
    TAP_GUARD(render_start_the_engines(starter));
    TAP_LOG("Map loaded.");
    return result; 
}
