#ifndef TAP_MATH_H
#define TAP_MATH_H

#include <math.h>
#include <assert.h>
#include "tap_mat.h"

static float tap_math_normalize(float val, float start, float end);
static float tap_math_triangle_area(float ax, float ay, float bx, float by, float cx, float cy);

typedef float TapMathRadian;

#define TAP_MATH_FLOAT_MAX FLT_MAX
#define TAP_MATH_PI 3.14159F
#ifndef TAP_MATH_EQUAL_FLOAT_DELTA
    #define TAP_MATH_EQUAL_FLOAT_DELTA 1E-6F
#endif
#define TAP_MATH_MIN(left, right) (((left) < (right)) ? (left) : (right))
#define TAP_MATH_MAX(left, right) (((left) > (right)) ? (left) : (right))
#define TAP_MATH_EQUAL_FLOAT(left, right) ((((left) - (right)) > -(TAP_MATH_EQUAL_FLOAT_DELTA)) && (((left) - (right)) < (TAP_MATH_EQUAL_FLOAT_DELTA)))
#define TAP_MATH_ANG2RAD (3.14159265F/180.0F)

/* Normalize 0-1. */
static float tap_math_normalize(float val, float start, float end)
{
    return (val - start)/(end - start);
}

static float tap_math_triangle_area(const float ax, const float ay, const float bx, const float by, const float cx, const float cy)
{
    return .5F * ((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

// Perspective projection matrix. 
// Right-handed (+z points towards camera), -1,1 NDC range. 
// Maps to openGL's gluPerspective(), GLM's perspectiveRH_NO(). 
static TapMat4 tap_math_projection_matrix_get(const float rad_fovy, const float aspect, const float z_near, const float z_far)
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
static TapMat4 tap_math_view_matrix_get(const TapVec3 eye, const TapVec3 center, const TapVec3 up)
{
    const TapVec3 f = tap_vec3_normalize(tap_vec3_sub(center, eye));
    const TapVec3 s = tap_vec3_normalize(tap_vec3_cross(f, up));
    const TapVec3 u = tap_vec3_cross(s, f);
    const TapMat4 M = {
        s.x, s.y, s.z, -tap_vec3_dot(s, eye), u.x, u.y, u.z, -tap_vec3_dot(u, eye), -f.x, -f.y, -f.z, tap_vec3_dot(f, eye), 0, 0, 0, 1,
    };
    return M;
}


#endif /* TAP_MATH_H */
