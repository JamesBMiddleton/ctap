#include "tap_api.h"
#include "tap_chunk.h"
#include "tap_input.h"
#include "tap_phys.h"
#include "tap_render.h"

#include "util/tap_guard.h"
#include "util/tap_math.h"
#include <stdbool.h>
#include <time.h>

static struct {
    int initialized;
} state = {0};

TapResult tap_api_init(const TapApiInitOpt opt)
{
    if (state.initialized)
        return (TapResult){0};

    TAP_GUARD(tap_chunk_init());

    TAP_GUARD(tap_render_init((TapRenderInitOpts){
        .viewport_width = opt.viewport_width, .viewport_height = opt.viewport_height, .rad_fovy = 90 * TAP_MATH_ANG2RAD, .z_near = 1.0F, .z_far = 100.0F}));

    TAP_GUARD(tap_input_init());
    TAP_GUARD(tap_phys_init());

    state.initialized = 1;
    return (TapResult){0};
}

TapResult tap_api_cycle(const TapApiInputEvents *in_events, const TapApiFramebuffer **out_framebuffer)
{
    const clock_t start = clock();

    if (!in_events || !out_framebuffer)
        TAP_GUARD_BAIL(TAP_ERRNO_NULLPTR);
    if (!state.initialized)
        TAP_GUARD_BAIL(TAP_ERRNO_MODULE_UNINITIALIZED);

    const TapChunkMesh *meshes = NULL;
    size_t num_chunks = 0;
    TAP_GUARD(tap_chunk_get_meshes((TapVec3){-20, -10, 30}, &meshes, &num_chunks));

    // this won't be as redundant when per-frame inputs include updates to viewport size etc... 
    const TapInputEvents events = {.mouse_dx = in_events->mouse_dx, .mouse_dy = in_events->mouse_dy, .keypress_charcode = in_events->keypress_charcode};
    TapInputIntent intent = {0};
    TAP_GUARD(tap_input_cycle(&events, &intent));

    // this won't be as redundant when 'intent' includes stuff going all over the place, like pressing esc to exit, equip slot1 etc. 
    const TapPhysPlayerMoveDeltas move_deltas = {
        .yaw = intent.yaw_delta, .pitch = intent.pitch_delta, .forward = intent.forward_delta, .side = intent.side_delta, .up = intent.up_delta};
    TapPhysPlayerPose pose;
    TAP_GUARD(tap_phys_cycle(&move_deltas, &pose));

    const TapVec3 up = {0, 1, 0};
    const TapVec3 eye = pose.position;
    const TapVec3 look = {cosf(pose.pitch) * sinf(pose.yaw), sinf(pose.pitch), cosf(pose.pitch) * cosf(pose.yaw)};
    const TapVec3 center = {eye.x + look.x, eye.y + look.y, eye.z + look.z};
    TAP_GUARD(tap_render_frame_setup(eye, center, up));
    for (size_t i = 0; i < num_chunks; ++i)
    {
        // const TapVec3 chunk_size = {CHUNK_SZ, CHUNK_SZ, CHUNK_SZ}; 
        bool outside_frustum = false;
        // TAP_GUARD(tap_render_frustum_check_aabb(&outside_frustum, meshes[i].world_coords, tap_vec3_add(meshes[i].world_coords, chunk_size))); 
        if (outside_frustum == false)
            TAP_GUARD(tap_render_frame_draw(meshes[i].faces, meshes[i].num_faces));
    }
    TAP_GUARD(tap_render_frame_get(out_framebuffer));

    const clock_t end = clock();
    TAP_LOG("fps: %f", 1 / ((double)(end - start) / CLOCKS_PER_SEC));

    // TAP_LOG("dx: %f, dy: %f\n", (double)in_events->mouse_dx, (double)in_events->mouse_dy);
    // TAP_LOG("keypress: %c\n", in_events->keypress_charcode);
    // TAP_LOG("player position x: %f, y:%f, z:%f \n", (double)pose.position.x, (double)pose.position.y, (double)pose.position.z);
    // TAP_LOG("player pitch: %f, yaw: %f\n", (double)pose.pitch, (double)pose.yaw);

    return (TapResult){0};
}

TapResult tap_api_deinit(void)
{
    TAP_GUARD(tap_render_deinit());
    TAP_GUARD(tap_chunk_deinit());
    TAP_GUARD(tap_input_deinit());
    TAP_GUARD(tap_phys_deinit());
    state.initialized = 0;
    return (TapResult){0};
}
