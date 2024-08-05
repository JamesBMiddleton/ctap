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

    char arr[100];
    utl_u32tostr(4294967295/2, arr);
    char arr2[100];
    utl_i32tostr(2147483647, arr2);
    printf("%s", arr2);

    return 0;
}
