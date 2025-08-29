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

// clang-format off
typedef struct { uint ok; core_Error err; } core_UintResult;
typedef struct { int ok; core_Error err; } core_IntResult;
typedef struct { float ok; core_Error err; } core_FloatResult;
typedef struct { EngineStarter ok; core_Error err; } EngineStarterResult;
// clang-format on

typedef struct {
    uint placeholder;
} core_InitArgs;
static core_Error core_Init(core_InitArgs args);
static core_IntResult StartTheEngines(EngineStarter starter);
static core_Error SpaghettifyValue(uint* value);

struct StateCore {
    uint placeholder;
    // pool_t
} static gStateCore = {0};

// placeholder comment
static core_IntResult StartTheEngines(EngineStarter starter)
{
    if (starter.startiness != 0)
        return (core_IntResult){.err = core_Error_MAP_INVALID};
    util_LOGF_DEBUG("Engines started with %u startiness and %u horses!",
                    {.u = starter.startiness}, {.u = starter.numHorses});
    return (core_IntResult){.ok = 1};
}

// another placeholder comment
static core_Error SpaghettifyValue(uint* value)
{
    *value = 0;
    return core_Error_MAP_INVALID;
}

// Initialise the core module.
static core_Error core_Init(core_InitArgs args)
{
    gStateCore.placeholder = args.placeholder;
    {
        const core_Error result = SpaghettifyValue(&gStateCore.placeholder);
        if (result) return result;
    }
    {
        const core_IntResult result = StartTheEngines(
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
