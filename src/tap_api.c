#define TAP_LOG_DEFINE
#define TAP_ERRNO_DEFINE

#include "tap_api.h"
#include "tap_render.h"

#include "util/tap_errno.h"
#include "util/tap_guard.h"
#include "util/tap_log.h"


// Initialise the ctap runtime.
TapResult tap_api_init(TapApiInitOpt opt)
{
    TapResult result = {TAP_RESULT_OK};
    TapRenderInitOpt render_opts = {0};

    tap_errno_get();
    TAP_LOG("hello world");

    render_opts.placeholder = opt.placeholder;
    TAP_GUARD(tap_render_init(render_opts));

    return result;
}
