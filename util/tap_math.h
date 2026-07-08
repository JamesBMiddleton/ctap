#ifndef TAP_MATH_H
#define TAP_MATH_H

#include <float.h> /* Temporary! */
#include <math.h> /* Temporary! */

static void tap_math_swap(int *a, int *b);
static void tap_math_swapf(float *a, float *b);
static float tap_math_round(float val);
static unsigned int tap_math_abs(int val);
static int tap_math_powi(int base, unsigned int exp);
static unsigned int tap_math_powu(unsigned int base, unsigned int exp);
static float tap_math_powf(float base, int exp);
static float tap_math_fabs(float val);
static int tap_math_isnan(float val);
static int tap_math_isinf(float val);
static float tap_math_cos(float angle);
static float tap_math_sin(float angle);
static float tap_math_tan(float angle);
static float tap_math_cot(float val);
static float tap_math_sqrtf(float val);
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

static float tap_math_tan(float angle)
{
    return tanf(angle);
}

static float tap_math_cot(const float val)
{
    return 1.0F / tanf(val);
}

static void tap_math_swapf(float *a, float *b)
{
    float tmp = *a;
    *a = *b;
    *b = tmp;
}

static void tap_math_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static float tap_math_round(const float val)
{
    return roundf(val);
}

/*
 * @param val - pos/neg value.
 * @return absolute value.
 */
static unsigned int tap_math_abs(const int val)
{
    return (val < 0) ? -(unsigned int)val : (unsigned int)val;
}

/*
 * @param base - base.
 * @param exp - exponent.
 * @return power.
 */
static int tap_math_powi(int base, unsigned int exp)
{
    int result = 1;
    while (1)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp == 0)
            break;
        base *= base;
    }
    return result;
}

/*
 * @param base - base.
 * @param exp - exponent.
 * @return power.
 */
static unsigned int tap_math_powu(unsigned int base, unsigned int exp)
{
    unsigned int result = 1;
    if (exp == 0)
        return result;
    while (1)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp == 0)
            break;
        base *= base;
    }
    return result;
}

/*
 * Can handle negative powers.
 *
 * @param base - base.
 * @param exp - exponent.
 * @return power.
 */
static float tap_math_powf(float base, int exp)
{
    float result = 1;
    unsigned int abs_exp = 0;
    if (exp == 0)
        return result;
    abs_exp = tap_math_abs(exp);
    while (1)
    {
        if (abs_exp & 1U)
            result *= base;
        abs_exp >>= 1U;
        if (abs_exp == 0)
            break;
        base *= base;
    }
    return (exp > 0) ? result : 1.0F / result;
}

/*
 * @param val - pos/neg value.
 * @return absolute value.
 */
static float tap_math_fabs(float val)
{
    return (val < 0) ? -val : val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially NaN float value.
 * @return 1 if NaN, else 0.
 */
static int tap_math_isnan(const float val)
{
    return val != val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially infinite float value.
 * @return 1 if inifity, else 0.
 */
static int tap_math_isinf(const float val)
{
    return !tap_math_isnan(val) && tap_math_isnan(val - val);
}

static float tap_math_cos(const float angle)
{
    return cosf(angle);
}

static float tap_math_sin(const float angle)
{
    return sinf(angle);
}

static float tap_math_sqrtf(float val)
{
    return sqrtf(val);
}

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
