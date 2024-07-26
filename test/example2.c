#include "src/ctap_api.h"
#include "utils/types.h"
#include "utils/log.h"
#include <stdio.h>

i32 (*utl_logger)(const char *__restrict __format, ...) = printf;

i32 main(void)
{
    LOG_E("hello world");
    ctp_api.load_map();
    printf("example_test!\n");
    return 0;
}
