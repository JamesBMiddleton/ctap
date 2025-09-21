#ifndef TAP_H
#define TAP_H

#include "util/tap_result.h"

typedef struct {
    unsigned int placeholder;
} TapApiInitOpt;

TapResult tap_api_init(TapApiInitOpt opt);

#endif /* TAP_H */
