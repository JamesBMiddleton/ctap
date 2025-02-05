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
static COR_InitRet_e COR_init(COR_InitArg_t arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 startiness;
    u32 num_horses;
} cor_EngineStarter_t;

typedef enum {
    cor_StartTheEnginesRet_OK,
    cor_StartTheEnginesRet_MAP_INVALID
} cor_StartTheEnginesRet_e;
/*
 * placeholder.
 *
 * @param starter - placeholder
*/
static cor_StartTheEnginesRet_e cor_start_the_engines(cor_EngineStarter_t starter)
{
    if (starter.startiness != 0)
        return cor_StartTheEnginesRet_MAP_INVALID;
    LOGF_DEBUG("Engines started with %u startiness and %u horses!",
               {.u = starter.startiness}, {.u = starter.num_horses});
    return cor_StartTheEnginesRet_OK;
}

typedef enum {
    cor_SpaghettifyValueRet_OK,
    cor_SpaghettifyValueRet_NOTOK
} cor_SpaghettifyValueRet_e;
static cor_SpaghettifyValueRet_e cor_spaghettify_value(u32* value)
{
    *value = 0;
    return cor_SpaghettifyValueRet_OK;
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
static COR_InitRet_e COR_init(COR_InitArg_t args)
{
    cor_state.placeholder = args.placeholder;

    if (cor_spaghettify_value(&cor_state.placeholder) != cor_SpaghettifyValueRet_OK)
        return COR_InitRet_MAP_INVALID;

    if (cor_start_the_engines((cor_EngineStarter_t){
            .num_horses = cor_state.placeholder,
            .startiness = cor_state.placeholder}) != cor_StartTheEnginesRet_OK)
        return COR_InitRet_MAP_INVALID;

    LOG_DEBUG("Map loaded.");

    return COR_InitRet_OK;
}

#pragma GCC diagnostic pop
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif // CORE_H
