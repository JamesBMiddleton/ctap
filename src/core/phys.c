#ifndef PHYS_C
#define PHYS_C
PHYS_C

#include "src/primitives.c"

typedef enum { phys_Error_OK, phys_Error_MAP_INVALID } phys_Error;

typedef struct {
    uint placeholder;
} phys_InitArgs;
static phys_Error phys_Init(phys_InitArgs args);

struct StatePhys {
    uint placeholder;
    // pool_t
} static gStatePhys = {0};

// Initialise the physics module.
static phys_Error phys_Init(phys_InitArgs args)
{
    gStatePhys.placeholder = 1;
    return (args.placeholder) ? phys_Error_OK : phys_Error_MAP_INVALID;
}

#endif // PHYS_C
