#include "src/ctap_api.h"
#include "utils/types.h"
#include <stdio.h>

i32 main(void)
{
    ctp_api.load_map();
    printf("example_test!\n");
    return 0;
}
