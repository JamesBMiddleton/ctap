#ifndef CTAP_H
#define CTAP_H

#include "lib.h"
#include "src/utils.c"
#include "core.c"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct ctp_State_t {
    u32 placeholder;
} static ctp_state = {0}; // NOLINT

/* 
 * Retrieve the last log message created.
 * Set 'CTP_update_cb' to be notified when a new log message is available.
 *
 * @return the last log message created
*/
CTP_Log_t CTP_GetLog(void)
{
    const UTL_Log_t inLog = UTL_GetLog();
    CTP_LogLvl_e lvl = CTP_LogLvl_DEBUG;
    switch (inLog.lvl)
    {
        case UTL_LogLvl_DEBUG: lvl = CTP_LogLvl_DEBUG; break;
        case UTL_LogLvl_WARN: lvl = CTP_LogLvl_WARN; break;
        case UTL_LogLvl_ERROR: lvl = CTP_LogLvl_ERROR; break;
        case UTL_LogLvl_PANIC: lvl = CTP_LogLvl_PANIC; break;
        case UTL_LogLvl_ASSERT: lvl = CTP_LogLvl_ASSERT; break;
    }
    CTP_Log_t outLog = {
        .lvl = lvl, .lineNum = inLog.lineNum, .funcName = inLog.funcName};
    UTL_Memcpy(outLog.message, inLog.message, UTL_Strlen(inLog.message) + 1);
    return outLog;
}

/* 
 * Initialise the ctap runtime.
 *
 * @param arg - runtime initialisation arguments
*/
CTP_InitRet_e CTP_Init(CTP_InitArg_t arg)
{
    switch (UTL_Init(
        (UTL_InitArg_t){.logUpdateCallback = arg.logUpdateCallback,
                         .panicCallback = arg.panicCallback}))
    {
        case UTL_InitRet_OK: LOG_DEBUG("utl initialisation success."); break;
        case UTL_InitRet_NULL_LOG:
        case UTL_InitRet_NULL_CALLBACK: PANIC();
    }

    const u32 placeholder = 42;
    switch (COR_Init((COR_InitArg_t){.placeholder = placeholder}))
    {
        case COR_InitRet_OK: LOG_DEBUG("cor initialisation success."); break;
        case COR_InitRet_MAP_INVALID: PANIC();
    }

    return CTP_InitRet_OK;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif // CTAP_H
