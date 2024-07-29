#include "ctap.h"
#include <stdio.h>

i32 main(void)
{
    bool i = false;
    ctp_logger = printf;
    ctp_load_map();
    LOG_W("i = %d\n", i);
    return 0;
}
