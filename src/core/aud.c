#ifndef AUD_C
#define AUD_C
AUD_C

#include "src/util.c"

struct StateAud {
    uint placeholder;
    // pool_t
} static gStateAud = {0};

typedef struct {
    uint placeholder;
} aud_InitA;
typedef enum { aud_InitR_OK, aud_InitR_MAP_INVALID } aud_InitR;

/*
 * Initialise the audio module. 
 *
 * @param args - initialisation arguments.
*/
static aud_InitR aud_Init(aud_InitA args)
{
    gStateAud.placeholder = 1;
    return (args.placeholder) ? aud_InitR_OK : aud_InitR_MAP_INVALID;
}

#ifdef UTEST_AUD

static void utest_aud_Untested(void)
{
    aud_Init((aud_InitA){0});
}

#ifdef UTEST
static void utest_aud_Main(void)
{
    // setup logger
    util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_aud_Untested();
}

int main(void)
{
    utest_aud_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_AUD
#endif // AUD_C
