#include "ctp_errno.h"

static enum ctp_errno ctp_errno = CTP_ERRNO_OK;
static struct ctp_errno_info ctp_debug_info = { .strerror = "", .source = "" };

inline void ctp_errno_set(const enum ctp_errno err)
{
    ctp_errno = err;
}

inline enum ctp_errno ctp_errno_get(void)
{
    return ctp_errno;
}

inline void ctp_errno_info_set(const struct ctp_errno_info info)
{
    ctp_debug_info = info;
}

inline struct ctp_errno_info ctp_errno_info_get(void)
{
    return ctp_debug_info;
}
