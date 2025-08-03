#ifndef CORE_C
#define CORE_C
CORE_C

#include "src/util.c"

typedef struct {
    uint startiness;
    uint numHorses;
} EngineStarter;

typedef enum {
    StartTheEnginesR_OK,
    StartTheEnginesR_MAP_INVALID
} StartTheEnginesR;
/*
 * placeholder.
 *
 * @param starter - placeholder
*/
static StartTheEnginesR StartTheEngines(EngineStarter starter)
{
    if (starter.startiness != 0)
        return StartTheEnginesR_MAP_INVALID;
    util_LOGF_DEBUG("Engines started with %u startiness and %u horses!",
               {.u = starter.startiness}, {.u = starter.num_horses});
    return StartTheEnginesR_OK;
}

typedef enum {
    SpaghettifyValueR_OK,
    SpaghettifyValueR_NOTOK
} SpaghettifyValueR;
static SpaghettifyValueR SpaghettifyValue(uint* value)
{
    *value = 0;
    return SpaghettifyValueR_OK;
}

struct StateCore {
    uint placeholder;
    // pool_t
} static gStateCore = {0};


typedef struct {
    uint placeholder;
} core_InitA;
typedef enum { core_InitR_OK, core_InitR_MAP_INVALID } core_InitR;

/*
 * Initialise the core module. 
 *
 * @param args - initialisation arguments.
*/
static core_InitR core_Init(core_InitA args)
{
    gStateCore.placeholder = args.placeholder;

    if (SpaghettifyValue(&gStateCore.placeholder) != SpaghettifyValueR_OK)
        return core_InitR_MAP_INVALID;

    if (StartTheEngines((EngineStarter){
            .numHorses = gStateCore.placeholder,
            .startiness = gStateCore.placeholder}) != StartTheEnginesR_OK)
        return core_InitR_MAP_INVALID;

    util_LOG_DEBUG("Map loaded.");

    return core_InitR_OK;
}

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
