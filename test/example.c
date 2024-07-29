#include "ctap.h"
#include <stdio.h>
#include <stdlib.h>

i32 main(void)
{
    bool i = false;
    ctp_logger_cb = printf;
    ctp_abort_cb = abort;
    ctp_load_map();
    LOG_W("i = %d\n", i);
    return 0;
}
