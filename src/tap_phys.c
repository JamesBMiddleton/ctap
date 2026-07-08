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
    state.pose.pitch +=
        (state.pose.pitch + in_movedeltas->pitch > TAP_MATH_PI / 2 || state.pose.pitch + in_movedeltas->pitch < -(TAP_MATH_PI / 2)) ? 0 : in_movedeltas->pitch;

    const TapVec3 eye_direction = tap_vec3_normalize((TapVec3){-in_movedeltas->side, in_movedeltas->up, -in_movedeltas->forward});
    const TapVec3 up = {0, 1, 0};
    const TapVec3 eye = {0, 0, 0}; 
    const TapVec3 look = {cosf(state.pose.pitch) * sinf(state.pose.yaw), sinf(state.pose.pitch), cosf(state.pose.pitch) * cosf(state.pose.yaw)};
    const TapVec3 center = {eye.x + look.x, eye.y + look.y, eye.z + look.z};
    const TapMat4 lookat = tap_math_view_matrix_get(eye, center, up);

    const TapVec3 world_direction = tap_vec4_truncate(tap_mat4_mulv(tap_mat4_invert(lookat), tap_vec3_homogenize(eye_direction)));

    state.pose.position = tap_vec3_add(state.pose.position, tap_vec3_scale(world_direction, 0.3F));

    *out_pose = state.pose;
    return (TapResult){0};
}

TapResult tap_phys_deinit(void)
{
    return (TapResult){0};
}
