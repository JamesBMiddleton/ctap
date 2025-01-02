#include "src/utils.h"

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// API ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    ctp_loglvl_ASSERT,
    ctp_loglvl_DEBUG,
    ctp_loglvl_WARN,
    ctp_loglvl_ERROR,
    ctp_loglvl_PANIC
} ctp_loglvl_e;

typedef struct {
    ctp_loglvl_e lvl;
    u32 line_num;
    const char* func_name;
    char message[MAX_LOG_SZ];
} ctp_log_t;

typedef struct {
    // map..
    // input_cb..
    void (*log_update_callback)(void); // Optional.
    void (*panic_callback)(void); // Optional.
} ctp_init_arg_t;
typedef enum { ctp_init_OK, ctp_init_NULL_CALLBACK } ctp_init_e;
ctp_init_e ctp_init(ctp_init_arg_t arg);

ctp_log_t ctp_get_log(void);

// typedef struct {
//     u32 framewidth;
//     u32 framelength;
// } ctp_get_frame_arg_t;
// typedef struct {
//     u32* data;
//     enum { ctp_get_frame_OK, ctp_get_frame_NOTOK } rc;
// } ctp_get_frame_t;
// ctp_get_frame_t ctp_get_frame(ctp_get_frame_arg_t arg);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// IMPL /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
} state_t;

//NOLINTBEGIN
static state_t state = {0}; // API func !l-value! usage only
//NOLINTEND

/* 
 * Retrieve the last log message created.
 * Set 'ctp_update_cb' to be notified when a new log message is available.
 *
 * @return the last log message created
*/
ctp_log_t ctp_get_log(void)
{
    const utl_log_t in_log = utl_get_log();
    ctp_loglvl_e lvl = ctp_loglvl_DEBUG;
    switch (in_log.lvl)
    {
        case utl_loglvl_DEBUG:
            lvl = ctp_loglvl_DEBUG;
            break;
        case utl_loglvl_WARN:
            lvl = ctp_loglvl_WARN;
            break;
        case utl_loglvl_ERROR:
            lvl = ctp_loglvl_ERROR;
            break;
        case utl_loglvl_PANIC:
            lvl = ctp_loglvl_PANIC;
            break;
        case utl_loglvl_ASSERT:
            lvl = ctp_loglvl_ASSERT;
            break;
    }
    ctp_log_t out_log = {
        .lvl = lvl, .line_num = in_log.line_num, .func_name = in_log.func_name};
    utl_memcpy(out_log.message, in_log.message, utl_strlen(in_log.message) + 1);
    return out_log;
}

/* 
 * Initialise the ctap runtime.
 *
 * @param args - runtime initialisation arguments
 * @throw NULL_CALLBACK, NULL_LOG 
*/
void ctp_init(ctp_init_args_t args, ctp_rc_e* rc)
{
    utl_rc_e utl_rc = {0};
    utl_init((utl_init_args_t){.log_update_callback = args.log_update_callback,
                               .panic_callback = args.panic_callback},
             &utl_rc);
    switch (utl_rc)
    {
        case utl_rc_OK:
            LOG_DEBUG("utl initialisation success.");
            break;
        case utl_rc_NULL_LOG:
            *rc = ctp_init_NULL_CALLBACK;
            return;
        default:
            PANIC();
    }

    const u32 placeholder = 42;
    cor_rc_e cor_rc = {0};
    cor_init((cor_init_args_t){.placeholder = placeholder}, &cor_rc);
    switch (cor_rc)
    {
        case cor_rc_OK:
            LOG_DEBUG("cor initialisation success.");
            break;
        default:
            PANIC();
    }

    *rc = ctp_rc_OK;
}
