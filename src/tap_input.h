#ifndef TAP_INPUT_H
#define TAP_INPUT_H

#include "util/tap_def.h"
#include "util/tap_result.h"
#include "util/tap_vec.h"

typedef struct {
    float mouse_dx;
    float mouse_dy;
    char keypress_charcode;
} TapInputEvents;

typedef struct {
    float yaw_delta;
    float pitch_delta;
    float forward_delta;
    float side_delta;
    float up_delta;
    TapBool action1placeholder; 
    TapBool push2talkplaceholder; /* aallll the intent */ 
} TapInputIntent;

TapResult tap_input_init(void);
TapResult tap_input_cycle(const TapInputEvents *in_events, TapInputIntent *out_intent);
TapResult tap_input_deinit(void);

#endif /* TAP_INPUT_H */
