#include "ctap.h"
#include "core.c"
#include "src/util.c"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct ctap__State {
    u32 placeholder;
} static ctap__state = {0};

/* 
 * Initialise the ctap runtime.
 *
 * @param arg - runtime initialisation arguments
*/
#pragma GCC diagnostic ignored "-Wincompatible-function-pointer-types"
#pragma GCC diagnostic ignored "-Wincompatible-function-pointer-types-strict"
ctap_InitRet ctap_Init(ctap_InitArg arg)
{
    if (arg.log_callback)
        util_SetLogCallback(arg.log_callback);
    if (arg.panic_callback)
        util_SetPanicCallback(arg.panic_callback);

    const u32 placeholder = 42;
    switch (core_Init((core_InitArg){.placeholder = placeholder}))
    {
        case core_InitRet_OK:
            util_LOG_DEBUG("cor initialisation success.");
            break;
        case core_InitRet_MAP_INVALID: util_PANIC();
    }

    ctap__state.placeholder = 1;

    return ctap_InitRet_OK;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UTEST IMPL /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef UTEST_CTAP

static void utest_ctap_Main(void)
{
}

#ifdef UTEST
i32 main(void)
{
    utest_ctap_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_CTAP
