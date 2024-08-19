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

void tst_logger_cb(ctp_log_t log);
void tst_abort_cb(void);
void tst_setup(void);

void tst_logger_cb(ctp_log_t log)
{
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
    printf("%s | %s::%d  %s\n", lvl, log.func_name, log.line_num, log.message);
    (void)fflush(stdout);
}

__attribute__((noreturn)) void tst_abort_cb(void)
{
    (void)fflush(stdout);
    abort();
}

// tst_user_stub()
// tst_str_equals()...

void tst_setup(void)
{
    ctp_log_cb = tst_logger_cb;
    ctp_panic_cb = tst_abort_cb;
}

