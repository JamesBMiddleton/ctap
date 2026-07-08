#ifndef TAP_VEC_H
#define TAP_VEC_H

#include "tap_math.h"

typedef struct { float x; float y; } TapVec2;
typedef struct { float x; float y; float z; } TapVec3;
typedef struct { float x; float y; float z; float w; } TapVec4;
typedef struct { int x; int y; } TapVec2I;
typedef struct { int x; int y; int z; } TapVec3I;
typedef struct { int x; int y; int z; int w; } TapVec4I;

static float tap_vec2_dot(const TapVec2 left, const TapVec2 right) { return (left.x * right.x) + (left.y * right.y); }
static float tap_vec2_cross(const TapVec2 left, const TapVec2 right) { return (left.x*right.y) - (left.y*right.x); }
static TapVec2 tap_vec2_add(const TapVec2 left, const TapVec2 right) { TapVec2 out = {left.x + right.x, left.y + right.y}; return out; }
static TapVec2 tap_vec2_sub(const TapVec2 left, const TapVec2 right) { TapVec2 out = {left.x - right.x, left.y - right.y}; return out; }
static TapVec2 tap_vec2_mul(const TapVec2 left, const TapVec2 right) { TapVec2 out = {left.x * right.x, left.y * right.y}; return out; }
static TapVec2 tap_vec2_div(const TapVec2 left, const TapVec2 right) { TapVec2 out = {left.x / right.x, left.y / right.y}; return out; }
static TapVec2 tap_vec2_scale(const TapVec2 vec, const float s) { TapVec2 out = {vec.x * s, vec.y * s}; return out; }
static TapVec2 tap_vec2_normalize(TapVec2 vec); 
static TapVec3 tap_vec2_homogenize(TapVec2 vec) { TapVec3 out = { vec.x, vec.y, 1}; return out; }
static float tap_vec2_norm(TapVec2 vec) { return tap_math_sqrtf((vec.x * vec.x) + (vec.y * vec.y)); }

static float tap_vec3_dot(const TapVec3 left, const TapVec3 right) { return (left.x * right.x) + (left.y * right.y) + (left.z * right.z); }
static TapVec3 tap_vec3_cross(TapVec3 left, TapVec3 right);
static TapVec3 tap_vec3_add(const TapVec3 left, const TapVec3 right) { TapVec3 out = {left.x + right.x, left.y + right.y, left.z + right.z}; return out; }
static TapVec3 tap_vec3_sub(const TapVec3 left, const TapVec3 right) { TapVec3 out = {left.x - right.x, left.y - right.y, left.z - right.z}; return out; }
static TapVec3 tap_vec3_mul(const TapVec3 left, const TapVec3 right) { TapVec3 out = {left.x * right.x, left.y * right.y, left.z * right.z}; return out; }
static TapVec3 tap_vec3_div(const TapVec3 left, const TapVec3 right) { TapVec3 out = {left.x / right.x, left.y / right.y, left.z / right.z}; return out; }
static TapVec3 tap_vec3_scale(const TapVec3 vec, const float s) { TapVec3 out = {vec.x * s, vec.y * s, vec.z * s}; return out; }
static TapVec3 tap_vec3_negate(TapVec3 vec) { TapVec3 out = {-vec.x, -vec.y, -vec.z}; return out; }
static TapVec3 tap_vec3_normalize(TapVec3 vec); 
static TapVec3 tap_vec3_reflect(TapVec3 vec, TapVec3 normal);
static TapVec4 tap_vec3_homogenize(TapVec3 vec) { TapVec4 out = { vec.x, vec.y, vec.z, 1}; return out; }
static TapVec2 tap_vec3_dehomogenize(TapVec3 vec) { TapVec2 out = { vec.x/vec.z, vec.y/vec.z }; return out; }
static TapVec2 tap_vec3_truncate(TapVec3 vec) { TapVec2 out = { vec.x, vec.y }; return out; }
static float tap_vec3_norm(TapVec3 vec) { return tap_math_sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z)); }
static TapVec3 tap_vec3_rotate(TapVec3 v, TapVec3 axis, float angle);
static float tap_vec3_signed_distance_to_plane(TapVec3 plane_point, TapVec3 plane_normal, TapVec3 point);

static float tap_vec4_dot(const TapVec4 left, const TapVec4 right) { return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w); }
static TapVec4 tap_vec4_add(const TapVec4 left, const TapVec4 right) { TapVec4 out = {left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w}; return out; }
static TapVec4 tap_vec4_sub(const TapVec4 left, const TapVec4 right) { TapVec4 out = {left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w}; return out; }
static TapVec4 tap_vec4_mul(const TapVec4 left, const TapVec4 right) { TapVec4 out = {left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w}; return out; }
static TapVec4 tap_vec4_div(const TapVec4 left, const TapVec4 right) { TapVec4 out = {left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w}; return out; }
static TapVec4 tap_vec4_scale(const TapVec4 vec, const float s) { TapVec4 out = {vec.x * s, vec.y * s, vec.z * s, vec.w * s}; return out; }
static TapVec4 tap_vec4_normalize(TapVec4 vec); 
static TapVec3 tap_vec4_dehomogenize(TapVec4 vec) { TapVec3 out = { vec.x/vec.w, vec.y/vec.w, vec.z/vec.w }; return out; }
static TapVec3 tap_vec4_truncate(TapVec4 vec) { TapVec3 out = { vec.x, vec.y, vec.z }; return out; }
static float tap_vec4_norm(TapVec4 vec) { return tap_math_sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w + vec.w)); }

static TapVec3I tap_vec3i_add(const TapVec3I left, const TapVec3I right) { TapVec3I out = {left.x + right.x, left.y + right.y, left.z + right.z}; return out; }
static TapVec3I tap_vec3i_sub(const TapVec3I left, const TapVec3I right) { TapVec3I out = {left.x - right.x, left.y - right.y, left.z - right.z}; return out; }
static TapVec3I tap_vec3i_scale(const TapVec3I vec, int s) { TapVec3I out = {vec.x * s, vec.y * s, vec.z * s}; return out; }

static TapVec2 tap_vec2_normalize(const TapVec2 vec)
{ 
    TapVec2 out = {0}; 
    const float length = tap_math_sqrtf((vec.x*vec.x) + (vec.y*vec.y));

    if (length > 0.0F)
    {
        const float inv_length = 1.0F/length;
        out = tap_vec2_scale(vec, inv_length);
    } 

    return out; 
}

static TapVec3 tap_vec3_normalize(const TapVec3 vec)
{ 
    TapVec3 out = {0}; 
    const float length = tap_math_sqrtf((vec.x*vec.x) + (vec.y*vec.y) + (vec.z*vec.z));

    if (length > 0.0F)
    {
        const float inv_length = 1.0F/length;
        out = tap_vec3_scale(vec, inv_length);
    } 

    return out; 
}

static TapVec3 tap_vec3_reflect(const TapVec3 vec, const TapVec3 normal)
{    
    TapVec3 out = { 0 };
    const float dot = (vec.x*normal.x) + (vec.y*normal.y) + (vec.z*normal.z);

    out.x = vec.x - (2.0F*normal.x)*dot;
    out.y = vec.y - (2.0F*normal.y)*dot;
    out.z = vec.z - (2.0F*normal.z)*dot;

    return out;
}

static TapVec3 tap_vec3_cross(const TapVec3 left, const TapVec3 right) 
{ 
    const TapVec3 out = { (left.y*right.z) - (left.z*right.y), (left.z*right.x) - (left.x*right.z), (left.x*right.y) - (left.y*right.x) }; 
    return out; 
}

/* https://en.wikipedia.org/wiki/Euler–Rodrigues_formula */
static TapVec3 tap_vec3_rotate(const TapVec3 vec, TapVec3 axis, float angle)
{
    TapVec3 out = {0};

    float length = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
    if (length == 0.0F) length = 1.0F;
    const float inv_length = 1.0F/length;
    axis.x *= inv_length;
    axis.y *= inv_length;
    axis.z *= inv_length;

    angle /= 2.0F;
    const float b = axis.x*sinf(angle);
    const float c = axis.y*sinf(angle);
    const float d = axis.z*sinf(angle);

    TapVec3 w = { b, c, d };
    TapVec3 wv = tap_vec3_cross(w, vec);
    TapVec3 wwv = tap_vec3_cross(w, wv);

    wv = tap_vec3_scale(wv, cosf(angle)*2);
    wwv = tap_vec3_scale(wwv, 2);

    out = vec;
    out = tap_vec3_add(out, wv);
    out = tap_vec3_add(out, wwv);

    return out;
}

static float tap_vec3_signed_distance_to_plane(TapVec3 plane_point, TapVec3 plane_normal, TapVec3 point)
{	
    const TapVec3 n = tap_vec3_normalize(plane_normal);
    return tap_vec3_dot(n, tap_vec3_sub(point, plane_point));
}

static TapVec4 tap_vec4_normalize(const TapVec4 vec)
{ 
    TapVec4 out = {0}; 
    const float length = tap_math_sqrtf((vec.x*vec.x) + (vec.y*vec.y) + (vec.z*vec.z) + (vec.w*vec.w));

    if (length > 0.0F)
    {
        const float inv_length = 1.0F/length;
        out = tap_vec4_scale(vec, inv_length);
    } 
    return out; 
}

#endif /* TAP_VEC_H */
