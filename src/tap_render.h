#ifndef TAP_RENDER_H
#define TAP_RENDER_H

#include "util/tap_result.h"

typedef struct {
    unsigned placeholder;
} TapRenderInitOpt;

TapResult tap_render_init(TapRenderInitOpt opt);

#endif // TAP_RENDER_H
