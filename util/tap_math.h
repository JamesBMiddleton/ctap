#ifndef TAP_MATH_H
#define TAP_MATH_H

#include <math.h>

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

#endif /* TAP_MATH_H */
