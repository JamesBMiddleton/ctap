#define TAP_LOG_DEFINE
#define TAP_ERRNO_DEFINE

#include <assert.h>
#include <stdio.h>

#include "src/tap_render.h"
#include "util/tap_errno.h"
#include "util/tap_log.h"

int main(void)
{
    TapRenderInitOpt opt;

    tap_log_set_printf(printf);

    opt.placeholder = 42;
    assert(tap_render_init(opt).retcode == TAP_RESULT_OK);
    return 0;
}
