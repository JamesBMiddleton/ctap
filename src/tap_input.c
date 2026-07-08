#include "tap_input.h"

#define ASCII_SPACE 32

TapResult tap_input_init(void)
{
    return (TapResult){0};
}

TapResult tap_input_cycle(const TapInputEvents *in_events, TapInputIntent *out_intent)
{
    const float m_yaw = 0.022F;
    const float m_pitch = 0.022F;
    const float sens = 0.25F;
    out_intent->yaw_delta = in_events->mouse_dx * m_yaw * sens;
    out_intent->pitch_delta = in_events->mouse_dy * m_pitch * sens;
    switch (in_events->keypress_charcode)
    {
        case 'w': out_intent->forward_delta = 1.0F; break;
        case 'a': out_intent->side_delta = 1.0F; break;
        case 's': out_intent->forward_delta = -1.0F; break;
        case 'd': out_intent->side_delta = -1.0F; break;
        case 'c': out_intent->up_delta = -1.0F; break;
        case ASCII_SPACE: out_intent->up_delta = 1.0F; break;
        default: break;
    }
    return (TapResult){0};
}

TapResult tap_input_deinit(void)
{
    return (TapResult){0};
}
