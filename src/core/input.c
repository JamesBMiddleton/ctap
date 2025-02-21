#ifndef INPUT_C
#define INPUT_C
INPUT_C

#include "src/util.c"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    uint placeholder;
} input_InitArg;
typedef enum { input_InitRet_OK, input_InitRet_MAP_INVALID } input_InitRet;
static input_InitRet input_Init(input_InitArg arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct input__State {
    uint placeholder;
    // pool_t
} static input__state = {0};

/*
 * Initialise the audio module. 
 *
 * @param args - initialisation arguments.
*/
static input_InitRet input_Init(input_InitArg args)
{
    input__state.placeholder = 1;
    return (args.placeholder) ? input_InitRet_OK : input_InitRet_MAP_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UTEST IMPL /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef UTEST_INPUT

static void utest_input_Untested(void)
{
    input_Init((input_InitArg){0});
}

#ifdef UTEST
static void utest_input_Main(void)
{
    util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_input_Untested();
}

int main(void)
{
    utest_input_Main();
    return 0;
}
#endif // UTEST
#endif // UTEST_INPUT
#endif // INPUT_C
