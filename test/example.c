#include "ctap.h"
#include <stdio.h>
#include <stdlib.h>

static void logger_cb(ctp_log_t log)
{
    printf("%s", log.message);
}


i32 main(void)
{
    // bool i = false;
    ctp_log_cb = logger_cb;
    ctp_panic_cb = abort;
    ctp_load_map();
    // LOG_W("i = %d\n", i);
    return 0;
}
