#include "ctp_errno.h"

static enum ctp_errno ctp_errno = CTP_ERRNO_OK;

inline void ctp_errno_set(enum ctp_errno err)
{
    ctp_errno = err;
}

inline enum ctp_errno ctp_errno_get(void)
{
    return ctp_errno;
}
