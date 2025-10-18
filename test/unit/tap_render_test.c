#define TAP_LOG_IMPLEMENTATION
#define TAP_ALLOC_IMPLEMENTATION
#define TAP_ERRNO_IMPLEMENTATION

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/tap_render.h"
#include "util/tap_alloc.h"
#include "util/tap_errno.h"
#include "util/tap_log.h"

int main(void)
{
    {
        TapLogLogger logger = {0};
        TapAllocAllocator allocator = {0};
        allocator.malloc = malloc;
        allocator.free = free;
        tap_alloc_set_allocator(allocator);
        logger.printf = printf;
        tap_log_set_logger(logger);
    }

    TapRenderInitOpt opt = {0};
    opt.placeholder = 42;
    assert(tap_render_init(opt).retcode == TAP_RESULT_OK);
    return 0;
}
