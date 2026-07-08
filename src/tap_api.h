#ifndef TAP_API_H
#define TAP_API_H

#include "util/tap_log.h"
#include "util/tap_result.h"

typedef unsigned int TapApiFramebuffer;

typedef struct {
    float mouse_dx;
    float mouse_dy;
    char keypress_charcode;
} TapApiInputEvents;

typedef struct {
    unsigned int viewport_width;
    unsigned int viewport_height;
} TapApiInitOpt;

TapResult tap_api_init(TapApiInitOpt opt);
TapResult tap_api_cycle(const TapApiInputEvents *in_events, const TapApiFramebuffer **out_framebuffer);
TapResult tap_api_deinit(void);

#endif /* TAP_API_H */
