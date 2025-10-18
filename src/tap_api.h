#ifndef TAP_API_H
#define TAP_API_H

#include "util/tap_result.h"
#include "util/tap_alloc.h"
#include "util/tap_log.h"

typedef struct {
    TapAllocAllocator allocator;
    TapLogLogger logger;
    unsigned int placeholder;
} TapApiInitOpt;

TapResult tap_api_init(TapApiInitOpt opt);

#endif /* TAP_API_H */
