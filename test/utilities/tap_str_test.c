#include <assert.h>
#include <limits.h>
#include <string.h>

#include "util/tap_str.h"

static void tap_str_printf_test(void)
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
    tap_str_printf(arr, tap_str_len(overflowcheck1), overflowcheck1,
                 (TapStrPrintfVaList){0});
    assert(strcmp(arr, "toomanychar") == 0);

    tap_str_printf(arr, sizeof(arr), "invalid %t specifier", (TapStrPrintfVaList){0});
    assert(strcmp(arr, "invalid ") == 0);

    tap_str_printf(
        arr, sizeof(arr), "%c%c%c%c%c%c too many specifiers",
        (TapStrPrintfVaList){ .elems = {
                {.c = 'h'}, {.c = 'e'}, {.c = 'l'}, {.c = 'l'}, {.c = 'o'}}});
    assert(strcmp(arr, "hello") == 0);

    tap_str_printf(arr, tap_str_len(overflowcheck2) - 1, overflowcheck2,
                 (TapStrPrintfVaList){.elems = {{.c = 's'}}});
    assert(strcmp(arr, "toomanychar") == 0);

    tap_str_printf(arr, tap_str_len(overflowcheck3) - 1, overflowcheck3,
                 (TapStrPrintfVaList){.elems = {{.s = "s"}}});
    assert(strcmp(arr, "toomanychar") == 0);

    tap_str_printf(arr, tap_str_len(overflowcheck4), overflowcheck4,
                 (TapStrPrintfVaList){.elems = {{.s = "manychars"}}});
    assert(strcmp(arr, "waytoo") == 0);

    tap_str_printf(arr, tap_str_len(overflowcheck5) + 1, overflowcheck5,
                 (TapStrPrintfVaList){.elems = {{.f = 1.0F}}});
    assert(strcmp(arr, "floatdon'tfit") == 0);

    tap_str_printf(arr, tap_str_len(overflowcheck6) + TAP_STR_NUMERIC_MAX_CHARS - 2,
                 overflowcheck6, (TapStrPrintfVaList){.elems = {{.d = 1}}});
    assert(strcmp(arr, "intjustdon'tfit") == 0);

    // Check uints.
    tap_str_printf(arr, sizeof(arr), "%u", (TapStrPrintfVaList){.elems = {{.u = uint_num}}});
    assert(strcmp(arr, "123") == 0);

    tap_str_printf(arr, sizeof(arr), "%u", (TapStrPrintfVaList){.elems = {{.u = 0}}});
    assert(strcmp(arr, "0") == 0);

    tap_str_printf(arr, sizeof(arr), "%u", (TapStrPrintfVaList){.elems = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "4294967295") == 0);

    tap_str_printf(arr, sizeof(arr), "a%ua",
                 (TapStrPrintfVaList){.elems = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "a4294967295a") == 0);

    // Check ints.
    tap_str_printf(arr, sizeof(arr), "%d", (TapStrPrintfVaList){.elems = {{.d = int_num}}});
    assert(strcmp(arr, "123") == 0);

    tap_str_printf(arr, sizeof(arr), "%d", (TapStrPrintfVaList){.elems = {{.d = -int_num}}});
    assert(strcmp(arr, "-123") == 0);

    tap_str_printf(arr, sizeof(arr), "%d", (TapStrPrintfVaList){.elems = {{.d = 0}}});
    assert(strcmp(arr, "0") == 0);

    tap_str_printf(arr, sizeof(arr), "%d", (TapStrPrintfVaList){.elems = {{.d = INT_MIN}}});
    assert(strcmp(arr, "-2147483648") == 0);

    tap_str_printf(arr, sizeof(arr), "%d", (TapStrPrintfVaList){.elems = {{.d = INT_MAX}}});
    assert(strcmp(arr, "2147483647") == 0);

    tap_str_printf(arr, sizeof(arr), "a%da",
                 (TapStrPrintfVaList){.elems = {{.d = INT_MIN}}});
    assert(strcmp(arr, "a-2147483648a") == 0);

    // Check floats.
    const float float_num = 123.456F;
    const float float_num_fract_part = .456F;
    const float float_num_int_part = 123.F;

    tap_str_printf(arr, sizeof(arr), "%f",
                 (TapStrPrintfVaList){.elems = {{.f = float_num}}});
    assert(strcmp(arr, "123.456") == 0);

    tap_str_printf(arr, sizeof(arr), "%f",
                 (TapStrPrintfVaList){.elems = {{.f = float_num_fract_part}}});
    assert(strcmp(arr, "0.456") == 0);

    tap_str_printf(arr, sizeof(arr), "%f",
                 (TapStrPrintfVaList){.elems = {{.f = float_num_int_part}}});
    assert(strcmp(arr, "123.000") == 0);

    tap_str_printf(arr, sizeof(arr), "%f", (TapStrPrintfVaList){.elems = {{.f = 0}}});
    assert(strcmp(arr, "0.000") == 0);

    tap_str_printf(arr, sizeof(arr), "%f", (TapStrPrintfVaList){.elems = {{.f = -0}}});
    assert(strcmp(arr, "0.000") == 0);

    tap_str_printf(arr, sizeof(arr), "%f",
                 (TapStrPrintfVaList){.elems = {{.f = -float_num}}});
    assert(strcmp(arr, "-123.456") == 0);

    tap_str_printf(arr, sizeof(arr), "%f",
                 (TapStrPrintfVaList){.elems = {{.f = -0.0F / 0.0F}}});
    assert(strcmp(arr, "NaN") == 0);

    tap_str_printf(arr, sizeof(arr), "%f",
                 (TapStrPrintfVaList){.elems = {{.f = -1.0F / 0.0F}}});
    assert(strcmp(arr, "Inf") == 0);

    // Check strings.
    tap_str_printf(arr, sizeof(arr), "%s", (TapStrPrintfVaList){.elems = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobar") == 0);

    tap_str_printf(arr, sizeof(arr), "a%s",
                 (TapStrPrintfVaList){.elems = {{.s = "foobar"}}});
    assert(strcmp(arr, "afoobar") == 0);

    tap_str_printf(arr, sizeof(arr), "%sa",
                 (TapStrPrintfVaList){.elems = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobara") == 0);

    tap_str_printf(arr, sizeof(arr), "%s%s",
                 (TapStrPrintfVaList){.elems = {{.s = "foo"}, {.s = "bar"}}});
    assert(strcmp(arr, "foobar") == 0);

    // Check chars.
    tap_str_printf(arr, sizeof(arr), "%c", (TapStrPrintfVaList){.elems = {{.c = 'a'}}});
    assert(strcmp(arr, "a") == 0);

    tap_str_printf(arr, sizeof(arr), "a%c", (TapStrPrintfVaList){.elems = {{.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);

    tap_str_printf(arr, sizeof(arr), "%cc", (TapStrPrintfVaList){.elems = {{.c = 'a'}}});
    assert(strcmp(arr, "ac") == 0);

    tap_str_printf(arr, sizeof(arr), "%c%c",
                 (TapStrPrintfVaList){.elems = {{.c = 'a'}, {.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);
}

int main(void)
{
    tap_str_printf_test();
    return 0;
}
