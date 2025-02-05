#include <assert.h>
#include <limits.h>
#include <float.h>
#include "test.h"
#include "src/utils.c"

static void PowiCheck(void)
{
    assert(UTL_Powi(0, 0) == 1);
    assert(UTL_Powi(0, 1) == 0);

    assert(UTL_Powi(1, 0) == 1);
    assert(UTL_Powi(1, 1) == 1);
    assert(UTL_Powi(1, 5) == 1);
    assert(UTL_Powi(2, 5) == 32);
    assert(UTL_Powi(5, 2) == 25);
    assert(UTL_Powi(14, 8) == 1475789056);

    assert(UTL_Powi(-1, 0) == 1);
    assert(UTL_Powi(-1, 1) == -1);
    assert(UTL_Powi(-1, 5) == -1);
    assert(UTL_Powi(-2, 5) == -32);
    assert(UTL_Powi(-5, 2) == 25);
    assert(UTL_Powi(-14, 7) == -105413504);
}

static void Powu_check(void)
{
    assert(UTL_Powu(0, 1) == 0);
    assert(UTL_Powu(1, 0) == 1);
    assert(UTL_Powu(0, 0) == 1);
    assert(UTL_Powu(1, 1) == 1);
    assert(UTL_Powu(1, 1) == 1);
    assert(UTL_Powu(1, 5) == 1);
    assert(UTL_Powu(2, 5) == 32);
    assert(UTL_Powu(5, 2) == 25);
    assert(UTL_Powu(15, 8) == 2562890625);
}

static void Powf_check(void)
{
    assert(UTL_FEQUAL(UTL_Powf(0, 0), 1));
    assert(UTL_FEQUAL(UTL_Powf(0, 1), 0));
    assert(UTL_FEQUAL(UTL_Powf(-1, 0), 1));
    assert(UTL_Isinf(UTL_Powf(0, -1)));

    assert(UTL_FEQUAL(UTL_Powf(42, 0), 1));
    assert(UTL_FEQUAL(UTL_Powf(42, 1), 42));
    assert(UTL_FEQUAL(UTL_Powf(42, 2), 1764));
    assert(UTL_FEQUAL(UTL_Powf(42, 3), 74088));

    assert(UTL_FEQUAL(UTL_Powf(-42, 0), 1));
    assert(UTL_FEQUAL(UTL_Powf(-42, 1), -42));
    assert(UTL_FEQUAL(UTL_Powf(-42, 2), 1764));
    assert(UTL_FEQUAL(UTL_Powf(-42, 3), -74088));

    assert(UTL_FEQUAL(UTL_Powf(0.1f, -1), 10));
    assert(UTL_FEQUAL(UTL_Powf(1.5f, 42), 24878997.7221f));
    assert(UTL_FEQUAL(UTL_Powf(1.5f, -42), 4.01945452614e-8f));

    assert(UTL_FEQUAL(UTL_Powf(-0.1f, -1), -10));
    assert(UTL_FEQUAL(UTL_Powf(-1.5f, 42), 24878997.7221f));
    assert(UTL_FEQUAL(UTL_Powf(-1.5f, -42), 4.01945452614e-8f));
}

static void AbsCheck(void)
{
    assert(UTL_Abs(1) == 1);
    assert(UTL_Abs(-1) == 1);
    assert(UTL_Abs(-0) == 0);
    assert(UTL_Abs(-123) == 123);
    assert(UTL_Abs(123) == 123);
    assert(UTL_Abs(INT_MIN + 1) == INT_MAX);
}

static void FabsCheck(void)
{
    assert(UTL_FEQUAL(UTL_Fabs(-0.0f), 0.0f));
    assert(UTL_FEQUAL(UTL_Fabs(-1.0f), 1.0f));
    assert(UTL_FEQUAL(UTL_Fabs(1.0f), 1.0f));
    assert(UTL_FEQUAL(UTL_Fabs(-123.456f), 123.456f));
    assert(UTL_FEQUAL(UTL_Fabs(123.456f), 123.456f));
}

static void Isnan_check(void)
{
    assert(UTL_Isnan(0.0f) == false);
    assert(UTL_Isnan(1.0f) == false);
    assert(UTL_Isnan(-1.0f) == false);
    assert(UTL_Isnan(-1.0f / 0.0f) == false);
    assert(UTL_Isnan(FLT_MAX) == false);
    assert(UTL_Isnan(FLT_MIN) == false);
    assert(UTL_Isnan(-0.0f / 0.0f) == true);
}
static void Isinf_check(void)
{
    assert(UTL_Isinf(0.0f) == false);
    assert(UTL_Isinf(1.0f) == false);
    assert(UTL_Isinf(-1.0f) == false);
    assert(UTL_Isinf(-0.0f / 0.0f) == false);
    assert(UTL_Isinf(FLT_MAX) == false);
    assert(UTL_Isinf(FLT_MIN) == false);
    assert(UTL_Isinf(-1.0f / 0.0f) == true);
}

i32 main(void)
{
    switch (UTL_Init((UTL_InitArg){0}))
    {
        case UTL_InitRet_OK: UTL_LOG_DEBUG("utl initialisation success."); break;
        case UTL_InitRet_NULL_LOG:
        case UTL_InitRet_NULL_CALLBACK: assert(1 == 2);
    }

    PowiCheck();
    Powu_check();
    Powf_check();
    AbsCheck();
    FabsCheck();
    Isnan_check();
    Isinf_check();

    return 0;
}
