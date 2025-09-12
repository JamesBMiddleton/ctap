#ifndef TAP_GUARD_H
#define TAP_GUARD_H

#include <stddef.h> 

#include "tap_result.h"
#include "tap_errno.h"

#define TAP_ERRNO_TOSTRING_(s) #s
#define TAP_ERRNO_TOSTRING(s) TAP_ERRNO_TOSTRING_(s)
#define TAP_ERRNO_LINENUM_AS_STRING TAP_ERRNO_TOSTRING(__LINE__)

#define TAP_GUARD(result) do { if (!tap_result_is_ok((result))) { return (TapResult){TAP_RESULT_ERROR}; }} while (0)
#define TAP_GUARD_BAIL(error)  do { \
    tap_errno_set((error)); \
    tap_errno_source_set(__FILE__ ":" TAP_ERRNO_LINENUM_AS_STRING ); \
    return (TapResult){TAP_RESULT_ERROR}; } while (0)
#define TAP_GUARD_IF(condition, error) do { if (!(condition)) { TAP_GUARD_BAIL((error)); }} while (0)
#define TAP_GUARD_PTR(ptr) do { if ((ptr) == NULL) { TAP_GUARD_BAIL(TAP_ERRNO_NULL); }} while (0)

#endif // TAP_GUARD_H
