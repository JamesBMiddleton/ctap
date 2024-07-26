#include "ctap.h"
#include <stdio.h>

i32 main(void)
{
    ctp_logger = printf;
    ctp_load_map();
    return 0;
}
