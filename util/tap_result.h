#ifndef TAP_RESULT_H
#define TAP_RESULT_H

#include <stdbool.h>

#if defined(__clang__) || defined(__GNUC__)
    typedef struct {
        enum { TAP_RESULT_OK = 0, TAP_RESULT_ERROR = -1 } retcode;
    } __attribute__((warn_unused_result)) TapResult;
#else
    typedef struct {
        enum { TAP_RESULT_OK = 0, TAP_RESULT_ERROR = -1 } retcode;
    } TapResult;
#endif

static inline void tap_result_ignore(TapResult result) { /* noop */ }
static inline bool tap_result_is_ok(TapResult result) { return result.retcode == TAP_RESULT_OK; }
static inline bool tap_result_is_error(TapResult result) { return result.retcode == TAP_RESULT_ERROR; }

#endif // TAP_RESULT_H
