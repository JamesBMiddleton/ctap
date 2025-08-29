#include "ctap.h"
#include "core.c"
#include "src/util.c"

struct eventHandlersCtap {
    void (*log)(ctap_Log);
} static gEventHandlersCtap = {0};

struct StateCtap {
    uint placeholder;
} static gStateCtap = {0};

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

// Initialise the ctap runtime.
ctap_Error ctap_Init(ctap_InitArgs arg)
{
    if (arg.eventHandlerLog)
    {
        util_RegisterEventHandlerLog(EventTriggerLog);
        gEventHandlersCtap.log = arg.eventHandlerLog;
    }
    if (arg.eventHandlerPanic)
        util_RegisterEventHandlerPanic(arg.eventHandlerPanic);

    const uint placeholder = 42;
    switch (core_Init((core_InitArgs){.placeholder = placeholder}))
    {
        case core_Error_OK:
            util_LOG_DEBUG("cor initialisation success.");
            break;
        case core_Error_MAP_INVALID: 
        case core_Error_MODULE_LOAD_FAILURE: 
            util_PANIC();
    }

    gStateCtap.placeholder = 1;

    return ctap_Error_OK;
}
