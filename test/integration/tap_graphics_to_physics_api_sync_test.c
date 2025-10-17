#include <assert.h>
#include <stdio.h>

#include "src/tap_api.h"
#include "util/tap_assert.h"
#include "util/tap_log.h"

int main(void)
{
    TapResult result = {0};
    TapApiInitOpt opts;

    tap_log_set_printf(printf);

    opts.placeholder = 42;
    result = tap_api_init(opts);

    printf("%d", result.retcode);

    TAP_ASSERT(1 == 1);
    return 0;
}
