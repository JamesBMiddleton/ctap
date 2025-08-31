#ifndef CORE_C
#define CORE_C
CORE_C

#include "core/phys.c"
#include "src/util.c"



typedef enum {
    core_Error_OK,
    core_Error_MAP_INVALID,
    core_Error_MODULE_LOAD_FAILURE
} core_Error;

typedef struct {
    uint startiness;
    uint numHorses;
} EngineStarter;

typedef struct {
    uint placeholder;
} core_InitArgs;
static core_Error core_Init(core_InitArgs args);
static core_intR StartTheEngines(EngineStarter starter);
static core_Error SpaghettifyValue(uint* value);

struct StateCore {
    uint placeholder;
    // pool_t
} static gStateCore = {0};

// placeholder comment
static int StartTheEngines(EngineStarter starter)
{
    if (starter.startiness != 0)
        return FAILURE
    util_LOGF_DEBUG("Engines started with %u startiness and %u horses!",
                    {.u = starter.startiness}, {.u = starter.numHorses});
    return SUCCESS;
}

// another placeholder comment
static int SpaghettifyValue(uint* value)
{
    *value = 0;
    return core_Error_MAP_INVALID;
}

// Initialise the core module.
static int core_Init(core_InitArgs args)
{
    gStateCore.placeholder = args.placeholder;
    {
        const core_Error result = SpaghettifyValue(&gStateCore.placeholder);
        if (result) return result;
    }
    {
        const core_intR result = StartTheEngines(
            (EngineStarter){.numHorses = gStateCore.placeholder});
        if (result.err) return result.err;
    }
    switch (phys_Init((phys_InitArgs){0}))
    {
        case phys_Error_OK: util_LOG_DEBUG("Physics Module Initialised.");
        case phys_Error_MAP_INVALID: return core_Error_MODULE_LOAD_FAILURE;
    }
    util_LOG_DEBUG("Map loaded.");

    return core_Error_OK;
}

#endif // CORE_C
