#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "utils.h"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
} gfx_init_arg_t;
typedef enum { gfx_init_OK, gfx_init_MAP_INVALID } gfx_init_e;
static gfx_init_e gfx_init(gfx_init_arg_t arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

struct state_gfx_t {
    u32 placeholder;
    // pool_t
} static state_gfx = {0}; // NOLINT

/*
 * Initialise the graphics module. 
 *
 * @param args - initialisation arguments.
*/
static gfx_init_e gfx_init(gfx_init_arg_t args)
{
    return (args.placeholder) ? gfx_init_OK : gfx_init_MAP_INVALID;
}

#pragma GCC diagnostic pop
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICS_H
