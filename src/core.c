#ifndef CORE_H
#define CORE_H

#include "src/utils.c"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
} COR_InitArg_t;
typedef enum { COR_InitRet_OK, COR_InitRet_MAP_INVALID } COR_InitRet_e;
static COR_InitRet_e COR_Init(COR_InitArg_t arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 startiness;
    u32 numHorses;
} utl_EngineStarter_t;

typedef enum {
    utl_StartTheEnginesRet_OK,
    utl_StartTheEnginesRet_MAP_INVALID
} utl_StartTheEnginesRet_e;
/*
 * placeholder.
 *
 * @param starter - placeholder
*/
static utl_StartTheEnginesRet_e utl_StartTheEngines(utl_EngineStarter_t starter)
{
    if (starter.startiness != 0)
        return utl_StartTheEnginesRet_MAP_INVALID;
    LOGF_DEBUG("Engines started with %u startiness and %u horses!",
               {.u = starter.startiness}, {.u = starter.numHorses});
    return utl_StartTheEnginesRet_OK;
}

typedef enum {
    utl_SpaghettifyValueRet_OK,
    utl_SpaghettifyValueRet_NOTOK
} utl_SpaghettifyValueRet_e;
static utl_SpaghettifyValueRet_e utl_SpaghettifyValue(u32* value)
{
    *value = 0;
    return utl_SpaghettifyValueRet_OK;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

struct cor_State_t {
    u32 placeholder;
    // pool_t
} static cor_state = {0}; // NOLINT

/*
 * Initialise the core module. 
 *
 * @param args - initialisation arguments.
*/
static COR_InitRet_e COR_Init(COR_InitArg_t args)
{
    cor_state.placeholder = args.placeholder;

    if (utl_SpaghettifyValue(&cor_state.placeholder) != utl_SpaghettifyValueRet_OK)
        return COR_InitRet_MAP_INVALID;

    if (utl_StartTheEngines((utl_EngineStarter_t){
            .numHorses = cor_state.placeholder,
            .startiness = cor_state.placeholder}) != utl_StartTheEnginesRet_OK)
        return COR_InitRet_MAP_INVALID;

    LOG_DEBUG("Map loaded.");

    return COR_InitRet_OK;
}

#pragma GCC diagnostic pop
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif // CORE_H
