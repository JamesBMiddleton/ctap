#ifndef PHYS_C
#define PHYS_C
PHYS_C

#include "src/util.c"

struct StatePhys {
    uint placeholder;
    // pool_t
} static gStatePhys = {0};

typedef struct {
    uint placeholder;
} phys_InitA;
typedef enum { phys_InitR_OK, phys_InitR_MAP_INVALID } phys_InitR;

/*
 * Initialise the audio module. 
 *
 * @param args - initialisation arguments.
*/
static phys_InitR phys_Init(phys_InitA args)
{
    gStatePhys.placeholder = 1;
    return (args.placeholder) ? phys_InitR_OK : phys_InitR_MAP_INVALID;
}

#ifdef UTEST_PHYS

static void utest_phys_Untested(void)
{
    phys_Init((phys_InitA){0});
}

#ifdef UTEST
static void utest_phys_Main(void)
{
    util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_phys_Untested();
}

int main(void)
{
    utest_phys_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_PHYS
#endif // PHYS_C
