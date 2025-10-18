#include <assert.h>
#include <limits.h>
#include <string.h>

#include "util/tap_str.h"

static void tap_str_printf_test(void)
{
    char arr[256] = {0};

    const unsigned int uint_num = 123;
    const int int_num = 123;

    const char *overflowcheck1 = "toomanychars";
    const char *overflowcheck2 = "toomanychar%c";
    const char *overflowcheck3 = "toomanychar%s";
    const char *overflowcheck4 = "waytoo%s";
    const char *overflowcheck5 = "floatdon'tfit%f";
    const char *overflowcheck6 = "intjustdon'tfit%d";

    const float float_num = 123.456F;
    const float float_num_fract_part = .456F;
    const float float_num_int_part = 123.F;

    const TapStrPrintfVaList valist_zero = {0};
    TapStrPrintfVaList valist = valist_zero;

    /* Check error cases. */
    tap_str_printf(arr, tap_str_len(overflowcheck1), overflowcheck1, valist);
    assert(strcmp(arr, "toomanychar") == 0);

    tap_str_printf(arr, sizeof(arr), "invalid %t specifier", valist);
    assert(strcmp(arr, "invalid ") == 0);

    valist = valist_zero;
    valist.elems[0].c = 'h';
    valist.elems[1].c = 'e';
    valist.elems[2].c = 'l';
    valist.elems[3].c = 'l';
    valist.elems[4].c = 'o';
    tap_str_printf(arr, sizeof(arr), "%c%c%c%c%c%c too many specifiers", valist);
    assert(strcmp(arr, "hello") == 0);

    valist = valist_zero;
    valist.elems[0].c = 's';
    tap_str_printf(arr, tap_str_len(overflowcheck2) - 1, overflowcheck2, valist);
    assert(strcmp(arr, "toomanychar") == 0);

    valist = valist_zero;
    valist.elems[0].s = "s";
    tap_str_printf(arr, tap_str_len(overflowcheck3) - 1, overflowcheck3, valist);
    assert(strcmp(arr, "toomanychar") == 0);

    valist = valist_zero;
    valist.elems[0].s = "manychars";
    tap_str_printf(arr, tap_str_len(overflowcheck4), overflowcheck4, valist);
    assert(strcmp(arr, "waytoo") == 0);

    valist = valist_zero;
    valist.elems[0].f = 1.0F;
    tap_str_printf(arr, tap_str_len(overflowcheck5) + 1, overflowcheck5, valist);
    assert(strcmp(arr, "floatdon'tfit") == 0);

    valist = valist_zero;
    valist.elems[0].d = 1;
    tap_str_printf(arr, tap_str_len(overflowcheck6) + STR_NUMERIC_MAX_CHARS - 2, overflowcheck6, valist);
    assert(strcmp(arr, "intjustdon'tfit") == 0);

    /* Check uints */
    valist = valist_zero;
    valist.elems[0].u = uint_num;
    tap_str_printf(arr, sizeof(arr), "%u", valist);
    assert(strcmp(arr, "123") == 0);

    valist = valist_zero;
    valist.elems[0].u = 0;
    tap_str_printf(arr, sizeof(arr), "%u", valist);
    assert(strcmp(arr, "0") == 0);

    valist = valist_zero;
    valist.elems[0].u = UINT_MAX;
    tap_str_printf(arr, sizeof(arr), "%u", valist);
    assert(strcmp(arr, "4294967295") == 0);

    valist = valist_zero;
    valist.elems[0].u = UINT_MAX;
    tap_str_printf(arr, sizeof(arr), "a%ua", valist);
    assert(strcmp(arr, "a4294967295a") == 0);

    /* Check ints. */
    valist = valist_zero;
    valist.elems[0].d = 123;
    tap_str_printf(arr, sizeof(arr), "%d", valist);
    assert(strcmp(arr, "123") == 0);

    valist = valist_zero;
    valist.elems[0].d = -int_num;
    tap_str_printf(arr, sizeof(arr), "%d", valist);
    assert(strcmp(arr, "-123") == 0);

    valist = valist_zero;
    valist.elems[0].d = 0;
    tap_str_printf(arr, sizeof(arr), "%d", valist);
    assert(strcmp(arr, "0") == 0);

    valist = valist_zero;
    valist.elems[0].d = INT_MIN;
    tap_str_printf(arr, sizeof(arr), "%d", valist);
    assert(strcmp(arr, "-2147483648") == 0);

    valist = valist_zero;
    valist.elems[0].d = INT_MAX;
    tap_str_printf(arr, sizeof(arr), "%d", valist);
    assert(strcmp(arr, "2147483647") == 0);

    valist = valist_zero;
    valist.elems[0].d = INT_MIN;
    tap_str_printf(arr, sizeof(arr), "a%da", valist);
    assert(strcmp(arr, "a-2147483648a") == 0);

    /* Check floats. */
    valist = valist_zero;
    valist.elems[0].f = float_num;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "123.456") == 0);

    valist = valist_zero;
    valist.elems[0].f = float_num_fract_part;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "0.456") == 0);

    valist = valist_zero;
    valist.elems[0].f = float_num_int_part;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "123.000") == 0);

    valist = valist_zero;
    valist.elems[0].f = 0;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "0.000") == 0);

    valist = valist_zero;
    valist.elems[0].f = -0;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "0.000") == 0);

    valist = valist_zero;
    valist.elems[0].f = -float_num;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "-123.456") == 0);

    valist = valist_zero;
    valist.elems[0].f = -0.0F / 0.0F;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "NaN") == 0);

    valist = valist_zero;
    valist.elems[0].f = -1.0F / 0.0F;
    tap_str_printf(arr, sizeof(arr), "%f", valist);
    assert(strcmp(arr, "Inf") == 0);

    /* Check strings. */
    valist = valist_zero;
    valist.elems[0].s = "foobar";
    tap_str_printf(arr, sizeof(arr), "%s", valist);
    assert(strcmp(arr, "foobar") == 0);

    valist = valist_zero;
    valist.elems[0].s = "foobar";
    tap_str_printf(arr, sizeof(arr), "a%s", valist);
    assert(strcmp(arr, "afoobar") == 0);

    valist = valist_zero;
    valist.elems[0].s = "foobar";
    tap_str_printf(arr, sizeof(arr), "%sa", valist);
    assert(strcmp(arr, "foobara") == 0);

    valist = valist_zero;
    valist.elems[0].s = "foo";
    valist.elems[1].s = "bar";
    tap_str_printf(arr, sizeof(arr), "%s%s", valist);
    assert(strcmp(arr, "foobar") == 0);

    /* Check chars. */
    valist = valist_zero;
    valist.elems[0].c = 'a';
    tap_str_printf(arr, sizeof(arr), "%c", valist);
    assert(strcmp(arr, "a") == 0);

    valist = valist_zero;
    valist.elems[0].c = 'a';
    tap_str_printf(arr, sizeof(arr), "a%c", valist);
    assert(strcmp(arr, "aa") == 0);

    valist = valist_zero;
    valist.elems[0].c = 'a';
    tap_str_printf(arr, sizeof(arr), "%cc", valist);
    assert(strcmp(arr, "ac") == 0);

    valist = valist_zero;
    valist.elems[0].c = 'a';
    valist.elems[1].c = 'a';
    tap_str_printf(arr, sizeof(arr), "%c%c", valist);
    assert(strcmp(arr, "aa") == 0);
}

int main(void)
{
    tap_str_printf_test();
    return 0;
}
