#define TAP_LOG_DEFINE
#define TAP_ERRNO_DEFINE

#include "tap.h"
#include "tap_render.h"

#include "util/tap_errno.h"
#include "util/tap_guard.h"
#include "util/tap_log.h"


// Initialise the ctap runtime.
TapResult tap_init(TapInitOpt opt)
{
    tap_errno_get();
    TAP_LOG("hello world");
    TAP_GUARD(tap_render_init((TapRenderInitOpt) { .placeholder = 1 }));
    return (TapResult){TAP_RESULT_OK};
}
