#ifndef CTP_ERRNO_H
#define CTP_ERRNO_H

enum ctp_errno {
    CTP_ERRNO_OK,
    CTP_ERRNO_NULL,
    CTP_ERRNO_NOMEM,
    CTP_ERRNO_ALARM_ANTENNA_MISSING
};

extern enum ctp_errno ctp_errno_value;
extern const char *ctp_errno_source;

static inline void ctp_errno_set(const enum ctp_errno err) { ctp_errno_value = err; }
static inline enum ctp_errno ctp_errno_get(void) { return ctp_errno_value; }
static inline void ctp_errno_source_set(const char *source) { ctp_errno_source = source; }
static inline const char *ctp_errno_source_get(void) { return ctp_errno_source; }

#endif // CTP_ERRNO_H
