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
} static ctap__state = {0}; // NOLINT

/* 
 * Retrieve the last log message created.
 * Set 'ctap_update_cb' to be notified when a new log message is available.
 *
 * @return the last log message created
*/
ctap_Log ctap_GetLog(void)
{
    const util_Log in_log = util_GetLog();
    ctap_LogLvl lvl = ctap_LogLvl_DEBUG;
    switch (in_log.lvl)
    {
        case util_LogLvl_DEBUG: lvl = ctap_LogLvl_DEBUG; break;
        case util_LogLvl_WARN: lvl = ctap_LogLvl_WARN; break;
        case util_LogLvl_ERROR: lvl = ctap_LogLvl_ERROR; break;
        case util_LogLvl_PANIC: lvl = ctap_LogLvl_PANIC; break;
        case util_LogLvl_ASSERT: lvl = ctap_LogLvl_ASSERT; break;
    }
    ctap_Log out_log = {
        .lvl = lvl, .line_num = in_log.line_num, .func_name = in_log.func_name};
    util_Memcpy(out_log.message, in_log.message,
                util_Strlen(in_log.message) + 1);
    return out_log;
}

/* 
 * Initialise the ctap runtime.
 *
 * @param arg - runtime initialisation arguments
*/
ctap_InitRet ctap_Init(ctap_InitArg arg)
{
    switch (
        util_Init((util_InitArg){.log_update_callback = arg.log_update_callback,
                                 .panic_callback = arg.panic_callback}))
    {
        case util_InitRet_OK:
            util_LOG_DEBUG("utl initialisation success.");
            break;
        case util_InitRet_NULL_LOG:
        case util_InitRet_NULL_CALLBACK: util_PANIC();
    }

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
#ifdef CTAP_UTEST

#pragma GCC diagnostic ignored "-Wunused-function"
static void utest_ctap(void)
{
}

#ifdef UTEST
i32 main(void)
{
    utest_ctap();
    return 0;
}
#endif // UTEST
#endif // CTAP_UTEST
