/*
#include "test.h" 

i32 main(void)
{
    tst_setup();

    return 0;
}
*/

#include "ctap.h"
#include <stdio.h>
#include <stdlib.h>

void tst_log_update_callback(void);
void tst_panic_callback(void);
void tst_init(void);

typedef struct {
    bool suppress_logs;
} state_tst_t;
static state_tst_t mutable_state_tst;
static const state_tst_t* state_tst = &mutable_state_tst;

void tst_log_update_callback(void)
{
    if (state_tst->suppress_logs == true)
        return;

    ctp_log_t log = ctp_get_log();

    const char* lvl = "UNKNOWN";
    switch (log.lvl)
    {
        case ctp_loglvl_DEBUG:
            lvl = "DEBUG";
            break;
        case ctp_loglvl_WARN:
            lvl = "WARN";
            break;
        case ctp_loglvl_ERROR:
            lvl = "ERROR";
            break;
        case ctp_loglvl_PANIC:
            lvl = "PANIC";
            break;
        case ctp_loglvl_ASSERT:
            lvl = "ASSERT";
            break;
    }
    printf("%s | %s::%d | %s\n", lvl, log.func_name, log.line_num, log.message);
    (void)fflush(stdout);
}

__attribute__((noreturn)) void tst_panic_callback(void)
{
    (void)fflush(stdout);
    abort();
}

// tst_user_stub()
// tst_str_equals()...

void tst_init(void)
{
    ctp_rc_e rc = {0};
    ctp_init((ctp_init_args_t){.log_update_callback = tst_log_update_callback,
                          .panic_callback = tst_panic_callback}, &rc);
    ASSERT(rc == ctp_rc_OK);
    mutable_state_tst.suppress_logs = false;
}
