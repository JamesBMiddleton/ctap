#include <assert.h>
#include <string.h>

#include "tap_render.h"

#include "util/tap_arena.h"
#include "util/tap_guard.h"
#include "util/tap_mat.h"
#include "util/tap_math.h"
#include "util/tap_vec.h"

typedef struct {
    TapVec3 points[6];
    TapVec3 normals[6];
} Frustum;

static struct {
    int initialized;
    TapArena arena;
    unsigned int *framebuffer;
    float *zbuffer;
    Frustum frustum;
    struct {
        unsigned int viewport_width;
        unsigned int viewport_height;
        float rad_fovy;
        float z_near;
        float z_far;
        TapVec3 eye;
        TapVec3 center;
        TapVec3 up;
    } metadata;
} state = {0};

TapResult tap_render_init(TapRenderInitOpts opts)
{
    if (state.initialized)
        return (TapResult){0};
    if (opts.rad_fovy < 0 || opts.rad_fovy > TAP_MATH_PI * 2 || opts.z_near < 0.01F || opts.z_far < opts.z_near || opts.viewport_width == 0 ||
        opts.viewport_height == 0)
        TAP_GUARD_BAIL(TAP_ERRNO_INVALID_VALUE);

    state.metadata.viewport_width = opts.viewport_width;
    state.metadata.viewport_height = opts.viewport_height;
    state.metadata.rad_fovy = opts.rad_fovy;
    state.metadata.z_near = opts.z_near;
    state.metadata.z_far = opts.z_far;

    state.arena = tap_arena_create(4096);
    state.framebuffer = tap_arena_alloc(&state.arena, unsigned int, (size_t)state.metadata.viewport_width *(size_t)state.metadata.viewport_height);
    state.zbuffer = tap_arena_alloc(&state.arena, float, (size_t)state.metadata.viewport_width *(size_t)state.metadata.viewport_height);

    state.initialized = 1;
    return (TapResult){0};
}

// Perspective projection matrix. 
// Right-handed (+z points towards camera), -1,1 NDC range. 
// Maps to openGL's gluPerspective(), GLM's perspectiveRH_NO(). 
static TapMat4 projection_matrix_get(const float rad_fovy, const float aspect, const float z_near, const float z_far)
{
    assert(rad_fovy > 0 && rad_fovy < TAP_MATH_PI * 2);
    assert(!TAP_MATH_EQUAL_FLOAT(aspect, 0));
    assert(z_near > 0.01F);
    assert(z_far > z_near);
    {
        const float a = tanf(rad_fovy / 2);
        const float b = 1 / (aspect / a);
        const float c = 1 / a;
        const float d = -(z_far + z_near) / (z_far - z_near);
        const float e = -(2 * z_far * z_near) / (z_far - z_near);
        const TapMat4 out = {
            b, 0, 0, 0, 0, c, 0, 0, 0, 0, d, e, 0, 0, -1, 1,
        };
        return out;
    }
}

// Affine transformation which converts our vertex from 'world frame' to 'camera frame'. 
// Camera looks down the negative z axis. 
// Maps to GLM's LookAtRH(), OpenGL's gluLookAt(). 
static TapMat4 view_matrix_get(const TapVec3 eye, const TapVec3 center, const TapVec3 up)
{
    const TapVec3 f = tap_vec3_normalize(tap_vec3_sub(center, eye));
    const TapVec3 s = tap_vec3_normalize(tap_vec3_cross(f, up));
    const TapVec3 u = tap_vec3_cross(s, f);
    const TapMat4 M = {
        s.x, s.y, s.z, -tap_vec3_dot(s, eye), u.x, u.y, u.z, -tap_vec3_dot(u, eye), -f.x, -f.y, -f.z, tap_vec3_dot(f, eye), 0, 0, 0, 1,
    };
    return M;
}

static void rasterize(const TapVec4 a_clip, const TapVec4 b_clip, const TapVec4 c_clip, const unsigned int viewport_width, const unsigned int viewport_height,
                      const unsigned int color, unsigned int *framebuffer, float *zbuffer)
{
    assert(framebuffer);
    assert(zbuffer);

    // Perspective divide 
    const TapVec3 a_ndc = tap_vec4_dehomogenize(a_clip);
    const TapVec3 b_ndc = tap_vec4_dehomogenize(b_clip);
    const TapVec3 c_ndc = tap_vec4_dehomogenize(c_clip);

    const float x_scale = (float)viewport_width / 2.0F;
    const float x_translate = (float)viewport_width / 2.0F;
    const float y_scale = (float)viewport_height / 2.0F;
    const float y_translate = (float)viewport_height / 2.0F;
    const TapVec3 a_screen = {(a_ndc.x * x_scale) + x_translate, (a_ndc.y * y_scale) + y_translate, a_ndc.z};
    const TapVec3 b_screen = {(b_ndc.x * x_scale) + x_translate, (b_ndc.y * y_scale) + y_translate, b_ndc.z};
    const TapVec3 c_screen = {(c_ndc.x * x_scale) + x_translate, (c_ndc.y * y_scale) + y_translate, c_ndc.z};

    // The matrix we'll use for barycentric calcs. 
    const TapMat3 ABC = {
        a_screen.x, a_screen.y, 1, b_screen.x, b_screen.y, 1, c_screen.x, c_screen.y, 1,
    };

    // backface culling; determinate gives us the area of the triangle made by points a,b,c. if the area is negative, triangle is back facing. 
    // less than 1 pixel culling; not worth drawing. 
    if (tap_mat3_det(ABC) < 1)
        return;

    // Triangle rasterization using zbuffered bounding box method. 
    {
        const int bbox_max_x = (int)roundf(TAP_MATH_MAX(a_screen.x, TAP_MATH_MAX(b_screen.x, c_screen.x)));
        const int bbox_max_y = (int)roundf(TAP_MATH_MAX(a_screen.y, TAP_MATH_MAX(b_screen.y, c_screen.y)));
        const int bbox_min_x = (int)roundf(TAP_MATH_MIN(a_screen.x, TAP_MATH_MIN(b_screen.x, c_screen.x)));
        const int bbox_min_y = (int)roundf(TAP_MATH_MIN(a_screen.y, TAP_MATH_MIN(b_screen.y, c_screen.y)));

        for (int x = bbox_min_x; x < bbox_max_x; ++x)
        {
            for (int y = bbox_min_y; y < bbox_max_y; ++y)
            {
                const TapVec3 P = {(float)x, (float)y, 1.0F}; // Our pixel coords. 
                const TapVec3 bary = tap_mat3_mulv(tap_mat3_transpose(tap_mat3_invert(ABC)), P); // x=alpha, y=beta, z=gamma 

                // Check if we're in the triangle. 
                if (bary.x < 0 || bary.y < 0 || bary.z < 0)
                    continue;

                // Check whether the pixel is obscured by another we've already drawn with a lower z value. -1 = z_near, +1 = z_far 
                const float z_bias = 0.003F; // Prevent Z-fighting. 
                const float z = (a_screen.z * bary.x) + (b_screen.z * bary.y) + (c_screen.z * bary.z);
                const unsigned int buf_index = (unsigned int)x + ((unsigned int)y * viewport_width);
                if (zbuffer[buf_index] < (z - z_bias))
                    continue;

                zbuffer[buf_index] = z;

                if (bary.x < 0.02F || bary.y < 0.02F || bary.z < 0.02F)
                    framebuffer[buf_index] = TAP_COLOR_BLACK;
                else
                    framebuffer[buf_index] = color;
            }
        }
    }
}

// math from Lighthouse 3D frustum culling tutorial 
static void frustum_create(Frustum *frustum, const float rad_fovy, const float ratio, const float z_near, const float z_far, const TapVec3 eye,
                           const TapVec3 center, const TapVec3 up)
{
    enum {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        NEARP,
        FARP
    };

    const float tang = tanf(rad_fovy * 0.5F);
    const float nh = z_near * tang;
    const float nw = nh * ratio;
    const TapVec3 Z = tap_vec3_normalize(tap_vec3_sub(eye, center));
    const TapVec3 X = tap_vec3_normalize(tap_vec3_mul(up, Z));
    const TapVec3 Y = tap_vec3_mul(Z, X);
    const TapVec3 nc = tap_vec3_sub(eye, tap_vec3_scale(Z, z_near));
    const TapVec3 fc = tap_vec3_sub(eye, tap_vec3_scale(Z, z_far));
    TapVec3 aux, normal;

    frustum->normals[NEARP] = tap_vec3_negate(Z);
    frustum->points[NEARP] = nc;

    frustum->normals[FARP] = Z;
    frustum->points[FARP] = fc;

    aux = tap_vec3_normalize(tap_vec3_sub(tap_vec3_add(nc, tap_vec3_scale(Y, nh)), eye));
    normal = tap_vec3_mul(aux, X);
    frustum->normals[TOP] = normal;
    frustum->points[TOP] = tap_vec3_add(nc, tap_vec3_scale(Y, nh));

    aux = tap_vec3_normalize(tap_vec3_sub(tap_vec3_sub(nc, tap_vec3_scale(Y, nh)), eye));
    normal = tap_vec3_mul(X, aux);
    frustum->normals[BOTTOM] = normal;
    frustum->points[BOTTOM] = tap_vec3_sub(nc, tap_vec3_scale(Y, nh));

    aux = tap_vec3_normalize(tap_vec3_sub(tap_vec3_sub(nc, tap_vec3_scale(X, nw)), eye));
    normal = tap_vec3_mul(aux, Y);
    frustum->normals[LEFT] = normal;
    frustum->points[LEFT] = tap_vec3_sub(nc, tap_vec3_scale(X, nw));

    aux = tap_vec3_normalize(tap_vec3_sub(tap_vec3_add(nc, tap_vec3_scale(X, nw)), eye));
    normal = tap_vec3_mul(Y, aux);
    frustum->normals[RIGHT] = normal;
    frustum->points[RIGHT] = tap_vec3_add(nc, tap_vec3_scale(X, nw));

    // convert to world frame 
    {
        const TapMat4 world_mat = tap_mat4_invert(view_matrix_get(eye, center, up));
        size_t i;
        for (i = 0; i < TAP_ARRAY_SIZE(frustum->normals); ++i)
        {
            frustum->normals[i] = tap_vec4_truncate(tap_mat4_mulv(world_mat, tap_vec3_homogenize(frustum->normals[i])));
            frustum->points[i] = tap_vec4_truncate(tap_mat4_mulv(world_mat, tap_vec3_homogenize(frustum->points[i])));
        }
    }
}

TapResult tap_render_frustum_check_aabb(bool *is_outside, const TapVec3 bbox_min, const TapVec3 bbox_max)
{
    if (!state.initialized)
        TAP_GUARD_BAIL(TAP_ERRNO_MODULE_UNINITIALIZED);
    if (!is_outside)
        TAP_GUARD_BAIL(TAP_ERRNO_NULLPTR);

    for (size_t i = 0; i < TAP_ARRAY_SIZE(state.frustum.points); ++i)
    {
        const TapVec3 pos = {
            (state.frustum.normals[i].x >= 0) ? bbox_max.x : bbox_min.x,
            (state.frustum.normals[i].y >= 0) ? bbox_max.y : bbox_min.y,
            (state.frustum.normals[i].z >= 0) ? bbox_max.z : bbox_min.z,
        };
        const TapVec3 neg = {
            (state.frustum.normals[i].x >= 0) ? bbox_min.x : bbox_max.x,
            (state.frustum.normals[i].y >= 0) ? bbox_min.y : bbox_max.y,
            (state.frustum.normals[i].z >= 0) ? bbox_min.z : bbox_max.z,
        };

        if ((tap_vec3_signed_distance_to_plane(state.frustum.points[i], state.frustum.normals[i], pos) < 0) &&
            (tap_vec3_signed_distance_to_plane(state.frustum.points[i], state.frustum.normals[i], neg) < 0))
        {
            *is_outside = false;
            return (TapResult){0};
        }
    }
    *is_outside = true;
    return (TapResult){0};
}

TapResult tap_render_frame_setup(TapVec3 eye, TapVec3 center, TapVec3 up)
{

    if (!state.initialized)
        TAP_GUARD_BAIL(TAP_ERRNO_MODULE_UNINITIALIZED);

    // Initialize all pixels in the frame to black. 
    memset(state.framebuffer, 0x00, (size_t)state.metadata.viewport_width * (size_t)state.metadata.viewport_height * sizeof(unsigned int));

    // z_far=1 in NDC, so we need to start our z buffer beyond that. 
    for (unsigned int i = 0; i < (state.metadata.viewport_width * state.metadata.viewport_height); ++i)
        state.zbuffer[i] = 2;

    state.metadata.eye = eye;
    state.metadata.center = center;
    state.metadata.up = up;

    const float ratio = (float)state.metadata.viewport_width / (float)state.metadata.viewport_height;
    frustum_create(&state.frustum, state.metadata.rad_fovy, ratio, state.metadata.z_near, state.metadata.z_far, state.metadata.eye, state.metadata.center,
                   state.metadata.up);

    return (TapResult){0};
}

TapResult tap_render_frame_draw(const TapFace *faces, size_t num_faces)
{
    const float ratio = (float)state.metadata.viewport_width / (float)state.metadata.viewport_height;
    const TapMat4 projection_mat = projection_matrix_get(state.metadata.rad_fovy, ratio, state.metadata.z_near, state.metadata.z_far);
    const TapMat4 eye_view_mat = view_matrix_get(state.metadata.eye, state.metadata.center, state.metadata.up);

    if (!faces)
        TAP_GUARD_BAIL(TAP_ERRNO_NULLPTR);
    if (!state.initialized)
        TAP_GUARD_BAIL(TAP_ERRNO_MODULE_UNINITIALIZED);

    for (size_t i = 0; i < num_faces; ++i)
    {
        const TapVec4 a_world = tap_vec3_homogenize(faces[i].vertices[0]);
        const TapVec4 b_world = tap_vec3_homogenize(faces[i].vertices[1]);
        const TapVec4 c_world = tap_vec3_homogenize(faces[i].vertices[2]);

        const TapVec4 a_eye = tap_mat4_mulv(eye_view_mat, a_world);
        const TapVec4 b_eye = tap_mat4_mulv(eye_view_mat, b_world);
        const TapVec4 c_eye = tap_mat4_mulv(eye_view_mat, c_world);

        const TapVec4 a_clip = tap_mat4_mulv(projection_mat, a_eye);
        const TapVec4 b_clip = tap_mat4_mulv(projection_mat, b_eye);
        const TapVec4 c_clip = tap_mat4_mulv(projection_mat, c_eye);

        // Currently just rejecting anything partially outside clip space, need to clip them (create new faces) 
        if (-a_clip.w <= a_clip.x && a_clip.x <= a_clip.w && -a_clip.w <= a_clip.y && a_clip.y <= a_clip.w && -a_clip.w <= a_clip.z && a_clip.z <= a_clip.w &&
            -b_clip.w <= b_clip.x && b_clip.x <= b_clip.w && -b_clip.w <= b_clip.y && b_clip.y <= b_clip.w && -b_clip.w <= b_clip.z && b_clip.z <= b_clip.w &&
            -c_clip.w <= c_clip.x && c_clip.x <= c_clip.w && -c_clip.w <= c_clip.y && c_clip.y <= c_clip.w && -c_clip.w <= c_clip.z && c_clip.z <= c_clip.w)
            rasterize(a_clip, b_clip, c_clip, state.metadata.viewport_width, state.metadata.viewport_height, faces[i].color, state.framebuffer, state.zbuffer);
    }
    return (TapResult){0};
}

TapResult tap_render_frame_get(const unsigned int **framebuffer)
{
    if (!state.initialized)
        TAP_GUARD_BAIL(TAP_ERRNO_MODULE_UNINITIALIZED);

    *framebuffer = state.framebuffer;

    return (TapResult){0};
}

TapResult tap_render_deinit(void)
{
    tap_arena_destroy(&state.arena);
    state.initialized = 0;
    return (TapResult){0};
}
