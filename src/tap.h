#ifndef TAP_H
#define TAP_H

#include "util/tap_result.h"

typedef struct {
    int placeholder;
    // map..
    // input_cb..
} TapInitOpt;

TapResult tap_init(TapInitOpt opt);

#endif // TAP_H
