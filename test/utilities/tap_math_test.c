#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>

#include "util/tap_math.h"

static void tap_math_powi_test(void)
{
    assert(tap_math_powi(0, 0) == 1);
    assert(tap_math_powi(0, 1) == 0);

    assert(tap_math_powi(1, 0) == 1);
    assert(tap_math_powi(1, 1) == 1);
    assert(tap_math_powi(1, 5) == 1);
    assert(tap_math_powi(2, 5) == 32);
    assert(tap_math_powi(5, 2) == 25);
    assert(tap_math_powi(14, 8) == 1475789056);

    assert(tap_math_powi(-1, 0) == 1);
    assert(tap_math_powi(-1, 1) == -1);
    assert(tap_math_powi(-1, 5) == -1);
    assert(tap_math_powi(-2, 5) == -32);
    assert(tap_math_powi(-5, 2) == 25);
    assert(tap_math_powi(-14, 7) == -105413504);
}

static void tap_math_powu_test(void)
{
    assert(tap_math_powu(0, 1) == 0);
    assert(tap_math_powu(1, 0) == 1);
    assert(tap_math_powu(0, 0) == 1);
    assert(tap_math_powu(1, 1) == 1);
    assert(tap_math_powu(1, 1) == 1);
    assert(tap_math_powu(1, 5) == 1);
    assert(tap_math_powu(2, 5) == 32);
    assert(tap_math_powu(5, 2) == 25);
    assert(tap_math_powu(15, 8) == 2562890625);
}

static void tap_math_powf_test(void)
{
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(0, 0), 1));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(0, 1), 0));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-1, 0), 1));
    assert(tap_math_isinf(tap_math_powf(0, -1)));

    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(42, 0), 1));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(42, 1), 42));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(42, 2), 1764));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(42, 3), 74088));

    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-42, 0), 1));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-42, 1), -42));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-42, 2), 1764));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-42, 3), -74088));

    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(0.1F, -1), 10));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(1.5F, 42), 24878997.7221F));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(1.5F, -42), 4.01945452614e-8F));

    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-0.1F, -1), -10));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-1.5F, 42), 24878997.7221F));
    assert(TAP_MATH_FLT_EQUAL(tap_math_powf(-1.5F, -42), 4.01945452614e-8F));
}

static void tap_math_abs_test(void)
{
    assert(tap_math_abs(1) == 1);
    assert(tap_math_abs(-1) == 1);
    assert(tap_math_abs(-0) == 0);
    assert(tap_math_abs(-123) == 123);
    assert(tap_math_abs(123) == 123);
    assert(tap_math_abs(INT_MIN + 1) == INT_MAX);
}

static void tap_math_fabs_test(void)
{
    assert(TAP_MATH_FLT_EQUAL(tap_math_fabs(-0.0F), 0.0F));
    assert(TAP_MATH_FLT_EQUAL(tap_math_fabs(-1.0F), 1.0F));
    assert(TAP_MATH_FLT_EQUAL(tap_math_fabs(1.0F), 1.0F));
    assert(TAP_MATH_FLT_EQUAL(tap_math_fabs(-123.456F), 123.456F));
    assert(TAP_MATH_FLT_EQUAL(tap_math_fabs(123.456F), 123.456F));
}

static void tap_math_isnan_test(void)
{
    assert(tap_math_isnan(0.0) == false);
    assert(tap_math_isnan(1.0) == false);
    assert(tap_math_isnan(-1.0) == false);
    assert(tap_math_isnan(-1.0 / 0.0) == false);
    assert(tap_math_isnan(FLT_MAX) == false);
    assert(tap_math_isnan(FLT_MIN) == false);
    assert(tap_math_isnan(-0.0 / 0.0) == true);
}
static void tap_math_isinf_test(void)
{
    assert(tap_math_isinf(0.0) == false);
    assert(tap_math_isinf(1.0) == false);
    assert(tap_math_isinf(-1.0) == false);
    assert(tap_math_isinf(-0.0 / 0.0) == false);
    assert(tap_math_isinf(FLT_MAX) == false);
    assert(tap_math_isinf(FLT_MIN) == false);
    assert(tap_math_isinf(-1.0 / 0.0) == true);
}

static void tap_math_max_test(void)
{
    const unsigned int zero = 0;
    assert(tap_math_max(1, 2) == 2);
    assert(tap_math_max(2, 1) == 2);
    assert(tap_math_max(-1, 0) == 0);
    assert(tap_math_max(zero, 0) == 0);
}

static void tap_math_min_test(void)
{
    const unsigned int zero = 0;
    assert(tap_math_min(1, 2) == 1);
    assert(tap_math_min(2, 1) == 1);
    assert(tap_math_min(-1, 0) == -1);
    assert(tap_math_min(zero, 0) == 0);
}

int main(void)
{
    tap_math_powi_test();
    tap_math_powu_test();
    tap_math_powf_test();
    tap_math_abs_test();
    tap_math_fabs_test();
    tap_math_isnan_test();
    tap_math_isinf_test();
    tap_math_max_test();
    tap_math_min_test();
    return 0;
}
