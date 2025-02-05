#ifndef INPUT_H
#define INPUT_H

#include "src/utils.c"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
} INP_InitArg;
typedef enum { INP_InitRet_OK, INP_InitRet_MAP_INVALID } INP_InitRet;
static INP_InitRet INP_Init(INP_InitArg arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

struct inp_State {
    u32 placeholder;
    // pool_t
} static inp_state = {0}; // NOLINT

/*
 * Initialise the audio module. 
 *
 * @param args - initialisation arguments.
*/
static INP_InitRet INP_Init(INP_InitArg args)
{
    return (args.placeholder) ? INP_InitRet_OK : INP_InitRet_MAP_INVALID;
}

#pragma GCC diagnostic pop
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#endif // INPUT_H
