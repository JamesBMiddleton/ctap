#include "tap_api.h"
#include "tap_chunk.h"
#include "tap_render.h"
#include "tap_input.h"
#include "tap_phys.h"

#include "util/tap_guard.h"
#include "util/tap_math.h"
#include <time.h>
#include <stdbool.h>

static struct {
    int initialized;
} state = {0};

TapResult tap_api_init(const TapApiInitOpt opt)
{
    TapResult result = {0};
    TapRenderInitOpts render_opts = {0};

    if (state.initialized)
        return result;

    TAP_GUARD(tap_chunk_init());

    render_opts.viewport_width = opt.viewport_width;
    render_opts.viewport_height = opt.viewport_height;
    render_opts.rad_fovy = 90 * TAP_MATH_ANG2RAD;
    render_opts.z_near = 1.0F;
    render_opts.z_far = 100.0F;
    TAP_GUARD(tap_render_init(render_opts));

    TAP_GUARD(tap_input_init());
    TAP_GUARD(tap_phys_init());

    state.initialized = 1;
    return result;
}

TapResult tap_api_cycle(const TapApiInputEvents *in_events, const TapApiFramebuffer **out_framebuffer)
{
    TapResult result = {0};
    clock_t start, end;
    start = clock();

    if (!in_events || !out_framebuffer)
        TAP_GUARD_BAIL(TAP_ERRNO_NULLPTR);
    if (!state.initialized)
        TAP_GUARD_BAIL(TAP_ERRNO_MODULE_UNINITIALIZED);

    const TapVec3 temp_chunk_center = {-20,-10,30};

    const TapChunkMesh *meshes = NULL;
    size_t num_chunks = 0;
    TAP_GUARD(tap_chunk_get_meshes(temp_chunk_center, &meshes, &num_chunks));

    TapInputEvents events = {0};
    TapInputIntent intent = {0};
    events.mouse_dx = in_events->mouse_dx; /* this won't be as redundant when per-frame inputs include updates to viewport size etc... */
    events.mouse_dy = in_events->mouse_dy;
    events.keypress_charcode = in_events->keypress_charcode;
    TAP_GUARD(tap_input_cycle(&events, &intent));

    TapPhysPlayerMoveDeltas move_deltas;
    TapPhysPlayerPose pose;
    move_deltas.yaw = intent.yaw_delta; /* this won't be as redundant when 'intent' includes stuff going all over the place, like pressing esc to exit, equip slot1 etc. */
    move_deltas.pitch = intent.pitch_delta;
    move_deltas.forward = intent.forward_delta;
    move_deltas.side = intent.side_delta;
    move_deltas.up = intent.up_delta;
    TAP_GUARD(tap_phys_cycle(&move_deltas, &pose));

    const TapVec3 up = {0, 1, 0};
    const TapVec3 eye = pose.position;
    TapVec3 look;
    TapVec3 center; 
    look.x = cosf(pose.pitch) * sinf(pose.yaw);
    look.y = sinf(pose.pitch);
    look.z = cosf(pose.pitch) * cosf(pose.yaw);
    center.x = eye.x + look.x;
    center.y = eye.y + look.y;
    center.z = eye.z + look.z;
    TAP_GUARD(tap_render_frame_setup(eye, center, up));
    for (size_t i = 0; i < num_chunks; ++i)
    {
        /* const TapVec3 chunk_size = {CHUNK_SZ, CHUNK_SZ, CHUNK_SZ}; */
        bool outside_frustum = false;
        /* TAP_GUARD(tap_render_frustum_check_aabb(&outside_frustum, meshes[i].world_coords, tap_vec3_add(meshes[i].world_coords, chunk_size))); */
        if (outside_frustum == false)
            TAP_GUARD(tap_render_frame_draw(meshes[i].faces, meshes[i].num_faces));
    }
    TAP_GUARD(tap_render_frame_get(out_framebuffer));

    end = clock();
    TAP_LOG("fps: %f", 1 / ((double)(end-start) / CLOCKS_PER_SEC));

    TAP_LOG("dx: %f, dy: %f\n", (double)in_events->mouse_dx, (double)in_events->mouse_dy);
    TAP_LOG("keypress: %c\n", in_events->keypress_charcode);
    TAP_LOG("player position x: %f, y:%f, z:%f \n", (double)pose.position.x, (double)pose.position.y, (double)pose.position.z);
    TAP_LOG("player pitch: %f, yaw: %f\n", (double)pose.pitch, (double)pose.yaw);

    return result;
}

TapResult tap_api_deinit(void)
{
    TapResult result = {0};
    TAP_GUARD(tap_render_deinit());
    TAP_GUARD(tap_chunk_deinit());
    TAP_GUARD(tap_input_deinit());
    TAP_GUARD(tap_phys_deinit());
    state.initialized = 0;
    return result;
}
