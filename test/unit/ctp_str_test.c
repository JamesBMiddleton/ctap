#include <assert.h>
#include <limits.h>
#include <string.h>

#include "util/ctp_str.h"

static void ctp_str_printf_test(void)
{
    char arr[256] = {0};

    const unsigned int uint_num = 123;
    const int int_num = 123;

    const char* overflowcheck1 = "toomanychars";
    const char* overflowcheck2 = "toomanychar%c";
    const char* overflowcheck3 = "toomanychar%s";
    const char* overflowcheck4 = "waytoo%s";
    const char* overflowcheck5 = "floatdon'tfit%f";
    const char* overflowcheck6 = "intjustdon'tfit%d";

    // Check error cases.
    ctp_str_printf(arr, ctp_str_len(overflowcheck1), overflowcheck1,
                 (struct ctp_str_printf_va_list){0});
    assert(strcmp(arr, "toomanychar") == 0);

    ctp_str_printf(arr, sizeof(arr), "invalid %t specifier", (struct ctp_str_printf_va_list){0});
    assert(strcmp(arr, "invalid ") == 0);

    ctp_str_printf(
        arr, sizeof(arr), "%c%c%c%c%c%c too many specifiers",
        (struct ctp_str_printf_va_list){ .elems = {
                {.c = 'h'}, {.c = 'e'}, {.c = 'l'}, {.c = 'l'}, {.c = 'o'}}});
    assert(strcmp(arr, "hello") == 0);

    ctp_str_printf(arr, ctp_str_len(overflowcheck2) - 1, overflowcheck2,
                 (struct ctp_str_printf_va_list){.elems = {{.c = 's'}}});
    assert(strcmp(arr, "toomanychar") == 0);

    ctp_str_printf(arr, ctp_str_len(overflowcheck3) - 1, overflowcheck3,
                 (struct ctp_str_printf_va_list){.elems = {{.s = "s"}}});
    assert(strcmp(arr, "toomanychar") == 0);

    ctp_str_printf(arr, ctp_str_len(overflowcheck4), overflowcheck4,
                 (struct ctp_str_printf_va_list){.elems = {{.s = "manychars"}}});
    assert(strcmp(arr, "waytoo") == 0);

    ctp_str_printf(arr, ctp_str_len(overflowcheck5) + 1, overflowcheck5,
                 (struct ctp_str_printf_va_list){.elems = {{.f = 1.0F}}});
    assert(strcmp(arr, "floatdon'tfit") == 0);

    ctp_str_printf(arr, ctp_str_len(overflowcheck6) + CTP_STR_NUMERIC_MAX_CHARS - 2,
                 overflowcheck6, (struct ctp_str_printf_va_list){.elems = {{.d = 1}}});
    assert(strcmp(arr, "intjustdon'tfit") == 0);

    // Check uints.
    ctp_str_printf(arr, sizeof(arr), "%u", (struct ctp_str_printf_va_list){.elems = {{.u = uint_num}}});
    assert(strcmp(arr, "123") == 0);

    ctp_str_printf(arr, sizeof(arr), "%u", (struct ctp_str_printf_va_list){.elems = {{.u = 0}}});
    assert(strcmp(arr, "0") == 0);

    ctp_str_printf(arr, sizeof(arr), "%u", (struct ctp_str_printf_va_list){.elems = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "4294967295") == 0);

    ctp_str_printf(arr, sizeof(arr), "a%ua",
                 (struct ctp_str_printf_va_list){.elems = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "a4294967295a") == 0);

    // Check ints.
    ctp_str_printf(arr, sizeof(arr), "%d", (struct ctp_str_printf_va_list){.elems = {{.d = int_num}}});
    assert(strcmp(arr, "123") == 0);

    ctp_str_printf(arr, sizeof(arr), "%d", (struct ctp_str_printf_va_list){.elems = {{.d = -int_num}}});
    assert(strcmp(arr, "-123") == 0);

    ctp_str_printf(arr, sizeof(arr), "%d", (struct ctp_str_printf_va_list){.elems = {{.d = 0}}});
    assert(strcmp(arr, "0") == 0);

    ctp_str_printf(arr, sizeof(arr), "%d", (struct ctp_str_printf_va_list){.elems = {{.d = INT_MIN}}});
    assert(strcmp(arr, "-2147483648") == 0);

    ctp_str_printf(arr, sizeof(arr), "%d", (struct ctp_str_printf_va_list){.elems = {{.d = INT_MAX}}});
    assert(strcmp(arr, "2147483647") == 0);

    ctp_str_printf(arr, sizeof(arr), "a%da",
                 (struct ctp_str_printf_va_list){.elems = {{.d = INT_MIN}}});
    assert(strcmp(arr, "a-2147483648a") == 0);

    // Check floats.
    const float float_num = 123.456F;
    const float float_num_fract_part = .456F;
    const float float_num_int_part = 123.F;

    ctp_str_printf(arr, sizeof(arr), "%f",
                 (struct ctp_str_printf_va_list){.elems = {{.f = float_num}}});
    assert(strcmp(arr, "123.456") == 0);

    ctp_str_printf(arr, sizeof(arr), "%f",
                 (struct ctp_str_printf_va_list){.elems = {{.f = float_num_fract_part}}});
    assert(strcmp(arr, "0.456") == 0);

    ctp_str_printf(arr, sizeof(arr), "%f",
                 (struct ctp_str_printf_va_list){.elems = {{.f = float_num_int_part}}});
    assert(strcmp(arr, "123.000") == 0);

    ctp_str_printf(arr, sizeof(arr), "%f", (struct ctp_str_printf_va_list){.elems = {{.f = 0}}});
    assert(strcmp(arr, "0.000") == 0);

    ctp_str_printf(arr, sizeof(arr), "%f", (struct ctp_str_printf_va_list){.elems = {{.f = -0}}});
    assert(strcmp(arr, "0.000") == 0);

    ctp_str_printf(arr, sizeof(arr), "%f",
                 (struct ctp_str_printf_va_list){.elems = {{.f = -float_num}}});
    assert(strcmp(arr, "-123.456") == 0);

    ctp_str_printf(arr, sizeof(arr), "%f",
                 (struct ctp_str_printf_va_list){.elems = {{.f = -0.0F / 0.0F}}});
    assert(strcmp(arr, "NaN") == 0);

    ctp_str_printf(arr, sizeof(arr), "%f",
                 (struct ctp_str_printf_va_list){.elems = {{.f = -1.0F / 0.0F}}});
    assert(strcmp(arr, "Inf") == 0);

    // Check strings.
    ctp_str_printf(arr, sizeof(arr), "%s", (struct ctp_str_printf_va_list){.elems = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobar") == 0);

    ctp_str_printf(arr, sizeof(arr), "a%s",
                 (struct ctp_str_printf_va_list){.elems = {{.s = "foobar"}}});
    assert(strcmp(arr, "afoobar") == 0);

    ctp_str_printf(arr, sizeof(arr), "%sa",
                 (struct ctp_str_printf_va_list){.elems = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobara") == 0);

    ctp_str_printf(arr, sizeof(arr), "%s%s",
                 (struct ctp_str_printf_va_list){.elems = {{.s = "foo"}, {.s = "bar"}}});
    assert(strcmp(arr, "foobar") == 0);

    // Check chars.
    ctp_str_printf(arr, sizeof(arr), "%c", (struct ctp_str_printf_va_list){.elems = {{.c = 'a'}}});
    assert(strcmp(arr, "a") == 0);

    ctp_str_printf(arr, sizeof(arr), "a%c", (struct ctp_str_printf_va_list){.elems = {{.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);

    ctp_str_printf(arr, sizeof(arr), "%cc", (struct ctp_str_printf_va_list){.elems = {{.c = 'a'}}});
    assert(strcmp(arr, "ac") == 0);

    ctp_str_printf(arr, sizeof(arr), "%c%c",
                 (struct ctp_str_printf_va_list){.elems = {{.c = 'a'}, {.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);
}

int main(void)
{
    ctp_str_printf_test();
    return 0;
}
