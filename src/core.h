#ifndef CORE_H
#define CORE_H

#include "utils.h"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// API DECL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
} cor_init_arg_t;
typedef enum { cor_init_OK, cor_init_MAP_INVALID } cor_init_e;
static cor_init_e cor_init(cor_init_arg_t arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 startiness;
    u32 num_horses;
} engine_starter_t;

typedef enum {
    start_the_engines_OK,
    start_the_engines_MAP_INVALID
} start_the_engines_e;
/*
 * placeholder.
 *
 * @param starter - placeholder
*/
static start_the_engines_e start_the_engines(engine_starter_t starter)
{
    if (starter.startiness != 0)
        return start_the_engines_MAP_INVALID;
    LOGF_DEBUG("Engines started with %u startiness and %u horses!",
               {.u = starter.startiness}, {.u = starter.num_horses});
    return start_the_engines_OK;
}

typedef enum {
    spaghettify_value_OK,
    spaghettify_value_NOTOK
} spaghettify_value_e;
static spaghettify_value_e spaghettify_value(u32* value)
{
    *value = 0;
    return spaghettify_value_OK;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

struct state_cor_t {
    u32 placeholder;
    // pool_t
} static state_cor = {0}; // NOLINT

/*
 * Initialise the core module. 
 *
 * @param args - initialisation arguments.
*/
static cor_init_e cor_init(cor_init_arg_t args)
{
    state_cor.placeholder = args.placeholder;

    if (spaghettify_value(&state_cor.placeholder) != spaghettify_value_OK)
        return cor_init_MAP_INVALID;

    if (start_the_engines((engine_starter_t){
            .num_horses = state_cor.placeholder,
            .startiness = state_cor.placeholder}) != start_the_engines_OK)
        return cor_init_MAP_INVALID;

    LOG_DEBUG("Map loaded.");

    return cor_init_OK;
}

#pragma GCC diagnostic pop
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif // CORE_H
