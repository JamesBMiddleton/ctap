#ifndef CORE_C
#define CORE_C
CORE_C

#include "src/util.c"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    uint placeholder;
} core_InitA;
typedef enum { core_InitR_OK, core_InitR_MAP_INVALID } core_InitR;
static core_InitR core_Init(core_InitA arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    uint startiness;
    uint numHorses;
} core__EngineStarter;

typedef enum {
    core__StartTheEnginesR_OK,
    core__StartTheEnginesR_MAP_INVALID
} core__StartTheEnginesR;
/*
 * placeholder.
 *
 * @param starter - placeholder
*/
static core__StartTheEnginesR core__StartTheEngines(core__EngineStarter starter)
{
    if (starter.startiness != 0)
        return core__StartTheEnginesR_MAP_INVALID;
    util_LOGF_DEBUG("Engines started with %u startiness and %u horses!",
               {.u = starter.startiness}, {.u = starter.num_horses});
    return core__StartTheEnginesR_OK;
}

typedef enum {
    core__SpaghettifyValueR_OK,
    core__SpaghettifyValueR_NOTOK
} core__SpaghettifyValueR;
static core__SpaghettifyValueR core__SpaghettifyValue(uint* value)
{
    *value = 0;
    return core__SpaghettifyValueR_OK;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct core__State {
    uint placeholder;
    // pool_t
} static core__state = {0};

/*
 * Initialise the core module. 
 *
 * @param args - initialisation arguments.
*/
static core_InitR core_Init(core_InitA args)
{
    core__state.placeholder = args.placeholder;

    if (core__SpaghettifyValue(&core__state.placeholder) != core__SpaghettifyValueR_OK)
        return core_InitR_MAP_INVALID;

    if (core__StartTheEngines((core__EngineStarter){
            .numHorses = core__state.placeholder,
            .startiness = core__state.placeholder}) != core__StartTheEnginesR_OK)
        return core_InitR_MAP_INVALID;

    util_LOG_DEBUG("Map loaded.");

    return core_InitR_OK;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UTEST IMPL /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef UTEST_CORE

static void utest_core_Untested(void)
{
    core_Init((core_InitA){0});
}

#ifdef UTEST
static void utest_core_Main(void)
{
    util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_core_Untested();
}

int main(void)
{
    utest_core_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_CORE
#endif // CORE_C
