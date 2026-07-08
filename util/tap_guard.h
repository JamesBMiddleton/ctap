#ifndef TAP_GUARD_H
#define TAP_GUARD_H

#include "tap_def.h"
#include "tap_errno.h"
#include "tap_result.h"

#define TAP_GUARD(res)                         \
    do                                         \
    {                                          \
        if (res.retcode != TAP_RESULT_OK)      \
        {                                      \
            result.retcode = TAP_RESULT_ERROR; \
            return result;                     \
        }                                      \
    } while (0)

#define TAP_GUARD_(res, cleanup)               \
    do                                         \
    {                                          \
        if (res.retcode != TAP_RESULT_OK)      \
        {                                      \
            cleanup;                           \
            result.retcode = TAP_RESULT_ERROR; \
            return result;                     \
        }                                      \
    } while (0)

#define TAP_GUARD_BAIL(error)                                          \
    do                                                                 \
    {                                                                  \
        tap_errno_set((error));                                        \
        tap_errno_source_set(__FILE__ ":" TAP_DEF_TOSTRING(__LINE__)); \
        result.retcode = TAP_RESULT_ERROR;                             \
        return result;                                                 \
    } while (0)

#define TAP_GUARD_BAIL_(error, cleanup)                                \
    do                                                                 \
    {                                                                  \
        cleanup;                                                       \
        tap_errno_set((error));                                        \
        tap_errno_source_set(__FILE__ ":" TAP_DEF_TOSTRING(__LINE__)); \
        result.retcode = TAP_RESULT_ERROR;                             \
        return result;                                                 \
    } while (0)

#endif /* TAP_GUARD_H */
