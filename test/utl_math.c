#include "test.h"

static void powi_check(void)
{
}

static void powu_check(void)
{
}

static void powf_check(void)
{
    ASSERT(FEQUAL(utl_powf(0, 1), 0));
    ASSERT(FEQUAL(utl_powf(-1, 0), -1));
    ASSERT(FEQUAL(utl_powf(0, 0), 1));
    ASSERT(FEQUAL(utl_powf(42, 0), 1));
    ASSERT(FEQUAL(utl_powf(0.1f, -1), 10));
    ASSERT(FEQUAL(utl_powf(1, 2), 1));
    ASSERT(FEQUAL(utl_powf(2, 2), 4));

    ASSERT(utl_isinf(utl_powf(0, -1)));
}

static void abs_check(void)
{
    ASSERT(utl_abs(1) == 1);
    ASSERT(utl_abs(-1) == 1);
    ASSERT(utl_abs(-0) == 0);
    ASSERT(utl_abs(-123) == 123);
    ASSERT(utl_abs(123) == 123);
    ASSERT(utl_abs(I32_MIN+1) == I32_MAX);
}

static void fabs_check(void)
{
    ASSERT(FEQUAL(utl_fabs(-0.0f), 0.0f));
    ASSERT(FEQUAL(utl_fabs(-1.0f), 1.0f));
    ASSERT(FEQUAL(utl_fabs(1.0f), 1.0f));
    ASSERT(FEQUAL(utl_fabs(-123.456f), 123.456f));
    ASSERT(FEQUAL(utl_fabs(123.456f), 123.456f));
}

static void isnan_check(void)
{
    ASSERT(utl_isnan(0.0f) == false);
    ASSERT(utl_isnan(1.0f) == false);
    ASSERT(utl_isnan(-1.0f) == false);
    ASSERT(utl_isnan(-1.0f / 0.0f) == false);
    ASSERT(utl_isnan(F32_MAX) == false);
    ASSERT(utl_isnan(F32_MIN) == false);
    ASSERT(utl_isnan(-0.0f / 0.0f) == true);

}
static void isinf_check(void)
{
    ASSERT(utl_isinf(0.0f) == false);
    ASSERT(utl_isinf(1.0f) == false); 
    ASSERT(utl_isinf(-1.0f) == false); 
    ASSERT(utl_isinf(-0.0f / 0.0f) == false); 
    ASSERT(utl_isinf(F32_MAX) == false);
    ASSERT(utl_isinf(F32_MIN) == false);
    ASSERT(utl_isinf(-1.0f / 0.0f) == true); 
}

i32 main(void)
{
    tst_init();

    powi_check();
    powu_check();
    powf_check();
    abs_check();
    fabs_check(); 
    isnan_check();
    isinf_check();

    return 0;
}
