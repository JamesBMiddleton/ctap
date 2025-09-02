#ifndef CTP_ERRNO_H
#define CTP_ERRNO_H

enum ctp_errno {
    CTP_ERRNO_OK,
    CTP_ERRNO_NULL,
    CTP_ERRNO_NOMEM,
    CTP_ERRNO_ALARM_ANTENNA_MISSING
};

void ctp_errno_set(enum ctp_errno err);
enum ctp_errno ctp_errno_get(void);
void ctp_errno_source_set(const char *source);
const char *ctp_errno_source_get(void);

#endif // CTP_ERRNO_H
