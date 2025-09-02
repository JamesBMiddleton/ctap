#include "ctp_errno.h"

static enum ctp_errno ctp_errno = CTP_ERRNO_OK;
static const char *ctp_errno_source = "";

void ctp_errno_set(const enum ctp_errno err)
{
    ctp_errno = err;
}

enum ctp_errno ctp_errno_get(void)
{
    return ctp_errno;
}

void ctp_errno_source_set(const char *source)
{
    ctp_errno_source = source;
}

const char *ctp_errno_source_get(void)
{
    return ctp_errno_source;
}
