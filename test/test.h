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

void tst_cb_log_update(void);
void tst_cb_panic(void);
void tst_init(void);

typedef struct {
    bool suppress_logs;
} state_tst_t;
static state_tst_t mutable_state_tst; 
static const state_tst_t* state_tst = &mutable_state_tst; 

void tst_cb_log_update(void)
{
    if (state_tst->suppress_logs == true)
        return;

    ctp_retcode_log_t ret = ctp_get_log();
    if (ret.retcode != ctp_retcode_OK)
        printf("PAANICIICICIC");

    const char* lvl = "UNKNOWN";
    switch (ret.log.lvl)
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
    printf("%s | %s::%d  %s\n", lvl, ret.log.func_name, ret.log.line_num, ret.log.message);
    (void)fflush(stdout);
}

__attribute__((noreturn)) void tst_cb_panic(void)
{
    (void)fflush(stdout);
    abort();
}

// tst_user_stub()
// tst_str_equals()...

void tst_init(void)
{
    ctp_init((ctp_args_init_t){.log_update_cb = tst_cb_log_update,
                               .panic_cb = tst_cb_panic});
    mutable_state_tst.suppress_logs = false;
}
