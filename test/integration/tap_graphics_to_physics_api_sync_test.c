#include <assert.h>
#include <stdio.h>

#include "src/tap.h"

int main(void)
{
    TapResult res = tap_init((TapInitOpt){.placeholder = 42});
    printf("%d", res.retcode);
    return 0;
}
