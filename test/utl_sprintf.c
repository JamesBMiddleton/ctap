#include <assert.h>
#include <string.h>
#include <limits.h>
#include "test.h"
#include "src/utils.c"

static char arr[utl_MAX_LOG_SZ] = {0};

static void ErrorCheck(void)
{
    UTL_Sprintf(arr, 12, "toomanychars", (UTL_Fmts){0});
    assert(strcmp(arr, "toomanychar") == 0);

    UTL_Sprintf(arr, sizeof(arr), "invalid %t specifier", (UTL_Fmts){0});
    assert(strcmp(arr, "invalid ") == 0);

    UTL_Sprintf(
        arr, sizeof(arr), "%c%c%c%c%c%c too many specifiers",
        (UTL_Fmts){
            .arr = {
                {.c = 'h'}, {.c = 'e'}, {.c = 'l'}, {.c = 'l'}, {.c = 'o'}}});
    assert(strcmp(arr, "hello") == 0);

    UTL_Sprintf(arr, 12, "toomanychar%c", (UTL_Fmts){.arr = {{.c = 's'}}});
    assert(strcmp(arr, "toomanychar") == 0);

    UTL_Sprintf(arr, 12, "toomanychar%s", (UTL_Fmts){.arr = {{.s = "s"}}});
    assert(strcmp(arr, "toomanychar") == 0);

    UTL_Sprintf(arr, 8, "waytoo%s", (UTL_Fmts){.arr = {{.s = "manychars"}}});
    assert(strcmp(arr, "waytoo") == 0);

    UTL_Sprintf(arr, 16, "floatdon'tfit%f", (UTL_Fmts){.arr = {{.f = 1.0f}}});
    assert(strcmp(arr, "floatdon'tfit") == 0);

    UTL_Sprintf(arr, 15 + utl_NUMERIC_MAX_CHARS, "intjustdon'tfit%d",
                (UTL_Fmts){.arr = {{.d = 1}}});
    assert(strcmp(arr, "intjustdon'tfit") == 0);
}

static void U32Check(void)
{
    UTL_Sprintf(arr, sizeof(arr), "%u", (UTL_Fmts){.arr = {{.u = 123}}});
    assert(strcmp(arr, "123") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%u", (UTL_Fmts){.arr = {{.u = 0}}});
    assert(strcmp(arr, "0") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%u",
                (UTL_Fmts){.arr = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "4294967295") == 0);

    UTL_Sprintf(arr, sizeof(arr), "a%ua",
                (UTL_Fmts){.arr = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "a4294967295a") == 0);
}

static void I32Check(void)
{
    UTL_Sprintf(arr, sizeof(arr), "%d", (UTL_Fmts){.arr = {{.d = 123}}});
    assert(strcmp(arr, "123") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%d", (UTL_Fmts){.arr = {{.d = -123}}});
    assert(strcmp(arr, "-123") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%d", (UTL_Fmts){.arr = {{.d = 0}}});
    assert(strcmp(arr, "0") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%d",
                (UTL_Fmts){.arr = {{.d = INT_MIN}}});
    assert(strcmp(arr, "-2147483648") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%d",
                (UTL_Fmts){.arr = {{.d = INT_MAX}}});
    assert(strcmp(arr, "2147483647") == 0);

    UTL_Sprintf(arr, sizeof(arr), "a%da",
                (UTL_Fmts){.arr = {{.d = INT_MIN}}});
    assert(strcmp(arr, "a-2147483648a") == 0);
}

static void f32_check(void)
{
    UTL_Sprintf(arr, sizeof(arr), "%f", (UTL_Fmts){.arr = {{.f = 123.456f}}});
    assert(strcmp(arr, "123.456") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%f", (UTL_Fmts){.arr = {{.f = .456f}}});
    assert(strcmp(arr, "0.456") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%f", (UTL_Fmts){.arr = {{.f = 123}}});
    assert(strcmp(arr, "123.000") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%f", (UTL_Fmts){.arr = {{.f = 0}}});
    assert(strcmp(arr, "0.000") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%f", (UTL_Fmts){.arr = {{.f = -0}}});
    assert(strcmp(arr, "0.000") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%f",
                (UTL_Fmts){.arr = {{.f = -123.456F}}});
    assert(strcmp(arr, "-123.456") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%f",
                (UTL_Fmts){.arr = {{.f = -0.0 / 0.0}}});
    assert(strcmp(arr, "NaN") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%f",
                (UTL_Fmts){.arr = {{.f = -1.0 / 0.0}}});
    assert(strcmp(arr, "Inf") == 0);
}

static void str_check(void)
{
    UTL_Sprintf(arr, sizeof(arr), "%s", (UTL_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobar") == 0);

    UTL_Sprintf(arr, sizeof(arr), "a%s",
                (UTL_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "afoobar") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%sa",
                (UTL_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobara") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%s%s",
                (UTL_Fmts){.arr = {{.s = "foo"}, {.s = "bar"}}});
    assert(strcmp(arr, "foobar") == 0);
}

static void char_check(void)
{
    UTL_Sprintf(arr, sizeof(arr), "%c", (UTL_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "a") == 0);

    UTL_Sprintf(arr, sizeof(arr), "a%c", (UTL_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%cc", (UTL_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "ac") == 0);

    UTL_Sprintf(arr, sizeof(arr), "%c%c",
                (UTL_Fmts){.arr = {{.c = 'a'}, {.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);
}

i32 main(void)
{
    switch (UTL_Init((UTL_InitArg){0}))
    {
        case UTL_InitRet_OK: UTL_LOG_DEBUG("utl initialisation success."); break;
        case UTL_InitRet_NULL_LOG:
        case UTL_InitRet_NULL_CALLBACK: assert(1 == 2);
    }

    ErrorCheck();
    U32Check();
    I32Check();
    f32_check();
    str_check();
    char_check();

    return 0;
}
