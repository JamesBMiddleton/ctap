#define CTP_LOG_DEFINE
#define CTP_ERRNO_DEFINE

#include "ctp.h"
#include "ctp_gfx.h"

#include "util/ctp_errno.h"
#include "util/ctp_guard.h"
#include "util/ctp_log.h"


// Initialise the ctap runtime.
ctp_result ctp_init(struct ctp_init_args args)
{
    ctp_errno_get();
    CTP_LOG_DEBUG("hello world");
    CTP_GUARD(ctp_gfx_init((struct ctp_gfx_init_args) { .placeholder = 1 }));
    return (args.placeholder) ? (ctp_result){CTP_RESULT_OK} : (ctp_result){CTP_RESULT_ERROR};
}
