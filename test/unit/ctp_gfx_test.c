#define CTP_LOG_DEFINE
#define CTP_ERRNO_DEFINE

#include <assert.h>
#include <stdio.h>

#include "util/ctp_log.h" 
#include "util/ctp_errno.h" 
#include "src/ctp_gfx.h"

int main(void)
{
    ctp_log_set_printf(printf);
    assert(ctp_gfx_init((struct ctp_gfx_init_args){.placeholder = 42}).retcode == CTP_RESULT_OK);
    return 0;
}
