#ifndef CTP_RESULT_H
#define CTP_RESULT_H

#include <stdbool.h>

#if defined(__clang__) || defined(__GNUC__)
    typedef struct {
        enum { CTP_RESULT_OK = 0, CTP_RESULT_ERROR = -1 } retcode;
    } __attribute__((warn_unused_result)) ctp_result;
#else
    typedef struct {
        enum { CTP_RESULT_OK = 0, CTP_RESULT_ERROR = -1 } retcode;
    } ctp_result;
#endif

static inline void ctp_result_ignore(ctp_result result) { /* noop */ }
static inline bool ctp_result_is_ok(ctp_result result) { return result.retcode == CTP_RESULT_OK; }
static inline bool ctp_result_is_error(ctp_result result) { return result.retcode == CTP_RESULT_ERROR; }

#endif // CTP_RESULT_H
