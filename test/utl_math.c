#include "test.h"

static void powi_check(void)
{
    LOG_D("%d", {.d=utl_powi(-5, 2)});
    LOG_D("%d", {.d=utl_powi(-1, 1)});
    ASSERT(utl_powi(0, 0) == 1);

    ASSERT(utl_powi(0, 1) == 0);
    ASSERT(utl_powi(1, 0) == 1);
    ASSERT(utl_powi(1, 1) == 1);
    ASSERT(utl_powi(1, 1) == 1);
    ASSERT(utl_powi(1, 5) == 1);
    ASSERT(utl_powi(2, 5) == 32);
    ASSERT(utl_powi(5, 2) == 25);
    ASSERT(utl_powi(14, 8) == 1475789056);

    ASSERT(utl_powi(-0, 1) == -0);
    ASSERT(utl_powi(-1, 0) == -1);
    ASSERT(utl_powi(-1, 1) == -1);
    ASSERT(utl_powi(-1, 1) == -1);
    ASSERT(utl_powi(-1, 5) == -1);
    ASSERT(utl_powi(-2, 5) == -32);
    ASSERT(utl_powi(-5, 2) == -25);
    ASSERT(utl_powi(-14, 8) == -1475789056);
}

static void powu_check(void)
{
    ASSERT(utl_powu(0, 1) == 0);
    ASSERT(utl_powu(1, 0) == 1);
    ASSERT(utl_powu(0, 0) == 1);
    ASSERT(utl_powu(1, 1) == 1);
    ASSERT(utl_powu(1, 1) == 1);
    ASSERT(utl_powu(1, 5) == 1);
    ASSERT(utl_powu(2, 5) == 32);
    ASSERT(utl_powu(5, 2) == 25);
    ASSERT(utl_powu(15, 8) == 2562890625);
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
    ASSERT(FEQUAL(utl_powf(2, 2), 4));
    ASSERT(FEQUAL(utl_powf(1.5f, 42), 24878997.7221f));
    ASSERT(FEQUAL(utl_powf(1.5f, -42), 4.01945452614e-8f));

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
