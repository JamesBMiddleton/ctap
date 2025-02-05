#ifndef CTAP_H
#define CTAP_H

#include "lib.h"
#include "src/utils.c"
#include "core.c"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct ctp_State {
    u32 placeholder;
} static ctp_state = {0}; // NOLINT

/* 
 * Retrieve the last log message created.
 * Set 'CTP_update_cb' to be notified when a new log message is available.
 *
 * @return the last log message created
*/
CTP_Log CTP_GetLog(void)
{
    const UTL_Log in_log = UTL_GetLog();
    CTP_LogLvl lvl = CTP_LogLvl_DEBUG;
    switch (in_log.lvl)
    {
        case UTL_LogLvl_DEBUG: lvl = CTP_LogLvl_DEBUG; break;
        case UTL_LogLvl_WARN: lvl = CTP_LogLvl_WARN; break;
        case UTL_LogLvl_ERROR: lvl = CTP_LogLvl_ERROR; break;
        case UTL_LogLvl_PANIC: lvl = CTP_LogLvl_PANIC; break;
        case UTL_LogLvl_ASSERT: lvl = CTP_LogLvl_ASSERT; break;
    }
    CTP_Log out_log = {
        .lvl = lvl, .line_num = in_log.line_num, .func_name = in_log.func_name};
    UTL_Memcpy(out_log.message, in_log.message, UTL_Strlen(in_log.message) + 1);
    return out_log;
}

/* 
 * Initialise the ctap runtime.
 *
 * @param arg - runtime initialisation arguments
*/
CTP_InitRet CTP_Init(CTP_InitArg arg)
{
    switch (UTL_Init(
        (UTL_InitArg){.log_update_callback = arg.log_update_callback,
                         .panic_callback = arg.panic_callback}))
    {
        case UTL_InitRet_OK: UTL_LOG_DEBUG("utl initialisation success."); break;
        case UTL_InitRet_NULL_LOG:
        case UTL_InitRet_NULL_CALLBACK: UTL_PANIC();
    }

    const u32 placeholder = 42;
    switch (COR_Init((COR_InitArg){.placeholder = placeholder}))
    {
        case COR_InitRet_OK: UTL_LOG_DEBUG("cor initialisation success."); break;
        case COR_InitRet_MAP_INVALID: UTL_PANIC();
    }

    return CTP_InitRet_OK;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif // CTAP_H
