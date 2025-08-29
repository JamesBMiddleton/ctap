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

#define vec_TYPE EngineStarter
#include "vec.c"

// clang-format off
// CPR = Const Pointer Result
typedef struct { uint ok; core_Error err; } core_uintR;
typedef struct { int ok; core_Error err; } core_intR;
typedef struct { float ok; core_Error err; } core_floatR;
typedef struct { const uchar* ok; core_Error err; } core_ucharCPR;
typedef struct { uchar* ok; core_Error err; } core_ucharPR;
typedef struct { const intVec* ok; core_Error err; } intVecCPR;
typedef struct { const EngineStarterVec* ok; core_Error err; } EngineStarterVecCPR;
// clang-format on

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
static core_intR StartTheEngines(EngineStarter starter)
{
    alloc_Arena scratch;
    floatVec vec = floatVecInit(&scratch);
    floatVecClear(&vec);

    if (starter.startiness != 0)
        return (core_intR){.err = core_Error_MAP_INVALID};
    util_LOGF_DEBUG("Engines started with %u startiness and %u horses!",
                    {.u = starter.startiness}, {.u = starter.numHorses});
    return (core_intR){.ok = 1};
}

// another placeholder comment
static core_Error SpaghettifyValue(uint* value)
{
    alloc_Arena scratch;
    intVec vec = intVecInit(&scratch);
    intVecPush(&vec, 1);
    intVecPop(&vec);
    intVecInsert(&vec, 0, 42);
    intVecClear(&vec);
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
