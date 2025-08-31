#ifndef CTP_GUARD_H
#define CTP_GUARD_H

#include <stddef.h> 

#include "ctp_result.h"
#include "ctp_errno.h"

#define CTP_GUARD(result) do { if (!ctp_result_is_ok((result))) { return (ctp_result){CTP_RESULT_ERROR}; }} while (0)
#define CTP_GUARD_BAIL(error)  do { ctp_errno_set((error)); return (ctp_result){CTP_RESULT_ERROR}; } while (0)
#define CTP_GUARD_IF(condition, error) do { if (!(condition)) { CTP_GUARD_BAIL((error)); }} while (0)
#define CTP_GUARD_PTR(ptr) do { if ((ptr) == NULL) { CTP_GUARD_BAIL(CTP_ERRNO_NULL); }} while (0)

#endif // CTP_GUARD_H
