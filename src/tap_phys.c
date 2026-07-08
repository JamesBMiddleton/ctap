#include "tap_phys.h"
#include "util/tap_math.h"

static struct {
    TapPhysPlayerPose pose;
} state = {0};

TapResult tap_phys_init(void)
{
    return (TapResult){0};
}

TapResult tap_phys_cycle(const TapPhysPlayerMoveDeltas *in_movedeltas, TapPhysPlayerPose *out_pose)
{
    state.pose.yaw += in_movedeltas->yaw;
    state.pose.pitch += (state.pose.pitch + in_movedeltas->pitch > TAP_MATH_PI/2 || state.pose.pitch + in_movedeltas->pitch < -(TAP_MATH_PI/2)) ? 0 : in_movedeltas->pitch;
    state.pose.position.x += in_movedeltas->side;
    state.pose.position.y += in_movedeltas->up;
    state.pose.position.z += in_movedeltas->forward;

    *out_pose = state.pose;
    return (TapResult){0};
}

TapResult tap_phys_deinit(void)
{
    return (TapResult){0};
}
