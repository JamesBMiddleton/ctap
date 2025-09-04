#ifndef CTP_MATH_H
#define CTP_MATH_H

#define ctp_math_min(left, right) (((left) < (right)) ? (left) : (right))
#define ctp_math_max(left, right) (((left) > (right)) ? (left) : (right))

#define CTP_MATH_FLT_EQUAL_DELTA 1E-6F
#define ctp_math_flt_equal(left, right)           \
    ((((left) - (right)) > -(FLT_EQUAL_DELTA)) && \
     (((left) - (right)) < (FLT_EQUAL_DELTA)))

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
static inline int ctp_math_powi(int base, unsigned exp)
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
static inline unsigned ctp_math_powu(unsigned base, unsigned exp)
{
    unsigned result = 1;
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
static inline float ctp_math_powf(float base, int exp)
{
    float result = 1;
    if (exp == 0)
        return result;
    unsigned abs_exp = ctp_math_abs(exp);
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
static inline float ctp_math_fabs(float val)
{
    return (val < 0) ? -val : val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially NaN float value.
 * @return 1 if NaN, else 0.
 */
static inline int ctp_math_isnan(const float val)
{
    return val != val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially infinite float value.
 * @return 1 if inifity, else 0.
 */
static inline int ctp_math_isinf(const float val)
{
    return !ctp_math_isnan(val) && ctp_math_isnan(val - val);
}

#endif // CTP_MATH_H
