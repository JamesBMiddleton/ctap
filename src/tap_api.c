#define TAP_LOG_IMPLEMENTATION
#define TAP_ERRNO_IMPLEMENTATION
#define TAP_ALLOC_IMPLEMENTATION

#include "tap_api.h"
#include "tap_render.h"

#include "util/tap_errno.h"
#include "util/tap_guard.h"
#include "util/tap_log.h"


/* Initialise the ctap runtime. */
TapResult tap_api_init(TapApiInitOpt opt)
{
    TapResult result = {TAP_RESULT_OK};

    if (opt.allocator.free == NULL || opt.allocator.malloc == NULL)
        TAP_GUARD_BAIL(TAP_ERRNO_INVALID_ALLOCATOR);
    tap_alloc_set_allocator(opt.allocator);


    tap_errno_get();
    TAP_LOG("hello world");
    TAP_LOG1("hello world: ", 42);

    {
        TapRenderInitOpt render_opts = {0};
        render_opts.placeholder = opt.placeholder;
        TAP_GUARD(tap_render_init(render_opts));
    }

    return result;
}
