#define TAP_LOG_DEFINE
#define TAP_ERRNO_DEFINE

#include <assert.h>
#include <stdio.h>

#include "util/tap_log.h" 
#include "util/tap_errno.h" 
#include "src/tap_render.h"

int main(void)
{
    tap_log_set_printf(printf);
    assert(tap_render_init((TapRenderInitOpt){.placeholder = 42}).retcode == TAP_RESULT_OK);
    return 0;
}
