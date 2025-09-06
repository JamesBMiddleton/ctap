#include <assert.h>
#include <stdio.h>

#include "src/ctp.h"

int main(void)
{
    ctp_result res = ctp_init((struct ctp_init_args){.placeholder = 42});
    printf("%d", res.retcode);
    return 0;
}
