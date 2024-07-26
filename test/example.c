#include "src/ctap_api.h"
#include "utils/types.h"
#include "utils/log.h"
#include <stdio.h>

typedef enum { BLUE } color_e; 
typedef enum { ERROR } result_e;

i32 main(void)
{
    result_e result = BLUE;

    utl_logger = printf;
    utl_logger("hello world!");
    ctp_api.load_map();
    printf("example_test!\n");
    return 0;
}
