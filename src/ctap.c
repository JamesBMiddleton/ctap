#include "ctap.h"
#include "core.c"
#include "src/util.c"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// EVENT IMPL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct ctap__EventHandlers {
    void (*log)(ctap_Log);
} static ctap__event_handlers = {0};

static void ctap__EventTriggerLog(const util_Log log)
{
    ctap_LogLvl lvl = ctap_LogLvl_DEBUG;
    switch (log.lvl)
    {
        case util_LogLvl_DEBUG: lvl = ctap_LogLvl_DEBUG; break;
        case util_LogLvl_WARN: lvl = ctap_LogLvl_WARN; break;
        case util_LogLvl_ERROR: lvl = ctap_LogLvl_ERROR; break;
        case util_LogLvl_PANIC: lvl = ctap_LogLvl_PANIC; break;
        case util_LogLvl_ASSERT: lvl = ctap_LogLvl_ASSERT; break;
    }
    ctap__event_handlers.log((ctap_Log){.lvl = lvl,
                                            .message = log.message,
                                            .line_num = log.line_num,
                                            .func_name = log.func_name});
}

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
ctap_InitRet ctap_Init(ctap_InitArg arg)
{
    if (arg.event_handler_log)
    {
        util_RegisterEventHandlerLog(ctap__EventTriggerLog);
        ctap__event_handlers.log = arg.event_handler_log;
    }
    if (arg.event_handler_panic)
        util_RegisterEventHandlerPanic(arg.event_handler_panic);

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
////////////////////////// CALLBACK HANDLERS IMPL //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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
