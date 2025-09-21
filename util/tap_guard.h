#ifndef TAP_GUARD_H
#define TAP_GUARD_H

#include <stddef.h>

#include "tap_errno.h"
#include "tap_result.h"

#define TAP_ERRNO_TOSTRING_(s) #s
#define TAP_ERRNO_TOSTRING(s) TAP_ERRNO_TOSTRING_(s)
#define TAP_ERRNO_LINENUM_AS_STRING TAP_ERRNO_TOSTRING(__LINE__)

#define TAP_GUARD(res)                         \
    do                                         \
    {                                          \
        if (res.retcode != TAP_RESULT_OK)      \
        {                                      \
            result.retcode = TAP_RESULT_ERROR; \
            return res;                        \
        }                                      \
    } while (0)

#define TAP_GUARD_BAIL(error)                                           \
    do                                                                  \
    {                                                                   \
        tap_errno_set((error));                                         \
        tap_errno_source_set(__FILE__ ":" TAP_ERRNO_LINENUM_AS_STRING); \
        result.retcode = TAP_RESULT_ERROR;                              \
        return result;                                                     \
    } while (0)

#endif /* TAP_GUARD_H */
