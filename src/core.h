#include "src/utils.h"

typedef enum { cor_rc_NONE, cor_rc_OK, cor_rc_MAP_INVALID } cor_rc_e;

typedef struct {
    u32 placeholder;
} cor_init_args_t;

static void cor_init(cor_init_args_t args, cor_rc_e* rc);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// IMPL /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
    // pool_t
} icor_state_t;

//NOLINTBEGIN
static icor_state_t icor_state = {0}; // API func usage only
//NOLINTEND

typedef struct {
    u32 startiness;
    u32 num_horses;
} icor_engine_starter_t;

/*
 * placeholder.
 *
 * @param starter - placeholder
 * @throw MAP_INVALID
*/
static void icor_start_the_engines(icor_engine_starter_t starter, cor_rc_e* rc)
{
    if (starter.startiness != 0)
    {
        *rc = cor_rc_MAP_INVALID;
        return;
    }
    LOGF_DEBUG("Engines started with %u startiness and %u horses!",
               {.u = starter.startiness}, {.u = starter.num_horses});
}

static void icor_spaghettify_value(u32* value, cor_rc_e* rc)
{
    *value = 0;
    *rc = cor_rc_OK;
}

/*
 * Initialise the core module. 
 *
 * @param args - initialisation arguments.
 * @throw MAP_INVALID
*/
static void cor_init(cor_init_args_t args, cor_rc_e* rc)
{
    icor_state.placeholder = args.placeholder;

    icor_spaghettify_value(&icor_state.placeholder, rc);
    if (*rc != cor_rc_OK)
        return;

    icor_start_the_engines(
        (icor_engine_starter_t){.num_horses = icor_state.placeholder,
                                .startiness = icor_state.placeholder},
        rc);
    if (*rc != cor_rc_OK)
        return;

    LOG_DEBUG("Map loaded.");
}

