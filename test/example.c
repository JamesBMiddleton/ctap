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

    utl_powf(1,1);
    utl_powi(1,1);
    utl_powu(1,1);

    char arr[100];
    utl_u32tostr(4294967295/2, arr);
    char arr2[100];
    utl_i32tostr(2147483647, arr2);
    char arr3[100];
    // utl_f32tostr(-1234.1000f, arr3, 1);
    // utl_f32tostr(-1234.1000f, arr3, 4);
    utl_f32tostr(-0.0/0.0, arr3, 4);
    // utl_f32tostr(-1.0/0.0, arr3, 4);
    printf("%s", arr3);

    return 0;
}
