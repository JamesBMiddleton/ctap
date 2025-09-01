#ifndef CTP_GUARD_H
#define CTP_GUARD_H

#include <stddef.h> 

#include "ctp_result.h"
#include "ctp_errno.h"
#include "ctp_str.h"

#define CTP_GUARD(result) do { if (!ctp_result_is_ok((result))) { return (ctp_result){CTP_RESULT_ERROR}; }} while (0)
#define CTP_GUARD_BAIL(error)  do { \
    ctp_errno_set((error)); \
    ctp_errno_info_set((struct ctp_errno_info){.source = __FILE__ ":" CTP_STR_FROM(__LINE__))); \
    return (ctp_result){CTP_RESULT_ERROR}; } while (0)
#define CTP_GUARD_IF(condition, error) do { if (!(condition)) { CTP_GUARD_BAIL((error)); }} while (0)
#define CTP_GUARD_PTR(ptr) do { if ((ptr) == NULL) { CTP_GUARD_BAIL(CTP_ERRNO_NULL); }} while (0)

#endif // CTP_GUARD_H
