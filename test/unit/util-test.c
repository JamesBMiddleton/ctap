#include "src/util.c"
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static void utest_util_EventHandlerLogPrintf(const util_Log log)
{
    const char* lvl = "UNKNOWN";
    switch (log.lvl)
    {
        case util_LogLvl_DEBUG: lvl = "DEBUG"; break;
        case util_LogLvl_WARN: lvl = "WARN"; break;
        case util_LogLvl_ERROR: lvl = "ERROR"; break;
        case util_LogLvl_PANIC: lvl = "PANIC"; break;
        case util_LogLvl_ASSERT: lvl = "ASSERT"; break;
    }
    printf("%s | %s::%d | %s\n", lvl, log.funcName, log.lineNum, log.message);
    (void)fflush(stdout);
}

static void utest_util_EventHandlerPanicDoNothing(void)
{
}

static void utest_util_Powi(void)
{
    assert(util_Powi(0, 0) == 1);
    assert(util_Powi(0, 1) == 0);

    assert(util_Powi(1, 0) == 1);
    assert(util_Powi(1, 1) == 1);
    assert(util_Powi(1, 5) == 1);
    assert(util_Powi(2, 5) == 32);
    assert(util_Powi(5, 2) == 25);
    assert(util_Powi(14, 8) == 1475789056);

    assert(util_Powi(-1, 0) == 1);
    assert(util_Powi(-1, 1) == -1);
    assert(util_Powi(-1, 5) == -1);
    assert(util_Powi(-2, 5) == -32);
    assert(util_Powi(-5, 2) == 25);
    assert(util_Powi(-14, 7) == -105413504);
}

static void utest_util_Powu(void)
{
    assert(util_Powu(0, 1) == 0);
    assert(util_Powu(1, 0) == 1);
    assert(util_Powu(0, 0) == 1);
    assert(util_Powu(1, 1) == 1);
    assert(util_Powu(1, 1) == 1);
    assert(util_Powu(1, 5) == 1);
    assert(util_Powu(2, 5) == 32);
    assert(util_Powu(5, 2) == 25);
    assert(util_Powu(15, 8) == 2562890625);
}

static void utest_util_Pow(void)
{
    assert(util_FLT_EQUAL(util_Powf(0, 0), 1));
    assert(util_FLT_EQUAL(util_Powf(0, 1), 0));
    assert(util_FLT_EQUAL(util_Powf(-1, 0), 1));
    assert(util_Isinf(util_Powf(0, -1)));

    assert(util_FLT_EQUAL(util_Powf(42, 0), 1));
    assert(util_FLT_EQUAL(util_Powf(42, 1), 42));
    assert(util_FLT_EQUAL(util_Powf(42, 2), 1764));
    assert(util_FLT_EQUAL(util_Powf(42, 3), 74088));

    assert(util_FLT_EQUAL(util_Powf(-42, 0), 1));
    assert(util_FLT_EQUAL(util_Powf(-42, 1), -42));
    assert(util_FLT_EQUAL(util_Powf(-42, 2), 1764));
    assert(util_FLT_EQUAL(util_Powf(-42, 3), -74088));

    assert(util_FLT_EQUAL(util_Powf(0.1F, -1), 10));
    assert(util_FLT_EQUAL(util_Powf(1.5F, 42), 24878997.7221F));
    assert(util_FLT_EQUAL(util_Powf(1.5F, -42), 4.01945452614e-8F));

    assert(util_FLT_EQUAL(util_Powf(-0.1F, -1), -10));
    assert(util_FLT_EQUAL(util_Powf(-1.5F, 42), 24878997.7221F));
    assert(util_FLT_EQUAL(util_Powf(-1.5F, -42), 4.01945452614e-8F));
}

static void utest_util_Abs(void)
{
    assert(util_Abs(1) == 1);
    assert(util_Abs(-1) == 1);
    assert(util_Abs(-0) == 0);
    assert(util_Abs(-123) == 123);
    assert(util_Abs(123) == 123);
    assert(util_Abs(INT_MIN + 1) == INT_MAX);
}

static void utest_util_Fabs(void)
{
    assert(util_FLT_EQUAL(util_Fabs(-0.0F), 0.0F));
    assert(util_FLT_EQUAL(util_Fabs(-1.0F), 1.0F));
    assert(util_FLT_EQUAL(util_Fabs(1.0F), 1.0F));
    assert(util_FLT_EQUAL(util_Fabs(-123.456F), 123.456F));
    assert(util_FLT_EQUAL(util_Fabs(123.456F), 123.456F));
}

static void utest_util_Isnan(void)
{
    assert(util_Isnan(0.0) == false);
    assert(util_Isnan(1.0) == false);
    assert(util_Isnan(-1.0) == false);
    assert(util_Isnan(-1.0 / 0.0) == false);
    assert(util_Isnan(FLT_MAX) == false);
    assert(util_Isnan(FLT_MIN) == false);
    assert(util_Isnan(-0.0 / 0.0) == true);
}
static void utest_util_Isinf(void)
{
    assert(util_Isinf(0.0) == false);
    assert(util_Isinf(1.0) == false);
    assert(util_Isinf(-1.0) == false);
    assert(util_Isinf(-0.0 / 0.0) == false);
    assert(util_Isinf(FLT_MAX) == false);
    assert(util_Isinf(FLT_MIN) == false);
    assert(util_Isinf(-1.0 / 0.0) == true);
}

static void utest_util_Sprintf(void)
{
    char arr[util_MAX_LOG_SZ] = {0};

    const uint uint_num = 123;
    const int int_num = 123;

    const char* overflowcheck1 = "toomanychars";
    const char* overflowcheck2 = "toomanychar%c";
    const char* overflowcheck3 = "toomanychar%s";
    const char* overflowcheck4 = "waytoo%s";
    const char* overflowcheck5 = "floatdon'tfit%f";
    const char* overflowcheck6 = "intjustdon'tfit%d";

    // Check error cases.
    util_Sprintf(arr, util_Strlen(overflowcheck1), overflowcheck1,
                 (util_Fmts){0});
    assert(strcmp(arr, "toomanychar") == 0);

    util_Sprintf(arr, sizeof(arr), "invalid %t specifier", (util_Fmts){0});
    assert(strcmp(arr, "invalid ") == 0);

    util_Sprintf(
        arr, sizeof(arr), "%c%c%c%c%c%c too many specifiers",
        (util_Fmts){
            .arr = {
                {.c = 'h'}, {.c = 'e'}, {.c = 'l'}, {.c = 'l'}, {.c = 'o'}}});
    assert(strcmp(arr, "hello") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck2) - 1, overflowcheck2,
                 (util_Fmts){.arr = {{.c = 's'}}});
    assert(strcmp(arr, "toomanychar") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck3) - 1, overflowcheck3,
                 (util_Fmts){.arr = {{.s = "s"}}});
    assert(strcmp(arr, "toomanychar") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck4), overflowcheck4,
                 (util_Fmts){.arr = {{.s = "manychars"}}});
    assert(strcmp(arr, "waytoo") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck5) + 1, overflowcheck5,
                 (util_Fmts){.arr = {{.f = 1.0F}}});
    assert(strcmp(arr, "floatdon'tfit") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck6) + NUMERIC_MAX_CHARS - 2,
                 overflowcheck6, (util_Fmts){.arr = {{.d = 1}}});
    assert(strcmp(arr, "intjustdon'tfit") == 0);

    // Check uints.
    util_Sprintf(arr, sizeof(arr), "%u", (util_Fmts){.arr = {{.u = uint_num}}});
    assert(strcmp(arr, "123") == 0);

    util_Sprintf(arr, sizeof(arr), "%u", (util_Fmts){.arr = {{.u = 0}}});
    assert(strcmp(arr, "0") == 0);

    util_Sprintf(arr, sizeof(arr), "%u", (util_Fmts){.arr = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "4294967295") == 0);

    util_Sprintf(arr, sizeof(arr), "a%ua",
                 (util_Fmts){.arr = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "a4294967295a") == 0);

    // Check ints.
    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = int_num}}});
    assert(strcmp(arr, "123") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = -int_num}}});
    assert(strcmp(arr, "-123") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = 0}}});
    assert(strcmp(arr, "0") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = INT_MIN}}});
    assert(strcmp(arr, "-2147483648") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = INT_MAX}}});
    assert(strcmp(arr, "2147483647") == 0);

    util_Sprintf(arr, sizeof(arr), "a%da",
                 (util_Fmts){.arr = {{.d = INT_MIN}}});
    assert(strcmp(arr, "a-2147483648a") == 0);

    // Check floats.
    const float float_num = 123.456F;
    const float float_num_fract_part = .456F;
    const float float_num_int_part = 123.F;

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = float_num}}});
    assert(strcmp(arr, "123.456") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = float_num_fract_part}}});
    assert(strcmp(arr, "0.456") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = float_num_int_part}}});
    assert(strcmp(arr, "123.000") == 0);

    util_Sprintf(arr, sizeof(arr), "%f", (util_Fmts){.arr = {{.f = 0}}});
    assert(strcmp(arr, "0.000") == 0);

    util_Sprintf(arr, sizeof(arr), "%f", (util_Fmts){.arr = {{.f = -0}}});
    assert(strcmp(arr, "0.000") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = -float_num}}});
    assert(strcmp(arr, "-123.456") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = -0.0F / 0.0F}}});
    assert(strcmp(arr, "NaN") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = -1.0F / 0.0F}}});
    assert(strcmp(arr, "Inf") == 0);

    // Check strings.
    util_Sprintf(arr, sizeof(arr), "%s", (util_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobar") == 0);

    util_Sprintf(arr, sizeof(arr), "a%s",
                 (util_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "afoobar") == 0);

    util_Sprintf(arr, sizeof(arr), "%sa",
                 (util_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobara") == 0);

    util_Sprintf(arr, sizeof(arr), "%s%s",
                 (util_Fmts){.arr = {{.s = "foo"}, {.s = "bar"}}});
    assert(strcmp(arr, "foobar") == 0);

    // Check chars.
    util_Sprintf(arr, sizeof(arr), "%c", (util_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "a") == 0);

    util_Sprintf(arr, sizeof(arr), "a%c", (util_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);

    util_Sprintf(arr, sizeof(arr), "%cc", (util_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "ac") == 0);

    util_Sprintf(arr, sizeof(arr), "%c%c",
                 (util_Fmts){.arr = {{.c = 'a'}, {.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);
}

static void utest_util_MAX(void)
{
    const uint zero = 0;
    assert(util_MAX(1, 2) == 2);
    assert(util_MAX(2, 1) == 2);
    assert(util_MAX(-1, 0) == 0);
    assert(util_MAX(zero, 0) == 0);
}

static void utest_util_MIN(void)
{
    const uint zero = 0;
    assert(util_MIN(1, 2) == 1);
    assert(util_MIN(2, 1) == 1);
    assert(util_MIN(-1, 0) == -1);
    assert(util_MIN(zero, 0) == 0);
}

static void utest_util_Untested(void)
{
    util_LOG_WARN("Hello World");
    util_LOGF_WARN("Hello World", {0});
    util_LOGF_ERROR("Hello World", {0});
    util_LOGF_DEBUG("Hello World", {0});
    util_LOG_DEBUG("Hello World");
    util_PANIC();
}

int main(void)
{
    util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_util_Sprintf();
    utest_util_Powi();
    utest_util_Powu();
    utest_util_Pow();
    utest_util_Abs();
    utest_util_Fabs();
    utest_util_Isnan();
    utest_util_Isinf();
    utest_util_MAX();
    utest_util_MIN();
    utest_util_Untested();
    return 0;
}
