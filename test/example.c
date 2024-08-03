#include "ctap.h"
#include <stdio.h>
#include <stdlib.h>

static void logger_cb(ctp_log_t log)
{
    printf("%s", log.message);
}


i32 main(void)
{
    ctp_log_cb = logger_cb;
    ctp_panic_cb = abort;
    ctp_load_map();

    return 0;
}
