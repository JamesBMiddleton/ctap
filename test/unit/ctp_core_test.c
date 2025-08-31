#include "src/core.c"
#include "stdio.h"

static void EventHandlerLogPrintf(const util_Log log)
{
    const char* lvl = "UNKNOWN";
    switch (log.lvl)
    {
        case util_LogLvl_DEBUG: lvl = "DEBUG"; break;
        case util_LogLvl_WARN: lvl = "WARN"; break;
        case util_LogLvl_ERROR: lvl = "ERROR"; break;
        case util_LogLvl_PANIC: lvl = "PANIC"; break;
        case util_LogLvl_ASSERT: lvl = "ASSERT"; break;
    }
    printf("%s | %s::%d | %s\n", lvl, log.funcName, log.lineNum, log.message);
    (void)fflush(stdout);
}

int main(void)
{
    util_RegisterEventHandlerLog(EventHandlerLogPrintf);
    core_Init((core_InitArgs){0});
    return 0;
}
