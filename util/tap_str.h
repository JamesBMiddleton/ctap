#ifndef TAP_STR_H
#define TAP_STR_H

#include "tap_def.h"
#include "tap_math.h"

#define TAP_STR_NULL_TERMINATOR_SZ 1
#define TAP_UINT_MAX_CHARS (10) /* '4294967295' */
#define TAP_UINT_MAX_CHAR_THRESHOLD 1000000000
#define TAP_STR_FLOAT_DECIMAL_CHARS (3)
#define TAP_STR_NUMERIC_MAX_CHARS (1 + TAP_UINT_MAX_CHARS + 1 + TAP_STR_FLOAT_DECIMAL_CHARS) /* '-2147483648.123' */

typedef struct {
    union {
        const char* s;
        char c;
        int d;
        unsigned int u;
        float f;
    } elems[5];
} TapStrPrintfVaList;

/*
 * If aligned copy 32bit chunks from dest to src, else copy bytes.
 * Assumes no overlap.
 *
 * @param dest - copy destination
 * @param src  - copy source
 * @param count - number of bytes to copy
 * @return copy destination
 */
static const void* tap_str_memcpy(void* dest, const void* src, size_t count)
{
    size_t i;

    if (((size_t)src | (size_t)dest | count) & (sizeof(unsigned int) - 1))
    {
        const unsigned char* src_byte = (const unsigned char*)src;
        unsigned char* dest_byte = (unsigned char*)dest;
        for (i = 0; i < count; ++i)
            *(dest_byte++) = *(src_byte++);
    }
    else
    {
        const unsigned int* src_word = (const unsigned int*)src;
        unsigned int* dest_word = (unsigned int*)dest;
        for (i = 0; i < count; i += sizeof(unsigned int))
            *(dest_word++) = *(src_word++);
    }
    return dest;
}

/*
 * @param str - null terminated string.
 * @return length of str, null terminator not included.
 * */
static size_t tap_str_len(const char* str)
{
    size_t len = 0;
    while (*str++ != '\0')
        ++len;
    return len;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @return buf
 */
static char* tap_str_from_uint(unsigned int val, char* buf)
{
    static const unsigned char base = 10;
    unsigned int digits;

    if (val >= TAP_UINT_MAX_CHAR_THRESHOLD)
        buf += TAP_UINT_MAX_CHARS - 1;
    else
        for (digits = base; digits <= val; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (char)('0' + (val % base));
        val /= base;
    } while (val != 0);
    return buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @return buf
 */
static char* tap_str_from_int(int val, char* buf)
{
    static const unsigned char base = 10;
    unsigned int abs = (unsigned int)val;
    unsigned int digits;

    if (val < 0)
    {
        abs = -(unsigned int)val;
        *buf++ = '-';
    }
    if (abs >= TAP_UINT_MAX_CHAR_THRESHOLD)
        buf += TAP_UINT_MAX_CHARS - 1;
    else
        for (digits = base; digits <= abs; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (char)('0' + (abs % base));
        abs /= base;
    } while (abs != 0);
    return (val < 0) ? --buf : buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 * We're checking for inf/nan before float casts, so silencing UB sanitizer.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @param decimals - number of decimal places
 * @return buf
 */
__attribute__((no_sanitize("undefined"))) static char* tap_str_from_float(const float val, char* buf,
                                                                   unsigned char decimals)
{
    static const float base = 10;
    static const char* nan = "NaN";
    static const char* inf = "Inf";
    int whole = (int)val;
    float fraction = tap_math_fabs((val - (float)whole));
    char* start;

    if (tap_math_isnan(val))
    {
        tap_str_memcpy(buf, nan, tap_str_len(nan) + TAP_STR_NULL_TERMINATOR_SZ);
        return buf;
    }

    if (tap_math_isinf(val))
    {
        tap_str_memcpy(buf, inf, tap_str_len(inf) + TAP_STR_NULL_TERMINATOR_SZ);
        return buf;
    }

    start = tap_str_from_int(whole, buf);
    buf += tap_str_len(buf);
    *buf++ = '.';

    if (decimals--)
    {
        fraction *= base;
        while ((int)fraction == 0 && decimals--)
        {
            *buf++ = '0';
            fraction *= base;
        }
        while (decimals--)
            fraction *= base;
        tap_str_from_uint((unsigned int)fraction, buf);
    }
    else
        *buf = '\0';
    return start;
}

/*
 * Takes format specifiers of type %d, %u, %f, %s or %c.
 * e.g. tap_str_sprintf(buf, bufsz, "%f", (TapStrPrintfVaList){.elems = {{.f = 43.0F}}})
 *
 * @param buf - the destination string buffer
 * @param bufsz - size of destination string buffer
 * @param format - the string format
 * @param va_list - struct wrapped array of format specifier values
 * @return - number of chars written or -1 on error
 */
static int tap_str_printf(char* buf, size_t bufsz,
                                const char* format, TapStrPrintfVaList va_list)
{
    int ret = 0;
    size_t i_vals = 0;
    const char* last = buf + bufsz - 1;
    const char* str;
    /* char specifier; */

    while (*format != '\0')
    {
        if (buf == last)
        {
            ret = -1;
            break; /* We ran out of space in the destination buffer. */
        }

        if (*format != '%')
        {
            *buf++ = *format++;
        }
        else
        {
            if (i_vals == (sizeof(va_list.elems) / sizeof(va_list.elems[0])))
            {
                ret = -1; /* Too many specifiers in format string. */
                break;
            }

            ++format;
            char specifier = *format++;

            if (((specifier == 'd' || specifier == 'u' || specifier == 'f') &&
                 (buf + TAP_STR_NUMERIC_MAX_CHARS) > last) ||
                ((specifier == 's') && (buf + tap_str_len(va_list.elems[i_vals].s) > last)))
            {
                ret = -1;
                break; /* Destination buffer too small */
            }

            switch (specifier)
            {
                case 'c': *buf++ = va_list.elems[i_vals++].c; continue;
                case 'd': tap_str_from_int(va_list.elems[i_vals++].d, buf); break;
                case 'u': tap_str_from_uint(va_list.elems[i_vals++].u, buf); break;
                case 'f': tap_str_from_float(va_list.elems[i_vals++].f, buf, 3); break;
                case 's': {
                    str = va_list.elems[i_vals++].s;
                    do
                    {
                        *buf = *str;
                    } while (*str++ != '\0' && ++buf);
                    break;
                }
                default:
                    *buf = '\0';
                    return -1; /* Invalid specifier used. */
            }
            buf += tap_str_len(buf);
        }
    }
    *buf = '\0';
    return ret;
}

#endif /* TAP_STR_H */
