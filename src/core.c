#ifndef CORE_H
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-macros"
#define CORE_H
#pragma GCC diagnostic pop

#include "src/util.c"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
} core_InitArg;
typedef enum { core_InitRet_OK, core_InitRet_MAP_INVALID } core_InitRet;
static core_InitRet core_Init(core_InitArg arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 startiness;
    u32 num_horses;
} core__EngineStarter;

typedef enum {
    core__StartTheEnginesRet_OK,
    core__StartTheEnginesRet_MAP_INVALID
} core__StartTheEnginesRet;
/*
 * placeholder.
 *
 * @param starter - placeholder
*/
static core__StartTheEnginesRet core__StartTheEngines(core__EngineStarter starter)
{
    if (starter.startiness != 0)
        return core__StartTheEnginesRet_MAP_INVALID;
    util_LOGF_DEBUG("Engines started with %u startiness and %u horses!",
               {.u = starter.startiness}, {.u = starter.num_horses});
    return core__StartTheEnginesRet_OK;
}

typedef enum {
    core__SpaghettifyValueRet_OK,
    core__SpaghettifyValueRet_NOTOK
} core__SpaghettifyValueRet;
static core__SpaghettifyValueRet core__SpaghettifyValue(u32* value)
{
    *value = 0;
    return core__SpaghettifyValueRet_OK;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

struct core__State {
    u32 placeholder;
    // pool_t
} static core__state = {0}; // NOLINT

/*
 * Initialise the core module. 
 *
 * @param args - initialisation arguments.
*/
static core_InitRet core_Init(core_InitArg args)
{
    core__state.placeholder = args.placeholder;

    if (core__SpaghettifyValue(&core__state.placeholder) != core__SpaghettifyValueRet_OK)
        return core_InitRet_MAP_INVALID;

    if (core__StartTheEngines((core__EngineStarter){
            .num_horses = core__state.placeholder,
            .startiness = core__state.placeholder}) != core__StartTheEnginesRet_OK)
        return core_InitRet_MAP_INVALID;

    util_LOG_DEBUG("Map loaded.");

    return core_InitRet_OK;
}

#pragma GCC diagnostic pop

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UTEST IMPL /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef CORE_UTEST

#pragma GCC diagnostic ignored "-Wunused-function"
static void utest_core(void)
{
}

#ifdef UTEST
i32 main(void)
{
    utest_core();
    return 0;
}
#endif // UTEST
#endif // CORE_UTEST
#endif // CORE_H
