#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>

#include "util/ctp_math.h"

static void ctp_math_powi_test(void)
{
    assert(ctp_math_powi(0, 0) == 1);
    assert(ctp_math_powi(0, 1) == 0);

    assert(ctp_math_powi(1, 0) == 1);
    assert(ctp_math_powi(1, 1) == 1);
    assert(ctp_math_powi(1, 5) == 1);
    assert(ctp_math_powi(2, 5) == 32);
    assert(ctp_math_powi(5, 2) == 25);
    assert(ctp_math_powi(14, 8) == 1475789056);

    assert(ctp_math_powi(-1, 0) == 1);
    assert(ctp_math_powi(-1, 1) == -1);
    assert(ctp_math_powi(-1, 5) == -1);
    assert(ctp_math_powi(-2, 5) == -32);
    assert(ctp_math_powi(-5, 2) == 25);
    assert(ctp_math_powi(-14, 7) == -105413504);
}

static void ctp_math_powu_test(void)
{
    assert(ctp_math_powu(0, 1) == 0);
    assert(ctp_math_powu(1, 0) == 1);
    assert(ctp_math_powu(0, 0) == 1);
    assert(ctp_math_powu(1, 1) == 1);
    assert(ctp_math_powu(1, 1) == 1);
    assert(ctp_math_powu(1, 5) == 1);
    assert(ctp_math_powu(2, 5) == 32);
    assert(ctp_math_powu(5, 2) == 25);
    assert(ctp_math_powu(15, 8) == 2562890625);
}

static void ctp_math_powf_test(void)
{
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(0, 0), 1));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(0, 1), 0));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-1, 0), 1));
    assert(ctp_math_isinf(ctp_math_powf(0, -1)));

    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(42, 0), 1));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(42, 1), 42));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(42, 2), 1764));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(42, 3), 74088));

    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-42, 0), 1));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-42, 1), -42));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-42, 2), 1764));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-42, 3), -74088));

    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(0.1F, -1), 10));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(1.5F, 42), 24878997.7221F));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(1.5F, -42), 4.01945452614e-8F));

    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-0.1F, -1), -10));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-1.5F, 42), 24878997.7221F));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_powf(-1.5F, -42), 4.01945452614e-8F));
}

static void ctp_math_abs_test(void)
{
    assert(ctp_math_abs(1) == 1);
    assert(ctp_math_abs(-1) == 1);
    assert(ctp_math_abs(-0) == 0);
    assert(ctp_math_abs(-123) == 123);
    assert(ctp_math_abs(123) == 123);
    assert(ctp_math_abs(INT_MIN + 1) == INT_MAX);
}

static void ctp_math_fabs_test(void)
{
    assert(CTP_MATH_FLT_EQUAL(ctp_math_fabs(-0.0F), 0.0F));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_fabs(-1.0F), 1.0F));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_fabs(1.0F), 1.0F));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_fabs(-123.456F), 123.456F));
    assert(CTP_MATH_FLT_EQUAL(ctp_math_fabs(123.456F), 123.456F));
}

static void ctp_math_isnan_test(void)
{
    assert(ctp_math_isnan(0.0) == false);
    assert(ctp_math_isnan(1.0) == false);
    assert(ctp_math_isnan(-1.0) == false);
    assert(ctp_math_isnan(-1.0 / 0.0) == false);
    assert(ctp_math_isnan(FLT_MAX) == false);
    assert(ctp_math_isnan(FLT_MIN) == false);
    assert(ctp_math_isnan(-0.0 / 0.0) == true);
}
static void ctp_math_isinf_test(void)
{
    assert(ctp_math_isinf(0.0) == false);
    assert(ctp_math_isinf(1.0) == false);
    assert(ctp_math_isinf(-1.0) == false);
    assert(ctp_math_isinf(-0.0 / 0.0) == false);
    assert(ctp_math_isinf(FLT_MAX) == false);
    assert(ctp_math_isinf(FLT_MIN) == false);
    assert(ctp_math_isinf(-1.0 / 0.0) == true);
}

static void ctp_math_max_test(void)
{
    const unsigned int zero = 0;
    assert(ctp_math_max(1, 2) == 2);
    assert(ctp_math_max(2, 1) == 2);
    assert(ctp_math_max(-1, 0) == 0);
    assert(ctp_math_max(zero, 0) == 0);
}

static void ctp_math_min_test(void)
{
    const unsigned int zero = 0;
    assert(ctp_math_min(1, 2) == 1);
    assert(ctp_math_min(2, 1) == 1);
    assert(ctp_math_min(-1, 0) == -1);
    assert(ctp_math_min(zero, 0) == 0);
}

int main(void)
{
    ctp_math_powi_test();
    ctp_math_powu_test();
    ctp_math_powf_test();
    ctp_math_abs_test();
    ctp_math_fabs_test();
    ctp_math_isnan_test();
    ctp_math_isinf_test();
    ctp_math_max_test();
    ctp_math_min_test();
    return 0;
}
