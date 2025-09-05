#ifndef CTP_ERRNO_H
#define CTP_ERRNO_H

enum ctp_errno {
    CTP_ERRNO_OK,
    CTP_ERRNO_NULL,
    CTP_ERRNO_NOMEM,
    CTP_ERRNO_WARN_FRAME_SKIPPED
};

void ctp_errno_set(enum ctp_errno err);
enum ctp_errno ctp_errno_get(void);
void ctp_errno_source_set(const char *source);
const char *ctp_errno_source_get(void);

#ifdef CTP_ERRNO_DEFINE
static enum ctp_errno ctp_errno_value;
static const char *ctp_errno_source;
void ctp_errno_set(const enum ctp_errno err) { ctp_errno_value = err; }
enum ctp_errno ctp_errno_get(void) { return ctp_errno_value; }
void ctp_errno_source_set(const char *source) { ctp_errno_source = source; }
const char *ctp_errno_source_get(void) { return ctp_errno_source; }
#endif // CTP_ERRNO_DEFINE

#endif // CTP_ERRNO_H
