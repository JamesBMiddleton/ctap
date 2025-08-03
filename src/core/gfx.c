#ifndef GFX_C
#define GFX_C
GFX_C

#include "src/util.c"

struct StateGfx {
    uint placeholder;
    // pool_t
} static gStateGfx = {0};

typedef struct {
    uint placeholder;
} gfx_InitA;
typedef enum { gfx_InitR_OK, gfx_InitR_MAP_INVALID } gfx_InitR;

/*
 * Initialise the audio module. 
 *
 * @param args - initialisation arguments.
*/
static gfx_InitR gfx_Init(gfx_InitA args)
{
    gStateGfx.placeholder = 1;
    return (args.placeholder) ? gfx_InitR_OK : gfx_InitR_MAP_INVALID;
}

#ifdef UTEST_GFX

static void utest_gfx_Untested(void)
{
    gfx_Init((gfx_InitA){0});
}

#ifdef UTEST
static void utest_gfx_Main(void)
{
    util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_gfx_Untested();
}

int main(void)
{
    utest_gfx_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_GFX
#endif // GFX_C
