#include <assert.h>
#include <stdio.h>

#include "src/tap_api.h"

int main(void)
{
    TapResult res = tap_api_init((TapApiInitOpt){.placeholder = 42});
    printf("%d", res.retcode);
    return 0;
}
