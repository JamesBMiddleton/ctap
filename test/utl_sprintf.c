#include "test.h"
#include <string.h>

static char arr[MAX_LOG_SZ] = {0};

static void error_check(void)
{
    utl_sprintf(arr, 12, "toomanychars", (utl_fmts_t){0});
    ASSERT(strcmp(arr, "toomanychar") == 0);

    utl_sprintf(arr, sizeof(arr), "invalid %t specifier", (utl_fmts_t){0});
    ASSERT(strcmp(arr, "invalid ") == 0);

    utl_sprintf(
        arr, sizeof(arr), "%c%c%c%c%c%c too many specifiers",
        (utl_fmts_t){
            .arr = {
                {.c = 'h'}, {.c = 'e'}, {.c = 'l'}, {.c = 'l'}, {.c = 'o'}}});
    ASSERT(strcmp(arr, "hello") == 0);

    utl_sprintf(arr, 12, "toomanychar%c", (utl_fmts_t){.arr = {{.c = 's'}}});
    ASSERT(strcmp(arr, "toomanychar") == 0);

    utl_sprintf(arr, 12, "toomanychar%s", (utl_fmts_t){.arr = {{.s = "s"}}});
    ASSERT(strcmp(arr, "toomanychar") == 0);

    utl_sprintf(arr, 8, "waytoo%s", (utl_fmts_t){.arr = {{.s = "manychars"}}});
    ASSERT(strcmp(arr, "waytoo") == 0);

    utl_sprintf(arr, 16, "floatdon'tfit%f", (utl_fmts_t){.arr = {{.f = 1.0f}}});
    ASSERT(strcmp(arr, "floatdon'tfit") == 0);

    utl_sprintf(arr, 15 + F32_MAX_CHARS, "intjustdon'tfit%d",
                (utl_fmts_t){.arr = {{.d = 1}}});
    ASSERT(strcmp(arr, "intjustdon'tfit") == 0);
}

static void u32_check(void)
{
    utl_sprintf(arr, sizeof(arr), "%u", (utl_fmts_t){.arr = {{.u = 123}}});
    ASSERT(strcmp(arr, "123") == 0);

    utl_sprintf(arr, sizeof(arr), "%u", (utl_fmts_t){.arr = {{.u = 0}}});
    ASSERT(strcmp(arr, "0") == 0);

    utl_sprintf(arr, sizeof(arr), "%u",
                (utl_fmts_t){.arr = {{.u = 4294967295}}});
    ASSERT(strcmp(arr, "4294967295") == 0);

    utl_sprintf(arr, sizeof(arr), "a%ua",
                (utl_fmts_t){.arr = {{.u = 4294967295}}});
    ASSERT(strcmp(arr, "a4294967295a") == 0);
}

static void i32_check(void)
{
    utl_sprintf(arr, sizeof(arr), "%d", (utl_fmts_t){.arr = {{.d = 123}}});
    ASSERT(strcmp(arr, "123") == 0);

    utl_sprintf(arr, sizeof(arr), "%d", (utl_fmts_t){.arr = {{.d = -123}}});
    ASSERT(strcmp(arr, "-123") == 0);

    utl_sprintf(arr, sizeof(arr), "%d", (utl_fmts_t){.arr = {{.d = 0}}});
    ASSERT(strcmp(arr, "0") == 0);

    utl_sprintf(arr, sizeof(arr), "%d",
                (utl_fmts_t){.arr = {{.d = -2147483648}}});
    ASSERT(strcmp(arr, "-2147483648") == 0);

    utl_sprintf(arr, sizeof(arr), "%d",
                (utl_fmts_t){.arr = {{.d = 2147483647}}});
    ASSERT(strcmp(arr, "2147483647") == 0);

    utl_sprintf(arr, sizeof(arr), "a%da",
                (utl_fmts_t){.arr = {{.d = -2147483648}}});
    ASSERT(strcmp(arr, "a-2147483648a") == 0);
}

static void f32_check(void)
{
    utl_sprintf(arr, sizeof(arr), "%f", (utl_fmts_t){.arr = {{.f = 123.456f}}});
    ASSERT(strcmp(arr, "123.456") == 0);

    utl_sprintf(arr, sizeof(arr), "%f", (utl_fmts_t){.arr = {{.f = .456f}}});
    ASSERT(strcmp(arr, "0.456") == 0);

    utl_sprintf(arr, sizeof(arr), "%f", (utl_fmts_t){.arr = {{.f = 123}}});
    ASSERT(strcmp(arr, "123.000") == 0);

    utl_sprintf(arr, sizeof(arr), "%f", (utl_fmts_t){.arr = {{.f = 0}}});
    ASSERT(strcmp(arr, "0.000") == 0);

    utl_sprintf(arr, sizeof(arr), "%f", (utl_fmts_t){.arr = {{.f = -0}}});
    ASSERT(strcmp(arr, "0.000") == 0);

    utl_sprintf(arr, sizeof(arr), "%f",
                (utl_fmts_t){.arr = {{.f = -123.456F}}});
    ASSERT(strcmp(arr, "-123.456") == 0);

    // utl_sprintf(arr, sizeof(arr), "%f",
    //             (utl_fmts_t){.arr = {{.f = 1111111.1f}}});
    // printf("%s", arr);
    // ASSERT(strcmp(arr, "-2147483648.1") == 0);
    //
    // utl_sprintf(arr, sizeof(arr), "%f",
    //             (utl_fmts_t){.arr = {{.f = -2147483648.1f}}});
    // ASSERT(strcmp(arr, "-2147483648.1") == 0);
}

static void str_check(void)
{
    utl_sprintf(arr, sizeof(arr), "%s", (utl_fmts_t){.arr = {{.s = "foobar"}}});
    ASSERT(strcmp(arr, "foobar") == 0);

    utl_sprintf(arr, sizeof(arr), "a%s",
                (utl_fmts_t){.arr = {{.s = "foobar"}}});
    ASSERT(strcmp(arr, "afoobar") == 0);

    utl_sprintf(arr, sizeof(arr), "%sa",
                (utl_fmts_t){.arr = {{.s = "foobar"}}});
    ASSERT(strcmp(arr, "foobara") == 0);

    utl_sprintf(arr, sizeof(arr), "%s%s",
                (utl_fmts_t){.arr = {{.s = "foo"}, {.s = "bar"}}});
    ASSERT(strcmp(arr, "foobar") == 0);
}

static void char_check(void)
{
    utl_sprintf(arr, sizeof(arr), "%c", (utl_fmts_t){.arr = {{.c = 'a'}}});
    ASSERT(strcmp(arr, "a") == 0);

    utl_sprintf(arr, sizeof(arr), "a%c", (utl_fmts_t){.arr = {{.c = 'a'}}});
    ASSERT(strcmp(arr, "aa") == 0);

    utl_sprintf(arr, sizeof(arr), "%cc", (utl_fmts_t){.arr = {{.c = 'a'}}});
    ASSERT(strcmp(arr, "ac") == 0);

    utl_sprintf(arr, sizeof(arr), "%c%c",
                (utl_fmts_t){.arr = {{.c = 'a'}, {.c = 'a'}}});
    ASSERT(strcmp(arr, "aa") == 0);
}

i32 main(void)
{
    tst_setup();

    ctp_logger(tst_null_logger_cb);
    error_check();
    ctp_logger(tst_logger_cb);

    u32_check();
    i32_check();
    f32_check();
    str_check();
    char_check();

    return 0;
}
