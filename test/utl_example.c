#include "test.h" 
#include "unistd.h"

i32 main(void)
{
    tst_setup();

    sleep(10);

    ASSERT(1==2);

    return 0;
}
