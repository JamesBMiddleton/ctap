#include "ctap.h"
#include "core.c"
#include "src/util.c"

struct eventHandlersCtap {
    void (*log)(ctap_Log);
} static gEventHandlersCtap = {0};

static void EventTriggerLog(const util_Log log)
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
    gEventHandlersCtap.log((ctap_Log){.lvl = lvl,
                                      .message = log.message,
                                      .lineNum = log.lineNum,
                                      .funcName = log.funcName});
}

struct StateCtap {
    uint placeholder;
} static gStateCtap = {0};

/* 
 * Initialise the ctap runtime.
 *
 * @param arg - runtime initialisation arguments
*/
ctap_InitR ctap_Init(ctap_InitA arg)
{
    if (arg.eventHandlerLog)
    {
        util_RegisterEventHandlerLog(EventTriggerLog);
        gEventHandlersCtap.log = arg.eventHandlerLog;
    }
    if (arg.eventHandlerPanic)
        util_RegisterEventHandlerPanic(arg.eventHandlerPanic);

    const uint placeholder = 42;
    switch (core_Init((core_InitA){.placeholder = placeholder}))
    {
        case core_InitR_OK:
            util_LOG_DEBUG("cor initialisation success.");
            break;
        case core_InitR_MAP_INVALID: util_PANIC();
    }

    gStateCtap.placeholder = 1;

    return ctap_InitR_OK;
}

#ifdef UTEST_CTAP

static void utest_ctap_Main(void)
{
}

#ifdef UTEST
int main(void)
{
    utest_ctap_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_CTAP
