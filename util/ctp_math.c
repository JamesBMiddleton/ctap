#include "ctp_math.h"

/*
 * @param val - pos/neg value.
 * @return absolute value.
 */
static inline unsigned ctp_math_abs(const int val)
{
    return (val < 0) ? -(unsigned)val : (unsigned)val;
}

/*
 * @param base - base.
 * @param exp - exponent.
 * @return power.
 */
static inline int util_math_powi(int base, unsigned exp)
{
    int result = 1;
    while (true)
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
static inline unsigned ctp_math_powu(unsigned base, unsigned exp)
{
    unsigned result = 1;
    if (exp == 0)
        return result;
    while (true)
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
static inline float ctp_math_powf(float base, int exp)
{
    float result = 1;
    if (exp == 0)
        return result;
    unsigned abs_exp = ctp_math_abs(exp);
    while (true)
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
static inline float ctp_math_fabs(float val)
{
    return (val < 0) ? -val : val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially NaN float value.
 * @return val is NaN.
 */
static inline bool ctp_math_isnan(const float val)
{
    return val != val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially infinite float value.
 * @return val is infinity.
 */
static inline bool ctp_math_isinf(const float val)
{
    return !ctp_math_isnan(val) && ctp_math_isnan(val - val);
}

