#ifndef TAP_RESULT_H
#define TAP_RESULT_H

#if defined(__clang__) || defined(__GNUC__)
    typedef struct {
        enum { TAP_RESULT_OK = 0, TAP_RESULT_ERROR = -1 } retcode;
    } __attribute__((warn_unused_result)) TapResult;
#else
    typedef struct {
        enum { TAP_RESULT_OK = 0, TAP_RESULT_ERROR = -1 } retcode;
    } TapResult;
#endif

/* static void tap_result_ignore(TapResult result) { (void)result; } */

#endif /* TAP_RESULT_H */
