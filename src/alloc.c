#ifndef ALLOC_C
#define ALLOC_C
ALLOC_C

#include "primitives.c"

// ------------------------------ API DECL ---------------------------------- //

// ------------------------------ API IMPL ---------------------------------- //

// ----------------------------- UTEST IMPL --------------------------------- //

#ifdef UTEST_ALLOC

static void utest_alloc_Untested(void)
{
    // alloc_Init((alloc_InitArg){0});
}

#ifdef UTEST
static void utest_alloc_Main(void)
{
    // util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    // util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_alloc_Untested();
}

int main(void)
{
    utest_alloc_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_ALLOC
#endif // ALLOC_C
